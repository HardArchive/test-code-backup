// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E2B26DD7_886E_11D1_9BF5_00A0243D1382__INCLUDED_)
#define AFX_MAINFRM_H__E2B26DD7_886E_11D1_9BF5_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef USING_NONMFC_TRAY  // This is set in project settings
#include "SystemTraySDK.h"
#else
#include "SystemTray.h"
#endif

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CSystemTray m_TrayIcon;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnAnimate();
	afx_msg void OnShowIcon();
	afx_msg void OnHideIcon();
	afx_msg void OnMoveToRight();
	afx_msg void OnShowBalloon();
	afx_msg void OnUpdateHideIcon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowBalloon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveToRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowIcon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetFocus(CCmdUI* pCmdUI);
	afx_msg void OnSetFocusToIcon();
	afx_msg void OnClose();
	afx_msg void OnDemoMaximise();
	afx_msg void OnUpdateDemoMaximise(CCmdUI* pCmdUI);
	afx_msg void OnDemoMinimise();
	afx_msg void OnUpdateDemoMinimise(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E2B26DD7_886E_11D1_9BF5_00A0243D1382__INCLUDED_)
