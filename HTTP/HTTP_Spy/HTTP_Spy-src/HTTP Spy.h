// HTTP Spy.h : main header file for the HTTP SPY application
//

#if !defined(AFX_HTTPSPY_H__9A0E98E5_9894_47AC_9D7C_4D36D2319EAC__INCLUDED_)
#define AFX_HTTPSPY_H__9A0E98E5_9894_47AC_9D7C_4D36D2319EAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHTTPSpyApp:
// See HTTP Spy.cpp for the implementation of this class
//

class CHTTPSpyApp : public CWinApp
{
public:
	CHTTPSpyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTTPSpyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHTTPSpyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPSPY_H__9A0E98E5_9894_47AC_9D7C_4D36D2319EAC__INCLUDED_)
