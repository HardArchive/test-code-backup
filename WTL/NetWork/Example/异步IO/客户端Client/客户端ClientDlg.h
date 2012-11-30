// 客户端ClientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__CBA32864_D5AF_4D91_816B_FEE3AF004F8E__INCLUDED_)
#define AFX_CLIENTDLG_H__CBA32864_D5AF_4D91_816B_FEE3AF004F8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DWORD WINAPI ConnectThreadFunc(LPVOID pParam);//线程
/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	HANDLE m_hConnectThread;//连接线程句柄
	SOCKET sClient; //客户端的套接字
	BOOL InItSocket();//装载套接字库
	BOOL Socket_Select(SOCKET hSocket);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__CBA32864_D5AF_4D91_816B_FEE3AF004F8E__INCLUDED_)
