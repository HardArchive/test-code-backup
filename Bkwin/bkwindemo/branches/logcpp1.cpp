#include "stdafx.h"
#include "logcpp1.h"
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

TCHAR g_szLogFilePath1[MAX_PATH] = _T("..\\patch.log");

DWORD __GetFileSize(LPCTSTR szPathName)
{
	DWORD dwFileSize = 0;
	HANDLE hFile = ::CreateFile(
		szPathName, 
		GENERIC_READ, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	dwFileSize = ::GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE)
	{
		dwFileSize = 0;
	}

	CloseHandle(hFile);

	return dwFileSize;
}

void set_log_file1(const TCHAR* szPath)
{
	if (szPath && _tcslen(szPath) < MAX_PATH)
	{
		_tcscpy(g_szLogFilePath1, szPath);
	}

	DWORD dwFileSize = __GetFileSize(g_szLogFilePath1);
	if (dwFileSize > 1* 1024 * 1024)
	{
		::DeleteFile(g_szLogFilePath1);
	}
}

int log_a1(const char* format, ...)
{
	int n_return = 0;
	const int cn_buf_len = 500;
	char log_buffer[cn_buf_len] = {0};
	va_list arglist;
	FILE* fp = NULL;
	
	va_start(arglist, format);
	n_return = _vsnprintf(log_buffer, cn_buf_len - 1, format, arglist);
	va_end(arglist);

	fp = _tfopen(g_szLogFilePath1, _T("a+"));
	if (!fp) return 0;

	SYSTEMTIME sys_time;
	::GetLocalTime(&sys_time);
	char time_buffer[50] = {0};
	sprintf(time_buffer, "%d-%d-%d %d:%d:%d\t", sys_time.wYear, sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute, sys_time.wSecond);
	fprintf(fp, "%s%s", time_buffer, log_buffer);
	fclose(fp);

	return n_return;
}

int log_w1(const wchar_t* format, ...)
{
	int n_return = 0;
	const int cn_buf_len = 500;
	wchar_t log_buffer[cn_buf_len] = {0};
	va_list arglist;
	FILE* fp = NULL;

	va_start(arglist, format);
	n_return = _vsnwprintf(log_buffer, cn_buf_len - 1, format, arglist);
	va_end(arglist);

	fp = _tfopen(g_szLogFilePath1, _T("a+"));
	if (!fp) return 0;

	SYSTEMTIME sys_time;
	::GetLocalTime(&sys_time);
	wchar_t time_buffer[50] = {0};
	wsprintf(time_buffer, L"%d-%d-%d %d:%d:%d\t", sys_time.wYear, sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute, sys_time.wSecond);
	n_return = fwprintf(fp, L"%s%s", time_buffer, log_buffer);
	fclose(fp);

	return n_return;

}