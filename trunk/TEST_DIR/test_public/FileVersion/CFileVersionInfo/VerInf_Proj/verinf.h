// verinf.h : main header file for the VERINF application
//

#if !defined(AFX_VERINF_H__F84EF348_6268_4C92_87EE_1BCF1DCF1FF2__INCLUDED_)
#define AFX_VERINF_H__F84EF348_6268_4C92_87EE_1BCF1DCF1FF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVerinfApp:
// See verinf.cpp for the implementation of this class
//

class CVerinfApp : public CWinApp
{
public:
	CVerinfApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerinfApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVerinfApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERINF_H__F84EF348_6268_4C92_87EE_1BCF1DCF1FF2__INCLUDED_)
