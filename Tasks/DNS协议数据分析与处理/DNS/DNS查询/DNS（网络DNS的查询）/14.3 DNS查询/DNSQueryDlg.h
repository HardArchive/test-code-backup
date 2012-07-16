// DNSQueryDlg.h : header file
//

#if !defined(AFX_DNSQUERYDLG_H__1B88D081_0809_4994_BD78_83D89FED6194__INCLUDED_)
#define AFX_DNSQUERYDLG_H__1B88D081_0809_4994_BD78_83D89FED6194__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDNSQueryDlg dialog
#include "DNSQuery.h"

class CDNSQueryDlg : public CDialog
{
// Construction
public:
	CDNSQueryDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDNSQueryDlg)
	enum { IDD = IDD_DNSQUERY_DIALOG };
	CEdit	m_edtResult;
	CListBox	m_lsbServer;
	CIPAddressCtrl	m_ipaServer;
	CString	m_strResult;
	CString	m_strName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDNSQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CDNSQuery m_query;

	// Generated message map functions
	//{{AFX_MSG(CDNSQueryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQuery();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DNSQUERYDLG_H__1B88D081_0809_4994_BD78_83D89FED6194__INCLUDED_)
