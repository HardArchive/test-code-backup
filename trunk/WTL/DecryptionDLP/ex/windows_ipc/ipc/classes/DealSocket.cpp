// DealSocket.cpp: 实现CDealSocket类.
//
#include "stdafx.h"
#include "DealSocket.h"

CDealSocket dealsocket;

//---------------------------------------------------------------------------
CDealSocket::CDealSocket()
{
	// 套接字初始化.
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	
	// 初始化WinSock.
	if (WSAStartup(wVersionRequested, &wsaData)!=0)
	{
		TRACE("WSAStartup\n");
		return;
	}

	// 检查 WinSock 版本.
	if (wsaData.wVersion != wVersionRequested)
	{
		TRACE("WinSock version not supported\n");
		WSACleanup();
		return;
	}
}

//---------------------------------------------------------------------------
CDealSocket::~CDealSocket()
{
	// 释放WinSock.
	WSACleanup();
}

//---------------------------------------------------------------------------
// 获得本机的IP地址.
CString CDealSocket::GetLocalAddr()
{
	char szName[255];
	hostent * host;
	char* addrIP;
	struct in_addr addr;
	
	memset(szName,0,255);
	if(gethostname(szName,255)!=SOCKET_ERROR)
	{
		host= gethostbyname(szName);
		if(host->h_addr_list[0])
		{
			memmove(&addr, host->h_addr_list[0],4);
			addrIP=inet_ntoa (addr);
		}
	}

	return addrIP;
}

//---------------------------------------------------------------------------
CString CDealSocket::GetResponse(SOCKET hSock)
{
	char szBufferA[MAX_RECV_LEN];  	// ASCII字符串. 
	int	iReturn;					// recv函数返回的值.
	
	CString szError;
	CString strPlus;
	strPlus.Empty();

	while(1)
	{
		// 从套接字接收资料.
		iReturn = recv (hSock, szBufferA, MAX_RECV_LEN, 0);
		szBufferA[iReturn]=0;
		strPlus +=szBufferA;

		TRACE(szBufferA);

		if (iReturn == SOCKET_ERROR)
		{
			szError.Format("No data is received, recv failed. Error: %d",
				WSAGetLastError ());
			MessageBox (NULL, szError, TEXT("Client"), MB_OK);
			break;
		}
		else if(iReturn<MAX_RECV_LEN){
			TRACE("Finished receiving data\n");
			break;
		}
	}

	return strPlus;
}

//---------------------------------------------------------------------------
SOCKET CDealSocket::GetConnect(CString host ,int port)
{
    SOCKET hSocket;
	SOCKADDR_IN saServer;          // 服务器套接字地址.
	PHOSTENT phostent = NULL;	   // 指向HOSTENT结构指针.

	// 创建一个绑定到服务器的TCP/IP套接字.
	if ((hSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		TRACE("Allocating socket failed. Error: %d\n",WSAGetLastError ());
		return INVALID_SOCKET;
	}
	
	// 使用TCP/IP协议.
	saServer.sin_family = AF_INET;
	
	// 获取与主机相关的信息.
	if ((phostent = gethostbyname (host)) == NULL) 
	{
		TRACE("Unable to get the host name. Error: %d\n",WSAGetLastError ());
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	// 给套接字IP地址赋值.
	memcpy ((char *)&(saServer.sin_addr), 
		phostent->h_addr, 
		phostent->h_length);
	
	// 设定套接字端口号.
	saServer.sin_port =htons (port); 

	// 建立到服务器的套接字连接.
	if (connect (hSocket,(PSOCKADDR) &saServer, 
		sizeof (saServer)) == SOCKET_ERROR) 
	{
		TRACE("Connecting to the server failed. Error: %d\n",WSAGetLastError ());
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	return hSocket;
}

//---------------------------------------------------------------------------
SOCKET CDealSocket::Listening(int port)
{
	SOCKET ListenSocket = INVALID_SOCKET;	// 监听套接字.
	SOCKADDR_IN local_sin;				    // 本地套接字地址.
	
	// 创建TCP/IP套接字.
	if ((ListenSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
	{
		TRACE("Allocating socket failed. Error: %d\n",WSAGetLastError ());
		return INVALID_SOCKET;
	}
	
	// 给套接字信息结构赋值.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (port); 
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
	
	// 进行本机地址与监听套接字绑定.
	if (bind (ListenSocket, 
		(struct sockaddr *) &local_sin, 
		sizeof (local_sin)) == SOCKET_ERROR) 
	{
		TRACE("Binding socket failed. Error: %d\n",WSAGetLastError ());
		closesocket (ListenSocket);
		return INVALID_SOCKET;
	}
	
	// 建立套接字对外部连接的监听.
	if (listen (ListenSocket, MAX_PENDING_CONNECTS) == SOCKET_ERROR) 
	{
		TRACE("Listening to the client failed. Error: %d\n",	
			WSAGetLastError ());
		closesocket (ListenSocket);
		return INVALID_SOCKET;
	}

	return ListenSocket;
}
