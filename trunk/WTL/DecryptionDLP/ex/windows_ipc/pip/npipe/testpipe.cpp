#include "stdafx.h"
#include "testpipe.h"
#include "testpipeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTestpipeApp, CWinApp)
END_MESSAGE_MAP()

CTestpipeApp::CTestpipeApp()
{
}

CTestpipeApp theApp;

BOOL CTestpipeApp::InitInstance()
{
	CTestpipeDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}
