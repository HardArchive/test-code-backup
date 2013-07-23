// NetClient.h : main header file for the NETCLIENT application
//

#if !defined(AFX_NETCLIENT_H__3ADA93B8_878C_458F_AB46_0EFD8EFB837E__INCLUDED_)
#define AFX_NETCLIENT_H__3ADA93B8_878C_458F_AB46_0EFD8EFB837E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNetClientApp:
// See NetClient.cpp for the implementation of this class
//

class CNetClientApp : public CWinApp
{
public:
	CNetClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNetClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NetCLIENT_H__3ADA93B8_878C_458F_AB46_0EFD8EFB837E__INCLUDED_)
