// 事件选择服务端Dlg.h : header file
//

#if !defined(AFX_DLG_H__D0C39B79_C169_49FF_9B38_A3AE600F1DB1__INCLUDED_)
#define AFX_DLG_H__D0C39B79_C169_49FF_9B38_A3AE600F1DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>  //需要在不带MFC的socket支持才行。否则可能出错
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ThreadFunc(LPVOID pParam);//开始线程
DWORD WINAPI WorkerThread(LPVOID lpParameter);//工作者线程
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
	BOOL InItSocket();
	SOCKET sSever,sCilent; //套接字
	int    TotalConn ; //数量 主要用于以下数组下标
	struct Clientinfo  //记录套接字和IP的结构
	{
		SOCKET clientsocket;		
		CString clientIP;
	};
    Clientinfo CliSocketArr[MAXIMUM_WAIT_OBJECTS]; //套接字数组
    WSAEVENT clientevent[MAXIMUM_WAIT_OBJECTS];//事件对象数组 需要一个单独的数组
	void Cleanup(int index);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__D0C39B79_C169_49FF_9B38_A3AE600F1DB1__INCLUDED_)
