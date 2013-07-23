// NetClientDlg.h : header file
//

#if !defined(AFX_NETCLIENTDLG_H__INCLUDED_)
#define AFX_NETCLIENTDLG_H__INCLUDED_

#include "dde.h"

/////////////////////////////////////////////////////////////////////////////
// CNetClientDlg dialog

class CNetClientDlg : public CDialog
{
// Construction
public:
	CNetClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNetClientDlg)
	enum { IDD = IDD_NETCLIENT_DIALOG };
	CString	m_strMailSlots;
	CString	m_strPipe;
	CString	m_strSockets;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CNetClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSendSockets();
	afx_msg void OnSendPipe();
	afx_msg void OnSendMailslots();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_NETCLIENTDLG_H__INCLUDED_)
