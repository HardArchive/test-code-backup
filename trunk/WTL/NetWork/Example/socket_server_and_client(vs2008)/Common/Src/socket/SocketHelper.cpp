#include "stdafx.h"
#include "SocketHelper.h"
#include <mstcpip.h>

#ifndef _WIN32_WCE
	#pragma comment(lib, "ws2_32")
#else
	#pragma comment(lib, "ws2")
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL IsIPAddress(LPCTSTR lpszAddress)
{
	if(!lpszAddress)
		return FALSE;

	size_t len = lstrlen(lpszAddress) + 1;
	
	if(len > 15)
		return false;

	int iDotIndex = 0;
	int iDotCount = 0;

	for(size_t i = 0; i < len; ++i)
	{
		TCHAR c = lpszAddress[i];
		if((c < '0' || c > '9') && c != '.' && c != '\0')
			return FALSE;
		else if(c == '.' || c == '\0')
		{
			size_t iSubLen = i - iDotIndex;

			if(iSubLen < 1 || iSubLen > 3)
				return FALSE;

			TCHAR szSub[4] = {0};
			_tcsncpy(szSub, lpszAddress + iDotIndex, iSubLen);

			int iSub = _ttoi(szSub);

			if(iSub < 0 || iSub > 255)
				return FALSE;

			if(c == '.')
			{
				iDotIndex = i + 1;
				++iDotCount;
			}
		}
	}

	return (iDotCount == 3 || iDotCount == 5);
}

BOOL GetIPAddress(LPCTSTR lpszHost, CStringA& strIP)
{
	BOOL isOK = TRUE;

	if(IsIPAddress(lpszHost))
		strIP = lpszHost;
	else
	{
		hostent* host = ::gethostbyname(CT2A(lpszHost));

		if(!host)
			return FALSE;

		strIP = inet_ntoa(*(struct in_addr *)*host->h_addr_list); 
	}

	return TRUE;
}

//获取扩展函数指针
PVOID GetExtensionFuncPtr(SOCKET sock, GUID guid)
{
	DWORD dwBytes;
	PVOID pfn = NULL;

	//I/O控制命令winsock1版是ioctlsocket与winsock2版是WSAIoctl,传一个套接字进来即可
	//通过WSAIoctl() (选项参数为SIO_GET_EXTENSION_FUNCTION_POINTER)动态获得函数的指针
	::WSAIoctl	(
					sock,
					SIO_GET_EXTENSION_FUNCTION_POINTER,
					&guid,
					sizeof(guid),
					&pfn,
					sizeof(pfn),
					&dwBytes,
					NULL,
					NULL
				);

	return pfn;
}

//AcceptEx函数 接受连接 
//功能：1、接受新连接 2、新连接的本地地址和远程地址都会返回 3、接收到了远程主机发来的第一块数据
LPFN_ACCEPTEX Get_AcceptEx_FuncPtr(SOCKET sock)
{
	GUID guid = WSAID_ACCEPTEX;  //AcceptEx函数
	return (LPFN_ACCEPTEX)GetExtensionFuncPtr(sock, guid);
}

LPFN_GETACCEPTEXSOCKADDRS Get_GetAcceptExSockaddrs_FuncPtr(SOCKET sock)
{
	GUID guid = WSAID_GETACCEPTEXSOCKADDRS;  //GetAcceptExSockAddrs
	return (LPFN_GETACCEPTEXSOCKADDRS)GetExtensionFuncPtr(sock, guid);
}

LPFN_CONNECTEX Get_ConnectEx_FuncPtr(SOCKET sock)
{
	GUID guid = WSAID_CONNECTEX;   //Connectex
	return (LPFN_CONNECTEX)GetExtensionFuncPtr(sock, guid);
}

