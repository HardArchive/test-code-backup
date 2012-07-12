// KuiDemo.cpp : main source file for KuiDemo.exe
//

#include "stdafx.h"

//#include <atlframe.h>
//#include <atlctrls.h>
//#include <atldlgs.h>
//
//#include "resource.h"
//
//#include "aboutdlg.h"
//#include "MainDlg.h"
//
//CAppModule _Module;
//
//int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
//{
//	CMessageLoop theLoop;
//	_Module.AddMessageLoop(&theLoop);
//
//	CMainDlg dlgMain;
//
//	if(dlgMain.Create(NULL) == NULL)
//	{
//		ATLTRACE(_T("Main dialog creation failed!\n"));
//		return 0;
//	}
//
//	dlgMain.ShowWindow(nCmdShow);
//
//	int nRet = theLoop.Run();
//
//	_Module.RemoveMessageLoop();
//	return nRet;
//}
//
//int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
//{
//	HRESULT hRes = ::CoInitialize(NULL);
//// If you are running on NT 4.0 or higher you can use the following call instead to 
//// make the EXE free threaded. This means that calls come in on a random RPC thread.
////	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
//	ATLASSERT(SUCCEEDED(hRes));
//
//	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
//	::DefWindowProc(NULL, 0, 0, 0L);
//
//	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
//
//	hRes = _Module.Init(NULL, hInstance);
//	ATLASSERT(SUCCEEDED(hRes));
//
//	int nRet = Run(lpstrCmdLine, nCmdShow);
//
//	_Module.Term();
//	::CoUninitialize();
//
//	return nRet;
//}

#include "MainDlg.h"

CAppModule _Module;

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

	BkString::Load(IDR_BK_STRING_DEF);
	BkFontPool::SetDefaultFont(BkString::Get(IDS_APP_FONT), -12);
	BkSkin::LoadSkins(IDR_BK_SKIN_DEF);
	BkStyle::LoadStyles(IDR_BK_STYLE_DEF);

	CMainDlg dlgMain;
	dlgMain.DoModal();

	_Module.Term();
	::CoUninitialize();

	return 0;
}