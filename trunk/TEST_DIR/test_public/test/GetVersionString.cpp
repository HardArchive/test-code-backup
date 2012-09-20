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
		//从版本资源中获取信息。调用这个函数前，必须先用GetFileVersionInfo函数获取版本资源信息。
		// 这个函数会检查资源信息，并将需要的数据复制到一个缓冲区里
		//"\" 获取文件的VS_FIXEDFILEINFO结构
		//"\VarFileInfo\Translation" 获取文件的翻译表
		//"\StringFileInfo\...." 获取文件的字串信息。参考注解
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
1、 函数名称： GetFileVersion
2、 功能描述： 获取传入文件的版本事情
3、 输入参数： char *ptInPath 文件路径
			  char* ptOutFVer,文件版本指针
			  ULONG& ulOutLen  文件版本buffer的长度
4、 返 回 值： 成功返回true，失败返回false
5、 动态内存： 有  要注意释放
6、 代码设计：  阿毛
7、 开发日期： 2011-9-7 9:47:20
8、 备    注： 此函数第二个参数由于长度未知  故可传入空，由ulOutLen指针得到其长度然后再次调用即可
********************************************************************************/ 
//bool GetFileVersion(TCHAR* ptInPath, TCHAR* ptOutFVer, ULONG& ulOutLen)
//{//TCHAR *szFileName, TCHAR *szValue, TCHAR *szBuffer, ULONG nLength
//	DWORD dwInfoLen = 0;             //存储版本信息Buffer大小
//	BYTE* pInfoBuf = NULL;           //指向存储版本信息Buffer指针
//	TCHAR tszFVer[MAX_PATH] = {0};    //版本信息
//	VS_FIXEDFILEINFO *pstuVsInfo = NULL;  
//	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
//
//	if (!(ptInPath&&_tcslen(ptInPath)))
//	{
//		return false;
//	}
//	//获取文件版本信息buffer大小
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

