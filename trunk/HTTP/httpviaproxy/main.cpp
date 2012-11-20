/************************************************************************
HTTP socket - DTS Project
Copyright (C) 2000 by Tair Abdurman, All Rights Reserved
http://www.tair.freeservers.com, dts@tair.freeservers.com
No any part of this code can be used in comercial purposes.
************************************************************************/

/************************************************************************
main. entry point for service
************************************************************************/

#include <stdio.h>
#include "httpsocket.h"

//Example class based on CHTTPSocket
class CMySock : public CHTTPSocket
{
	char szErrMessage[255];
public:
	void OnError();
	void OnResponse(); 

};

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

	CHTTPSocket::OnResponse();
};

//entry point
//-----------------------------------------------------------------------
//call style:
//-----------------------------------------------------------------------
// dts.exe /URL http://www.9cpp.com [/PRX 127.0.0.1] [/PRT 8080]
//-----------------------------------------------------------------------
// where /URL - U see
//       /PRX - proxy's internet address
//       /PRT - proxy's port
//-----------------------------------------------------------------------
// You must have KERNEL32.DLL, USER32.DLL and WS2_32.DLL installed.
//-----------------------------------------------------------------------
void main(int argc,char* argv[])
{

	CMySock cs;
	cs.m_bUseProxy=FALSE;
	int i=0;
	char* page=NULL;
	char* serverHost=NULL;
	char* serverPort=NULL;

	while(i<argc)
	{
		if (strcmp(argv[i],"/URL")==0)
		{
			if (argv[++i]!=NULL)
				page=argv[i];
			else
				page=NULL;
		}

		if (strcmp(argv[i],"/PRX")==0)
		{
			if (argv[++i]!=NULL)
				serverHost=argv[i];
			else
				serverHost=NULL;
		}

		if (strcmp(argv[i],"/PRT")==0)
		{
			if (argv[++i]!=NULL)
				serverPort=argv[i];
			else
				serverPort=NULL;
		}
		i++;

	}

	if (page==NULL)
	{
		cs.ThrowError(0,0,"Please specify URL to fetch!");
		return;
	}

	if (serverHost!=NULL)
	{
		//sets proxy server's internet address
		cs.SetServerHost((const char*)serverHost);
		i=0;
		if(serverPort!=NULL)
			i=atoi(serverPort);
		if (i==0)
			i=8080;
		//sets proxy server's port number (8080 by default)
		cs.m_nServerPort=i;
		//says use proxy to CHTTPSocket derived class
		cs.m_bUseProxy=TRUE;
	}


	printf("URL to fetch: %s\r\n",page);
	printf("Use proxy %s\r\n",serverHost);
	printf("Port for proxy %d\r\n",i);


	//page request here
	cs.Request(page);
}
