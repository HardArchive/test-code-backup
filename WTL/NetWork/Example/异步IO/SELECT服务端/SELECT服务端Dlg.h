// SELECT服务端Dlg.h : header file
//

#if !defined(AFX_SELECTDLG_H__E7732192_05DA_4FED_A161_FA88E7C59C5E__INCLUDED_)
#define AFX_SELECTDLG_H__E7732192_05DA_4FED_A161_FA88E7C59C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DWORD WINAPI ThreadFunc(LPVOID pParam);//开始线程
DWORD WINAPI WorkerThread(LPVOID lpParameter);//工作者线程
/////////////////////////////////////////////////////////////////////////////
// CSELECTDlg dialog


class CSELECTDlg : public CDialog
{
// Construction
public:
	CSELECTDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSELECTDlg)
	enum { IDD = IDD_SELECT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSELECTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSELECTDlg)
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
	int    TotalConn ; //连接数量
	struct Clientinfo  //记录套接字和IP的结构
	{
		SOCKET cliensocket;
		CString clientIP;
	};
    Clientinfo CliSocketArr[64]; //套接字数组
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTDLG_H__E7732192_05DA_4FED_A161_FA88E7C59C5E__INCLUDED_)
