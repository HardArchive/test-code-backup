// XHtmlTreeTest.h : main header file for the TEST1 application
//

#ifndef XHTMLTREETEST_H
#define XHTMLTREETEST_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXHtmlTreeTest:
//

class CXHtmlTreeTest : public CWinApp
{
public:
	CXHtmlTreeTest();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXHtmlTreeTest)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXHtmlTreeTest)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XHTMLTREETEST_H
