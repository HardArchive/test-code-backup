// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <conio.h>
#include <tchar.h>

#include <Windows.h>
#include <iostream>
using namespace std;
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//用ntsd得到程序的DMP
LONG WINAPI ExpFilter1(struct _EXCEPTION_POINTERS *pExp)
{
	char szExec[256];
	//c:\dbgtools\ntsd.exe -p %ld -e %ld -g -c ".dump c:\dumps\jit.dmp;q" 
	sprintf(szExec, "ntsd -c \".dump /f 123.dmp;q\" -p %d", 
		::GetCurrentProcessId());

	WinExec(szExec, SW_SHOWNORMAL);

	Sleep(1000);

	return EXCEPTION_EXECUTE_HANDLER;
}

#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")
#include <Shlwapi.h>
#include "helper.h"
#define CURRENTTIME2 COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")
#include <string>
#include <sstream> 
#include <ATLComTime.h>
using namespace std;
string GetFileName()
{
	TCHAR tszTem[MAX_PATH] = {0};
	if (!GetExePath(tszTem)) return false;
	string strFileName;
	stringstream strStream;  
	strStream <<string(tszTem)<<CURRENTTIME2<<".bmp";  
	strFileName = strStream.str(); 
	return strFileName;	
}
//用Dbghelp.dll提供的MiniDumpWriteDump函数取得程序的DMP
LONG WINAPI ExpFilter2(struct _EXCEPTION_POINTERS *pExp)
{
	TCHAR tszPath[MAX_PATH] = {0};
	
	HANDLE hFile = ::CreateFile(
		_T("tes.dmp"), 
		GENERIC_WRITE, 
		0, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if(INVALID_HANDLE_VALUE != hFile)
	{
		MINIDUMP_EXCEPTION_INFORMATION einfo;
		einfo.ThreadId			= ::GetCurrentThreadId();
		einfo.ExceptionPointers	= pExp;
		//einfo.ClientPointers	= FALSE;
		einfo.ClientPointers	= true;
		//MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory | 
		//	MiniDumpWithDataSegs | 
		//	MiniDumpWithHandleData |
		//	MiniDumpWithFullMemoryInfo | 
		//	MiniDumpWithThreadInfo | 
		//	MiniDumpWithUnloadedModules ); 
		::MiniDumpWriteDump(
			::GetCurrentProcess(), 
			::GetCurrentProcessId(), 
			hFile, 
			/*mdt,*/ MiniDumpWithFullMemory,
			&einfo, 
			NULL, 
			NULL);
		::CloseHandle(hFile);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}


LONG WINAPI ExpFilter(struct _EXCEPTION_POINTERS *pExp)
{
	cout << "Unhandled Exception!!!" << endl;

	return EXCEPTION_EXECUTE_HANDLER;
}


void StartUnhandledExceptionFilter()
{
	::SetUnhandledExceptionFilter(ExpFilter2);
}

int main()
{	
	cout << "begin !" << endl;

	//StartUnhandledExceptionFilter();

	int i = 0;
	i = i / i;

	cout << "end !" << endl;

	getch();

	return 0;
}