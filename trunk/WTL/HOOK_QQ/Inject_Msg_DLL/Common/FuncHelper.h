#pragma once

//ʹ��string ֧�ֿ�խ�ַ�
#include <string>
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
using namespace std;
#include <TlHelp32.h>
//��ȡ��Ӧ��������ID 
inline DWORD GetSpecifiedProcessId(tstring& tstrProcessName) 
{ 
	DWORD dwID = 0; 
	//���ϵͳ���վ�� (ͨ�׵Ľ�, ���ǵõ���ǰ�����н���) 
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	PROCESSENTRY32 pInfo; //���ڱ��������Ϣ��һ�����ݽṹ 
	pInfo.dwSize = sizeof(pInfo); 
	//�ӿ����л�ȡ�����б� 
	Process32First(hSnapShot, &pInfo) ; //�ӵ�һ�����̿�ʼѭ�� 
	do 
	{ 
		//tstrProcessNameΪ��Ľ������� 				
		if(0 == tstrProcessName.compare(tstring(pInfo.szExeFile))) 
		{ 
			dwID = pInfo.th32ProcessID ; 
			break ; 
		} 
	}while(Process32Next(hSnapShot, &pInfo) != false); 
	return dwID; //dwID ������Ҫ�Ľ���PID ��.. 
}


#include "CWindowIterator.h"
inline HWND GetSpecifiedProcessHwnd(const DWORD dwProcessID)
{
	if (dwProcessID<0)
	{
		return NULL;
	}
	CMainHWnd clsCMainHWnd;
	clsCMainHWnd.EnumWndsByPid(dwProcessID);
	HWND hWnd = clsCMainHWnd.GetHWnd(0);
	return hWnd;
}

inline DWORD GetSpecifiedThreadID(const DWORD dwProcessID)
{
	DWORD pTID;
	_asm 
	{ 
		mov eax, fs:[0x18] 
		add eax, 36 
		mov [pTID], eax 
	} 
	DWORD dwThreadID = 0;
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ, false, dwProcessID); 
	ReadProcessMemory(hProcess, (const void *)pTID, &dwThreadID, 4, NULL); 
	CloseHandle(hProcess); 

	return dwThreadID; 
}

//DWORD GetTargetThreadIdFromProcname(char *procName) 
//{ 
//	PROCESSENTRY32 pe; 
//	HANDLE thSnapshot, hProcess; 
//	BOOL retval, ProcFound = false; 
//	DWORD pTID, dwThreadID; 
//
//	thSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
//
//	if(thSnapshot == INVALID_HANDLE_VALUE) 
//	{ 
//		MessageBox(NULL, 
//			"Error: unable to create toolhelp snapshot", 
//			"Loader", 
//			NULL
//			); 
//		return false; 
//	} 
//
//	pe.dwSize = sizeof(PROCESSENTRY32); 
//
//	retval = Process32First(thSnapshot, &pe); 
//
//	while(retval) 
//	{ 
//		if(StrStrI(pe.szExeFile, procName) ) 
//		{ 
//			ProcFound = true; 
//			break; 
//		} 
//
//		retval    = Process32Next(thSnapshot,&pe); 
//		pe.dwSize = sizeof(PROCESSENTRY32); 
//	} 
//
//	CloseHandle(thSnapshot); 
//
//	_asm { 
//		mov eax, fs:[0x18] 
//		add eax, 36 
//			mov [pTID], eax 
//	} 
//
//	hProcess = OpenProcess(PROCESS_VM_READ, false, pe.th32ProcessID); 
//	ReadProcessMemory(hProcess, (const void *)pTID, &dwThreadID, 4, NULL); 
//	CloseHandle(hProcess); 
//
//	return dwThreadID; 
//}


extern HHOOK g_hHook = NULL;
extern HMODULE g_hModule = NULL;

//ж�ع���
inline bool UninstallHook()
{
	bool bRet = false;
	if (!g_hHook)
	{
		return bRet;
	}

	if (!g_hModule)
	{
		g_hModule = LoadLibrary(_T("spyxhk.dll"));
		//return NULL;
	}

	typedef BOOL (__stdcall* EndHook_TYPE)(HHOOK);
	EndHook_TYPE EndHook = (EndHook_TYPE)GetProcAddress(g_hModule, "EndHook");

	if (!EndHook)
	{
		DWORD dwErrorCode = GetLastError();
		return bRet;
	}
	bRet = EndHook(g_hHook);

	FreeLibrary(g_hModule);
	return bRet;
}

//ͨ�����ھ����װ����
inline HHOOK InstallHook(const HWND hWnd, const DWORD dwProcessID)
{
	if (!hWnd)
	{
		return NULL;
	}

	if (!g_hModule)
	{
		g_hModule = LoadLibrary(_T("spyxhk.dll"));
		//return NULL;
	}

	typedef HHOOK (__stdcall * InitHookProc_TYPE)(HWND, DWORD);
	InitHookProc_TYPE InitHookProc = (InitHookProc_TYPE)GetProcAddress(g_hModule, "InitHookProc");

	if (!InitHookProc)
	{
		DWORD dwErrorCode = GetLastError();

		return NULL;
	}

	if (g_hHook)
	{
		UninstallHook();
		g_hHook = NULL;
	}


	//g_hHook = InitHookProc(hWnd, GetSpecifiedThreadID(dwProcessID));
	g_hHook = InitHookProc(hWnd, GetWindowThreadProcessId(hWnd, 0));

	DWORD dwErrorCode = GetLastError();
	return g_hHook;
}
