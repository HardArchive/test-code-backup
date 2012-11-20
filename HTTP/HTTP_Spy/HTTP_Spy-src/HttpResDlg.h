#if !defined(AFX_HTTPRESDLG_H__159317FB_B91D_4E9D_845E_B27CBD070514__INCLUDED_)
#define AFX_HTTPRESDLG_H__159317FB_B91D_4E9D_845E_B27CBD070514__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HttpResDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHttpResDlg dialog

class CHttpResDlg : public CDialog
{
// Construction
public:
	CHttpResDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHttpResDlg)
	enum { IDD = IDD_RESPONSE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpResDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHttpResDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPRESDLG_H__159317FB_B91D_4E9D_845E_B27CBD070514__INCLUDED_)
