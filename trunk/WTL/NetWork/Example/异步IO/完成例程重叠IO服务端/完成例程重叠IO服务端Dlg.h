// 完成例程重叠IO服务端Dlg.h : header file
//

#if !defined(AFX_IODLG_H__6665BC4F_85C6_4DC5_A88F_AAAADD22639F__INCLUDED_)
#define AFX_IODLG_H__6665BC4F_85C6_4DC5_A88F_AAAADD22639F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_BUF_SIZE 1024

#include <afxtempl.h>

#include <winsock2.h>  //需要在不带MFC的socket支持才行。否则可能出错
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ThreadFunc(LPVOID pParam);//开始线程
DWORD WINAPI WorkerThread(LPVOID lpParameter);//工作者线程
//完成例程回调函数
void CALLBACK CompletionROUTINE(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);


/////////////////////////////////////////////////////////////////////////////
// CIODlg dialog

class CIODlg : public CDialog
{
// Construction
public:
	CIODlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIODlg)
	enum { IDD = IDD_IO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	struct Cilentinfo
	{
		SOCKET cinsock;
		CString cinIP;
	}cilentin;
	BOOL InItSocket();
	SOCKET sSever,sCilent; //套接字
	CArray <Cilentinfo,Cilentinfo&>m_Cilentinfo;//动态数组客户套接字及IP

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IODLG_H__6665BC4F_85C6_4DC5_A88F_AAAADD22639F__INCLUDED_)
