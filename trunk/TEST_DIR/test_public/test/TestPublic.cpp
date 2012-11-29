#include "stdafx.h"
#include "TestPublic.h"

#include "public/Socket.h"
//#include "public/RegUtil.h"
//#include "public/IniUtil.h"
#include "public/PathUtil.h"
#include <iostream>
using namespace std;

#define TRACE_MAX_LEN 0x400   //���TRACE����Ϊ1024
//#define TRACE_MAX_LEN 0xa   //���TRACE����Ϊ1024
//#define RUNINDEBUGMODE
#define USES_RELEASE_TRACE
#define TRACE_LOG_FILE_PATH _T("Log.txt")


#include <TraceUtil.h>

int TraceUtil()
{
	TCHAR tszTem[32] = {_T("Test^^^^^^TCHAR")};


	QNA::TRACE(_T("TCHAR����\r\n"));
	for (int i=1; i<=15;i++)
	{
		QNA::TRACE(_T("%02d��%s\n"), i, tszTem);
	}

	return 1;
}
//////int IniUtil()
//////{
//////	QNA::CIniUtil clsIniUtil;
//////
//////
//////	return 1;
//////}
//////
//////int FileUtil()
//////{
//////	TCHAR tszPath[MAX_PATH] = {0};
//////	if (!QNA::GetExePath(tszPath))
//////	{
//////		return -1;
//////	}
//////
//////	_tcscat_s(tszPath, _T("\\123"));
//////	
//////	if (!QNA::CreateMultipleDirectory(tszPath))
//////	{
//////		return -2;
//////	}
//////
//////	_tcscpy_s(tszPath, _T("D:\\123"));
//////
//////	if (!QNA::CreateMultipleDirectory(tszPath))
//////	{
//////		return -2;
//////	}
//////	return 1;
//////}


#include "public/FileVersionInfo.h"


//VI_STR_COMMENTS - Comments
//VI_STR_COMPANYNAME - CompanyName
//VI_STR_FILEDESCRIPTION - FileDescription
//VI_STR_FILEVERSION - FileVersion
//VI_STR_INTERNALNAME - InternalName
//VI_STR_LEGALCOPYRIGHT - LegalCopyright
//VI_STR_LEGALTRADEMARKS - LegalTrademarks
//VI_STR_ORIGINALFILENAME - OriginalFilename
//VI_STR_PRIVATEBUILD - PrivateBuild
//VI_STR_PRODUCTNAME - ProductName
//VI_STR_PRODUCTVERSION - ProductVersion
//VI_STR_SPECIALBUILD - SpecialBuild
//VI_STR_OLESELFREGISTER - OLESelfRegister 
int TestFileVersionInfo()
{
	//CFileVersionInfo	cls_fvi;
	//cls_fvi.Open(_T("C:\\windows\\explorer.exe"));
	//cls_fvi.GetVersionInfo();
	//cls_fvi.GetLIDName()

	return 1;
}


//int TestPathUtil()
//{
//	CPath
//	CPathUtil
//}

#define MAX_URL_LENGTH    255
#define HTTP_OK _T("HTTP/1.1 200 OK")

const TCHAR g_szHttpHead[] = 
_T("HEAD / HTTP/1.1\r\n\
Host: %s\r\n\
User-Agent: msnbot/1.0 (+http://search.msn.com/msnbot.htm)\r\n\
Accept: */*\r\n\
Connection: close\r\n\
\r\n");


int CheckHTTPHEAD(char* pstrDomain, DWORD dwIPValue, UINT uiPort)
{
	int iRet = 0;
	int iLen = 0;
	int iNum = 0;
	TCHAR tszTem[16] = {0};
	TCHAR tszSend[1024] = {0};
	QNA::CSocket clsSocket;
	iRet = clsSocket.Connect(dwIPValue, 80);
	if (1 != iRet)
	{
		QNA::TRACE(_T("���ӳ���ֵ:%d(1 �ɹ� -1 ���ó��� -2 Socket ����ʧ�� -3 ���÷��ͳ�ʱʧ�� -4 ���ý��ճ�ʱʧ�� -5 ���ӳ�ʱ -6 ���ӳ���)\r\n"), iRet);
		return -1;
	}
	iLen = _stprintf(tszSend, g_szHttpHead, pstrDomain);
	iRet = clsSocket.Send(clsSocket.GetSocket(), (PBYTE)tszSend, iLen);
	QNA::TRACE(tszSend);
	if (1 != iRet)
	{
		QNA::TRACE(_T("���ͳ���ֵ:%d(1�ɹ��� -1����ָ��Ϊ�գ� -2 ���ȴ��� -3 SOCKET_ERROR -4 WSA_IO_PENDING -5 iSent)\r\n"), iRet);
		return -2;
	}
	ZeroMemory(tszSend, sizeof(tszSend));
	iLen = clsSocket.Recv(clsSocket.GetSocket(), (PBYTE)tszSend, sizeof(tszSend));
	if (0 > iLen)	return -3;		//���ճ���  
	QNA::TRACE(tszSend);

	ZeroMemory(tszTem, sizeof(tszTem));
	iLen = _tcslen(HTTP_OK);
	_tcsncpy(tszTem, (TCHAR*)tszSend, iLen);
	if (!_tcsicmp(tszTem, HTTP_OK))  return 1;	
	QNA::TRACE(_T("HTTP HEAD ����\r\n"));
	return 0;
}

