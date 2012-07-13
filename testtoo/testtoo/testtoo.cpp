// testtoo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinInet.h>


int DownLoadFile(char* strFileUrl, char* strLocalPath)
{
	OutputDebugString(strFileUrl);
	HINTERNET hInternet = InternetOpen("Testing", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	//初始化一个应用程序，以使用 WinINet 函数。
	if (hInternet == NULL)
	{
		return 1;
	}
	HINTERNET hRequest =InternetOpenUrl(hInternet, strFileUrl, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	//通过一个完整的FTP，Gopher或HTTP网址打开一个资源
	if (hRequest == NULL)
	{
		return 1;
	}
	char FileSize[1024] = {0};
	DWORD size = sizeof(FileSize);
	DWORD FileLength = 0;
	if (HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, FileSize, &size, NULL))
	{
		FileLength = atol(FileSize);
	}
	else
	{
		return 1;
	}
	CString  strTargetDir;
	strTargetDir = strLocalPath;
	CFile FileSource;
	BOOL Istrue = FileSource.Open(strTargetDir, CFile::modeCreate | CFile::modeReadWrite);
	if ( Istrue == 0 ) 
	{
		OutputDebugString("创建文件失败");
		return 1;
	}
	char FileBuffer[1024*100];
	DWORD  dwSize;
	while(1)
	{
		memset(FileBuffer,0,1024*100);
		dwSize = 0;
		if (InternetReadFile(hRequest, FileBuffer, 1024*100-1, &dwSize))
		{
			if (dwSize <= 0)
			{
				break;
			}
			FileSource.Write(FileBuffer, dwSize);
		}
		else
		{
			return 1;
		}
	}
	FileSource.Close();
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

