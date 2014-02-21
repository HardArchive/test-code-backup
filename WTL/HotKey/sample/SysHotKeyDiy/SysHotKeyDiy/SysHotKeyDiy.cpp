// SysHotKeyDiy.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SysHotKeyDiy.h"
#include "SysHotKeyDiyDlg.h"
#include "inc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSysHotKeyDiyApp

BEGIN_MESSAGE_MAP(CSysHotKeyDiyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSysHotKeyDiyApp construction

CSysHotKeyDiyApp::CSysHotKeyDiyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSysHotKeyDiyApp object

CSysHotKeyDiyApp theApp;


// CSysHotKeyDiyApp initialization

BOOL CSysHotKeyDiyApp::InitInstance()
{
	BOOL bFound = FALSE;
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, _T("MyAppProg"));
	if ( GetLastError() == ERROR_ALREADY_EXISTS ){
		bFound = TRUE;
	}
	if ( hMutex != NULL ){
		ReleaseMutex(hMutex);
	}
	if ( bFound == TRUE ){
		HWND hWnd = FindAppWnd();
		if ( hWnd ){
			if (IsWindowVisible(hWnd) == FALSE){
				DelTrayIcon(hWnd, FALSE);
			}
			::ShowWindow(hWnd, SW_SHOWNA);
			::ShowWindow(hWnd, SW_RESTORE);
			SetForegroundWindow(hWnd);
		}
		return FALSE;
	}
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CSysHotKeyDiyDlg dlg;
	m_pMainWnd = &dlg;

	dlg.Create(IDD_SYSHOTKEYDIY_DIALOG);
	BOOL bInitHideWnd = ((CSysHotKeyDiyDlg *)m_pMainWnd)->ReadInitTrayIcon();
	if ( bInitHideWnd == FALSE ) {
		ShowWindow(m_pMainWnd->GetSafeHwnd(), SW_RESTORE);
	}
	dlg.RunModalLoop();

	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: Place code here to handle when the dialog is
	//	//  dismissed with OK
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: Place code here to handle when the dialog is
	//	//  dismissed with Cancel
	//}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