//����HTTP HEAD���� �ɹ�����1
int CheckIsEffective(char* pstrDomain, char* pstrIP)
{
	int iRet = 0;
	int iNum = 0;
	//struct in_addr addr;
	WSADATA WSAData = {0};
	HOSTENT *host_entry = NULL;   //�ṹָ��
	//QNA::CIpLookupSocket clsIpLookupSocket;

	WSAStartup(MAKEWORD(2, 2), &WSAData);

	/* ��Ҫ������������������ */
	host_entry = ::gethostbyname(pstrDomain);
	if (NULL == host_entry)	return -1;
	while(host_entry->h_addr_list[iNum])
	{		
		//addr.S_un.S_addr = *(u_long *)host_entry->h_addr_list[iNum];
		//QNA::TRACE(_T("IP��ַ:%s\r"), inet_ntoa(addr));
		iRet = /*clsIpLookupSocket.*/CheckHTTPHEAD(pstrDomain, *(u_long *)host_entry->h_addr_list[iNum], 80);
		if (1 == iRet)
		{
			return 1;
		}
		iNum++;
	}	
	return 0;
}

int TestSocket(TCHAR* pstrDomain, TCHAR* pstrIP)
{
	int iRet = 0;
	int iLen = 0;
	int iNum = 0;
	TCHAR tszTem[16] = {0};
	TCHAR tszSend[1024] = {0};
	struct in_addr addr;
	HOSTENT *host_entry = NULL;   //�ṹָ��
	char szDomian[128] = {0};
	QNA::CSocket clsSocket;
	/* ��Ҫ������������������ */

	wcstombs(szDomian, pstrDomain, _tcslen(pstrDomain)*2);
	host_entry = gethostbyname(szDomian);
	if (NULL == host_entry)
	{
		return -1;
	}
	while(host_entry->h_addr_list[iNum])
	{		
		addr.S_un.S_addr = *(u_long *)host_entry->h_addr_list[iNum];
		QNA::TRACE(_T("IP��ַ:%s\r"), inet_ntoa(addr));
		iRet = clsSocket.Connect(*(u_long *)host_entry->h_addr_list[iNum], 80);
		if (1 != iRet)
		{
			QNA::TRACE(_T("���ӳ���ֵ:%d(1 �ɹ� -1 ���ó��� -2 Socket ����ʧ�� -3 ���÷��ͳ�ʱʧ�� -4 ���ý��ճ�ʱʧ�� -5 ���ӳ�ʱ -6 ���ӳ���)\r\n"), iRet);
			return -2;
		}
		iLen = _stprintf(tszSend, g_szHttpHead, pstrDomain);
		iRet = clsSocket.Send(clsSocket.GetSocket(), (PBYTE)tszSend, iLen);
		QNA::TRACE(tszSend);
		if (1 != iRet)
		{
			QNA::TRACE(_T("���ͳ���ֵ:%d(1�ɹ��� -1����ָ��Ϊ�գ� -2 ���ȴ��� -3 SOCKET_ERROR -4 WSA_IO_PENDING -5 iSent)\r\n"), iRet);
			return -3;
		}
		ZeroMemory(tszSend, sizeof(tszSend));
		iLen = clsSocket.Recv(clsSocket.GetSocket(), (PBYTE)tszSend, sizeof(tszSend));
		iLen = clsSocket.Recv(clsSocket.GetSocket(), (PBYTE)tszSend, sizeof(tszSend));
		if (0 < iLen)
		{
			return -4;
		}

		QNA::TRACE(tszSend);

		ZeroMemory(tszTem, sizeof(tszTem));
		iLen = _tcslen(HTTP_OK);
		_tcsncpy(tszTem, (TCHAR*)tszSend, iLen);
		if (!_tcsicmp(tszTem, HTTP_OK))
		{
			QNA::TRACE(_T("HTTP HEAD ����\r\n"));
			//if (!_tcsncmp(pstrIP, inet_ntoa(addr), _tcslen(pstrIP)))
			//{
			//	QNA::TRACE("####################HTTP HEAD ���ɹ�#######################\r\n");
			//	break;
			//}
			return 1;
		}
		else
		{
			QNA::TRACE(_T("HTTP HEAD ������\r\n"));
		}
		
		iNum++;
	}	
	return 0;
}