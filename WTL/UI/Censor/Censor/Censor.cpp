// Censor.cpp : main source file for Censor.exe
//

#include "stdafx.h"
#include "Censor.h"

#include "AboutDlg.h"
#include "MainFrm.h"

// 唯一的一个 CMainApp 对象
CMainApp theApp;

CMainApp::CMainApp()
{

}

/*virtual*/ BOOL CMainApp::InitInstance(void)
{
	HBITMAP hBitmap = LoadBitmap( theApp.GetResourceInstance(), MAKEINTRESOURCE(IDB_SKINMGR) );
	if ( !SkinMgr_Initialize(theApp.get_m_hInst(), hBitmap) )
		return FALSE;

	return TRUE;
}

/*virtual*/ int CMainApp::ExitInstance(void)
{
	return 0;
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	theApp.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}
	wndMain.CenterWindow();
	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	theApp.RemoveMessageLoop();
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

	hRes = theApp.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	// 应用程序初始化失败
	if ( !theApp.InitInstance() )
		return -1;

	int nRet = Run(lpstrCmdLine, nCmdShow);

	// 应用程序退出前释放
	theApp.ExitInstance();
	theApp.Term();
	::CoUninitialize();

	return nRet;
}