LPFN_TRANSMITFILE Get_TransmitFile_FuncPtr(SOCKET sock)
{
	GUID guid = WSAID_TRANSMITFILE;  //TransmitFile
	return (LPFN_TRANSMITFILE)GetExtensionFuncPtr(sock, guid);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int SSO_UpdateAcceptContext(SOCKET soClient, SOCKET soBind)
{
	return setsockopt(soClient, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (CHAR*)&soBind, sizeof(SOCKET));
}

int SSO_UpdateConnectContext(SOCKET soClient, int iOption)
{
	return setsockopt(soClient, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, (CHAR*)&iOption, sizeof(int));
}

int SSO_NoDelay(SOCKET sock, BOOL bNoDelay)
{
	return setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (CHAR*)&bNoDelay, sizeof(BOOL));
}

int SSO_DontLinger(SOCKET sock, BOOL bDont)
{
	return setsockopt(sock, SOL_SOCKET, SO_DONTLINGER, (CHAR*)&bDont, sizeof(BOOL));
}

int SSO_Linger(SOCKET sock, USHORT l_onoff, USHORT l_linger)
{
	linger ln = {l_onoff, l_linger};
	return setsockopt(sock, SOL_SOCKET, SO_LINGER, (CHAR*)&ln, sizeof(linger));
}

int SSO_KeepAlive(SOCKET sock, BOOL bKeepAlive)
{
	return setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (CHAR*)&bKeepAlive, sizeof(BOOL));
}

int SSO_KeepAliveVals(SOCKET sock, u_long onoff, u_long time, u_long interval)
{
	DWORD dwBytes;
	tcp_keepalive out;
	tcp_keepalive in = {onoff, time, interval};
	return ::WSAIoctl(
						sock, 
						SIO_KEEPALIVE_VALS, 
						(LPVOID)&in, 
						sizeof(in), 
						(LPVOID)&out, 
						sizeof(out), 
						&dwBytes, 
						NULL, 
						NULL
					);
}

int SSO_RecvBuffSize(SOCKET sock, int size)
{
	return setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (CHAR*)&size, sizeof(int));
}

int SSO_SendBuffSize(SOCKET sock, int size)
{
	return setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (CHAR*)&size, sizeof(int));
}

int SSO_ReuseAddress(SOCKET sock, BOOL bReuse)
{
	return setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&bReuse, sizeof(BOOL));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int ManualCloseSocket(SOCKET sock, BOOL bGraceful, BOOL bReuseAddress)
{
	if(!bGraceful)
		SSO_Linger(sock, 1, 0);

	if(bReuseAddress)
		SSO_ReuseAddress(sock, bReuseAddress);

	return closesocket(sock);
}

int PostAccept(LPFN_ACCEPTEX pfnAcceptEx, SOCKET soListen, SOCKET soClient, TBufferObj* pBufferObj)
{
	int result = NO_ERROR;

	pBufferObj->operation	= SO_ACCEPT;
	pBufferObj->client		= soClient;

	if(!pfnAcceptEx	(
						soListen,
						soClient,
						pBufferObj->buff.buf,
						0,
						sizeof(SOCKADDR_IN) + 16,
						sizeof(SOCKADDR_IN) + 16,
						NULL,
						&pBufferObj->ov
					)
	)
	{
		result = ::WSAGetLastError();
		if(result == WSA_IO_PENDING)
			result = NO_ERROR;
	}

	return result;
}

int PostSend(TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	int result = NO_ERROR;

	pBufferObj->operation	= SO_SEND;

	DWORD dwBytes; 
	if(::WSASend(
					pSocketObj->socket,
					&pBufferObj->buff,
					1,
					&dwBytes,
					0,
					&pBufferObj->ov,
					NULL
				) == SOCKET_ERROR)
	{
		result = ::WSAGetLastError();
		if(result == WSA_IO_PENDING)
			result = NO_ERROR;
	}

	return result;
}

int PostReceive(TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	int result = NO_ERROR;

	pBufferObj->operation	= SO_RECEIVE;

	DWORD dwBytes, dwFlag = 0; 
	if(::WSARecv(
					pSocketObj->socket,
					&pBufferObj->buff,
					1,
					&dwBytes,
					&dwFlag,
					&pBufferObj->ov,
					NULL
				) == SOCKET_ERROR)
	{
		result = ::WSAGetLastError();
		if(result == WSA_IO_PENDING)
			result = NO_ERROR;
	}

	return result;
}