#include "stdafx.h"
#include "http_Check.h"
#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// Á´½Óµ½WS2_32.lib
#include "httpsocket.h"
#define HTTP_OK "HTTP/1.1 200 OK"
//error trigger
void CMySock::OnError()
{
	wsprintf(szErrMessage,"Error: %d, %d, %s",m_nErrCode,m_nExtErrCode,m_nErrInfo);
	MessageBox(NULL,szErrMessage,"Error",MB_OK);
	CHTTPSocket::OnError();
};


//response trigger
void CMySock::OnResponse()
{

	printf("----m_ulResponseSize=%d\r\n",m_ulResponseSize);
	printf("%s\r\n",(char *)m_szResponse);

	//CHTTPSocket::OnResponse();
};

bool CMySock::IsEffective()
{
	char szTem[16] = {0};
	int iLen = strlen(HTTP_OK);
	strncpy(szTem, (char*)m_szResponse, iLen);
	if (!strncmp(szTem, HTTP_OK, iLen))
	{
		CHTTPSocket::OnResponse();
		return true;
	}
	CHTTPSocket::OnResponse();
	return false;
};


bool CheckIsEffective(const char* url)
{

	CMySock cs;
	cs.m_bUseProxy=FALSE;

	return cs.Request("www.9cpp.com");
}