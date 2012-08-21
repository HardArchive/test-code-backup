#include "StdAfx.h"
#include "SkinMgr.h"
#include "SkinRes.h"

#define BUTTON_WIDTH	129
#define BUTTON_HEIGHT	23

#define BUTTON_MNU	0
#define BUTTON_MIN	1
#define BUTTON_MAX	2
#define BUTTON_CLS	3

#define HS_SIDE 3
#define HS_CORNER 12

namespace SkinMgr
{
	enum FRAME_HTCODE
	{
		HIT_NULL = 0, 
		HIT_ICON, 
		HIT_TOPLEFT, 
		HIT_TOP, 
		HIT_TOPRIGHT, 
		HIT_LEFT, 
		HIT_RIGHT, 
		HIT_BOTTOMLEFT, 
		HIT_BOTTOM, 
		HIT_BOTTOMRIGHT, 
		HIT_CAPTION, 
		HIT_BUTTONMNU, 
		HIT_BUTTONMIN, 
		HIT_BUTTONMAX, 
		HIT_BUTTONCLS, 
	};

	//////////////////////////////////////////////////////////////////////////
	// CSkinFrame
	CSkinFrame::CSkinFrame(void)
	{
		// 初始化标题栏按钮的状态为0
		memset(m_ButtonState, 0, sizeof(DWORD) * BUTTON_COUNT);
		m_nMinWidth = 0;
		m_nMinHeight = 0;

		// 创建标题栏字体
		LOGFONT lf;
		GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("Microsoft YaHei"));
		lf.lfHeight = -12;
		lf.lfWeight |= FW_BOLD;
		m_hCapFont = CreateFontIndirect(&lf);

		m_CurHoverButton = -1;
		m_CurPushButton = -1;

