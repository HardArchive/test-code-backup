// NamePipeClt.h : main header file for the NAMEPIPECLT application
//

#if !defined(AFX_NAMEPIPECLT_H__686E04AF_CACA_418D_9462_CA819A0FFC39__INCLUDED_)
#define AFX_NAMEPIPECLT_H__686E04AF_CACA_418D_9462_CA819A0FFC39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltApp:
// See NamePipeClt.cpp for the implementation of this class
//

class CNamePipeCltApp : public CWinApp
{
public:
	CNamePipeCltApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamePipeCltApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNamePipeCltApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEPIPECLT_H__686E04AF_CACA_418D_9462_CA819A0FFC39__INCLUDED_)
