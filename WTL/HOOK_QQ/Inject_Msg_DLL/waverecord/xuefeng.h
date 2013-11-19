// xuefeng.h : main header file for the XUEFENG application
//

#if !defined(AFX_XUEFENG_H__69A8F4F2_ABE1_4EE2_AE0E_7E6DA11D5A47__INCLUDED_)
#define AFX_XUEFENG_H__69A8F4F2_ABE1_4EE2_AE0E_7E6DA11D5A47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXuefengApp:
// See xuefeng.cpp for the implementation of this class
//

class CXuefengApp : public CWinApp
{
public:
	CXuefengApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXuefengApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXuefengApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XUEFENG_H__69A8F4F2_ABE1_4EE2_AE0E_7E6DA11D5A47__INCLUDED_)
