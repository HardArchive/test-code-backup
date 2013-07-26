// PiPeServer.h : main header file for the PIPESERVER application
//

#if !defined(AFX_PIPESERVER_H__ED18F0FE_036A_47EB_B9CB_428ED17AFEBA__INCLUDED_)
#define AFX_PIPESERVER_H__ED18F0FE_036A_47EB_B9CB_428ED17AFEBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPiPeServerApp:
// See PiPeServer.cpp for the implementation of this class
//

class CPiPeServerApp : public CWinApp
{
public:
	CPiPeServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPiPeServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPiPeServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPESERVER_H__ED18F0FE_036A_47EB_B9CB_428ED17AFEBA__INCLUDED_)
