// SELECT服务端.h : main header file for the SELECT服务端 application
//

#if !defined(AFX_SELECT_H__0F90DC0D_A96D_4CBE_AC14_FC5FD665E98D__INCLUDED_)
#define AFX_SELECT_H__0F90DC0D_A96D_4CBE_AC14_FC5FD665E98D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSELECTApp:
// See SELECT服务端.cpp for the implementation of this class
//

class CSELECTApp : public CWinApp
{
public:
	CSELECTApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSELECTApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSELECTApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECT_H__0F90DC0D_A96D_4CBE_AC14_FC5FD665E98D__INCLUDED_)
