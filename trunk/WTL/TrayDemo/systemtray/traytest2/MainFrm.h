/////////////////////////////////////////////////////////////////////////////
// Project：TRAYTEST
// Author：Paul Dilascia
// Date：星期四, 十一月 07, 2002
// Description：一个简单的系统托盘程序例子，将API函数封装在一个C++类中，实现托盘程序的扩展。
//
/////////////////////////////////////////////////////////////////////////////
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__9ED70A69_C975_4F20_9D4E_B2877E3575D0__INCLUDED_)
#define AFX_MAINFRM_H__9ED70A69_C975_4F20_9D4E_B2877E3575D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "trayicon.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
//ZXN	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//ZXN	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

	CTrayIcon	m_trayIcon;		// my tray icon
	CEdit			m_wndEdit;		// to display tray notifications
	int			m_iWhichIcon;	// 0/1 which HICON to use
	BOOL			m_bShutdown;	// OK to terminate TRAYTEST
	BOOL			m_bShowTrayNotifications;	// display info in main window

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
	afx_msg int	 OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToggleIcon();
	afx_msg void OnViewClear();
	afx_msg void OnViewNotifications();
	afx_msg void OnUpdateViewClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewNotifications(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnAppOpen();
	afx_msg void OnAppSuspend();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9ED70A69_C975_4F20_9D4E_B2877E3575D0__INCLUDED_)
