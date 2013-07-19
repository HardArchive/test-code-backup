// MyPropertySheetDlgDlg.h : header file
//

#if !defined(AFX_MYPROPERTYSHEETDLGDLG_H__D51C04FC_AD94_4A26_99A9_BC66E3CA84FF__INCLUDED_)
#define AFX_MYPROPERTYSHEETDLGDLG_H__D51C04FC_AD94_4A26_99A9_BC66E3CA84FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheetDlgDlg dialog
#include "ServerPge.h"
#include "UserPge.h"
class CMyPropertySheetDlgDlg : public CDialog
{
// Construction
public:
	CMyPropertySheetDlgDlg(CWnd* pParent = NULL);	// standard constructor
	CServerPge m_pgeServer;
	CUserPge m_pgeUser;
// Dialog Data
	//{{AFX_DATA(CMyPropertySheetDlgDlg)
	enum { IDD = IDD_MYPROPERTYSHEETDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertySheetDlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyPropertySheetDlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROPERTYSHEETDLGDLG_H__D51C04FC_AD94_4A26_99A9_BC66E3CA84FF__INCLUDED_)
