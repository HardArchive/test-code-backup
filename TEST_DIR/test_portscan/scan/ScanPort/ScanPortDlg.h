// ScanPortDlg.h : header file
//

#if !defined(AFX_SCANPORTDLG_H__153FD1B0_957D_45E2_BE36_0A77152DE9ED__INCLUDED_)
#define AFX_SCANPORTDLG_H__153FD1B0_957D_45E2_BE36_0A77152DE9ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CScanPortDlg dialog

class CScanPortDlg : public CDialog
{
// Construction
public:
	CScanPortDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CScanPortDlg)
	enum { IDD = IDD_SCANPORT_DIALOG };
	CEdit	m_ctrSpecifiedPorts;
	CButton	m_ctrCheck;
	CButton	m_ctrAddPortButton;
	CTreeCtrl	m_ctrResultTree;
	CButton	m_ctrBeginScan;
	CString	m_csHostOrIp;
	BOOL	m_bIsSpecifiedPort;
	UINT	m_nMaxThread;
	UINT	m_iStartPort;
	DWORD	m_dwTimeOut;
	UINT	m_iEndPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanPortDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CScanPortDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnIsSpecifiedPort();
	afx_msg void OnScan();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeSpecifiedPorts();
	//}}AFX_MSG
	afx_msg LRESULT OnScanThread(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void SaveDefaultPorts();
	void ReadDefaultPorts();
	short m_nHasComplete;
	HANDLE m_hThread;
	CProgressCtrl* m_pProgressCtrl;
	DWORD m_dwPercentSeconds;
	HTREEITEM m_hParentTree;
	BOOL m_bTerminate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANPORTDLG_H__153FD1B0_957D_45E2_BE36_0A77152DE9ED__INCLUDED_)
