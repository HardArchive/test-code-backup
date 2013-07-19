#if !defined(AFX_SERVERPGE_H__DD0826FA_95AA_4BA4_9FD5_A22F54383B83__INCLUDED_)
#define AFX_SERVERPGE_H__DD0826FA_95AA_4BA4_9FD5_A22F54383B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerPge.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerPge dialog

class CServerPge : public CPropertyPage
{
	DECLARE_DYNCREATE(CServerPge)

// Construction
public:
	CServerPge();
	~CServerPge();

// Dialog Data
	//{{AFX_DATA(CServerPge)
	enum { IDD = IDD_SERVERPGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CServerPge)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CServerPge)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERPGE_H__DD0826FA_95AA_4BA4_9FD5_A22F54383B83__INCLUDED_)
