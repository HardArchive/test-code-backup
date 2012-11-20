// 213465.h : main header file for the 213465 application
//

#if !defined(AFX_213465_H__ABF45BD0_DC4F_4354_9137_CC8CD0C36316__INCLUDED_)
#define AFX_213465_H__ABF45BD0_DC4F_4354_9137_CC8CD0C36316__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy213465App:
// See 213465.cpp for the implementation of this class
//

class CMy213465App : public CWinApp
{
public:
	CMy213465App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy213465App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy213465App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_213465_H__ABF45BD0_DC4F_4354_9137_CC8CD0C36316__INCLUDED_)
