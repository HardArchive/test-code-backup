#pragma once

#include "WinNotifyDoc.h"

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI); 

	DECLARE_MESSAGE_MAP()

  CWinNotifyDoc* GetDoc();
};
