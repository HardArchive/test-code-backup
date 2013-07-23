// DealSocket.cpp: ʵ��CDealSocket��.
//
#include "stdafx.h"
#include "DealSocket.h"

CDealSocket dealsocket;

//---------------------------------------------------------------------------
CDealSocket::CDealSocket()
{
	// �׽��ֳ�ʼ��.
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	
	// ��ʼ��WinSock.
	if (WSAStartup(wVersionRequested, &wsaData)!=0)
	{
		TRACE("WSAStartup\n");
		return;
	}

	// ��� WinSock �汾.
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
	// �ͷ�WinSock.
	WSACleanup();
}

//---------------------------------------------------------------------------
// ��ñ�����IP��ַ.
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
	char szBufferA[MAX_RECV_LEN];  	// ASCII�ַ���. 
	int	iReturn;					// recv�������ص�ֵ.
	
	CString szError;
	CString strPlus;
	strPlus.Empty();

	while(1)
	{
		// ���׽��ֽ�������.
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
	SOCKADDR_IN saServer;          // �������׽��ֵ�ַ.
	PHOSTENT phostent = NULL;	   // ָ��HOSTENT�ṹָ��.

	// ����һ���󶨵���������TCP/IP�׽���.
	if ((hSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		TRACE("Allocating socket failed. Error: %d\n",WSAGetLastError ());
		return INVALID_SOCKET;
	}
	
	// ʹ��TCP/IPЭ��.
	saServer.sin_family = AF_INET;
	
	// ��ȡ��������ص���Ϣ.
	if ((phostent = gethostbyname (host)) == NULL) 
	{
		TRACE("Unable to get the host name. Error: %d\n",WSAGetLastError ());
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	// ���׽���IP��ַ��ֵ.
	memcpy ((char *)&(saServer.sin_addr), 
		phostent->h_addr, 
		phostent->h_length);
	
	// �趨�׽��ֶ˿ں�.
	saServer.sin_port =htons (port); 

	// ���������������׽�������.
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
	SOCKET ListenSocket = INVALID_SOCKET;	// �����׽���.
	SOCKADDR_IN local_sin;				    // �����׽��ֵ�ַ.
	
	// ����TCP/IP�׽���.
	if ((ListenSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
	{
		TRACE("Allocating socket failed. Error: %d\n",WSAGetLastError ());
		return INVALID_SOCKET;
	}
	
	// ���׽�����Ϣ�ṹ��ֵ.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (port); 
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
	
	// ���б�����ַ������׽��ְ�.
	if (bind (ListenSocket, 
		(struct sockaddr *) &local_sin, 
		sizeof (local_sin)) == SOCKET_ERROR) 
	{
		TRACE("Binding socket failed. Error: %d\n",WSAGetLastError ());
		closesocket (ListenSocket);
		return INVALID_SOCKET;
	}
	
	// �����׽��ֶ��ⲿ���ӵļ���.
	if (listen (ListenSocket, MAX_PENDING_CONNECTS) == SOCKET_ERROR) 
	{
		TRACE("Listening to the client failed. Error: %d\n",	
			WSAGetLastError ());
		closesocket (ListenSocket);
		return INVALID_SOCKET;
	}

	return ListenSocket;
}
