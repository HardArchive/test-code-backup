// NetWorkClient.cpp : main source file for NetWorkClient.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

CAppModule _Module;
HMODULE g_hModRichEdit2 = NULL;  
VOID InitRichEdit2()
{
	if ( g_hModRichEdit2 == NULL )
		g_hModRichEdit2 = ::LoadLibrary(_T("RICHED20.DLL"));
}


int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{	
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

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

	//利用CreateWindowEx创建RichEdit Control之前，需先加载RICHED20.dll这个库，
	//然后才可以CreateWindowEx(..,_T("RichEdit20W"),...);
	//否则的话,CreateWindowEx会失败，GetLastError()返回值为1407: 找不到窗口类名. 
	InitRichEdit2();
	int nRet = Run(lpstrCmdLine, nCmdShow);
	// 最后释放DLL  
	FreeLibrary(g_hModRichEdit2);
	g_hModRichEdit2 = NULL; 
	_Module.Term();
	::CoUninitialize();

	return nRet;
}
