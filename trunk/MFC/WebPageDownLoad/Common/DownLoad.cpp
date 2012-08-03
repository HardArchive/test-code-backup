#include "StdAfx.h"
#include "DownLoad.h"
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")   //����վ�����ļ�������Ŀ�
#include <shlwapi.h>  
#pragma comment(lib,"Shlwapi.lib") 


CDownLoad::CDownLoad(void)
{
}

CDownLoad::~CDownLoad(void)
{
}


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
int CDownLoad::DownLoadFile(TCHAR* pInUrl, TCHAR* pInSavePath)
{
	DWORD dwReadLen = 0;               //���ζ�ȡ���ļ�����
	DWORD dwWriteLen = 0;
	BYTE szbuffer[512] = {0};
	HANDLE hFile = NULL;               //�����ļ����
	HINTERNET hNetOpen = NULL;         //WinInet��ʼ�����
	HINTERNET hUrl = NULL;             //URL���

	OutputDebugString(pInUrl);
	OutputDebugString(pInSavePath);
	if (!pInUrl || !ChickDirExist(pInSavePath))
	{
		OutputDebugString(_T("CDownLoad::DownLoadFile input error!"));
		return -1;
	}


	//��ʼ��һ��Ӧ�ó�����ʹ�� WinINet ����
	hNetOpen = InternetOpen(_T("Testing"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hNetOpen == NULL)
	{
		OutputDebugString(_T("CDownLoad::DownLoadFile Internet open failed!"));
		return -2;
	}

	//OutputDebugStringW(_T("��ʼ�����ļ�������ַ; ����·��\r\n"));

	//ͨ��һ��������HTTP��FTP ��Gopher��ַ��һ����Դ
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
	//OutputDebugStringW(_T("���سɹ�����\r\n"));
	//CloseHandle(hFile);
	return true;
}


//��鵱ǰĿ¼��Ҫ���ļ����Ƿ����,����������򴴽���ʧ���򷵻�false
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


	//����·���Ƿ����	
	if (GetFileAttributes(tszPath) != FILE_ATTRIBUTE_DIRECTORY)
	{	
		return CreateDirectory(pInPath, NULL);
	}

	return true;
}