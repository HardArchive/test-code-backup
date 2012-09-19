// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

int GetUrl(TCHAR* ptszInFullPath, TCHAR* ptszOutUrl)
{
	TCHAR tszFullPath[MAX_PATH] = {0};
	TCHAR tszTem[1024] = {0};
	if (!ptszInFullPath)
	{
		return -1;
	}

	GetPrivateProfileString(_T("InternetShortcut"),
		_T("URL"),
		_T(""),
		tszTem, 
		1023,
		ptszInFullPath);
	if (!_tcslen(tszTem))
	{
		return -2;
	}
	_tcscpy(ptszOutUrl, tszTem);
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR tszFullPath[MAX_PATH] = {0};
	TCHAR tszUrl[MAX_PATH] = {0};
	_tcscpy(tszFullPath, _T("D:\\WinPath\\Favorites\\代码发芽网.url"));
	GetUrl(tszFullPath, tszUrl);

	getchar();


	return 0;
}

