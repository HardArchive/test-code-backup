// Use1.h : main header file for the USE1 application
//

#if !defined(AFX_USE1_H__CF8E37FC_6070_4CD6_B14B_5C9B494DDA28__INCLUDED_)
#define AFX_USE1_H__CF8E37FC_6070_4CD6_B14B_5C9B494DDA28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUse1App:
// See Use1.cpp for the implementation of this class
//

class CUse1App : public CWinApp
{
public:
	CUse1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUse1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUse1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USE1_H__CF8E37FC_6070_4CD6_B14B_5C9B494DDA28__INCLUDED_)
