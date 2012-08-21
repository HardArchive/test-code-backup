// SkinnedUI.cpp : main source file for SkinnedUI.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

#include "resource.h"

#include "atldib.h"
#include "atldibfilters.h"
#include "DibCtrls.h"

#include "RoundDibButton.h"
#include "RoundDibFrame.h"
#include "ShadedLabel.h"
#include "RoundColorFrame.h"

#include "maindlg.h"

CAppModule _Module;


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(iccx);
	iccx.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	BOOL bRet = ::InitCommonControlsEx(&iccx);
	bRet;
	ATLASSERT(bRet);

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

   int nRet;
   {
      CMainDlg dlgMain;
	   nRet = dlgMain.DoModal();
   }

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
