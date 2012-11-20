// IpLookUp_dll.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Dlg_DLL.h"


extern "C" __declspec(dllexport) bool ShowDllWindow()
{
	
	return true;
}