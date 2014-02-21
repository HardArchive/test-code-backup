// accelDemoView.h : interface of the CAccelDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCELDEMOVIEW_H__DAF5654D_E249_4473_B8ED_ECA0041C14B5__INCLUDED_)
#define AFX_ACCELDEMOVIEW_H__DAF5654D_E249_4473_B8ED_ECA0041C14B5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAccelDemoView : public CWindowImpl<CAccelDemoView, CEdit>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CEdit::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CAccelDemoView)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCELDEMOVIEW_H__DAF5654D_E249_4473_B8ED_ECA0041C14B5__INCLUDED_)
