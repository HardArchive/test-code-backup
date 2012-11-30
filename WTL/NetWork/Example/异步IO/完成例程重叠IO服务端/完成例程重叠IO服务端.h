// 完成例程重叠IO服务端.h : main header file for the 完成例程重叠IO服务端 application
//

#if !defined(AFX_IO_H__D89A7CAD_D494_445C_A893_EE0C358FCF32__INCLUDED_)
#define AFX_IO_H__D89A7CAD_D494_445C_A893_EE0C358FCF32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIOApp:
// See 完成例程重叠IO服务端.cpp for the implementation of this class
//

class CIOApp : public CWinApp
{
public:
	CIOApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIOApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIOApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IO_H__D89A7CAD_D494_445C_A893_EE0C358FCF32__INCLUDED_)
