// DragDrop.h : main header file for the DRAGDROP application
//

#if !defined(AFX_DRAGDROP_H__3DEF9425_73BE_11D6_9CD8_444553540000__INCLUDED_)
#define AFX_DRAGDROP_H__3DEF9425_73BE_11D6_9CD8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDragDropApp:
// See DragDrop.cpp for the implementation of this class
//

class CDragDropApp : public CWinApp
{
public:
	CDragDropApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragDropApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDragDropApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGDROP_H__3DEF9425_73BE_11D6_9CD8_444553540000__INCLUDED_)
