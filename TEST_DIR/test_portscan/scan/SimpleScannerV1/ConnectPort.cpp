// ConnectPort.cpp: implementation of the CConnectPort class.
// Author Asian Chai
// date   2005-05-21
// function: connect to the special port of the remote computer
// to see if it open 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleScanner.h"
#include "ConnectPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConnectPort::CConnectPort()
{

//	InitWsaDll();

}

CConnectPort::~CConnectPort()
{

}

//start the wsa, and check the winsock version validity 
UINT CConnectPort::InitWsaDll()
{
	WSADATA wsaData;  
    if(WSAStartup(0x0002, &wsaData ))
    {
        AfxMessageBox("Init TCP/IP stack error!");
        return 1;
    }
    if(wsaData.wVersion != 0x0002)
    {
        printf("Winsock version is incorrect!");
        WSACleanup();
        return 1;
    }
	return 0;

}

//connect to the remote port,if open, return true, otherwise, return false
BOOL CConnectPort::PortConnect(CString strHost,int port, UINT flag)
{
 
 if( InitWsaDll()== 1) //初始化WsaDLL
 {
	 return false;
 }

  
  struct sockaddr_in sin;  
  SOCKET sd;
  // Create the local socket
  if(flag==0)//TCP Scan 
  {  
   if ((sd = socket (AF_INET, SOCK_STREAM, IPPROTO_IP)) == INVALID_SOCKET) 
   {
		AfxMessageBox("Create socket error!");
		return 1;
   }
  }
  else if(flag==1)//Udp Scan
  {
    if ((sd = socket (AF_INET, SOCK_DGRAM, IPPROTO_IP)) == INVALID_SOCKET) 
   {
		AfxMessageBox("Create socket error!");
		return 1;
   }
	  
  }

  	char IpAddr[16];
	strcpy(IpAddr,strHost);
  // Connect to the victim IP  Address
	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=inet_addr(IpAddr);
	sin.sin_port=htons(port);
  // 判断连接是否成功
	if (connect (sd, (struct sockaddr *)&sin, sizeof (sin)) != SOCKET_ERROR) 
	{  
      closesocket(sd);
	  WSACleanup();
       return true;
	}
	else
	{
		closesocket(sd);
		WSACleanup();
		return false;
	}
	
}
