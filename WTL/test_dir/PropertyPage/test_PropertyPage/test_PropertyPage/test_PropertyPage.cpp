// test_PropertyPage.cpp : main source file for test_PropertyPage.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "RgPropSheet.h"
CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	//CMainDlg dlgMain;

	//if(dlgMain.Create(NULL) == NULL)
	//{
	//	ATLTRACE(_T("Main dialog creation failed!\n"));
	//	return 0;
	//}

	//dlgMain.ShowWindow(nCmdShow);

	CRgPropSheet ps(_T("PSSHETEE"));
	//CPropertySheetEx ps(IDS_PROPSHEET_CAPTION);
	CPropertyPage<IDD_DIALOG1> page;
	CPropertyPage<IDD_MAINDLG> page2;
	CPropertyPage<IDD_ABOUTBOX> page3;

	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(&ps);
	pLoop->AddIdleHandler(&ps);
	page.SetTitle(_T("1234"));
	ps.AddPage(page);
	ps.AddPage(page2);
	ps.AddPage(page);
	ps.AddPage(page3);
	//ps.Create(this.m_hWnd);
	ps.DoModal();
	::PostQuitMessage(1);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
