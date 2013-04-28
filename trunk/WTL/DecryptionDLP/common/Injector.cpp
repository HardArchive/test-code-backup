// Injector.cpp�ļ�.
#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>

//-----------------------------------------------------------------------------------------
// �ڽ��̿ռ�ע���DLL.
BOOL WINAPI LoadLib(DWORD dwProcessId, LPTSTR lpszLibName)
{
	BOOL   bResult          = FALSE; 
	HANDLE hProcess         = NULL;
	HANDLE hThread          = NULL;
	PSTR   pszLibFileRemote = NULL;

	__try 
	{
		// �����Ҫע�����Ľ��̵ľ��.
		hProcess = OpenProcess(
			PROCESS_ALL_ACCESS, 
			FALSE, 
			dwProcessId
			);

		if (hProcess == NULL) 
			__leave;
		
	    // ����DLL·������Ҫ���ֽ���.
		int cch = 1 + _tcslen(lpszLibName);

		// ��Զ���߳���Ϊ·��������ռ�.
		pszLibFileRemote = (PSTR)VirtualAllocEx(
			hProcess, 
			NULL, 
			cch, 
			MEM_COMMIT, 
			PAGE_READWRITE
			);

		if (pszLibFileRemote == NULL) 
			__leave;
	    
		// ��DLL��·�������Ƶ�Զ�̽��̵��ڴ�ռ�.
		if (!WriteProcessMemory(
			hProcess, 
			(PVOID)pszLibFileRemote, 
			(PVOID)lpszLibName, 
			cch, 
			NULL)) 
			__leave;
		HMODULE hMod = ::GetModuleHandle(TEXT("kernel32"));
	
	   // ���LoadLibraryA��Kernel32.dll�е�������ַ. 
#ifdef _UNICODE
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
#else
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryA");
#endif

		if (pfnThreadRtn == NULL) 
			__leave;

	    // ����Զ���̣߳���ͨ��Զ���̵߳����û���DLL�ļ�. 
		hThread = CreateRemoteThread(
			hProcess, 
			NULL, 
			0, 
			pfnThreadRtn, 
			(PVOID)pszLibFileRemote, 
			0, 
			NULL
			);
		if (hThread == NULL) 
			__leave;

		// �ȴ�Զ���߳���ֹ.
		WaitForSingleObject(hThread, INFINITE);

		bResult = TRUE; 
	}
	__finally 
	{ 
		// �رվ��. 
		if (pszLibFileRemote != NULL) 
			VirtualFreeEx(hProcess, (PVOID)pszLibFileRemote, 0, MEM_RELEASE);

		if (hThread  != NULL) 
			CloseHandle(hThread);

		if (hProcess != NULL) 
			CloseHandle(hProcess);
	}
	return bResult;
}

//-----------------------------------------------------------------------------------------
// �ڽ��̿ռ��ͷ�ע���DLL.
BOOL WINAPI FreeLib(DWORD dwProcessId, LPTSTR lpszLibName) 
{ 
	BOOL   bResult     = FALSE; 
	HANDLE hProcess    = NULL; 
	HANDLE hThread     = NULL; 
	HANDLE hthSnapshot = NULL; 
	MODULEENTRY32 hMod = {sizeof(hMod)}; 
	
	__try 
	{
		// �򿪽���. 
		hProcess = OpenProcess(
			PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, 
			FALSE, 
			dwProcessId
			); 
		
		if (hProcess == NULL) 
			__leave;
	

		// ȡ��FreeLibrary������Kernel32.dll�еĵ�ַ. 
		PTHREAD_START_ROUTINE pfnThreadRtn = 
			(PTHREAD_START_ROUTINE)GetProcAddress( 
			GetModuleHandle(_T("Kernel32.dll")), "FreeLibrary"); 

		if (pfnThreadRtn == NULL) 
			__leave;

		// ȡ��ָ�����̵�����ģ��ӳ��. 
		hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId); 
		if (hthSnapshot == NULL) 
			__leave;
		
		// ȡ������ģ���б��е�ָ����ģ��. 
		BOOL bMoreMods = Module32First(hthSnapshot, &hMod); 
		if (bMoreMods == FALSE) 
			__leave;
		
		// ѭ��ȡ����Ҫ��ģ��. 
		for (;bMoreMods; bMoreMods = Module32Next(hthSnapshot, &hMod)) 
		{ 			
			if ((_tcscmp(hMod.szExePath, lpszLibName) == 0) ||
				(_tcscmp(hMod.szModule, lpszLibName) == 0))
			{
				break; 
			}
		} 
		
		// ����Զ���߳���ִ��FreeLibrary����. 
		hThread = CreateRemoteThread(hProcess, 
			NULL, 
			0, 
			pfnThreadRtn, 
			hMod.modBaseAddr, 
			0, 
			NULL); 
		if (hThread == NULL) 
			__leave;
		
		// �ȴ�Զ���߳���ֹ.
		WaitForSingleObject(hThread, INFINITE); 
		
		bResult=TRUE;
		
	}
	__finally 
	{ 
		// �رվ��. 
		if (hThread != NULL) 
			CloseHandle(hThread); 

		if (hthSnapshot  != NULL) 
			CloseHandle(hthSnapshot); 
		
		if (hProcess  != NULL) 
			CloseHandle(hProcess); 
	}
	
	return bResult; 
}