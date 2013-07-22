// IPCWorkshop.h : main header file for the IPCWORKSHOP application
//

#if !defined(AFX_IPCWORKSHOP_H__80B5AD26_3AD7_44E6_91A9_CCA1D17B2E39__INCLUDED_)
#define AFX_IPCWORKSHOP_H__80B5AD26_3AD7_44E6_91A9_CCA1D17B2E39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIPCWorkshopApp:
// See IPCWorkshop.cpp for the implementation of this class
//

class CIPCWorkshopApp : public CWinApp
{
public:
	CIPCWorkshopApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCWorkshopApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIPCWorkshopApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCWORKSHOP_H__80B5AD26_3AD7_44E6_91A9_CCA1D17B2E39__INCLUDED_)
