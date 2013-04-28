// Injector.cpp文件.
#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>

//-----------------------------------------------------------------------------------------
// 在进程空间注入的DLL.
BOOL WINAPI LoadLib(DWORD dwProcessId, LPTSTR lpszLibName)
{
	BOOL   bResult          = FALSE; 
	HANDLE hProcess         = NULL;
	HANDLE hThread          = NULL;
	PSTR   pszLibFileRemote = NULL;

	__try 
	{
		// 获得想要注入代码的进程的句柄.
		hProcess = OpenProcess(
			PROCESS_ALL_ACCESS, 
			FALSE, 
			dwProcessId
			);

		if (hProcess == NULL) 
			__leave;
		
	    // 计算DLL路径名需要的字节数.
		int cch = 1 + _tcslen(lpszLibName);

		// 在远程线程中为路径名分配空间.
		pszLibFileRemote = (PSTR)VirtualAllocEx(
			hProcess, 
			NULL, 
			cch, 
			MEM_COMMIT, 
			PAGE_READWRITE
			);

		if (pszLibFileRemote == NULL) 
			__leave;
	    
		// 将DLL的路径名复制到远程进程的内存空间.
		if (!WriteProcessMemory(
			hProcess, 
			(PVOID)pszLibFileRemote, 
			(PVOID)lpszLibName, 
			cch, 
			NULL)) 
			__leave;
		HMODULE hMod = ::GetModuleHandle(TEXT("kernel32"));
	
	   // 获得LoadLibraryA在Kernel32.dll中的真正地址. 
#ifdef _UNICODE
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
#else
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryA");
#endif

		if (pfnThreadRtn == NULL) 
			__leave;

	    // 创建远程线程，并通过远程线程调用用户的DLL文件. 
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

		// 等待远程线程终止.
		WaitForSingleObject(hThread, INFINITE);

		bResult = TRUE; 
	}
	__finally 
	{ 
		// 关闭句柄. 
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
// 在进程空间释放注入的DLL.
BOOL WINAPI FreeLib(DWORD dwProcessId, LPTSTR lpszLibName) 
{ 
	BOOL   bResult     = FALSE; 
	HANDLE hProcess    = NULL; 
	HANDLE hThread     = NULL; 
	HANDLE hthSnapshot = NULL; 
	MODULEENTRY32 hMod = {sizeof(hMod)}; 
	
	__try 
	{
		// 打开进程. 
		hProcess = OpenProcess(
			PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, 
			FALSE, 
			dwProcessId
			); 
		
		if (hProcess == NULL) 
			__leave;
	

		// 取得FreeLibrary函数在Kernel32.dll中的地址. 
		PTHREAD_START_ROUTINE pfnThreadRtn = 
			(PTHREAD_START_ROUTINE)GetProcAddress( 
			GetModuleHandle(_T("Kernel32.dll")), "FreeLibrary"); 

		if (pfnThreadRtn == NULL) 
			__leave;

		// 取得指定进程的所有模块映象. 
		hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId); 
		if (hthSnapshot == NULL) 
			__leave;
		
		// 取得所有模块列表中的指定的模块. 
		BOOL bMoreMods = Module32First(hthSnapshot, &hMod); 
		if (bMoreMods == FALSE) 
			__leave;
		
		// 循环取得想要的模块. 
		for (;bMoreMods; bMoreMods = Module32Next(hthSnapshot, &hMod)) 
		{ 			
			if ((_tcscmp(hMod.szExePath, lpszLibName) == 0) ||
				(_tcscmp(hMod.szModule, lpszLibName) == 0))
			{
				break; 
			}
		} 
		
		// 创建远程线程来执行FreeLibrary函数. 
		hThread = CreateRemoteThread(hProcess, 
			NULL, 
			0, 
			pfnThreadRtn, 
			hMod.modBaseAddr, 
			0, 
			NULL); 
		if (hThread == NULL) 
			__leave;
		
		// 等待远程线程终止.
		WaitForSingleObject(hThread, INFINITE); 
		
		bResult=TRUE;
		
	}
	__finally 
	{ 
		// 关闭句柄. 
		if (hThread != NULL) 
			CloseHandle(hThread); 

		if (hthSnapshot  != NULL) 
			CloseHandle(hthSnapshot); 
		
		if (hProcess  != NULL) 
			CloseHandle(hProcess); 
	}
	
	return bResult; 
}