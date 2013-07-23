// MySocket.cpp: 实现CMySocket类.
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
// 用于客户程序的函数.
void CMySocket::SocketClient(LPCSTR szHost, int nPort,LPCSTR szRequest)
{
	SOCKET ServerSock = INVALID_SOCKET; // 绑定到服务程序的套接字.
	
    ServerSock=dealsocket.GetConnect(szHost,nPort);
	if(ServerSock == INVALID_SOCKET){
		return;
	}

	m_strRequest=szRequest;

	// 向服务程序发送一字符串.
	if (send (ServerSock, m_strRequest, m_strRequest.GetLength(), 0)
		== SOCKET_ERROR) 
	{
		TRACE("Sending data to the server failed. Error: %d\n",
			WSAGetLastError ());
	}

	// 保存从服务程序接收的字符串.
	m_strReply=dealsocket.GetResponse(ServerSock);

	// 停止ServerSock发送.
	shutdown (ServerSock, 0x01);

	// 停止ServerSock的接收.
	shutdown (ServerSock, 0x00);
	
	// 关闭套接字.
	closesocket (ServerSock);
}

//---------------------------------------------------------------------------
// 用于服务程序的函数.
void CMySocket::SocketServer(int nPort,LPCSTR szReply)
{
	m_strReply=szReply;
	m_nPort =nPort;
	m_pThread=AfxBeginThread(ServerReadProc, this); // 启动线程.
}

//---------------------------------------------------------------------------
UINT CMySocket::ServerReadProc(LPVOID lpVoid) 
{
	CMySocket *Parent=(CMySocket *)lpVoid;

	SOCKET WinSocket = INVALID_SOCKET,	// Window 套接字.
	      ClientSock = INVALID_SOCKET;	// 用于通信的套接字.
	
	SOCKADDR_IN  accept_sin;	        // 接收连接体的地址.
	int accept_sin_len;			        // accept_sin长度.

	accept_sin_len = sizeof (accept_sin);

    WinSocket=dealsocket.Listening(Parent->m_nPort);

	// 接受WinSocket上的接入.
	ClientSock = accept (WinSocket, 
		(struct sockaddr *) &accept_sin, 
		(int *) &accept_sin_len);
	
	// 停止对来自客户程序连接的侦听.
	closesocket (WinSocket);
	
	if (ClientSock == INVALID_SOCKET) 
	{
		TRACE("Accepting connection with client failed. Error: %d\n", 
			   WSAGetLastError ());
		return 1;
	}
	
	// 保存从客户程序接收的字符串.
	Parent->m_strRequest=dealsocket.GetResponse(ClientSock);
	
	// 从服务程序向客户程序发送字符串.
	if (send (ClientSock, Parent->m_strReply,
		Parent->m_strReply.GetLength(), 0)== SOCKET_ERROR) 
	{
		TRACE("Sending data to the client failed. Error: %d\n",
			WSAGetLastError ());
	}
	
	// 终止在ClientSock的发送和接收.
	shutdown (ClientSock, 0x02);
	
	// 关闭ClientSock.
	closesocket (ClientSock);

	return 0;
}
