//file MessageboxEx.h 

#ifndef __MESSAGEBOXEX_HEADER_
#define __MESSAGEBOXEX_HEADER_
namespace QNA
{
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
	int CDECL MessageBoxEx(HWND hWnd, UINT uType, TCHAR * szCaption, TCHAR * szFormat, ...)
	{
		TCHAR szBuffer [1024] = {0};
		va_list pArgList ;
		va_start (pArgList, szFormat);
		_vsntprintf (szBuffer, sizeof (szBuffer) / sizeof (TCHAR), 
			szFormat, pArgList);
		va_end (pArgList) ;
		return MessageBox(hWnd, szBuffer, szCaption, uType);
	}
}
#endif

