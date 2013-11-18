// stdafx.cpp : source file that includes just the standard includes
//	QQ.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void GC_TRACE(LPCTSTR lpstrTrace,...)
{
	va_list args;
	int len;
	LPTSTR buffer;

	va_start( args, lpstrTrace );
	len = _vsctprintf( lpstrTrace, args ) // _vscprintf doesn't count
		+ 1; // terminating '\0'
	buffer = (LPTSTR)malloc( len * sizeof(LPTSTR) );
	_vstprintf( buffer, lpstrTrace, args );
	::OutputDebugStringA(buffer);

	//errLog("d:\\zzy.log",COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S").GetBuffer());
	//errLogLn("d:\\zzy.log",buffer);
	free(buffer);
	return;
}