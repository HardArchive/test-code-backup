// OneKeyRestart.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")   //���ش�����
typedef BOOL (/*__stdcall*/ *ForceRebootEx)(void);

ForceRebootEx g_ForceReboot = NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	
	HMODULE g_hDll = NULL;
	g_hDll = LoadLibrary("PowerOff.dll");
	if (!g_hDll)
	{
		MessageBox(NULL, "���ض�̬��ʧ�ܣ�", "Error", MB_OK);
		return 0;
	}

	g_ForceReboot = (ForceRebootEx)GetProcAddress(g_hDll, "ForceReboot");

	if (!g_ForceReboot)
	{
		MessageBox(NULL, "��ȡDLL����ʧ�ܣ�", "Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	if (!g_ForceReboot())
	{
		MessageBox(NULL, "���ٹػ�ʧ�ܣ�����", "Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	FreeLibrary(g_hDll);

	return 0;
}

