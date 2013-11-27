// ------------------------------------------------------------------------------------
// Copyright ©2002 Intel Corporation
// All Rights Reserved
// 
// Permission is granted to use, copy, distribute and prepare derivative works of this 
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  This software is provided "AS IS." 
//
// Intel specifically disclaims all warranties, express or implied, and all liability,
// including consequential and other indirect damages, for the use of this software, 
// including liability for infringement of any proprietary rights, and including the 
// warranties of merchantability and fitness for a particular purpose.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
// ------------------------------------------------------------------------------------
//
// AVDConf.cpp : Defines the class behaviors for the application.
//
// Purpose: Parent class to the AVD Conferencing application.
//

#include "stdafx.h"
#include "AVDConf.h"
#include "AVDConfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAVDConfApp

BEGIN_MESSAGE_MAP(CAVDConfApp, CWinApp)
	//{{AFX_MSG_MAP(CAVDConfApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAVDConfApp construction

CAVDConfApp::CAVDConfApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAVDConfApp object

CAVDConfApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAVDConfApp initialization

BOOL CAVDConfApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CAVDConfDlg dlg;
	m_pMainWnd = &dlg;
	
	// Since we are a dialog based application, any IDOK or IDCANCEL message results
	// in application exiting.
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
