// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__9CFBDBF9_1383_4D6E_9317_43808200282B__INCLUDED_)
#define AFX_MAINFRM_H__9CFBDBF9_1383_4D6E_9317_43808200282B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// #include <string>

#define EDGE_WINDOW											4
#define HEIGHT_TITLE										GetSystemMetrics(SM_CYCAPTION)		// maybe 19

#define WM_NCUAHDRAWCAPTION									0x00AE
#define WM_NCUAHDRAWFRAME									0x00AF
#define WM_MSGUNKNOWN										0xC2ED

#define SPACE_WINCTRL_TBORDER								3
#define SPACE_WINCTRL_RBORDER								7
#define WIDTH_WINCTRL_CLOSE									44
#define WIDTH_WINCTRL_RESTORE								28
#define WIDTH_WINCTRL_MINIMIZE								27
#define HEIGHT_WINCTRL										19
#define HEIGHT_CAPTION										25


enum EWinCtrl{
	winctrl_none,
	winctrl_close,
	winctrl_restore,
	winctrl_maximize,
	winctrl_minimize,
	winctrl_help
};

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CTstWtlView m_view;

	CCommandBarCtrl m_CmdBar;

	BOOL m_bActive;
	BOOL m_bOnPressed;
	LRESULT m_nButtonPress;
	LRESULT m_nButtonHover;
	// EWinCtrl m_eWinCtrlPushed;
	
	RECT m_rcWinCtrlMinimize;
	RECT m_rcWinCtrlRestoreMaximize;
	RECT m_rcWinCtrlClose;
	RECT m_rcWinCtrlHelp;

	CMainFrame()
	{
		m_bActive = TRUE;
		m_bOnPressed = FALSE;
		m_nButtonPress = HTNOWHERE;
		m_nButtonHover = HTNOWHERE;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (WM_NCUAHDRAWCAPTION == pMsg->message /*|| WM_NCUAHDRAWFRAME == pMsg->message*/)
		{
			pMsg->message = WM_NCPAINT;
			pMsg->wParam = 0;
			pMsg->lParam = 0;
		}

		if (WM_CREATE == pMsg->message /*|| WM_NCUAHDRAWFRAME == pMsg->message*/)
		{
			int n = 5;
			n++;
		}

// 		std::string str = "PreTranslateMessage\r\n";
// 		ATLTRACE(str.c_str());

		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		MESSAGE_HANDLER(WM_ACTIVATEAPP, OnActivateApp)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
		MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNcLButtonDown)
		MESSAGE_HANDLER(WM_NCLBUTTONUP, OnNcLButtonUp)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
		MESSAGE_HANDLER(WM_NCCALCSIZE, OnNcCalcSize)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_NCUAHDRAWCAPTION,OnNcDrawCaption)
		MESSAGE_HANDLER(WM_NCUAHDRAWFRAME,OnNcDrawFrame)
		MESSAGE_HANDLER(WM_MSGUNKNOWN,,OnMsgUnknown)
		MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_NCMOUSEMOVE, OnNcMouseMove)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_SETTEXT,OnNcPaint)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// m_bActive = TRUE;
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();

		m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0/*WS_EX_CLIENTEDGE*/);

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 0);
		UISetCheck(ID_VIEW_STATUS_BAR, 0);
		::ShowWindow(m_hWndToolBar, SW_HIDE);
		::ShowWindow(m_hWndStatusBar, SW_HIDE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		int nHeightTitle = GetSystemMetrics(SM_CYCAPTION);

		CalcSizeWinCtrl();

		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
	LRESULT OnActivateApp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
// 		m_bActive = wParam;
//  		DrawNC();

 		bHandled = FALSE;

		return 0;
	}
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		LRESULT lRes = DefWindowProc(uMsg,wParam,lParam);
		m_bActive = wParam;
		DrawNC();

		bHandled = TRUE;

		ATLTRACE("---OnNcActivate--- : %d \r\n",m_bActive);

		return lRes;
	}

	LRESULT OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		RECT rcWindow;
		GetWindowRect(&rcWindow);
		POINT pt = { GET_X_LPARAM(lParam)-rcWindow.left, GET_Y_LPARAM(lParam)-rcWindow.top };
		m_nButtonPress = HTNOWHERE;
		
		switch( wParam ) 
		{
			case HTMINBUTTON:
			case HTMAXBUTTON:
			case HTCLOSE:
			case HTHELP:
// 				m_nButtonPress  = wParam;
// 				m_bOnPressed    = TRUE;
// 				bHandled = TRUE;	// 表示已经处理过了，否则系统会重画按钮且 WM_NC_LBUTTONUP 不会被触发。
// 				/*pT->*//*DrawCaption()*/DrawNC();
				// wParam = HTCAPTION;
				bHandled = TRUE;	// 表示已经处理过了，否则系统会重画按钮且 WM_NC_LBUTTONUP 不会被触发。
				// DrawNC();	// Only need to redraw caption bar
				break;
			case HTCAPTION:
				bHandled = FALSE;    // 继续让系统进行缺省处理
				{
				DWORD nStyle = /*pT->*/GetStyle();
				if(nStyle & WS_MINIMIZE)
				{
					return 0; // BUG: Prevents move of iconic window, but fixes Windows freeze
				}
				else
				if(nStyle & WS_MAXIMIZE)
				{
					// 在系统最大化的时候需要在鼠标按下时重画标题栏
					bHandled = TRUE; // 表示已经处理过了，否则系统会重画按钮。
					// 让系统对 HTCAPTION 进行处理
					//::LockWindowUpdate(m_hWnd);
					/*pT->*/DefWindowProc(uMsg, wParam, lParam);
					//::LockWindowUpdate(NULL);
					/*pT->*//*ReDrawCaption()*/DrawNC();
				}
			}
			break;
		default:
			bHandled = FALSE;    // 继续让系统进行缺省处理
			break;
		}

		if (PtInRect(&m_rcWinCtrlClose,pt))
		{
			m_nButtonPress = HTCLOSE;
			m_bOnPressed = TRUE;
			bHandled = TRUE;	// 表示已经处理过了，否则系统会重画按钮且 WM_NC_LBUTTONUP 不会被触发。
			DrawNC();
		}

		if (PtInRect(&m_rcWinCtrlRestoreMaximize,pt))
		{
			m_nButtonPress = HTMAXBUTTON;
			m_bOnPressed = TRUE;
			bHandled = TRUE;	// 表示已经处理过了，否则系统会重画按钮且 WM_NC_LBUTTONUP 不会被触发。
			DrawNC();
		}
		if (PtInRect(&m_rcWinCtrlMinimize,pt))
		{
			m_nButtonPress = HTMINBUTTON;
			m_bOnPressed = TRUE;
			bHandled = TRUE;	// 表示已经处理过了，否则系统会重画按钮且 WM_NC_LBUTTONUP 不会被触发。
			DrawNC();
		}
		if (PtInRect(&m_rcWinCtrlHelp,pt))
		{
			m_nButtonPress = HTHELP;
			m_bOnPressed = TRUE;
			bHandled = TRUE;	// 表示已经处理过了，否则系统会重画按钮且 WM_NC_LBUTTONUP 不会被触发。
			DrawNC();
		}

		return 0;
	}

	LRESULT OnNcLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		m_bOnPressed = FALSE;

		if((LRESULT)wParam == m_nButtonPress)
		{
			// 鼠标按下和释放在相同的按钮上
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			switch( wParam ) 
			{
				case HTMINBUTTON:
					PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(pt.x, pt.y));
					bHandled = TRUE;    // 禁止系统做缺省处理
					break;
				case HTMAXBUTTON:
					// m_nButtonMove = HTNOWHERE;        // 最大画后鼠标位置将改变
					if(GetStyle() & WS_MAXIMIZE)
						PostMessage(WM_SYSCOMMAND, SC_RESTORE,  MAKELPARAM(pt.x, pt.y));
					else
						PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(pt.x, pt.y));
					bHandled = TRUE;    // 禁止系统做缺省处理
					break;
				case HTCLOSE:
					PostMessage(WM_CLOSE);
					bHandled = TRUE;    // 禁止系统做缺省处理
					break;
				case HTHELP:
					PostMessage(WM_HELP);
					bHandled = TRUE;    // 禁止系统做缺省处理
					break;
				case HTSYSMENU:
					default:
					bHandled = FALSE;    // 让系统进行缺省处理
			}
		}

		m_nButtonPress = HTNOWHERE;

		// bHandled = FALSE;
		DrawNC();
		return 0;
	}

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.

		DrawNC();

		return 0;
	}

	void DrawWinCtrl(HDC hDC)
	{
		HBITMAP hBmp,hOldBmp;
		HDC hMemDC;
		hBmp = ::LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(IDB_WINCTRLS));

		if (NULL == hBmp)
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(0,128,128));
			FillRect(hDC,&m_rcWinCtrlClose,hBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(128,0,128));
			FillRect(hDC,&m_rcWinCtrlRestoreMaximize,hBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(128,128,0));
			FillRect(hDC,&m_rcWinCtrlMinimize,hBrush);
			DeleteObject(hBrush);
			return;
		}

		hMemDC = CreateCompatibleDC(hDC);
		hOldBmp = (HBITMAP)SelectObject(hMemDC,hBmp);

		int nLeft, nTop, nWidth, nHeight, nXSrc, nYSrc;

		nYSrc = 0;
		if(!m_bActive)
			nYSrc = 4*HEIGHT_WINCTRL;

		// Close
		nLeft = m_rcWinCtrlClose.left;
		nTop = m_rcWinCtrlClose.top;
		nWidth = m_rcWinCtrlClose.right - m_rcWinCtrlClose.left;
		nHeight = m_rcWinCtrlClose.bottom - m_rcWinCtrlClose.top;
		nXSrc = WIDTH_WINCTRL_MINIMIZE+2*WIDTH_WINCTRL_RESTORE;
		if (HTCLOSE == m_nButtonHover)
			nYSrc = HEIGHT_WINCTRL;
		if (HTCLOSE == m_nButtonPress)
			nYSrc = 2*HEIGHT_WINCTRL;
		BitBlt(hDC,nLeft, nTop, nWidth, nHeight, hMemDC, nXSrc, nYSrc, SRCCOPY);
		nYSrc = 0;
		if(!m_bActive)
			nYSrc = 4*HEIGHT_WINCTRL;

		// Restore or Maximize
		nLeft = m_rcWinCtrlRestoreMaximize.left;
		nTop = m_rcWinCtrlRestoreMaximize.top;
		nWidth = m_rcWinCtrlRestoreMaximize.right - m_rcWinCtrlRestoreMaximize.left;
		nHeight = m_rcWinCtrlRestoreMaximize.bottom - m_rcWinCtrlRestoreMaximize.top;
		nXSrc = WIDTH_WINCTRL_MINIMIZE;
		if (HTMAXBUTTON == m_nButtonHover)
			nYSrc = HEIGHT_WINCTRL;
		if (HTMAXBUTTON == m_nButtonPress)
			nYSrc = 2*HEIGHT_WINCTRL;
		BitBlt(hDC,nLeft, nTop, nWidth, nHeight, hMemDC, nXSrc, nYSrc, SRCCOPY);
		nYSrc = 0;
		if(!m_bActive)
			nYSrc = 4*HEIGHT_WINCTRL;

		// Minimize
		nLeft = m_rcWinCtrlMinimize.left;
		nTop = m_rcWinCtrlMinimize.top;
		nWidth = m_rcWinCtrlMinimize.right - m_rcWinCtrlMinimize.left;
		nHeight = m_rcWinCtrlMinimize.bottom - m_rcWinCtrlMinimize.top;
		nXSrc = 0;
		if (HTMINBUTTON == m_nButtonHover)
			nYSrc = HEIGHT_WINCTRL;
		if (HTMINBUTTON == m_nButtonPress)
			nYSrc = 2*HEIGHT_WINCTRL;
		BitBlt(hDC,nLeft, nTop, nWidth, nHeight, hMemDC, nXSrc, nYSrc, SRCCOPY);
		nYSrc = 0;
		if(!m_bActive)
			nYSrc = 4*HEIGHT_WINCTRL;

