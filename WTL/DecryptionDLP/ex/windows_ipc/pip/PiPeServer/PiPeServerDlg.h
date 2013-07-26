// PiPeServerDlg.h : header file
//

#if !defined(AFX_PIPESERVERDLG_H__0C41E4E5_1CA9_4F90_A118_4FC5D7E55F3E__INCLUDED_)
#define AFX_PIPESERVERDLG_H__0C41E4E5_1CA9_4F90_A118_4FC5D7E55F3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPiPeServerDlg dialog

class CPiPeServerDlg : public CDialog
{
// Construction
public:
	CPiPeServerDlg(CWnd* pParent = NULL);	// standard constructor



// Dialog Data
	//{{AFX_DATA(CPiPeServerDlg)
	enum { IDD = IDD_PIPESERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPiPeServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPiPeServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnExit();
	afx_msg void OnClose();
	afx_msg void OnStart();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPESERVERDLG_H__0C41E4E5_1CA9_4F90_A118_4FC5D7E55F3E__INCLUDED_)
