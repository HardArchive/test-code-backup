// TraceSender.h : main header file for the TRACESENDER application
//

#if !defined(AFX_TRACESENDER_H__1C4C251E_C617_4F26_8A8A_3F53BA4EF1BF__INCLUDED_)
#define AFX_TRACESENDER_H__1C4C251E_C617_4F26_8A8A_3F53BA4EF1BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTraceSenderApp:
// See TraceSender.cpp for the implementation of this class
//

class CTraceSenderApp : public CWinApp
{
public:
	CTraceSenderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTraceSenderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTraceSenderApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACESENDER_H__1C4C251E_C617_4F26_8A8A_3F53BA4EF1BF__INCLUDED_)