// 		nLeft = m_rcWinCtrlHelp.left;
// 		nTop = m_rcWinCtrlHelp.top;
// 		nWidth = m_rcWinCtrlHelp.right - m_rcWinCtrlHelp.left;
// 		nHeight = m_rcWinCtrlHelp.bottom - m_rcWinCtrlHelp.top;
// 		nXSrc = 0;
//		if (HTHELP == m_nButtonPress)
//			nYSrc = 2*HEIGHT_WINCTRL;
// 		BitBlt(hDC,nLeft, nTop, nWidth, nHeight, hMemDC, nXSrc, nYSrc, SRCCOPY);

		// Release GDI Object
		SelectObject(hMemDC,hOldBmp);
		DeleteObject(hBmp);
		DeleteDC(hMemDC);
	}

	void DrawCaption(HDC hDC)
	{
		RECT rcWindow,rcDest,rcTitle;
		HBRUSH hBrush;
		
		GetWindowRect(&rcWindow);
		rcDest.left = 0;
		rcDest.top = 0;
		rcDest.right = rcWindow.right - rcWindow.left;
		rcDest.bottom = rcWindow.bottom - rcWindow.top;

		// Fill the caption bar
		COLORREF clrTitle = RGB(0,255,0);
		if (!m_bActive)
		{
			clrTitle = RGB(0,255,255);
		}
		rcTitle.left = EDGE_WINDOW;
		rcTitle.top = EDGE_WINDOW;
		rcTitle.right = rcDest.right - EDGE_WINDOW;
		rcTitle.bottom = EDGE_WINDOW + HEIGHT_TITLE;
		hBrush = CreateSolidBrush(clrTitle);
		FillRect(hDC,&rcTitle,hBrush);
		// Release GDI object
		DeleteObject(hBrush);
		
		// Draw the splitter line
		HPEN hPen,hOldPen;
		POINT point;
		hPen = CreatePen(PS_SOLID,1,RGB(128,0,0));
		hOldPen = (HPEN)SelectObject(hDC,hPen);
		MoveToEx(hDC,EDGE_WINDOW,EDGE_WINDOW+HEIGHT_TITLE-1,&point);
		LineTo(hDC,rcDest.right-EDGE_WINDOW,EDGE_WINDOW+HEIGHT_TITLE-1);

		DrawWinCtrl(hDC);

		// Release GDI object
		SelectObject(hDC,hOldPen);
		DeleteObject(hPen);
	}

	void DrawFrame(HDC hDC)
	{
		RECT rcWindow,rcDest;
		HBRUSH hBrush;
		
		GetWindowRect(&rcWindow);
		rcDest.left = 0;
		rcDest.top = 0;
		rcDest.right = rcWindow.right - rcWindow.left;
		rcDest.bottom = rcWindow.bottom - rcWindow.top;

		// Fill window's four edges area
		COLORREF clrNc = RGB(0,128,0);
		if (!m_bActive)
		{
			clrNc = RGB(128,0,128);
		}

		hBrush = CreateSolidBrush(clrNc);

		BOOL bTstFillSpeed0 = FALSE;
		if (bTstFillSpeed0)
		{
			FillRect(hDC,&rcDest,hBrush);
		}
		else
		{
			// Only fill the frame area
			RECT rcClient,rcClip, rcLeft, rcTop,rcRight,rcBottom;

			BOOL bTstFillSpeed1 = TRUE;
			if (bTstFillSpeed1)
			{
				GetClientRect(&rcClient);
				rcClip.left = EDGE_WINDOW;
				rcClip.top = HEIGHT_TITLE+EDGE_WINDOW;
				rcClip.right = rcClient.right - rcClient.left + rcClip.left;
				rcClip.bottom = rcClient.bottom - rcClient.top + rcClip.top;

				ExcludeClipRect(hDC,rcClip.left,rcClip.top,rcClip.right,rcClip.bottom);
				FillRect(hDC,&rcDest,hBrush);
			}
			else
			{
				FillRect(hDC,&rcLeft,hBrush);
				FillRect(hDC,&rcTop,hBrush);
				FillRect(hDC,&rcRight,hBrush);
				FillRect(hDC,&rcBottom,hBrush);
			}

		}

		// Release GDI object
		DeleteObject(hBrush);
	}

	void DrawNC()
	{
		HDC hWindowDC = GetWindowDC();
		if (NULL == hWindowDC)
			return;

		RECT rcWindow,rcDest;
		HDC hMemDC;
		HBITMAP hBmp,hOldBmp;
		RECT rcClient,rcClip;
		int nWidth,nHeight;

		GetWindowRect(&rcWindow);
		rcDest.left = 0;
		rcDest.top = 0;
		rcDest.right = rcWindow.right - rcWindow.left;
		rcDest.bottom = rcWindow.bottom - rcWindow.top;
		nWidth = rcWindow.right - rcWindow.left;
		nHeight = rcWindow.bottom - rcWindow.top;

		GetClientRect(&rcClient);
		rcClip.left = EDGE_WINDOW;
		rcClip.top = HEIGHT_TITLE+EDGE_WINDOW;
		rcClip.right = rcClient.right - rcClient.left + rcClip.left;
		rcClip.bottom = rcClient.bottom - rcClient.top + rcClip.top;
		
		// Prepare memory dc
		hMemDC = CreateCompatibleDC(hWindowDC);
		hBmp = CreateCompatibleBitmap(hWindowDC,nWidth,nHeight);
		hOldBmp = (HBITMAP)SelectObject(hMemDC,hBmp);

		DrawFrame(hMemDC);
		DrawCaption(hMemDC);

		// From memory dc bitblt out nc area to vram
		ExcludeClipRect(hWindowDC,rcClip.left,rcClip.top,rcClip.right,rcClip.bottom);
		BitBlt(hWindowDC,0,0,nWidth,nHeight,hMemDC,0,0,SRCCOPY);
		
		// Release GDI object
		ReleaseDC(hWindowDC);
		
		SelectObject(hMemDC,hOldBmp);
		DeleteObject(hBmp);
		DeleteDC(hMemDC);
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		bHandled = FALSE;
		CalcSizeWinCtrl();
// 		DrawNC();
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.

		bHandled = TRUE;
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam); 

		if (HTCLOSE == lRet || HTHELP == lRet || HTMINBUTTON == lRet || HTMAXBUTTON == lRet || HTSYSMENU == lRet)
		{
			lRet = HTCAPTION;
		}

		RECT rcWindow;
		GetWindowRect(&rcWindow);
		POINT pt = { GET_X_LPARAM(lParam)-rcWindow.left, GET_Y_LPARAM(lParam)-rcWindow.top };

		if (PtInRect(&m_rcWinCtrlClose,pt))
		{
			lRet = HTCLOSE;
		}
		else
		if (PtInRect(&m_rcWinCtrlRestoreMaximize,pt))
		{
			lRet = HTMAXBUTTON;
		}
		else
		if (PtInRect(&m_rcWinCtrlMinimize,pt))
		{
			lRet = HTMINBUTTON;
		}
		else
		if (PtInRect(&m_rcWinCtrlHelp,pt))
		{
			lRet = HTHELP;
		}

		return lRet;
	}

	LRESULT OnNcDrawCaption(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// uMsg = WM_NCPAINT; wParam = 0; lParam = 0;
		DrawNC();
		bHandled = TRUE;
		return 0;
	}
	LRESULT OnNcDrawFrame(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// uMsg = WM_NCPAINT; wParam = 0; lParam = 0;
		return 0;
	}
	
	LRESULT OnMsgUnknown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// uMsg = WM_NCPAINT; wParam = 0; lParam = 0;
		DrawNC();
		return 0;
	}
	LRESULT OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		BOOL bSysMenu = HIWORD(lParam);
		if (bSysMenu)
			DrawNC();

		return 0;
	}
	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
