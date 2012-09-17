// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

typedef struct 
{
	FARPROC pfnMessageBox;
	TCHAR	szText[100];
} INJDATA;

typedef VOID (__stdcall * PINJCODE)(PVOID);

BOOL InjectThread(PPROCESS_INFORMATION ppi, PINJCODE pCode, ULONG nCodeLen, PVOID pData, ULONG nDataLen);

VOID WINAPI InjProc(INJDATA *pInjData)
{
	pInjData->pfnMessageBox(0, pInjData->szText, pInjData->szText, MB_OK);
}


int main()
{
	STARTUPINFO			si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	INJDATA InjData;

	InjData.pfnMessageBox = MessageBox;
	strcpy(InjData.szText, _T("Hello"));

	if(CreateProcess(0, _T("notepad.exe"), 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		InjectThread(&pi, InjProc, 100, &InjData, sizeof(InjData));

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	return 0;
}


BOOL InjectThread(PPROCESS_INFORMATION ppi, PINJCODE pCode, ULONG nCodeLen, PVOID pData, ULONG nDataLen)
{
	CONTEXT	context = { CONTEXT_FULL };

	ULONG	cleanup[12];
	ULONG	callenv[3];
	ULONG   stackptr;

	if(-1 == SuspendThread(ppi->hThread))
		return FALSE;

	GetThreadContext(ppi->hThread, &context);

	// construct cleanup frame
	cleanup[11] = context.EFlags;
	cleanup[10] = context.SegCs;
	cleanup[9]  = context.Eip;		// return address for IRETD
	cleanup[8]  = context.Eax;
	cleanup[7]  = context.Ecx;
	cleanup[6]  = context.Edx;
	cleanup[5]  = context.Ebx;
	cleanup[4]  = 0xffffffff;		// esp, ignored by POPAD
	cleanup[3]  = context.Ebp;
	cleanup[2]  = context.Esi;
	cleanup[1]  = context.Edi;
	cleanup[0]  = 0x00CF615C;		// POP ESP / POPAD / IRETD

	stackptr = context.Esp - sizeof(cleanup);
	WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, cleanup, sizeof(cleanup), 0);

	// push user-supplied data
	stackptr -= nDataLen;
	WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, pData, nDataLen, 0);

	// push user-supplied code
	stackptr= (stackptr - nCodeLen) & ~0x1f;
	WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, pCode, nCodeLen, 0);

	// push code environment
	callenv[2] = context.Esp - sizeof(cleanup) + 4;			// ESP for executing cleanup
	callenv[1] = context.Esp - sizeof(cleanup) - nDataLen;	// Argument - pointer to user data
	callenv[0] = context.Esp - sizeof(cleanup);				// Return address - to cleanup code

	stackptr -= sizeof(callenv);
	WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, callenv, sizeof(callenv), 0);

	// resume execution on thread's stack
	context.Esp = stackptr;
	context.Eip = stackptr + sizeof(callenv);

	FlushInstructionCache(ppi->hProcess, (PVOID)context.Eip, nCodeLen);
	SetThreadContext(ppi->hThread, &context);

	ResumeThread(ppi->hThread);

	return TRUE;
}