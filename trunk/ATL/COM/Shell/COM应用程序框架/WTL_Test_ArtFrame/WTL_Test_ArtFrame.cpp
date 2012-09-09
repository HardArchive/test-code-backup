// WTL_Test_ArtFrame.cpp : main source file for WTL_Test_ArtFrame.exe
//

#include "stdafx.h"
#include "resource.h"

#include "MainNotifyObject.h"
#include <memory>
#include "..\inc\ArtFrame_i.c"

CAppModule _Module;
CMainNotifyObject *g_MainnObject = NULL;


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

	GUID guid;
	hRes = _Module.Init(NULL, hInstance, &guid);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;
	//// BLOCK: Run application
	//{
	//	CMainDlg dlgMain;
	//	nRet = dlgMain.DoModal();
	//}

	std::auto_ptr<CMainNotifyObject>  _MainObject(new CMainNotifyObject());

	g_MainnObject = _MainObject.get();
	g_MainnObject->WinMain();

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
