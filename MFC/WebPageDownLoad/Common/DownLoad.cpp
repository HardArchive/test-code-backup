#include "StdAfx.h"
#include "DownLoad.h"
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")   //从网站下载文件需包含的库
#include <shlwapi.h>  
#pragma comment(lib,"Shlwapi.lib") 


CDownLoad::CDownLoad(void)
{
}

CDownLoad::~CDownLoad(void)
{
}


/******************************************************************************* 
1、 函数名称： DownLoadFile
2、 功能描述： 从指定网址下载文件保存到指定目录 
3、 输入参数： TCHAR* pInUrl 需下载的网址, TCHAR* pInSavePath 保存的本地完整路径
4、 返 回 值： 成功返回1 ，失败回返-数
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2012-7-2 11:00:51
8、 备    注： 
********************************************************************************/ 
int CDownLoad::DownLoadFile(TCHAR* pInUrl, TCHAR* pInSavePath)
{
	DWORD dwReadLen = 0;               //本次读取的文件长度
	DWORD dwWriteLen = 0;
	BYTE szbuffer[512] = {0};
	HANDLE hFile = NULL;               //本地文件句柄
	HINTERNET hNetOpen = NULL;         //WinInet初始化句柄
	HINTERNET hUrl = NULL;             //URL句柄

	OutputDebugString(pInUrl);
	OutputDebugString(pInSavePath);
	if (!pInUrl || !ChickDirExist(pInSavePath))
	{
		OutputDebugString(_T("CDownLoad::DownLoadFile input error!"));
		return -1;
	}


	//初始化一个应用程序，以使用 WinINet 函数
	hNetOpen = InternetOpen(_T("Testing"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hNetOpen == NULL)
	{
		OutputDebugString(_T("CDownLoad::DownLoadFile Internet open failed!"));
		return -2;
	}

	//OutputDebugStringW(_T("开始下载文件……网址; 保存路径\r\n"));

	//通过一个完整的HTTP、FTP 或Gopher网址打开一个资源
	hUrl = InternetOpenUrl(hNetOpen, pInUrl, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hUrl == NULL)
	{	
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hNetOpen);
		OutputDebugString(_T("CDownLoad::DownLoadFile Internet open url failed!"));
		return -3;
	}
	hFile = CreateFile(pInSavePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hNetOpen);
		OutputDebugString(_T("CDownLoad::DownLoadFile Create File failed!"));
		return -4;
	}

	while(true)
	{
		ZeroMemory(szbuffer, sizeof(szbuffer));
		if (!InternetReadFile(hUrl, szbuffer, sizeof(szbuffer), &dwReadLen))
		{
			CloseHandle(hFile);
			InternetCloseHandle(hUrl);
			InternetCloseHandle(hNetOpen);
			OutputDebugString(_T("CDownLoad::DownLoadFile Internet Read File failed!"));
			return -5;
		}

		if(dwReadLen <= 0)
		{	   	 
			InternetCloseHandle(hUrl);
			InternetCloseHandle(hNetOpen);
			CloseHandle(hFile);
			break;
		}

		if (!WriteFile(hFile, szbuffer, dwReadLen, &dwWriteLen, NULL))
		{		
			InternetCloseHandle(hUrl);
			InternetCloseHandle(hNetOpen);
			CloseHandle(hFile);
			OutputDebugString(_T("CDownLoad::DownLoadFile Write File failed!"));
			return -6;
		}
	}
	//OutputDebugStringW(_T("下载成功……\r\n"));
	//CloseHandle(hFile);
	return true;
}


//检查当前目录需要的文件夹是否存在,如果不存在则创建，失败则返回false
bool CDownLoad::ChickDirExist(TCHAR* pInPath)
{
	TCHAR* ptFileName = NULL;
	TCHAR tszPath[MAX_PATH] = {0};
	if (!pInPath || _tcsclen(pInPath)<4)
	{
		return false;
	}

	ptFileName = _tcsrchr(pInPath, '\\');
	_tcsncpy(tszPath, pInPath, _tcslen(pInPath)- _tcslen(ptFileName));


	//检验路径是否存在	
	if (GetFileAttributes(tszPath) != FILE_ATTRIBUTE_DIRECTORY)
	{	
		return CreateDirectory(pInPath, NULL);
	}

	return true;
}