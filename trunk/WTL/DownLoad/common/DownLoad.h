#pragma once
/******************************************************************************* 
* 1、 文件名称： DownLoad
* 2、 版　　本： Version *.*
* 3、 描    述：网页文件下载类，通过WinInt实现 
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-7-2 13:06:49
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")   //从网站下载文件需包含的库
#include <shlwapi.h>  
#pragma comment(lib,"Shlwapi.lib") 
#include "DebugTrace.h"

namespace QNA
{
	class CDownLoad
	{
	public:
		CDownLoad(void)
		{
			//m_hNetOpen = NULL;         //WinInet初始化句柄
			//m_hUrl = NULL;             //URL句柄
		}
		~CDownLoad(void)
		{
			//InternetCloseHandle(m_hUrl);
			//InternetCloseHandle(m_hNetOpen);
			//m_hNetOpen = NULL;         //WinInet初始化句柄
			//m_hUrl = NULL;             //URL句柄
		}

	public:
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
		int DownLoadFile(TCHAR* pInUrl, TCHAR* pInSavePath)
		{
			DWORD dwFlags = 0;
			DWORD dwReadLen = 0;               //本次读取的文件长度
			DWORD dwWriteLen = 0;
			HINTERNET hNetOpen = NULL;         //WinInet初始化句柄
			HINTERNET hUrl = NULL;             //URL句柄
			BYTE szbuffer[512] = {0};
			HANDLE hFile = NULL;               //本地文件句柄

			TRACE(pInUrl);
			TRACE(pInSavePath);
			if (!pInUrl || !ChickDirExist(pInSavePath))
			{
				TRACE(_T("CDownLoad::DownLoadFile input error!"));
				return -1;
			}

			if (!InternetGetConnectedState(&dwFlags, 0))
			{
				TRACE(_T("CDownLoad::DownLoadFile InternetGetConnectedState Error Code:%d!\r\n"), GetLastError());
				return -2;
			}

			//初始化一个应用程序，以使用 WinINet 函数
			hNetOpen = InternetOpen(_T("DownLoad"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

			if (NULL == hNetOpen)
			{
				TRACE(_T("CDownLoad::DownLoadFile 连接错误!!!"));
				return -3;
			}
			////设置超时
			//DWORD dwTimeOut = 30;
			//if( !InternetSetOption(hNetOpen, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut)) )
			//{
			//	printf("set timeout failed:%d\n", GetLastError());
			//	InternetCloseHandle(hNetOpen);
			//	return -4;
			//}

			//通过一个完整的HTTP、FTP 或Gopher网址打开一个资源
			hUrl = InternetOpenUrl(hNetOpen, pInUrl, NULL, 0, 
				INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
			if (hUrl == NULL)
			{	
				TRACE(_T("CDownLoad::DownLoadFile 不能打开该URL!！!"));
				return -5;
			}

			//DWORD dwBytesToRead =0;   //文件长度
			//DWORD dwSizeOfRq = sizeof(dwBytesToRead);
			//DWORD dwIndex=0;

			////查询Internet文件的长度信息
			//if( !( HttpQueryInfo(hUrl, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,
			//	(LPVOID)&dwBytesToRead, &dwSizeOfRq, &dwIndex) ) )
			//{
			//	printf("HttpQueryInfo failed:%d\n",GetLastError());
			//	dwBytesToRead = 0;
			//	CloseHandle(hFile);
			//	InternetCloseHandle(hUrl);
			//	InternetCloseHandle(hNetOpen);
			//	return -6;
			//}
			//TRACE(_T("CDownLoad::DownLoadFile 需下载的文件总长度为:%dKB!\r\n"), dwBytesToRead/1024);
			

			hFile = CreateFile(pInSavePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				CloseHandle(hFile);
				InternetCloseHandle(hUrl);
				InternetCloseHandle(hNetOpen);
				TRACE(_T("CDownLoad::DownLoadFile Create File failed!"));
				return -7;
			}

			while(true)
			{
				ZeroMemory(szbuffer, sizeof(szbuffer));
				if (!InternetReadFile(hUrl, szbuffer, sizeof(szbuffer), &dwReadLen))
				{
					CloseHandle(hFile);
					InternetCloseHandle(hUrl);
					InternetCloseHandle(hNetOpen);
					TRACE(_T("CDownLoad::DownLoadFile Internet Read File failed!"));
					return -8;
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
					TRACE(_T("CDownLoad::DownLoadFile Write File failed!"));
					return -9;
				}
			}
			//TRACE(_T("下载成功……\r\n"));
			//CloseHandle(hFile);
			return 1;
		}

	private:
		//检查当前目录需要的文件夹是否存在,如果不存在则创建，失败则返回false
		bool ChickDirExist(TCHAR* pInPath)
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
	//	DWORD QueryFileLen()
	//	{
	//		DWORD dwFileLen = 0;
	//		//查询Internet文件的长度信息
	//		if( !( HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,
	//			(LPVOID)&dwFileLen, &dwSizeOfRq, &dwIndex) ) )
	//		{
	//			printf("HttpQueryInfo failed:%d\n",GetLastError());
	//			dwBytesToRead = 0;
	//			fclose(fp);
	//			InternetCloseHandle(hOpen);
	//			return -1;
	//		}
	//		printf("Avaliable data:%u bytes\n",dwBytesToRead);


	//		return dwFileLen;
	//	}
	//private:
	//	HINTERNET m_hNetOpen;         //WinInet初始化句柄
	//	HINTERNET m_hUrl;             //URL句柄
	};
}
