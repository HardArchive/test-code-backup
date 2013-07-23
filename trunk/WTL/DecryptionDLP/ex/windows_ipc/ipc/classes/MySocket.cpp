// MySocket.cpp: ʵ��CMySocket��.
//
#include "stdafx.h"
#include "MySocket.h"

//---------------------------------------------------------------------------
CMySocket::CMySocket()
{
	m_pThread = NULL;
}

//---------------------------------------------------------------------------
CMySocket::~CMySocket()
{
	if(m_pThread){ 
		if(TerminateThread(m_pThread->m_hThread,0))
		{
			if(m_pThread) 
				delete	m_pThread;
			m_pThread = NULL;
		}
	}
}

//---------------------------------------------------------------------------
// ���ڿͻ�����ĺ���.
void CMySocket::SocketClient(LPCSTR szHost, int nPort,LPCSTR szRequest)
{
	SOCKET ServerSock = INVALID_SOCKET; // �󶨵����������׽���.
	
    ServerSock=dealsocket.GetConnect(szHost,nPort);
	if(ServerSock == INVALID_SOCKET){
		return;
	}

	m_strRequest=szRequest;

	// ����������һ�ַ���.
	if (send (ServerSock, m_strRequest, m_strRequest.GetLength(), 0)
		== SOCKET_ERROR) 
	{
		TRACE("Sending data to the server failed. Error: %d\n",
			WSAGetLastError ());
	}

	// ����ӷ��������յ��ַ���.
	m_strReply=dealsocket.GetResponse(ServerSock);

	// ֹͣServerSock����.
	shutdown (ServerSock, 0x01);

	// ֹͣServerSock�Ľ���.
	shutdown (ServerSock, 0x00);
	
	// �ر��׽���.
	closesocket (ServerSock);
}

//---------------------------------------------------------------------------
// ���ڷ������ĺ���.
void CMySocket::SocketServer(int nPort,LPCSTR szReply)
{
	m_strReply=szReply;
	m_nPort =nPort;
	m_pThread=AfxBeginThread(ServerReadProc, this); // �����߳�.
}

//---------------------------------------------------------------------------
UINT CMySocket::ServerReadProc(LPVOID lpVoid) 
{
	CMySocket *Parent=(CMySocket *)lpVoid;

	SOCKET WinSocket = INVALID_SOCKET,	// Window �׽���.
	      ClientSock = INVALID_SOCKET;	// ����ͨ�ŵ��׽���.
	
	SOCKADDR_IN  accept_sin;	        // ����������ĵ�ַ.
	int accept_sin_len;			        // accept_sin����.

	accept_sin_len = sizeof (accept_sin);

    WinSocket=dealsocket.Listening(Parent->m_nPort);

	// ����WinSocket�ϵĽ���.
	ClientSock = accept (WinSocket, 
		(struct sockaddr *) &accept_sin, 
		(int *) &accept_sin_len);
	
	// ֹͣ�����Կͻ��������ӵ�����.
	closesocket (WinSocket);
	
	if (ClientSock == INVALID_SOCKET) 
	{
		TRACE("Accepting connection with client failed. Error: %d\n", 
			   WSAGetLastError ());
		return 1;
	}
	
	// ����ӿͻ�������յ��ַ���.
	Parent->m_strRequest=dealsocket.GetResponse(ClientSock);
	
	// �ӷ��������ͻ��������ַ���.
	if (send (ClientSock, Parent->m_strReply,
		Parent->m_strReply.GetLength(), 0)== SOCKET_ERROR) 
	{
		TRACE("Sending data to the client failed. Error: %d\n",
			WSAGetLastError ());
	}
	
	// ��ֹ��ClientSock�ķ��ͺͽ���.
	shutdown (ClientSock, 0x02);
	
	// �ر�ClientSock.
	closesocket (ClientSock);

	return 0;
}
