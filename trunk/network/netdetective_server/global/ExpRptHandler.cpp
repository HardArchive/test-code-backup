#include "StdAfx.h"
#include "ExpRptHandler.h"

TCHAR CExpRptHandler::m_tszdmpFileName[MAX_PATH*2];
LPTOP_LEVEL_EXCEPTION_FILTER CExpRptHandler::m_pFrontFilter;

CExpRptHandler g_clsExpRptHandler;

CExpRptHandler::CExpRptHandler(void)
{
	SYSTEMTIME	systime = {0};
	TCHAR		tszDataBuffer[MAX_PATH] = {0};
	m_pFrontFilter = SetUnhandledExceptionFilter(ExpFilter);

	memset(m_tszdmpFileName, 0, sizeof(TCHAR)*MAX_PATH*2);
	GetModuleFileName(0, m_tszdmpFileName, MAX_PATH);

	GetLocalTime(&systime);
	_stprintf_s(tszDataBuffer, MAX_PATH, TEXT("(%04d-%02d-%02d_%02d-%02d-%02d).dmp"), 
		systime.wYear, systime.wMonth, systime.wDay, systime.wHour,
		systime.wMinute, systime.wSecond);

	PTSTR pszDot = _tcsrchr(m_tszdmpFileName, _T('.')); 
	if (pszDot)
	{
		if (_tcslen(pszDot) >= 3)
			_tcscpy_s(pszDot, MAX_PATH, tszDataBuffer);
	}
}

CExpRptHandler::~CExpRptHandler(void)
{
	SetUnhandledExceptionFilter(m_pFrontFilter);
}

//用Dbghelp.dll提供的MiniDumpWriteDump函数取得程序的DMP
LONG WINAPI CExpRptHandler::ExpFilter(PEXCEPTION_POINTERS lpExceptionInfo)
{
	MINIDUMP_EXCEPTION_INFORMATION mdei = {0};
	MINIDUMP_TYPE	mdt;
	BOOL			retv = FALSE;
	HANDLE			hFile = 0;

	hFile = CreateFile(m_tszdmpFileName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE )
		return FALSE;

	mdei.ThreadId			= GetCurrentThreadId();
	mdei.ExceptionPointers	= lpExceptionInfo;
	mdei.ClientPointers		= FALSE;

	//mdt	= MiniDumpWithFullMemory; 
	//mdt       = MiniDumpNormal; 
	mdt       = (MINIDUMP_TYPE)(MiniDumpWithFullMemory | 
		MiniDumpWithFullMemoryInfo | 
		MiniDumpWithHandleData | 
		MiniDumpWithThreadInfo | 
		MiniDumpWithUnloadedModules );

	retv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hFile, mdt, ( lpExceptionInfo != 0 ) ? &mdei : 0, 0, 0);

	CloseHandle(hFile);
	return (LONG)retv;
}