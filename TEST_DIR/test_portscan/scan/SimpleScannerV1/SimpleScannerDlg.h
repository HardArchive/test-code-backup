 // SimpleScannerDlg.h : header file
//

#if !defined(AFX_SIMPLESCANNERDLG_H__2304832A_D1E9_4841_A63B_751BCEBA9559__INCLUDED_)
#define AFX_SIMPLESCANNERDLG_H__2304832A_D1E9_4841_A63B_751BCEBA9559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimpleScannerDlg dialog
class CSimpleScannerDlg;

typedef struct _threadinfo {
	int     HostIndex;
	int		portIndex;	
	CSimpleScannerDlg *dlg;
}
THREADINFO;

class CSimpleScannerDlg : public CDialog
{
// Construction
public:
	CRITICAL_SECTION cs;
	void SetUIState(DWORD dwState);
	BOOL IfHostIn(CString strHost); //判断主机地址是否在主机的范围之内
	CString GetNextHost(CString strHost,int num);
	void CompareIp(CString strFrom,CString strTo);
	int GetTotalHost(); // get number of all hosts
	CSimpleScannerDlg(CWnd* pParent = NULL);	// standard constructor
	CString HostList[256];
	UINT m_nScanFlag;
	
// Dialog Data
	//{{AFX_DATA(CSimpleScannerDlg)
	enum { IDD = IDD_SIMPLESCANNER_DIALOG };
	CListBox	m_ctrUdpList;
	CListBox	m_ctrTcpList;
	int		m_nHostSel;
	CString	m_strIP;
	CString	m_strTo;
	CString	m_strFrom;
	int		m_IHostNum;
	int		m_iTcpNum;
	int		m_iUdpNum;
	int		m_nScanMode;
	BOOL	m_bTcpPort;
	BOOL	m_bUdpPort;
	long	m_lTcpFrom;
	long	m_lTcpTo;
	long	m_lUdpFrom;
	long	m_lUdpTo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleScannerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:	
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CSimpleScannerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRhostsel();
	afx_msg void OnRadio2();
	afx_msg void OnBPing();
	afx_msg void OnKillfocusEip();
	afx_msg void OnKillfocusEfrom();
	afx_msg void OnKillfocusEto();
	afx_msg void OnBdetailresult();
	afx_msg void OnCtcpport();
	afx_msg void OnCudpport();
	afx_msg void OnBaddtcpport();
	afx_msg void OnBaddudpport();
	afx_msg void OnBdeltcpport();
	afx_msg void OnBdeludpport();
	afx_msg void OnBstartscan();
	afx_msg void OnBstopscan();
	afx_msg void OnBviewresult();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	THREADINFO *in_UdpThread;
	THREADINFO *in_TCPthread;
	void ViewFile();
	long m_iUdpPortNum;
	long m_iTcpPortNum;
	void GetScanFlag();
	long GetUdpPortList();
	long GetTcpPortList();
	void DeleteSelItem(CListBox * ctrListBox);
	int InsertString(CListBox *ctrListBox, long from, long to);
	BOOL CheckAdd(CListBox* ctrListBox, long from, long to);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLESCANNERDLG_H__2304832A_D1E9_4841_A63B_751BCEBA9559__INCLUDED_)
