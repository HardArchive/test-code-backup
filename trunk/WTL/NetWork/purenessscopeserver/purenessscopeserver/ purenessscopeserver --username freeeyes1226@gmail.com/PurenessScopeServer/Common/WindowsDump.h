#ifndef _WINDOWSDUMP_H
#define _WINDOWSDUMP_H

#ifdef WIN32

#include <Windows.h>
#include <dbghelp.h>

#define DUMP_FILE ".\\WindowsP.dmp"

void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)  
{  
	// ����Dump�ļ�  
	//  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  

	// Dump��Ϣ  
	//  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
	dumpInfo.ExceptionPointers = pException;  
	dumpInfo.ThreadId = GetCurrentThreadId();  
	dumpInfo.ClientPointers = TRUE;  

	// д��Dump�ļ�����  
	//  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);  

	CloseHandle(hDumpFile);  
}  

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)  
{     
	// ���ﵯ��һ������Ի����˳�����  
	CreateDumpFile(DUMP_FILE, pException);
	FatalAppExit(-1,  "*** Unhandled Exception! ***");  

	return EXCEPTION_EXECUTE_HANDLER;  
}  

#endif

#endif
