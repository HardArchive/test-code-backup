// OneKeyPowerOff.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")   //隐藏窗口用
typedef BOOL (/*__stdcall*/ *ForceShutDownEx)(void);

ForceShutDownEx g_ForceShutDown = NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE g_hDll = NULL;
	g_hDll = LoadLibrary("PowerOff.dll");
	if (!g_hDll)
	{
		MessageBox(NULL, "加载动态库失败！", "Error", MB_OK);
		return 0;
	}

	g_ForceShutDown = (ForceShutDownEx)GetProcAddress(g_hDll, "ForceShutDown");

	if (!g_ForceShutDown)
	{
		MessageBox(NULL, "获取DLL函数失败！", "Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	if (!g_ForceShutDown())
	{
		MessageBox(NULL, "快速关机失败！！！", "Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	FreeLibrary(g_hDll);
	return 0;
}

