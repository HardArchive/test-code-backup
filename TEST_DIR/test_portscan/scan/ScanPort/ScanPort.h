// ScanPort.h : main header file for the SCANPORT application
//

#if !defined(AFX_SCANPORT_H__C7D50AE2_DB32_403F_BDE9_8722791CC368__INCLUDED_)
#define AFX_SCANPORT_H__C7D50AE2_DB32_403F_BDE9_8722791CC368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CScanPortApp:
// See ScanPort.cpp for the implementation of this class
//

class CScanPortApp : public CWinApp
{
public:
	CScanPortApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanPortApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CScanPortApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANPORT_H__C7D50AE2_DB32_403F_BDE9_8722791CC368__INCLUDED_)
