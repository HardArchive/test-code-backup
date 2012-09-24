// SQLite TestDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "CarsTbl.h"


// CSQLiteTestDlg dialog
class CSQLiteTestDlg : public CDialog
{
// Construction
public:
	CSQLiteTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SQLITETEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
    CCarsTbl m_cars;

    CListCtrl m_list;
    CButton m_open;
    CButton m_close;
    CButton m_new;
    CButton m_add;
    CButton m_del;
	HICON m_hIcon;
    bool m_file_open;
    CString m_path;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    void Update();
    void Clean();
    void LoadList();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOpen();
    afx_msg void OnBnClickedClose();
    afx_msg void OnBnClickedNew();
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedDel();
    afx_msg void GoSort();
    afx_msg void GoFilter();
};

extern CSQLiteTestDlg* g_pDlg;