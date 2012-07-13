// testtoo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <WinInet.h>


int DownLoadFile(char* strFileUrl, char* strLocalPath)
{
	OutputDebugString(strFileUrl);
	HINTERNET hInternet = InternetOpen("Testing", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	//��ʼ��һ��Ӧ�ó�����ʹ�� WinINet ������
	if (hInternet == NULL)
	{
		return 1;
	}
	HINTERNET hRequest =InternetOpenUrl(hInternet, strFileUrl, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	//ͨ��һ��������FTP��Gopher��HTTP��ַ��һ����Դ
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
		OutputDebugString("�����ļ�ʧ��");
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

