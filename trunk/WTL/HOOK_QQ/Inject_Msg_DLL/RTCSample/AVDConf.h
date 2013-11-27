// AVDConf.h : main header file for the AVDCONF application
//

#if !defined(AFX_AVDCONF_H__6142F942_A17D_4DC7_89CF_2D9924E20320__INCLUDED_)
#define AFX_AVDCONF_H__6142F942_A17D_4DC7_89CF_2D9924E20320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAVDConfApp:
// See AVDConf.cpp for the implementation of this class
//

class CAVDConfApp : public CWinApp
{
public:
	CAVDConfApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAVDConfApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAVDConfApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVDCONF_H__6142F942_A17D_4DC7_89CF_2D9924E20320__INCLUDED_)
