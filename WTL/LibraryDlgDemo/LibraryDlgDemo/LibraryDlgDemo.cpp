// LibraryDlgDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "WTL_Dlg_DLL.h"
int _tmain(int argc, _TCHAR* argv[])
{
	printf("3S后调用DLL中的WTL对话框！！！\r\n");
	Sleep(3000);
	ShowDlg();
	printf("调用DLL中的WTL对话框完毕,程序退出！\r\n");
	return 0;
}

