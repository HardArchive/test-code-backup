// NetServerDlg.h : header file
//

#if !defined(AFX_NETSERVERDLG_H__INCLUDED_)
#define AFX_NETSERVERDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDE.h"

/////////////////////////////////////////////////////////////////////////////
// CNetServerDlg dialog

class CNetServerDlg : public CDialog
{
// Construction
public:
	CNetServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNetServerDlg)
	enum { IDD = IDD_NETSERVER_DIALOG };
	CString	m_strPipe;
	CString	m_strSockets;
	CString	m_strMailSlots;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNetServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETSERVERDLG_H__INCLUDED_)
