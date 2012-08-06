// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include<windows.h>&nbsp; &nbsp;
#include<wininet.h>&nbsp; &nbsp;

#pragma comment(lib,"wininet.lib") 

void main()
{
	HINTERNET   hSession = NULL; 
	HINTERNET   hConnect = NULL; 
	HINTERNET   hRequest = NULL; 

	//InternetOpenA
	LPCSTR	lpszAgent = "Application";
	DWORD	dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
	
	hSession = InternetOpenA(lpszAgent,dwAccessType,NULL,NULL,0);
	if (!hSession)
	{
		return ;
	}
	
	//InternetConnectA
	LPCSTR lpszServerName = "mail.sina.com.cn";
	INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTP_PORT; 
	DWORD dwService = INTERNET_SERVICE_HTTP;
	
	hConnect = InternetConnectA(hSession,lpszServerName,nServerPort,NULL,NULL,dwService,0,1);
	if (!hConnect)
	{
		return ;
	}
	
	//HttpOpenRequestA
	LPCSTR lpszVerb = "POST";
	LPCSTR lpszObjectName = "cgi-bin/login.php"; //��վ�Ķ���������ַ��InternetConnectA�ڶ�������ָ��
	LPCSTR lpszVersion = "HTTP/1.1"; 
	LPCSTR lpszReferrer = "Referer: http://mail.sina.com.cn/cgi-bin/login.php"; 
	LPCSTR lpszAcceptTypes = "Accept: text/html, application/xhtml+xml, */*"; 
	DWORD  dwFlags = INTERNET_FLAG_RELOAD; 
	
	hRequest = HttpOpenRequestA(hConnect,lpszVerb,lpszObjectName,lpszVersion,lpszReferrer,&lpszAcceptTypes,dwFlags,1);
	if (!hRequest)
	{
		return;
	}

	//HttpSendRequestA
	LPCSTR	lpszHeaders = "Content-Type: application/x-www-form-urlencoded\r\n";
	DWORD	dwHeadersLength=-1L;
	char	szFormData[1024]={0};
	strcpy(szFormData,"logintype=uid&u=jimmy_cmd&psw=123abc&product=mail&%B5%C7%C2%BC=%B5%C7+%C2%BC\r\n");

	LPVOID pBuf = (LPVOID)szFormData;
	BOOL bReturn = HttpSendRequestA(hRequest,lpszHeaders,dwHeadersLength,pBuf,strlen(szFormData));
	if(!bReturn)
	{
		return;
	}
	
	char	szRecvBuf[1024+1];          // �������ݻ�����
	DWORD	dwNumberOfBytesRead;        // ���������ش�С
	DWORD	dwRecvTotalSize=0;          // ���������ܴ�С
	DWORD	dwRecvBuffSize=0;           // ��������buf�Ĵ�С
	memset(szRecvBuf,0,1024+1);
	LPCSTR	lpFileName = "recv.html";
	DWORD	dwNumberOfBytesWritten ;

	HANDLE hOpenFile = (HANDLE)CreateFileA(lpFileName,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,NULL,NULL);
	
	if (hOpenFile == INVALID_HANDLE_VALUE)
	{
		hOpenFile = NULL;
		return ;
	}

	do 
	{
		bReturn = InternetReadFile(hRequest,szRecvBuf,1024,&dwNumberOfBytesRead);
		if(!bReturn)
		{
			break;
		}

		// ͳ�ƽ������ݵĴ�С
		szRecvBuf[dwNumberOfBytesRead] = '\0';
		dwRecvTotalSize += dwNumberOfBytesRead;
		dwRecvBuffSize  += strlen(szRecvBuf);

		WriteFile(hOpenFile,szRecvBuf,dwNumberOfBytesRead,&dwNumberOfBytesWritten,NULL);

	} while (dwNumberOfBytesRead !=0);
	

	CloseHandle(hOpenFile);

	InternetCloseHandle(hSession);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);
}