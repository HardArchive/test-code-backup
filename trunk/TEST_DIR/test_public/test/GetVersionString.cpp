//
//	GetVersionString.c
//
//	Retrieve strings from a file's version information
//
//	www.catch22.net
//
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "version.lib")

//
//	Get the specified file-version information string from a file
//	
//	szItem	- version item string, e.g:
//		"FileDescription", "FileVersion", "InternalName", 
//		"ProductName", "ProductVersion", etc  (see MSDN for others)
//
TCHAR *GetVersionString(TCHAR *szFileName, TCHAR *szValue, TCHAR *szBuffer, ULONG nLength)
{
	DWORD  len;
	PVOID  ver;	
	DWORD  *codepage;
	char   fmt[0x40];
	PVOID  ptr = 0;
	BOOL   result = FALSE;
	
	szBuffer[0] = '\0';

	len = GetFileVersionInfoSize(szFileName, 0);

	if(len == 0 || (ver = malloc(len)) == 0)
		return NULL;

	if(GetFileVersionInfo(szFileName, 0, len, ver))
	{
		//�Ӱ汾��Դ�л�ȡ��Ϣ�������������ǰ����������GetFileVersionInfo������ȡ�汾��Դ��Ϣ��
		// �������������Դ��Ϣ��������Ҫ�����ݸ��Ƶ�һ����������
		//"\" ��ȡ�ļ���VS_FIXEDFILEINFO�ṹ
		//"\VarFileInfo\Translation" ��ȡ�ļ��ķ����
		//"\StringFileInfo\...." ��ȡ�ļ����ִ���Ϣ���ο�ע��
		if(VerQueryValue(ver, _T("\\VarFileInfo\\Translation"), (void **)&codepage, (PUINT)&len))
		{
			wsprintf(fmt, _T("\\StringFileInfo\\%04x%04x\\%s"), (*codepage) & 0xFFFF, 
					(*codepage) >> 16, szValue);
			
			if(VerQueryValue(ver, fmt, &ptr, (PUINT)&len))
			{
				lstrcpyn(szBuffer, (TCHAR*)ptr, min(nLength, len));
				result = TRUE;
			}
		}
	}

	free(ver);
	return result ? szBuffer : NULL;
}


/******************************************************************************* 
1�� �������ƣ� GetFileVersion
2�� ���������� ��ȡ�����ļ��İ汾����
3�� ��������� char *ptInPath �ļ�·��
			  char* ptOutFVer,�ļ��汾ָ��
			  ULONG& ulOutLen  �ļ��汾buffer�ĳ���
4�� �� �� ֵ�� �ɹ�����true��ʧ�ܷ���false
5�� ��̬�ڴ棺 ��  Ҫע���ͷ�
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-9-7 9:47:20
8�� ��    ע�� �˺����ڶ����������ڳ���δ֪  �ʿɴ���գ���ulOutLenָ��õ��䳤��Ȼ���ٴε��ü���
********************************************************************************/ 
//bool GetFileVersion(TCHAR* ptInPath, TCHAR* ptOutFVer, ULONG& ulOutLen)
//{//TCHAR *szFileName, TCHAR *szValue, TCHAR *szBuffer, ULONG nLength
//	DWORD dwInfoLen = 0;             //�洢�汾��ϢBuffer��С
//	BYTE* pInfoBuf = NULL;           //ָ��洢�汾��ϢBufferָ��
//	TCHAR tszFVer[MAX_PATH] = {0};    //�汾��Ϣ
//	VS_FIXEDFILEINFO *pstuVsInfo = NULL;  
//	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
//
//	if (!(ptInPath&&_tcslen(ptInPath)))
//	{
//		return false;
//	}
//	//��ȡ�ļ��汾��Ϣbuffer��С
//	dwInfoLen = GetFileVersionInfoSize(ptInPath, 0);
//	if (dwInfoLen)
//	{
//		pInfoBuf = new BYTE[dwInfoLen];
//		if (!pInfoBuf)
//		{
//			pInfoBuf = NULL;
//			return false;
//		}
//		ZeroMemory(pInfoBuf, dwInfoLen);
//		if(GetFileVersionInfo(ptInPath, 0, dwInfoLen, pInfoBuf))     
//		{     
//			if(VerQueryValue(pInfoBuf, _T("\\"), (void **)&pstuVsInfo, &iFileInfoSize))     
//			{     
//				swprintf(
//					tszFVer,
//					_T("%d.%d.%d.%d"),
//					HIWORD(pstuVsInfo->dwFileVersionMS),
//					LOWORD(pstuVsInfo->dwFileVersionMS),
//					HIWORD(pstuVsInfo->dwFileVersionLS),
//					LOWORD(pstuVsInfo->dwFileVersionLS)
//					);				
//				if (ptOutFVer&&((int)_tcslen(tszFVer)<=ulOutLen))
//				{
//					wcscpy(ptOutFVer, tszFVer);
//				}
//				else
//				{
//					ulOutLen = _tcslen(tszFVer);
//				}
//				delete pInfoBuf;
//				return true;
//			} 
//		}     
//		delete pInfoBuf;    
//	}
//
//	return false;
//}

//FileVersion
//FileDescription
// 

//////enum EILEVERSION
//////{
//////	_T("StringFileInfo"),
//////	_T("CompanyName"),
//////	_T("FileDescription"),
//////	_T("FileVersion"),
//////	_T("InternalName"),
//////	_T("LegalCopyright"),
//////	_T("OriginalFilename"), 
//////	_T("PrivateBuild"), 
//////	_T("ProductName"),
//////	_T("ProductVersion"), 
//////	_T("SpecialBuild"), 
//////	_T("VarFileInfo"),
//////	_T("Translation")
//////};


int main2()
{
	TCHAR buf[MAX_PATH];

	// Get file description for explorer.exe
	if(GetVersionString(_T("C:\\windows\\explorer.exe"), _T("FileDescription"), buf, MAX_PATH))
		printf(_T("%s\n"), buf);

	return 0;
}

