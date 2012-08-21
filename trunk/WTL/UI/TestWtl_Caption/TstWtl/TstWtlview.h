// TstWtlView.h : interface of the CTstWtlView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSTWTLVIEW_H__AC2B0A05_7F9C_4E32_99F8_A7B1A25AC689__INCLUDED_)
#define AFX_TSTWTLVIEW_H__AC2B0A05_7F9C_4E32_99F8_A7B1A25AC689__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTstWtlView : public CWindowImpl<CTstWtlView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CTstWtlView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
// 		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);

		//TODO: Add your drawing code here
		RECT rcWindow,rcDest;
		HBRUSH hBrush;
		GetWindowRect(&rcWindow);
		rcDest.left = 0;
		rcDest.top = 0;
		rcDest.right = rcWindow.right - rcWindow.left;
		rcDest.bottom = rcWindow.bottom - rcWindow.top;
		
		hBrush = CreateSolidBrush(RGB(128,128,0));
		FillRect(dc.m_hDC,&rcDest,hBrush);

		return 0;
	}
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		return 0;
	}
// 	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// 	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
// 		HWND hTab = CreateWindow(_T("SysTabControl32"),_T(""),WS_CHILD|WS_VISIBLE,10,10,200,150,m_hWnd,0,0,0);
// 		TCITEM tabCtrlItem;
// 		int nItem = 1;
// 		::SendMessage(hTab, TCM_INSERTITEM, nItem, (LPARAM)&tabCtrlItem);
// 		nItem = 2;
// 		::SendMessage(hTab, TCM_INSERTITEM, nItem, (LPARAM)&tabCtrlItem);
// 		return 0;
// 	}
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSTWTLVIEW_H__AC2B0A05_7F9C_4E32_99F8_A7B1A25AC689__INCLUDED_)
