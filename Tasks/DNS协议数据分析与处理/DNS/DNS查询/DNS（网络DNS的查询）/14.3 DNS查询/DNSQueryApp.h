// DNSQuery.h : main header file for the DNSQUERY application
//

#if !defined(AFX_DNSQUERY_H__52CAA0FC_D2D7_4089_9AB4_9827A35CD1B4__INCLUDED_)
#define AFX_DNSQUERY_H__52CAA0FC_D2D7_4089_9AB4_9827A35CD1B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDNSQueryApp:
// See DNSQuery.cpp for the implementation of this class
//

class CDNSQueryApp : public CWinApp
{
public:
	CDNSQueryApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDNSQueryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDNSQueryApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DNSQUERY_H__52CAA0FC_D2D7_4089_9AB4_9827A35CD1B4__INCLUDED_)
