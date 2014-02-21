// testhookDll.h : main header file for the TESTHOOKDLL application
//

#if !defined(AFX_TESTHOOKDLL_H__2123847E_692E_4196_9993_6DB39955340D__INCLUDED_)
#define AFX_TESTHOOKDLL_H__2123847E_692E_4196_9993_6DB39955340D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTesthookDllApp:
// See testhookDll.cpp for the implementation of this class
//

class CTesthookDllApp : public CWinApp
{
public:
	CTesthookDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTesthookDllApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTesthookDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTHOOKDLL_H__2123847E_692E_4196_9993_6DB39955340D__INCLUDED_)
