// SQLite Test.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SQLite Test.h"
#include "SQLite TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSQLiteTestApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()

//globals
CSQLiteTestApp g_app;
CSQLiteTestDlg* g_pDlg;

BOOL CSQLiteTestApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("SQLite Test"));

	CSQLiteTestDlg dlg;
	m_pMainWnd = &dlg;
    g_pDlg = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	return FALSE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()