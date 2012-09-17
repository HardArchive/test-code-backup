// verinfDlg.h : header file
//

#if !defined(AFX_VERINFDLG_H__C6C8FF90_3BEF_4595_B87F_CA5F23683BE1__INCLUDED_)
#define AFX_VERINFDLG_H__C6C8FF90_3BEF_4595_B87F_CA5F23683BE1__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#ifndef __VERINFO_H__
	#include "Src/FileVersionInfo.h"
#endif

#include "Hyperlink.h"

typedef struct tagMAP
{
	DWORD		dwVal;
	LPCTSTR		lpszVal;
} MAP;

/////////////////////////////////////////////////////////////////////////////
// CVerinfDlg dialog

class CVerinfDlg : public CDialog
{
// Construction
public:
	CVerinfDlg(CWnd* pParent = NULL);	// standard constructor

public:
	void	FillVersionData( LPCTSTR lpszFileName = NULL );
	void	InitializeControls( void );
	LPCTSTR MapExtractData( MAP* lpMap, INT nSize, DWORD dwIndex );
	void	SetProgramData( LPCTSTR lpszFile );
	BOOL	GetProgramIcons( IN  LPCTSTR lpszFileName, OUT HICON* lphIconSmall, OUT HICON* lphIconLarge );

// Dialog Data
	//{{AFX_DATA(CVerinfDlg)
	enum { IDD = IDD_VERINF_DIALOG };
	CComboBox	m_wndCboLang;
	CListCtrl	m_wndLstFvb;
	CListCtrl	m_wndLstFvi;	
	CString	m_strFileName;
	CString	m_strFileType;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerinfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;


	static LPCTSTR	s_lpszFVI[ 7 ];
	static MAP		s_lpVOS[ 14 ];
	static MAP		s_lpFILT[ 7 ];
	static MAP		s_lpFNTT[ 4 ];
	static MAP		s_lpDRVT[ 12 ];
	
	//TCHAR			m_szModule[ MAX_PATH ];
	CFileVersionInfo	m_fvi;
	CHyperLink			m_wndLink;
	TCHAR				m_szModule[ MAX_PATH ];

	// Generated message map functions
	//{{AFX_MSG(CVerinfDlg)
	virtual BOOL OnInitDialog();	
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCmdOpen();
	afx_msg void OnCmdReset();
	afx_msg void OnSelchangeCboLang();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERINFDLG_H__C6C8FF90_3BEF_4595_B87F_CA5F23683BE1__INCLUDED_)
