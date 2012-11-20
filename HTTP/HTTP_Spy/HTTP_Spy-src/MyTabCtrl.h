#if !defined(AFX_MYTABCTRL_H__ACC7174B_D353_43E2_A9F3_7AAFB8250077__INCLUDED_)
#define AFX_MYTABCTRL_H__ACC7174B_D353_43E2_A9F3_7AAFB8250077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//
#include "HttpResDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl window

class CMyTabCtrl : public CTabCtrl
{
// Construction
public:
	CMyTabCtrl();
	CHttpResDlg *m_Dialog[2];
	short m_TabCount;
	void  InitDialogs();
	void  ActivateTabDialogs();
	void  setDialogText(int, CString);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTabCtrl)
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__ACC7174B_D353_43E2_A9F3_7AAFB8250077__INCLUDED_)
