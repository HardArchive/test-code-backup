// SimpleScanner.h : main header file for the SIMPLESCANNER application
//

#if !defined(AFX_SIMPLESCANNER_H__7582E521_4860_42A2_AA15_014564E69B52__INCLUDED_)
#define AFX_SIMPLESCANNER_H__7582E521_4860_42A2_AA15_014564E69B52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimpleScannerApp:
// See SimpleScanner.cpp for the implementation of this class
//

class CSimpleScannerApp : public CWinApp
{
public:
	CSimpleScannerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleScannerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSimpleScannerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLESCANNER_H__7582E521_4860_42A2_AA15_014564E69B52__INCLUDED_)
