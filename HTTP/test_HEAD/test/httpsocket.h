/************************************************************************
HTTP socket - DTS Project
Copyright (C) 2000 by Tair Abdurman, All Rights Reserved
http://www.tair.freeservers.com, dts@tair.freeservers.com
No any part of this code can be used in comercial purposes.
************************************************************************/

/************************************************************************
clicksocket.h
************************************************************************/

#ifndef __HTTPSOCKET__H__
#define __HTTPSOCKET__H__


#include <windows.h>
//rem next  line if no debug dump wanted 
#define DEBON
#include <stdio.h>

//default send and recieve timeouts in sec
#define HTTPRTIMEOUTDEF 90000
#define HTTPSTIMEOUTDEF 90000

#define MAXHOSTLENGTH   65
#define MAXIPLENGTH     16
#define MAXBLOCKSIZE    1024
#define MAXURLLENGTH    255
#define MAXHEADERLENGTH 269

#define MEM_PHYSICAL       0x400000

#define ERR_OK           0
#define ERR_WSAINTERNAL  1
#define ERR_URLNOTHING   2
#define ERR_URLTOOLONG   3
#define ERR_HOSTUNKNOWN  4
#define ERR_PROXYUNKNOWN 5
#define ERR_PROTOPARSE   6
#define ERR_BADHOST      7
#define ERR_BADPORT      8


class CHTTPSocket
{
 static int nInstanceCount;

SOCKET              sckHTTPSocket;
struct sockaddr_in  sinHTTPSocket;
struct sockaddr_in  sinHTTPServer;
// remote server host address, size 64 bytes, 65th set to \0
	   char         m_szServerHost[MAXHOSTLENGTH];
// host
	   char         m_szHost[MAXHOSTLENGTH];
// requested URI/URL
	   char         m_szURL[MAXURLLENGTH];
// remote server IP address, size 15 bytes, 16th set to \0
	   char         m_szServerHostIP[MAXIPLENGTH];
//-- Win32 specific
WSADATA           wsaData;

void szcopy(char* dest,const char* src,int nMaxBytes);
void szsent(SOCKET sckDest,const char* szHttp);

public:
// set to TRUE in InitInstance if TIME_WAIT not need ()
       BOOL  m_bNoTimeWait;
// recieve timeout change in InitInstance
       int   m_nRecvTimeout;
// send timeout change in InitInstance
       int   m_nSendTimeout;
// remote server port
	   int m_nServerPort;
// use proxy flag
       BOOL  m_bUseProxy;
// error code
	   int m_nErrCode;
// extended error code;
	   int m_nExtErrCode;
// error info
	   char m_nErrInfo[255];
// response content
	   LPVOID m_szResponse;
// response size
	   ULONG m_ulResponseSize;
public:
   //const/destr
   CHTTPSocket();
   virtual ~CHTTPSocket();
   
   //utils
   // sets proxy or http server's host
   void SetServerHost(const char* src);
   // sets proxy or http server's ip 
   //(should be skipped if SetServerHost used)
   void SetServerHostIP(const char* src);
   //starts request transaction
   bool Request(const char* url="http://www.tair.freeservers.com");
   //used for free memory allocated for page
   //(should be skipped if You use CHTTPSocket::OnResponse call in OnResponse)
   void memPostup();
   //fire your OnError with specific error cdes and message
   void ThrowError(int err, int xerr, const char* errdesc);
   
   //overridable
   
   //shoul be used for additional inits
   virtual BOOL InitInstance();
   
   //trigger on any transaction error 
   virtual void OnError();
   
   //trigger on response recieved
   //(its great if U will call CHTTPSocket::OnResponse inside,
   //to free allocated memory pages)
   virtual void OnResponse();

    virtual bool IsEffective();

};

#endif
