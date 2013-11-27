#if !defined(AFX_MAKECALLDLG_H__7C212BEC_316B_4794_B630_2F41F563A14C__INCLUDED_)
#define AFX_MAKECALLDLG_H__7C212BEC_316B_4794_B630_2F41F563A14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeCallDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MakeCallDlg dialog

class MakeCallDlg : public CDialog
{
// Construction
public:
	MakeCallDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MakeCallDlg)
	enum { IDD = IDD_MAKE_A_CALL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MakeCallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MakeCallDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKECALLDLG_H__7C212BEC_316B_4794_B630_2F41F563A14C__INCLUDED_)
