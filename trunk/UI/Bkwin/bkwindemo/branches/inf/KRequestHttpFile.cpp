#include "StdAfx.h"
#include "KRequestHttpFile.h"
#include "KStreamWrapper.h"
#include "WinHttpWrapper.h"

// 一个进程内只能有一个WinHttpWrapper对象，不能多次加载winhttp.dll wininet.dll;
WinHttpWrapper g_WinHttpWrapper;
BOOL		   g_bWinHttpInited = FALSE;

BOOL WinHttp_Initialize()
{
	g_bWinHttpInited = g_WinHttpWrapper.Initialize();
	return g_bWinHttpInited;
}

BOOL WinHttp_Uninitialize()
{
	return g_WinHttpWrapper.Uninitialize();
}

BOOL WinHttp_RequestHttpFile(IN LPCWSTR lpszUrl)
{
	BOOL bSuccess = FALSE;

	if (g_bWinHttpInited)
		bSuccess = g_WinHttpWrapper.SendHttpRequest(lpszUrl, NULL, NULL);

	return bSuccess;
}

BOOL WinHttp_GetHttpFileSize(IN LPCWSTR lpszUrl, IN OUT DWORD& dwFileSize)
{
	BOOL bSuccess = FALSE;

	if (g_bWinHttpInited)
		bSuccess = g_WinHttpWrapper.GetHttpFileSize(lpszUrl, dwFileSize);

	return bSuccess;
}

BOOL WinHttp_RequestHttpFile(IN LPCWSTR lpszUrl, 
							 IN LPCWSTR lpszFile, 
							 IN IStatusCallback * _pIStatusCallback/* = NULL*/,
							 IN BOOL bIsNeedContinue,
							 IN int nDownSpeed)
{
	BOOL bSuccess = FALSE;

	if (g_bWinHttpInited)
	{
		size_t nSize = 0;
		//KISUP::IO::File::GetFileSize(lpszFile, nSize);

		long lFileSize = 0;
		CFileOutStream _OutStream(lpszFile, FALSE);

		if (bIsNeedContinue)
			_OutStream.Seek(0, FILE_END);
		else 
			nSize = 0;
		

		bSuccess = g_WinHttpWrapper.SendHttpRequestEx(lpszUrl, nSize, &_OutStream, _pIStatusCallback);
	}

	return bSuccess;
}
