// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <detours.h>

#pragma comment(lib, "detours.lib")

typedef int (WINAPI *PFNMessageBoxW)(
			__in_opt HWND hWnd,
			__in_opt LPCWSTR lpText,
			__in_opt LPCWSTR lpCaption,
			__in UINT uType);

PFNMessageBoxW g_pfnMessagebox;

int WINAPI HOOK_MessageBoxW(
			__in_opt HWND hWnd,
			__in_opt LPCWSTR lpText,
			__in_opt LPCWSTR lpCaption,
			__in UINT uType)
{
	return g_pfnMessagebox(NULL, _T("HOOK Messagebox succees!!!!!"), _T("OK"), MB_OK);
}



int _tmain(int argc, _TCHAR* argv[])
{
	g_pfnMessagebox = (PFNMessageBoxW)GetProcAddress(GetModuleHandle(_T("user32.dll")), "MessageBoxW");

	MessageBox(NULL, __T("Hello, test1 ……"), _T("Hello"), MB_OK);

	if (g_pfnMessagebox)
	{
			DetourRestoreAfterWith();

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)g_pfnMessagebox, HOOK_MessageBoxW);
			DetourTransactionCommit();
	}

	MessageBox(NULL, _T("Hello, test2 ……"), _T("Hello"), MB_OK);

	if (g_pfnMessagebox)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)g_pfnMessagebox, HOOK_MessageBoxW);
		DetourTransactionCommit();
	}

	return 0;
}

