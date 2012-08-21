// TstWtl.cpp : main source file for TstWtl.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

#include "resource.h"

#include "TstWtlView.h"
#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;


int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	int nTop = GetSystemMetrics(SM_CXBORDER);
	int nTop0 = GetSystemMetrics(SM_CYBORDER);
	int nTop1 = GetSystemMetrics(SM_CYCAPTION);
	int nTop2 = GetSystemMetrics(SM_CXEDGE);
	int nTop3 = GetSystemMetrics(SM_CYEDGE);
	int nTop4 = GetSystemMetrics(SM_CXFRAME);
	int nTop5 = GetSystemMetrics(SM_CYFRAME);
	int nTop6 = GetSystemMetrics(SM_CXFIXEDFRAME);
	int nTop7 = GetSystemMetrics(SM_CYFIXEDFRAME);
	int nTop8 = GetSystemMetrics(SM_CXDLGFRAME);
	int nTop9 = GetSystemMetrics(SM_CYDLGFRAME);	// SM_CYFULLSCREEN
	
	int nHeightCaption = GetSystemMetrics(SM_CYCAPTION);
	int nWidthEdge = GetSystemMetrics(SM_CXFRAME);

	ATLTRACE("SM_CXBORDER: %d, SM_CYBORDER: %d,\r\n SM_CYCAPTION: %d,\r\n SM_CXEDGE: %d, SM_CYEDGE: %d,\r\n SM_CXFRAME: %d, SM_CYFRAME: %d,\r\n SM_CXFIXEDFRAME: %d, SM_CYFIXEDFRAME: %d,\r\n SM_CXDLGFRAME: %d, SM_CYDLGFRAME: %d\r\n\r\n", 
		nTop, nTop0, nTop1, nTop2, nTop3, nTop4, nTop5, nTop6, nTop7, nTop8, nTop9);

	BOOL bContinue = FALSE;
	while(bContinue)
	{
		int nX = ::GetSystemMetrics(SM_CXSCREEN);
		int nY = ::GetSystemMetrics(SM_CYSCREEN);
	}
	// SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, "D:\\Win78.bmp", SPIF_SENDWININICHANGE/*|SPIF_UPDATEINIFILE*/);

	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
