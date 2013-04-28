
// ThreadSyncClassesDemoDlg.h : header file
//

#pragma once


// CThreadSyncClassesDemoDlg dialog
class CThreadSyncClassesDemoDlg : public CDialogEx
{
// Construction
public:
	CThreadSyncClassesDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_THREADSYNCCLASSESDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonMutexTimeout();
    afx_msg void OnBnClickedButtonMutexAbandoned();
    afx_msg void OnBnClickedButtonMutexWaitfailed();
    afx_msg void OnBnClickedButtonSemTimeout();
    afx_msg void OnBnClickedButtonSemWaitfailed();
    afx_msg void OnBnClickedButtonMultiTimeout();
    afx_msg void OnBnClickedButtonMultiAbandoned();
    afx_msg void OnBnClickedButtonMultiWaitfailed();
    CProgressCtrl m_ProgressDemo;
    afx_msg void OnBnClickedButtonUiMultiWait();
    afx_msg LRESULT OnProgressUpdate(WPARAM,LPARAM);
private:
    CString GetUniqueKernelObjName();
    static UINT WaitAbandonedThread(LPVOID lpVoid);
    static UINT ExceptionSimulatorThread(LPVOID lpVoid);
    static UINT MultiWaitAbandonedThread(LPVOID lpVoid);
    static UINT UIWaitDemoThread(LPVOID lpVoid);
};