// 		m_bActive = TRUE;
// 		if (WA_INACTIVE == wParam)
// 			m_bActive = FALSE;
// 
// 		DrawNC();

		bHandled = FALSE;

		return 0;
	}
	LRESULT OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.

		RECT rcWindow;
		GetWindowRect(&rcWindow);
		POINT pt = { GET_X_LPARAM(lParam)-rcWindow.left, GET_Y_LPARAM(lParam)-rcWindow.top };
		m_nButtonHover = HTNOWHERE;

		if(PtInRect(&m_rcWinCtrlClose,pt))
		{
			m_nButtonHover = HTCLOSE;
		}
		else
		if(PtInRect(&m_rcWinCtrlRestoreMaximize,pt))
		{
			m_nButtonHover = HTMAXBUTTON;
		}
		else
		if(PtInRect(&m_rcWinCtrlMinimize,pt))
		{
			m_nButtonHover = HTMINBUTTON;
		}

		bHandled = TRUE;
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);

		DrawNC();

		return lRes;
	}
	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		bHandled = FALSE;
		return 0;
	}
	void CalcSizeWinCtrl()
	{
		RECT rcWindow,rcWinCtrl;
		GetWindowRect(&rcWindow);
		int nWidth = rcWindow.right - rcWindow.left;

		int nOffset = SPACE_WINCTRL_TBORDER;

		m_rcWinCtrlClose.left = nWidth - SPACE_WINCTRL_RBORDER - WIDTH_WINCTRL_CLOSE;
		m_rcWinCtrlClose.top = nOffset;
		m_rcWinCtrlClose.right = nWidth - SPACE_WINCTRL_RBORDER;
		m_rcWinCtrlClose.bottom = nOffset + HEIGHT_WINCTRL;

		m_rcWinCtrlRestoreMaximize.left = m_rcWinCtrlClose.left - WIDTH_WINCTRL_RESTORE;
		m_rcWinCtrlRestoreMaximize.top = nOffset;
		m_rcWinCtrlRestoreMaximize.right = m_rcWinCtrlClose.left;
		m_rcWinCtrlRestoreMaximize.bottom = nOffset + HEIGHT_WINCTRL;
		
		m_rcWinCtrlMinimize.left = m_rcWinCtrlRestoreMaximize.left - WIDTH_WINCTRL_MINIMIZE;
		m_rcWinCtrlMinimize.top = nOffset;
		m_rcWinCtrlMinimize.right = m_rcWinCtrlRestoreMaximize.left;
		m_rcWinCtrlMinimize.bottom = nOffset + HEIGHT_WINCTRL;

		
		m_rcWinCtrlHelp.left = 0;
		m_rcWinCtrlHelp.top = 0;
		m_rcWinCtrlHelp.right = 0;
		m_rcWinCtrlHelp.bottom = 0;

		rcWinCtrl.left = m_rcWinCtrlMinimize.left-3;
		rcWinCtrl.top = 0;
		rcWinCtrl.right = nWidth;
		rcWinCtrl.bottom = HEIGHT_CAPTION;

		// ::hrgnUpdate();
		RedrawWindow(&rcWinCtrl);
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9CFBDBF9_1383_4D6E_9317_43808200282B__INCLUDED_)
