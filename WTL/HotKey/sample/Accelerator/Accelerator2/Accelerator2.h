// Accelerator2.h : main header file for the ACCELERATOR2 application
//

#if !defined(AFX_ACCELERATOR2_H__72787792_5B52_46CF_AC09_6D7111C3F727__INCLUDED_)
#define AFX_ACCELERATOR2_H__72787792_5B52_46CF_AC09_6D7111C3F727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAccelerator2App:
// See Accelerator2.cpp for the implementation of this class
//

class CAccelerator2App : public CWinApp
{
public:
	CAccelerator2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccelerator2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAccelerator2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCELERATOR2_H__72787792_5B52_46CF_AC09_6D7111C3F727__INCLUDED_)
