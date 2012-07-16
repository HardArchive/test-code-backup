// stdafx.cpp : 只包括标准包含文件的源文件
// PhoneClient.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
// 
CLog_T g_objLog;   //日志类

//增加调试模式标志
BOOL g_bRunInDebugMode = false;
char g_szMinitorFolderPah[MAX_PATH] = {0};

void TracePrint(int nLogLevel, LPCTSTR lpstrTrace,... )
{
	if (0 == (nLogLevel & LOG_DEFAULT_LEVEL)) return;

	va_list args;
	int len;
	LPTSTR buffer;

	va_start( args, lpstrTrace );

	try
	{
		// _vscprintf doesn't count // terminating '\0'
		len = _vsctprintf( lpstrTrace, args ) + 1; 

		buffer = (LPTSTR)malloc( len * sizeof(LPTSTR) );
		_vstprintf( buffer, lpstrTrace, args );

		// 李照修改 2010-08-03 调试模式时直接输出串到控制台
		if (g_bRunInDebugMode) _tprintf(buffer);
		else OutputDebugString(buffer);

		free(buffer);
	}
	catch (...)
	{
	}
}