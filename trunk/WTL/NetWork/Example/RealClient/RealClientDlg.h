// RealClientDlg.h : header file
//

#if !defined(AFX_REALCLIENTDLG_H__E48CA8F6_C846_4A77_BEC0_7E9125E7E15C__INCLUDED_)
#define AFX_REALCLIENTDLG_H__E48CA8F6_C846_4A77_BEC0_7E9125E7E15C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRealClientDlg dialog

class CRealClientDlg : public CDialog
{
// Construction
public:
	static DWORD WINAPI ThreadRecv(LPVOID lpParameter);
	void SetDlgText(CString strText);
	SOCKET m_ConnectSock;
	CRealClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRealClientDlg)
	enum { IDD = IDD_REALCLIENT_DIALOG };
	CIPAddressCtrl	m_Ip;
	UINT	m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRealClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSend();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnDisconnect();
	afx_msg void OnBtnClear();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnSub();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALCLIENTDLG_H__E48CA8F6_C846_4A77_BEC0_7E9125E7E15C__INCLUDED_)
