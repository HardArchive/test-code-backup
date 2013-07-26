// FileWatcher.h : main header file for the FILEWATCHER application
//

#if !defined(AFX_FILEWATCHER_H__D4C6B482_3739_4E6E_B8EA_D8DD1FA74887__INCLUDED_)
#define AFX_FILEWATCHER_H__D4C6B482_3739_4E6E_B8EA_D8DD1FA74887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFileWatcherApp:
// See FileWatcher.cpp for the implementation of this class
//

class CFileWatcherApp : public CWinApp
{
public:
	CFileWatcherApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileWatcherApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFileWatcherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEWATCHER_H__D4C6B482_3739_4E6E_B8EA_D8DD1FA74887__INCLUDED_)
