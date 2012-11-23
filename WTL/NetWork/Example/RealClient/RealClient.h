// RealClient.h : main header file for the REALCLIENT application
//

#if !defined(AFX_REALCLIENT_H__8EC9DE36_DC70_43B2_A696_765E434A28CE__INCLUDED_)
#define AFX_REALCLIENT_H__8EC9DE36_DC70_43B2_A696_765E434A28CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRealClientApp:
// See RealClient.cpp for the implementation of this class
//

class CRealClientApp : public CWinApp
{
public:
	CRealClientApp();
	~CRealClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRealClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALCLIENT_H__8EC9DE36_DC70_43B2_A696_765E434A28CE__INCLUDED_)
