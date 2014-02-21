// Accelerator1.h : main header file for the ACCELERATOR1 application
//

#if !defined(AFX_ACCELERATOR1_H__E55BB619_54E8_4913_89EE_0617653E9CC0__INCLUDED_)
#define AFX_ACCELERATOR1_H__E55BB619_54E8_4913_89EE_0617653E9CC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAccelerator1App:
// See Accelerator1.cpp for the implementation of this class
//

class CAccelerator1App : public CWinApp
{
public:
	CAccelerator1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccelerator1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAccelerator1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCELERATOR1_H__E55BB619_54E8_4913_89EE_0617653E9CC0__INCLUDED_)
