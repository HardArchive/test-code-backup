#include "stdafx.h"
#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

void CreateMiniDump3( LPEXCEPTION_POINTERS lpExceptionInfo) {
	// Open a file
	HANDLE hFile = CreateFile(_T("MiniDump.dmp"), GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if ( hFile != NULL &&  hFile != INVALID_HANDLE_VALUE ) {

		// Create the minidump 
		MINIDUMP_EXCEPTION_INFORMATION mdei;
		mdei.ThreadId          = GetCurrentThreadId();
		mdei.ExceptionPointers = lpExceptionInfo;
		mdei.ClientPointers    = FALSE;

		MINIDUMP_TYPE mdt      = MiniDumpNormal;
		BOOL retv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
			hFile, mdt, ( lpExceptionInfo != 0 ) ? &mdei : 0, 0, 0);

		if ( !retv ) {
			_tprintf( _T("MiniDumpWriteDump failed. Error: %u \n"), GetLastError() ); 
		} else { 
			_tprintf( _T("Minidump created.\n") ); 
		}

		// Close the file
		CloseHandle( hFile );

	} else {
		_tprintf( _T("CreateFile failed. Error: %u \n"), GetLastError() ); 
	}
}


int _tmain3(int argc, _TCHAR* argv[])
{
	__try {
		int * p = NULL;
		*p = 20;
	}
	__except(CreateMiniDump3(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {
	}

	return 0;
}
