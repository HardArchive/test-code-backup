// ShowPass.h : main header file for the SHOWPASS application
//

#if !defined(AFX_SHOWPASS_H__FDFEA47B_F14F_4120_AA68_A69CE90DF7A1__INCLUDED_)
#define AFX_SHOWPASS_H__FDFEA47B_F14F_4120_AA68_A69CE90DF7A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CShowPassApp:
// See ShowPass.cpp for the implementation of this class
//

class CShowPassApp : public CWinApp
{
public:
	CShowPassApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowPassApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CShowPassApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWPASS_H__FDFEA47B_F14F_4120_AA68_A69CE90DF7A1__INCLUDED_)