		m_ButtonDC.CreateMemDC(BUTTON_WIDTH, BUTTON_HEIGHT);
	}

	CSkinFrame::~CSkinFrame(void)
	{
		
	}

	RECT CSkinFrame::rcIcon = {4, 4, 4 + GetSystemMetrics(SM_CXSMICON), 4 + GetSystemMetrics(SM_CYSMICON)};


	UINT CSkinFrame::HitTest(int x, int y)
	{
		ASSERT( ::IsWindow(m_hWnd) );

		RECT rc;
		GetWindowRect(m_hWnd, &rc);

		int cx = rc.right - rc.left;
		int cy= rc.bottom - rc.top;

		// 鼠标不在窗口内
		if (x < 0 || y < 0 || x > cx || y > cy)
			return HIT_NULL;

		// 如果窗口不是最大化或者最小化才进行Size区域判断
		if ( !IsZoomed(m_hWnd) && !IsIconic(m_hWnd) )
		{
			// 判断是否在左上角
			if ( (x <= HS_SIDE && y <= HS_CORNER) ||
				(x <= HS_CORNER && y <= HS_SIDE) )
				return HIT_TOPLEFT;

			// 判断是否在右上角
			if ( (x >= cx - HS_CORNER && y <= HS_SIDE) ||
				(x >= cx - HS_SIDE && y <= HS_CORNER) )
				return HIT_TOPRIGHT;

			// 判断是否在左下角
			if ( (x <= HS_SIDE && y >= cy - HS_CORNER) ||
				(x <= HS_CORNER && y >= cy - HS_CORNER) )
				return HIT_BOTTOMLEFT;

			// 判断是否在右下角
			if ( (x >= cx - HS_SIDE && y >= cy - HS_CORNER) ||
				(x >= cx - HS_CORNER && y >= cy - HS_CORNER) )
				return HIT_BOTTOMRIGHT;

			// 判断是否在顶边
			if ( y <= HS_SIDE)
				return HIT_TOP;

			// 判断是否在左边
			if ( x <= HS_SIDE)
				return HIT_LEFT;

			// 判断是否在右边
			if ( x >= cx - HS_SIDE)
				return HIT_RIGHT;

			// 判断是否在底边
			if ( y >= cy - HS_SIDE)
				return HIT_BOTTOM;
		}

		// 判断是否单击的是图标
		POINT pt = {x, y};
		if (PtInRect(&CSkinFrame::rcIcon, pt))
			return HIT_ICON;

		// 判断是否在标题栏按钮内
		if ( x >= cx - 129 && y <= 23)
		{
			if ( x <= cx - 99) return HIT_BUTTONMNU;
			if ( x <= cx - 69) return HIT_BUTTONMIN;
			if ( x <= cx - 39) return HIT_BUTTONMAX;
			return HIT_BUTTONCLS;
		}

		// 检测是否点击的是标题栏
		if ( y <= 23 )
			return HIT_CAPTION;

		return HIT_NULL;
	}

	BOOL CSkinFrame::Attach(HWND hWnd)
	{
		if ( !CSkinWnd::Attach(hWnd) )
			return FALSE;

		DWORD dwStyle = (DWORD)::GetWindowLong(hWnd, GWL_STYLE);
		dwStyle &= ~WS_CAPTION;
		dwStyle |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		::SetWindowLong(hWnd, GWL_STYLE, (LONG)dwStyle);
		::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, 
			SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

		RECT rcClient;
		GetClientRect(m_hWnd, &rcClient);
		int ccx = rcClient.right - rcClient.left;
		int ccy = rcClient.bottom - rcClient.top;

		SetWindowRgn();
		m_MemDC.CreateMemDC(ccx, ccy);
		DrawButton();
		DrawSkin(TRUE);

		return TRUE;
	}
	
	BOOL CSkinFrame::Detach(void)
	{
		return CSkinWnd::Detach();
	}

	void CSkinFrame::SetMinSize(int nMinWidth, int nMinHeight) { m_nMinWidth = nMinWidth; m_nMinHeight = nMinHeight; }

	void CSkinFrame::SetMinWidth(int nMinWidth) { m_nMinWidth = nMinWidth; }
	int CSkinFrame::GetMinWidth(void) const { return m_nMinWidth; }

	void CSkinFrame::SetMinHeight(int nMinHeight) { m_nMinHeight = nMinHeight; }
	int CSkinFrame::GetMinHeight(void) const { return m_nMinHeight; }

	BOOL CSkinFrame::DrawSkin(BOOL bUpdate/* = FALSE*/)
	{
		int cx = m_MemDC.GetWidth();
		int cy = m_MemDC.GetHeight();

		int nOldSbMode = m_MemDC.SetStretchBltMode(HALFTONE);

		m_MemDC.GridBlt(0, 0,cx, cy,g_ResDC.m_hDC, 0, 0, 8, 120, 3, 91, 3, 28, SRCCOPY);

		// 绘画图标
		HICON hIcon = (HICON)::SendMessage(m_hWnd, WM_GETICON, ICON_SMALL, NULL);

		m_MemDC.DrawIconEx(CSkinFrame::rcIcon.left, CSkinFrame::rcIcon.top, 
			hIcon, (CSkinFrame::rcIcon.right - CSkinFrame::rcIcon.left), 
			(CSkinFrame::rcIcon.bottom - CSkinFrame::rcIcon.top));

		int nLength = ::GetWindowTextLength(m_hWnd);
		if (nLength > 0)
		{
			TCHAR *pchBuff = (TCHAR*)::malloc( (nLength + 1) * sizeof(TCHAR) );
			if (pchBuff)
			{
				// 获取窗口文本
				::GetWindowText(m_hWnd, pchBuff, nLength + 1);
				// 计算标题绘制区域
				RECT rcText;
				SetRect(&rcText, 24, 2,cx - m_ButtonDC.GetWidth(), 21);

				// 设置字体
				HFONT hOldFont = m_MemDC.SelectFont(m_hCapFont);

				// 设置文字颜色
				COLORREF clrOld = m_MemDC.SetTextColor(RGB(255, 255, 255));

				// 设置字体背景模式为透明
				int nOldBkMode = m_MemDC.SetBkMode(TRANSPARENT);

				m_MemDC.DrawTextExW(pchBuff, nLength, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

				// 恢复DC属性和释放资源
				m_MemDC.SetBkMode(nOldBkMode);
				m_MemDC.SelectFont(hOldFont);
				m_MemDC.SetTextColor(clrOld);

				free( (void*)pchBuff );
				pchBuff = NULL;
			}
		}

		// 拷贝按钮场景
		m_MemDC.BitBlt(m_MemDC.GetWidth() - m_ButtonDC.GetWidth(), 0, m_ButtonDC.GetWidth(), m_ButtonDC.GetHeight(), m_ButtonDC.m_hDC, 0, 0, SRCCOPY);

		m_MemDC.SetStretchBltMode(nOldSbMode);

		if ( bUpdate && ::IsWindow(m_hWnd) )
		{
			CClientDC dc(m_hWnd);
			dc.BitBlt(0, 0,cx, cy, m_MemDC.m_hDC, 0, 0, SRCCOPY);
		}
		return TRUE;
	}

	BOOL CSkinFrame::DrawButton(BOOL bUpdate/* = FALSE*/)
	{
		int cx = m_ButtonDC.GetWidth();
		int cy = m_ButtonDC.GetHeight();

		// 绘画菜单按钮
		m_ButtonDC.BitBlt(0, 0, 30, cy, g_ResDC.m_hDC, 8, m_ButtonState[BUTTON_MNU] * cy, SRCCOPY);

		// 绘画最小化按钮
		m_ButtonDC.BitBlt(30, 0, 30, cy, g_ResDC.m_hDC, 38, m_ButtonState[BUTTON_MIN] * cy, SRCCOPY);

		if ( ::IsWindow(m_hWnd) && ::IsZoomed(m_hWnd) ) // 绘画还原按钮
			m_ButtonDC.BitBlt(60, 0, 30, cy, g_ResDC.m_hDC, 98, m_ButtonState[BUTTON_MAX] * cy, SRCCOPY);
		else // 绘画最大化按钮
			m_ButtonDC.BitBlt(60, 0, 30, cy, g_ResDC.m_hDC, 68, m_ButtonState[BUTTON_MAX] * cy, SRCCOPY);

		// 绘画关闭按钮
		m_ButtonDC.BitBlt(90, 0, 39, cy, g_ResDC.m_hDC, 128, m_ButtonState[BUTTON_CLS] * cy, SRCCOPY);

		if ( bUpdate && ::IsWindow(m_hWnd) )
		{
			// 拷贝按钮场景
			m_MemDC.BitBlt(m_MemDC.GetWidth() - m_ButtonDC.GetWidth(), 0, m_ButtonDC.GetWidth(), m_ButtonDC.GetHeight(), m_ButtonDC.m_hDC, 0, 0, SRCCOPY);

			CClientDC dc(m_hWnd);
			dc.BitBlt(0, 0, m_MemDC.GetWidth(), m_MemDC.GetHeight(), m_MemDC.m_hDC, 0, 0, SRCCOPY);
		}
		return TRUE;
	}

	BOOL CSkinFrame::SetWindowRgn(void)
	{
		RECT rc;
		::GetWindowRect(m_hWnd, &rc);

		int cx = rc.right - rc.left;
		int cy= rc.bottom - rc.top;

		HRGN hRgn = ::CreateRectRgn(0, 0, cx + 1, cy +1);
		HRGN hLTRgn = ::CreateRectRgn(0, 0, 1, 1);
		HRGN hRTRgn = ::CreateRectRgn(cx - 1, 0, cx + 1, 1);
		HRGN hLBRgn = ::CreateRectRgn(0, cy - 1, 1, cy + 1);
		HRGN hRBRgn = ::CreateRectRgn(cx -1, cy - 1, cx + 1, cy + 1);

		::CombineRgn(hRgn, hRgn, hLTRgn, RGN_DIFF);
		::CombineRgn(hRgn, hRgn, hRTRgn, RGN_DIFF);
		::CombineRgn(hRgn, hRgn, hLBRgn, RGN_DIFF);
		::CombineRgn(hRgn, hRgn, hRBRgn, RGN_DIFF);

		::DeleteObject((HGDIOBJ)hLTRgn);
		::DeleteObject((HGDIOBJ)hRTRgn);
		::DeleteObject((HGDIOBJ)hLBRgn);
		::DeleteObject((HGDIOBJ)hRBRgn);

		::SetWindowRgn(m_hWnd, hRgn, TRUE);

		::DeleteObject((HGDIOBJ)hRgn);

		return TRUE;
	}

	void CSkinFrame::TrackSystemMenu(void)
	{
		HMENU hSysMenu = GetSystemMenu(m_hWnd, FALSE);
		if (hSysMenu)
		{
			int nMenuCount = ::GetMenuItemCount(hSysMenu);
			for (int i = 0; i < nMenuCount; i++)
			{
				UINT uMenuID = ::GetMenuItemID(hSysMenu, i);
				if (SC_RESTORE == uMenuID)
					EnableMenuItem(hSysMenu, uMenuID, (IsZoomed(m_hWnd) ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)) | MF_BYCOMMAND);
				else if (SC_MAXIMIZE == uMenuID)
					EnableMenuItem(hSysMenu, uMenuID, (IsZoomed(m_hWnd) ? (MF_DISABLED | MF_GRAYED) : MF_ENABLED) | MF_BYCOMMAND);
				else if (SC_MOVE == uMenuID)
					EnableMenuItem(hSysMenu, uMenuID, (IsZoomed(m_hWnd) ? (MF_DISABLED | MF_GRAYED) : MF_ENABLED) | MF_BYCOMMAND);
				else if (SC_SIZE == uMenuID)
					EnableMenuItem(hSysMenu, uMenuID, (IsZoomed(m_hWnd) ? (MF_DISABLED | MF_GRAYED) : MF_ENABLED) | MF_BYCOMMAND);
				else
					EnableMenuItem(hSysMenu, uMenuID, MF_ENABLED | MF_BYCOMMAND);
			}

			POINT pt;
			::GetCursorPos(&pt);
			int nID = ::TrackPopupMenu(hSysMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x + 1, pt.y + 1, FALSE, m_hWnd, NULL);
			if (-1 != nID) ::PostMessage(m_hWnd, WM_SYSCOMMAND, nID, NULL);
		}
	}

	/*virtual*/ LRESULT CSkinFrame::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SKINMGR_MSG_HANDLER(WM_ERASEBKGND, OnEraseBkgnd);
		SKINMGR_MSG_HANDLER(WM_PAINT, OnPaint);
		SKINMGR_MSG_HANDLER(WM_NCCALCSIZE, OnNcCalcSize);
		SKINMGR_MSG_HANDLER(WM_NCACTIVATE, OnNcActivate);
		SKINMGR_MSG_HANDLER(WM_NCHITTEST, OnNcHitTest);
		SKINMGR_MSG_HANDLER(WM_SIZE, OnSize);

		SKINMGR_MSG_HANDLER(WM_LBUTTONDBLCLK, OnMouseDblClk);
		SKINMGR_MSG_HANDLER(WM_MBUTTONDBLCLK, OnMouseDblClk);
		SKINMGR_MSG_HANDLER(WM_RBUTTONDBLCLK, OnMouseDblClk);

		SKINMGR_MSG_HANDLER(WM_MOUSEMOVE, OnMouseMove);

		SKINMGR_MSG_HANDLER(WM_LBUTTONDOWN, OnMouseDown);
		SKINMGR_MSG_HANDLER(WM_MBUTTONDOWN, OnMouseDown);
		SKINMGR_MSG_HANDLER(WM_RBUTTONDOWN, OnMouseDown);

		SKINMGR_MSG_HANDLER(WM_LBUTTONUP, OnMouseUp);
		SKINMGR_MSG_HANDLER(WM_MBUTTONUP, OnMouseUp);
		SKINMGR_MSG_HANDLER(WM_RBUTTONUP, OnMouseUp);

		SKINMGR_MSG_HANDLER(WM_MOUSELEAVE, OnMouseLeave);

		SKINMGR_MSG_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo);

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinFrame::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		return TRUE;
	}

	LRESULT CSkinFrame::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		BitBlt(ps.hdc, 0, 0, m_MemDC.GetWidth(), m_MemDC.GetHeight(), m_MemDC.m_hDC, 0, 0,SRCCOPY);
		EndPaint(m_hWnd, &ps);
		return TRUE;
	}

	LRESULT CSkinFrame::OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		return TRUE;
	}

	LRESULT CSkinFrame::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/)
	{
		return (wParam ? FALSE : TRUE);
	}

	LRESULT CSkinFrame::OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam)
	{
		POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
		ScreenToClient(m_hWnd, &pt);

		FRAME_HTCODE nCode = (FRAME_HTCODE)HitTest(pt.x, pt.y);
		LRESULT lResult= HTCLIENT;
		switch (nCode)
		{
		case HIT_TOP:			lResult = HTTOP;			break;
		case HIT_TOPLEFT:		lResult = HTTOPLEFT;		break;
		case HIT_TOPRIGHT:		lResult = HTTOPRIGHT;		break;
		case HIT_LEFT:			lResult = HTLEFT;			break;
		case HIT_RIGHT:			lResult = HTRIGHT;			break;
		case HIT_BOTTOM:		lResult = HTBOTTOM;			break;
		case HIT_BOTTOMLEFT:	lResult = HTBOTTOMLEFT;		break;
		case HIT_BOTTOMRIGHT:	lResult = HTBOTTOMRIGHT;	break;
		case HIT_ICON:			lResult = HTMENU;			break;
		}

		return lResult;
	}

	LRESULT CSkinFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// 最小化的时候不作处理
		if (SIZE_MINIMIZED == wParam) return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);

		RECT rcClient;
		GetClientRect(m_hWnd, &rcClient);
		int ccx = rcClient.right - rcClient.left;
		int ccy = rcClient.bottom - rcClient.top;

		static BOOL bRedrawButton = FALSE;
		if (SIZE_MAXIMIZED == wParam) {
			bRedrawButton = TRUE;
			DrawButton();
		} else if (bRedrawButton) {
			bRedrawButton = FALSE;
			DrawButton();
		}

		m_MemDC.CreateMemDC(ccx, ccy);
		SetWindowRgn();
		DrawSkin(TRUE);

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinFrame::OnMouseDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FRAME_HTCODE nCode = (FRAME_HTCODE)HitTest(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		if (WM_LBUTTONDBLCLK == uMsg)
		{
			if (HIT_BUTTONMNU == nCode
				|| HIT_BUTTONMIN == nCode
				|| HIT_BUTTONMAX == nCode
				|| HIT_BUTTONCLS == nCode ) {

					m_ButtonState[BUTTON_MNU] = (HIT_BUTTONMNU == nCode) ? 2 : 0;
					m_ButtonState[BUTTON_MIN] = (HIT_BUTTONMIN == nCode) ? 2 : 0;
					m_ButtonState[BUTTON_MAX] = (HIT_BUTTONMAX == nCode) ? 2 : 0;
					m_ButtonState[BUTTON_CLS] = (HIT_BUTTONCLS == nCode) ? 2 : 0;
					DrawButton(TRUE);
			} else if (HIT_CAPTION == nCode) {
				::ReleaseCapture();
				::SendMessage(m_hWnd, WM_SYSCOMMAND, (::IsZoomed(m_hWnd) ? SC_RESTORE : SC_MAXIMIZE), NULL);
			}
		}

		if (HIT_ICON == nCode) MessageBox(m_hWnd, _T("A"), _T("b"), MB_OK);

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinFrame::OnMouseDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::SetCapture(m_hWnd);
		FRAME_HTCODE nCode = (FRAME_HTCODE)HitTest(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		if (WM_LBUTTONDOWN == uMsg)
		{
			if (HIT_BUTTONMNU == nCode || HIT_BUTTONMIN == nCode || HIT_BUTTONMAX == nCode || HIT_BUTTONCLS == nCode) 
			{
				m_ButtonState[BUTTON_MNU] = (HIT_BUTTONMNU == nCode) ? 2 : 0;
				m_ButtonState[BUTTON_MIN] = (HIT_BUTTONMIN == nCode) ? 2 : 0;
				m_ButtonState[BUTTON_MAX] = (HIT_BUTTONMAX == nCode) ? 2 : 0;
				m_ButtonState[BUTTON_CLS] = (HIT_BUTTONCLS == nCode) ? 2 : 0;
				DrawButton(TRUE);

			} else if (HIT_CAPTION == nCode) {
				::ReleaseCapture();
				::SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
			}

			m_CurPushButton = nCode;
		}
		
		if (HIT_ICON == nCode) TrackSystemMenu();

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// 鼠标按下的时候不进行MouseMove处理
		if ( (wParam & MK_LBUTTON) == MK_LBUTTON || (wParam & MK_RBUTTON) == MK_RBUTTON ) return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);

		TRACKMOUSEEVENT tm = {
			sizeof(TRACKMOUSEEVENT), 
			TME_LEAVE, 
			m_hWnd, 
			0
		};
		_TrackMouseEvent(&tm);

		FRAME_HTCODE nCode = (FRAME_HTCODE)HitTest(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		if (HIT_BUTTONMNU == nCode || HIT_BUTTONMIN == nCode || HIT_BUTTONMAX == nCode || HIT_BUTTONCLS == nCode) {

			if (HIT_BUTTONMNU == nCode && 1 == m_ButtonState[BUTTON_MNU]) goto DoExitDrawButton;
			if (HIT_BUTTONMIN == nCode && 1 == m_ButtonState[BUTTON_MIN]) goto DoExitDrawButton;
			if (HIT_BUTTONMAX == nCode && 1 == m_ButtonState[BUTTON_MAX]) goto DoExitDrawButton;
			if (HIT_BUTTONCLS == nCode && 1 == m_ButtonState[BUTTON_CLS]) goto DoExitDrawButton;

			m_ButtonState[BUTTON_MNU] = (HIT_BUTTONMNU == nCode) ? 1 : 0;
			m_ButtonState[BUTTON_MIN] = (HIT_BUTTONMIN == nCode) ? 1 : 0;
			m_ButtonState[BUTTON_MAX] = (HIT_BUTTONMAX == nCode) ? 1 : 0;
			m_ButtonState[BUTTON_CLS] = (HIT_BUTTONCLS == nCode) ? 1 : 0;

			DrawButton(TRUE);
			goto DoExitDrawButton;
		} else {
			for (int i = 0; i < BUTTON_COUNT; i++)
			{
				if (0 != m_ButtonState[i])
				{
					ZeroMemory(m_ButtonState, BUTTON_COUNT * sizeof(DWORD));
					DrawButton(TRUE);				
					break;
				}
			}

		}
DoExitDrawButton:
		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinFrame::OnMouseUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::ReleaseCapture();
		FRAME_HTCODE nCode = (FRAME_HTCODE)HitTest(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		if (WM_LBUTTONUP == uMsg)
		{ 
			if (m_CurPushButton == nCode)
			{
				if (  HIT_BUTTONMNU == nCode) {
				} else if (  HIT_BUTTONMIN == nCode) {
					::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
				} else if ( HIT_BUTTONMAX == nCode)	{
					::SendMessage(m_hWnd, WM_SYSCOMMAND, (::IsZoomed(m_hWnd) ? SC_RESTORE : SC_MAXIMIZE), NULL);
				} else if ( HIT_BUTTONCLS == nCode)	{
					::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL);
				}
			}
			m_CurPushButton = -1;
		} else if (WM_RBUTTONUP == uMsg && HIT_CAPTION == nCode) {
			TrackSystemMenu();
		}

		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			if (0 != m_ButtonState[i])
			{
				ZeroMemory(m_ButtonState, BUTTON_COUNT * sizeof(int));
				DrawButton(TRUE);
				break;
			}
		}

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinFrame::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		m_CurPushButton = -1;
		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			if (0 != m_ButtonState[i])
			{
				ZeroMemory(m_ButtonState, BUTTON_COUNT * sizeof(int));
				DrawButton(TRUE);
				break;
			}
		}

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinFrame::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		
		LPMINMAXINFO pmi = reinterpret_cast<LPMINMAXINFO>(lParam);
		if (m_nMinWidth > 0) pmi->ptMinTrackSize.x = m_nMinWidth;
		if (m_nMinHeight > 0) pmi->ptMinTrackSize.y = m_nMinHeight;

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}
}