// 完成端口服务端Dlg.h : header file
//

#if !defined(AFX_DLG_H__13B5A351_2F80_4836_8BF5_4B9A5E5BAAF0__INCLUDED_)
#define AFX_DLG_H__13B5A351_2F80_4836_8BF5_4B9A5E5BAAF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include <winsock2.h>  //需要在不带MFC的socket支持才行。否则可能出错
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ThreadFunc(LPVOID pParam);//开始线程
DWORD WINAPI WorkerThread(LPVOID CompletionPortID);//工作者线程

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class CMyDlg : public CDialog
{
// Construction
public:
	CMyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL InItSocket();//装载套接字库
	SOCKET sSever,sClient; //套接字
	struct Cilentinfo
	{
		SOCKET cinsock;
		CString cinIP;
	}cilentin;
	CArray <Cilentinfo,Cilentinfo&>m_Cilentinfo;//动态数组客户套接字及IP
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__13B5A351_2F80_4836_8BF5_4B9A5E5BAAF0__INCLUDED_)
