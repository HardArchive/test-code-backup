// testWin7.cpp : �������̨Ӧ�ó������ڵ㡣
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
		MessageBox(NULL, L"���ض�̬��ʧ�ܣ�", L"Error", MB_OK);
		return 0;
	}

	g_UnLogonImage = (UnLogonImageEx)GetProcAddress(g_hDll, "UnLogonImage");

	if (!g_UnLogonImage)
	{
		MessageBox(NULL, L"��ȡDLL����ʧ�ܣ�", L"Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	g_LogonImage = (LogonImageEx)GetProcAddress(g_hDll, "LogonImage");

	if (!g_LogonImage(L"D:\\backgroundDefault.jpg"))
	{
		MessageBox(NULL, L"�滻ͼƬʧ�ܣ�����", L"Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	FreeLibrary(g_hDll);
	return 0;
}

