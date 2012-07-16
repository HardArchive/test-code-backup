// testWin7.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
typedef bool (/*__stdcall*/ *UnLogonImageEx)();
UnLogonImageEx g_UnLogonImage = NULL;
typedef bool (/*__stdcall*/ *LogonImageEx)(WCHAR*);
LogonImageEx g_LogonImage = NULL;
int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE g_hDll = NULL;
	g_hDll = LoadLibrary(L"LogonImage.dll");
	if (!g_hDll)
	{
		MessageBox(NULL, L"加载动态库失败！", L"Error", MB_OK);
		return 0;
	}

	g_UnLogonImage = (UnLogonImageEx)GetProcAddress(g_hDll, "UnLogonImage");

	if (!g_UnLogonImage)
	{
		MessageBox(NULL, L"获取DLL函数失败！", L"Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	g_LogonImage = (LogonImageEx)GetProcAddress(g_hDll, "LogonImage");

	if (!g_LogonImage(L"D:\\backgroundDefault.jpg"))
	{
		MessageBox(NULL, L"替换图片失败！！！", L"Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	FreeLibrary(g_hDll);
	return 0;
}

