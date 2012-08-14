#pragma once
/******************************************************************************* 
* 1�� �ļ����ƣ� DownLoad
* 2�� �桡������ Version *.*
* 3�� ��    ������ҳ�ļ������࣬ͨ��WinIntʵ�� 
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-7-2 13:06:49
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")   //����վ�����ļ�������Ŀ�
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
			//m_hNetOpen = NULL;         //WinInet��ʼ�����
			//m_hUrl = NULL;             //URL���
		}
		~CDownLoad(void)
		{
			//InternetCloseHandle(m_hUrl);
			//InternetCloseHandle(m_hNetOpen);
			//m_hNetOpen = NULL;         //WinInet��ʼ�����
			//m_hUrl = NULL;             //URL���
		}

	public:
		/******************************************************************************* 
		1�� �������ƣ� DownLoadFile
		2�� ���������� ��ָ����ַ�����ļ����浽ָ��Ŀ¼ 
		3�� ��������� TCHAR* pInUrl �����ص���ַ, TCHAR* pInSavePath ����ı�������·��
		4�� �� �� ֵ�� �ɹ�����1 ��ʧ�ܻط�-��
		5�� ��̬�ڴ棺 ��
		6�� ������ƣ�  ��ë
		7�� �������ڣ� 2012-7-2 11:00:51
		8�� ��    ע�� 
		********************************************************************************/ 
		int DownLoadFile(TCHAR* pInUrl, TCHAR* pInSavePath)
		{
			DWORD dwFlags = 0;
			DWORD dwReadLen = 0;               //���ζ�ȡ���ļ�����
			DWORD dwWriteLen = 0;
			HINTERNET hNetOpen = NULL;         //WinInet��ʼ�����
			HINTERNET hUrl = NULL;             //URL���
			BYTE szbuffer[512] = {0};
			HANDLE hFile = NULL;               //�����ļ����

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

			//��ʼ��һ��Ӧ�ó�����ʹ�� WinINet ����
			hNetOpen = InternetOpen(_T("DownLoad"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

			if (NULL == hNetOpen)
			{
				TRACE(_T("CDownLoad::DownLoadFile ���Ӵ���!!!"));
				return -3;
			}
			////���ó�ʱ
			//DWORD dwTimeOut = 30;
			//if( !InternetSetOption(hNetOpen, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut)) )
			//{
			//	printf("set timeout failed:%d\n", GetLastError());
			//	InternetCloseHandle(hNetOpen);
			//	return -4;
			//}

			//ͨ��һ��������HTTP��FTP ��Gopher��ַ��һ����Դ
			hUrl = InternetOpenUrl(hNetOpen, pInUrl, NULL, 0, 
				INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
			if (hUrl == NULL)
			{	
				TRACE(_T("CDownLoad::DownLoadFile ���ܴ򿪸�URL!��!"));
				return -5;
			}

			//DWORD dwBytesToRead =0;   //�ļ�����
			//DWORD dwSizeOfRq = sizeof(dwBytesToRead);
			//DWORD dwIndex=0;

			////��ѯInternet�ļ��ĳ�����Ϣ
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
			//TRACE(_T("CDownLoad::DownLoadFile �����ص��ļ��ܳ���Ϊ:%dKB!\r\n"), dwBytesToRead/1024);
			

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
			//TRACE(_T("���سɹ�����\r\n"));
			//CloseHandle(hFile);
			return 1;
		}

	private:
		//��鵱ǰĿ¼��Ҫ���ļ����Ƿ����,����������򴴽���ʧ���򷵻�false
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


			//����·���Ƿ����	
			if (GetFileAttributes(tszPath) != FILE_ATTRIBUTE_DIRECTORY)
			{	
				return CreateDirectory(pInPath, NULL);
			}

			return true;
		}
	//	DWORD QueryFileLen()
	//	{
	//		DWORD dwFileLen = 0;
	//		//��ѯInternet�ļ��ĳ�����Ϣ
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
	//	HINTERNET m_hNetOpen;         //WinInet��ʼ�����
	//	HINTERNET m_hUrl;             //URL���
	};
}
