#include "StdAfx.h"
#include "SkinMgr.h"
#include "SkinRes.h"

#define MAX_TABITEM	15	// 标签项最大的数目

#define TABITEM_WIDTH	64
#define TABITEM_HEIGHT	59
#define TABITEM_DISTANCE	3

namespace SkinMgr
{
	CSkinTabCtrl::CSkinTabCtrl(void)
	{
		ASSERT(MAX_TABITEM > 0);
		m_ItemCount = 0;
		m_pItems = new SKINTABITEM[MAX_TABITEM];
		if (!m_pItems)
			ATLTRACE("内存不足！\n");
		ZeroMemory(m_pItems, sizeof(SKINTABITEM) * MAX_TABITEM);

		m_CurSelected = -1;
		m_CurHorver = -1;

		// 创建字体
		LOGFONT lf;
		GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("Microsoft YaHei"));
		lf.lfHeight = -12;
		lf.lfWeight &= ~FW_BOLD;
		m_hFont = CreateFontIndirect(&lf);

	}

	CSkinTabCtrl::~CSkinTabCtrl(void)
	{
		for (UINT i = 0; i < m_ItemCount; i++)
		{
			if (m_pItems[i].lpszCaption)
			{
				free( (LPVOID)m_pItems[i].lpszCaption );
				m_pItems[i].lpszCaption = NULL;
			}
		}

		delete []m_pItems;
		m_pItems = NULL;
		m_ItemCount = 0;

		if (m_hFont)
			::DeleteObject((HGDIOBJ)m_hFont);
		m_hFont = NULL;
	}

	BOOL CSkinTabCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpWindowName, 
				DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, UINT nID)
	{
		ASSERT(lpszClassName != NULL);
		
		if ( !PreRegisterClass(lpszClassName) ) return FALSE;
		
		HWND hWnd = ::CreateWindow(lpszClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)(ULONG_PTR)nID, theInstance, NULL);
		if ( !hWnd ) return FALSE;

		CSkinWnd::Attach(hWnd);

		RECT rcClient;
		GetClientRect(m_hWnd, &rcClient);
		int ccx = rcClient.right - rcClient.left;
		int ccy = rcClient.bottom - rcClient.top;

		m_MemDC.CreateMemDC(ccx, ccy);
		DrawSkin(TRUE);

		return TRUE;
	}
	
	BOOL CSkinTabCtrl::AddItem(LPCTSTR lpszCaption, HICON hIcon/* = 0*/)
	{
		ASSERT(m_hWnd != NULL && m_ItemCount < MAX_TABITEM - 1);
		size_t nLength;

		if ( lpszCaption && ( nLength = _tcslen(lpszCaption) ) )
		{
			m_pItems[m_ItemCount].lpszCaption = (LPTSTR)malloc( (nLength + 1) * sizeof(TCHAR) );
			_tcscpy_s( m_pItems[m_ItemCount].lpszCaption, (nLength + 1), lpszCaption );
		}
		m_pItems[m_ItemCount].hIcon = hIcon;
		int nLeft = m_ItemCount * (TABITEM_WIDTH + TABITEM_DISTANCE);
		SetRect(&m_pItems[m_ItemCount].rc, nLeft, 0, nLeft + TABITEM_WIDTH, TABITEM_HEIGHT);

		m_ItemCount++;
		if ( -1 == m_CurSelected )
			SetSelected(0);
		else
			DrawSkin(TRUE);
		return TRUE;
	}

	int CSkinTabCtrl::SetSelected(int Index)
	{
		ASSERT(m_hWnd != NULL && m_ItemCount < MAX_TABITEM - 1);
		ASSERT(Index < (int)m_ItemCount);

		int nIndex = m_CurSelected;
		m_CurSelected = Index;
		DrawSkin(TRUE);

		SendNotifyMessage( SKINMGR_TCN_SELCHANGE, m_CurSelected );

		return nIndex;
	}

	BOOL CSkinTabCtrl::DrawSkin(BOOL bUpdate/* = FALSE*/)
	{
		if (NULL == m_MemDC.m_hDC) return FALSE;

		int cx = m_MemDC.GetWidth();
		int cy = m_MemDC.GetHeight();

		HFONT hOldFont = m_MemDC.SelectFont(m_hFont);
		m_MemDC.SetStretchBltMode(HALFTONE);
		m_MemDC.SetBkMode(TRANSPARENT);
		m_MemDC.SetTextColor(RGB(255, 255, 255));

		m_MemDC.StretchBlt(0, 0, cx, cy, g_ResDC.m_hDC, 3, 24, 2, 59, SRCCOPY);

		for (UINT i = 0; i < m_ItemCount; i++)
		{
			RECT rcText;
			SetRect(&rcText, m_pItems[i].rc.left + 2, TABITEM_HEIGHT - 20, m_pItems[i].rc.left + TABITEM_WIDTH - 2, TABITEM_HEIGHT - 2);

			int nSrcX = 8;

			if (m_CurSelected == i)
				nSrcX = 136;
			else if (m_CurHorver == i) 
				nSrcX = 72; 

			m_MemDC.BitBlt(m_pItems[i].rc.left, 0, 
				(m_pItems[i].rc.right - m_pItems[i].rc.left), 
				(m_pItems[i].rc.bottom - m_pItems[i].rc.top), 
				g_ResDC.m_hDC, nSrcX, 69, SRCCOPY);


			if (m_pItems[i].hIcon)
			{
				// 计算居中绘制图标的xy坐标， 图标大小为32 x 32
				int x = m_pItems[i].rc.left + (TABITEM_WIDTH - 32) / 2;
				// y坐标减去20是预留20像素绘制文本
				int y = m_pItems[i].rc.top + (TABITEM_HEIGHT - 20 - 32) / 2;
				::DrawIconEx(m_MemDC, x, y, m_pItems[i].hIcon, 32, 32, 0, 0, DI_NORMAL);
			}

			// 绘制标题文本
			m_MemDC.DrawText(m_pItems[i].lpszCaption,(int)_tcslen(m_pItems[i].lpszCaption), 
							&rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		m_MemDC.SelectFont(hOldFont);
		if ( bUpdate && ::IsWindow(m_hWnd) )
		{
			CClientDC dc(m_hWnd);
			dc.BitBlt(0, 0,cx, cy, m_MemDC.m_hDC, 0, 0, SRCCOPY);
		}

		return TRUE;
	}

	int CSkinTabCtrl::HitTest(int x, int y)
	{
		// 这个函数检测坐标下的标签索引， 如果不在任何一个标签， 则返回-1
		POINT pt = {x, y};

		for (UINT i = 0; i < m_ItemCount; i++)
		{
			if ( PtInRect(&m_pItems[i].rc, pt) )
				return i;
		}
		return -1;
	}

	LRESULT CSkinTabCtrl::SendNotifyMessage(UINT code, int Index)
	{
		SKINTABNOTIFY stNotify = {0};
		HWND hWndParent = ::GetParent(m_hWnd);
		int nID = ::GetDlgCtrlID(m_hWnd);
		stNotify.hdr.hwndFrom = m_hWnd;
		stNotify.hdr.idFrom = nID;
		stNotify.hdr.code = code;
		stNotify.Index = Index;
		return ::SendMessage(hWndParent, WM_NOTIFY, (WPARAM)nID, (LPARAM)&stNotify);
	}

	BOOL CSkinTabCtrl::PreRegisterClass(LPCTSTR lpszClassName)
	{
		// 创建窗口前， 先注册窗口类
		ASSERT(lpszClassName != NULL);
		ASSERT(theInstance != NULL);
		WNDCLASS wc = {0};

		if ( GetClassInfo( NULL, lpszClassName, &wc ) || GetClassInfo( theInstance, lpszClassName, &wc ) )
			return TRUE;
		
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hInstance = theInstance;
		wc.lpfnWndProc = ::DefWindowProc;
		wc.lpszClassName = lpszClassName;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

		if ( !RegisterClass(&wc) )
			return FALSE;
		
		return TRUE;
	}

	/*virtual*/ LRESULT CSkinTabCtrl::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SKINMGR_MSG_HANDLER(WM_ERASEBKGND, OnEraseBkgnd);
		SKINMGR_MSG_HANDLER(WM_PAINT, OnPaint);
		SKINMGR_MSG_HANDLER(WM_SIZE, OnSize);
		SKINMGR_MSG_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk);
		SKINMGR_MSG_HANDLER(WM_LBUTTONDOWN, OnLButtonDown);
		SKINMGR_MSG_HANDLER(WM_MOUSEMOVE, OnMouseMove);
		SKINMGR_MSG_HANDLER(WM_MOUSELEAVE, OnMouseMoveLeave);

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinTabCtrl::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		return TRUE;
	}

	LRESULT CSkinTabCtrl::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		::BitBlt(ps.hdc, 0, 0, m_MemDC.GetWidth(), m_MemDC.GetHeight(), m_MemDC.m_hDC, 0, 0, SRCCOPY);
		EndPaint(m_hWnd, &ps);

		return TRUE;
	}

	LRESULT CSkinTabCtrl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RECT rcClient;
		GetClientRect(m_hWnd, &rcClient);
		int ccx = rcClient.right - rcClient.left;
		int ccy = rcClient.bottom - rcClient.top;

		m_MemDC.CreateMemDC(ccx, ccy);
		DrawSkin();

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinTabCtrl::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SendNotifyMessage( SKINMGR_TCN_LBUTTONDBLCLK, HitTest(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) );
		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinTabCtrl::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int nHitTest = HitTest( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (nHitTest != -1 && m_CurSelected != nHitTest) 
		{
			SetSelected(nHitTest);
		}
		SendNotifyMessage( SKINMGR_TCN_LBUTTONDOWN, nHitTest );
		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinTabCtrl::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if ( (wParam & MK_LBUTTON) == MK_LBUTTON || (wParam & MK_RBUTTON) == MK_RBUTTON ) return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);

		TRACKMOUSEEVENT tm = {
			sizeof(TRACKMOUSEEVENT), 
			TME_LEAVE, 
			m_hWnd, 
			0
		};
		_TrackMouseEvent(&tm);

		int nHitTest = HitTest( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (m_CurHorver != nHitTest)
		{
			m_CurHorver = nHitTest;
			DrawSkin(TRUE);
		}

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CSkinTabCtrl::OnMouseMoveLeave(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (-1 != m_CurHorver)
		{
			m_CurHorver = -1;
			DrawSkin(TRUE);
		}

		return CSkinWnd::CallWindowProc(uMsg, wParam, lParam);
	}
}