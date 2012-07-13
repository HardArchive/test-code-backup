// AtlTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

#include "AboutDlg.h"
#include "PaintBkgnd.h"
#include "MyWindow.h"

CComModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	// TODO: Place code here.
	_Module.Init(NULL, hInstance);
	CMyWindow wndMain;
	MSG msg;
	
	if (NULL == wndMain.Create(NULL, CWindow::rcDefault, _T("My First ATL Window")))
	{
		// Failed
		return 1;
	}
	
	wndMain.ShowWindow(nCmdShow);
	wndMain.UpdateWindow();
	
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	_Module.Term();
	
	return msg.wParam;
	return 0;

}