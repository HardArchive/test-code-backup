// FileWatcherDlg.h : header file
//

#if !defined(AFX_FILEWATCHERDLG_H__CA611DDF_62C0_4DC0_B787_57E4F6F8B4A8__INCLUDED_)
#define AFX_FILEWATCHERDLG_H__CA611DDF_62C0_4DC0_B787_57E4F6F8B4A8__INCLUDED_

#include "Watcher.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileWatcherDlg dialog

#define WM_MY_NOTIFY		WM_USER + 100

class CFileWatcherDlg : public CDialog
{
// Construction
public:
	BOOL m_bStart;
	BOOL IsFolder(LPCTSTR lpszPath);
	void OnWatch(DWORD dwAct, CString csStrName1, CString csStrName2);
	BOOL NotifyTray(BOOL bCreate);
	CString SelectPath();
	void InitializeListCtrl();
	CWatcher m_csWatcher;
	CFileWatcherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileWatcherDlg)
	enum { IDD = IDD_FILEWATCHER_DIALOG };
	CListCtrl	m_csListData;
	CEdit	m_csEdtPath;
	CButton	m_csBtnCtrl;
	CString	m_csStrPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileWatcherDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFileWatcherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRclickListData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMnuOpen();
	afx_msg void OnMnuDelsel();
	afx_msg void OnMnuDelall();
	afx_msg void OnBtnCtrl();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEWATCHERDLG_H__CA611DDF_62C0_4DC0_B787_57E4F6F8B4A8__INCLUDED_)
