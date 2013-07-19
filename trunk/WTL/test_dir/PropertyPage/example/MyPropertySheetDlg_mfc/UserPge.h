#if !defined(AFX_USERPGE_H__ABB9E48B_E17D_4B29_9ED6_A85AE537F705__INCLUDED_)
#define AFX_USERPGE_H__ABB9E48B_E17D_4B29_9ED6_A85AE537F705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserPge.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserPge dialog

class CUserPge : public CPropertyPage
{
	DECLARE_DYNCREATE(CUserPge)

// Construction
public:
	CUserPge();
	~CUserPge();

// Dialog Data
	//{{AFX_DATA(CUserPge)
	enum { IDD = IDD_USERPGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUserPge)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUserPge)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERPGE_H__ABB9E48B_E17D_4B29_9ED6_A85AE537F705__INCLUDED_)
