///////////////////////////////////////////////////////
// OverlappedServer.cpp文件

#include "../common/initsock.h"

#include <Mswsock.h>
#include <stdio.h>
#include <windows.h>

CInitSocket clsInitSock;
LPFN_GETACCEPTEXSOCKADDRS g_pfnGetAcceptExSockaddrs = NULL; //获取连接addr
#define BUFFER_SIZE 1024

typedef struct _SOCKET_OBJ
{
	SOCKET s;						// 套节字句柄
	int nOutstandingOps;			// 记录此套节字上的重叠I/O数量
	LPFN_ACCEPTEX lpfnAcceptEx;		// 扩展函数AcceptEx的指针（仅对监听套节字而言）
} SOCKET_OBJ, *PSOCKET_OBJ;

typedef struct _BUFFER_OBJ
{	
	OVERLAPPED ol;			// 重叠结构
	char *buff;				// send/recv/AcceptEx所使用的缓冲区
	int nLen;				// buff的长度
	PSOCKET_OBJ pSocket;	// 此I/O所属的套节字对象

	int nOperation;			// 提交的操作类型
#define OP_ACCEPT	1
#define OP_RECV		2
#define OP_SEND	    3

	SOCKET sAccept;			// 用来保存AcceptEx接受的客户套节字（仅对监听套节字而言）
	_BUFFER_OBJ *pNext;
} BUFFER_OBJ, *PBUFFER_OBJ;

HANDLE g_events[WSA_MAXIMUM_WAIT_EVENTS];	// I/O事件句柄数组  64 第0个为本地监听事件句柄
int g_nBufferCount;							// 上数组中有效句柄数量
PBUFFER_OBJ g_pBufferHead, g_pBufferTail;	// 记录缓冲区对象组成的链表表头地址

//获取扩展函数指针
PVOID GetExtensionFuncPtr(SOCKET sock)
{
	DWORD dwBytes;
	PVOID pfn = NULL;
	GUID guid = WSAID_GETACCEPTEXSOCKADDRS;  //GetAcceptExSockAddrs

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
// 申请套节字对象和释放套节字对象的函数
PSOCKET_OBJ GetSocketObj(SOCKET s)
{
	PSOCKET_OBJ pSocket = (PSOCKET_OBJ)::GlobalAlloc(GPTR, sizeof(SOCKET_OBJ));
	if(pSocket != NULL)
	{
		pSocket->s = s;
	}
	return pSocket;
}
void FreeSocketObj(PSOCKET_OBJ pSocket)
{
	if(pSocket->s != INVALID_SOCKET)
		::closesocket(pSocket->s);
	::GlobalFree(pSocket);
}

PBUFFER_OBJ GetBufferObj(PSOCKET_OBJ pSocket, ULONG nLen)
{
	if(g_nBufferCount > WSA_MAXIMUM_WAIT_EVENTS - 1)
		return NULL;

	PBUFFER_OBJ pBuffer = (PBUFFER_OBJ)::GlobalAlloc(GPTR, sizeof(BUFFER_OBJ));
	if(pBuffer != NULL)
	{
		pBuffer->buff = (char*)::GlobalAlloc(GPTR, nLen);
		pBuffer->ol.hEvent = ::WSACreateEvent();
		pBuffer->pSocket = pSocket;
		pBuffer->sAccept = INVALID_SOCKET;
		pBuffer->nLen = nLen;

		// 将新的BUFFER_OBJ添加到列表中
		if(g_pBufferHead == NULL)
		{
			g_pBufferHead = g_pBufferTail = pBuffer;
		}
		else
		{
			g_pBufferTail->pNext = pBuffer;
			g_pBufferTail = pBuffer;
		}
		g_events[++ g_nBufferCount] = pBuffer->ol.hEvent;
	}
	return pBuffer;
}

void FreeBufferObj(PBUFFER_OBJ pBuffer)
{
	// 从列表中移除BUFFER_OBJ对象
	PBUFFER_OBJ pTest = g_pBufferHead;
	BOOL bFind = FALSE;
	if(pTest == pBuffer)
	{
		g_pBufferHead = g_pBufferTail = NULL;
		bFind = TRUE;
	}
	else
	{
		while(pTest != NULL && pTest->pNext != pBuffer)
			pTest = pTest->pNext;
		if(pTest != NULL)
		{
			pTest->pNext = pBuffer->pNext;
			if(pTest->pNext == NULL)
				g_pBufferTail = pTest;
			bFind = TRUE;
		}
	}
	// 释放它占用的内存空间
	if(bFind)
	{
		g_nBufferCount --;
		::CloseHandle(pBuffer->ol.hEvent);
		::GlobalFree(pBuffer->buff);
		::GlobalFree(pBuffer);	
	}
}

PBUFFER_OBJ FindBufferObj(HANDLE hEvent)
{
	PBUFFER_OBJ pBuffer = g_pBufferHead;
	while(pBuffer != NULL)
	{
		if(pBuffer->ol.hEvent == hEvent)
			break;
		pBuffer = pBuffer->pNext;
	}
	return pBuffer;
}

//重建缓冲区链表与IO事件对象句柄的对应关系
void RebuildArray()
{
	PBUFFER_OBJ pBuffer = g_pBufferHead;
	int i =  1;
	while(pBuffer != NULL)
	{
		g_events[i++] = pBuffer->ol.hEvent;
		pBuffer = pBuffer->pNext;
	}
}

//投递一个接受连接请求
BOOL PostAccept(PBUFFER_OBJ pBuffer)
{
	PSOCKET_OBJ pSocket = pBuffer->pSocket;
	

	if(pSocket->lpfnAcceptEx != NULL)
	{	
		// 设置I/O类型，增加套节字上的重叠I/O计数
		pBuffer->nOperation = OP_ACCEPT;
		pSocket->nOutstandingOps ++;

		// 投递此重叠I/O  
		DWORD dwBytes;
		pBuffer->sAccept = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);  //创建socket
		//开始接收
		BOOL b = pSocket->lpfnAcceptEx(pSocket->s,   //监听套接字
			pBuffer->sAccept,   //接收套接字
			pBuffer->buff, 
			BUFFER_SIZE - ((sizeof(sockaddr_in) + 16) * 2), //buf大小
			sizeof(sockaddr_in) + 16,  //缓冲区中为本地地址预留的长度，必须比最大专址长度多16
			sizeof(sockaddr_in) + 16,  //缓冲区中为远程地址预留的长度，必须比最大专址长度多16
			&dwBytes,                  //用来接收到的数据长度
			&pBuffer->ol);             //用来处理本请求的重叠结构(OVERLAPPED)
		if(!b)
		{
			if(::WSAGetLastError() != WSA_IO_PENDING)
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
};

//投递一个接收
BOOL PostRecv(PBUFFER_OBJ pBuffer)
{	
	// 设置I/O类型，增加套节字上的重叠I/O计数
	pBuffer->nOperation = OP_RECV;
	pBuffer->pSocket->nOutstandingOps ++;

	// 投递此重叠I/O
	DWORD dwBytes;
	DWORD dwFlags = 0;
	WSABUF buf;
	buf.buf = pBuffer->buff;
	buf.len = pBuffer->nLen;
	if(::WSARecv(pBuffer->pSocket->s, &buf, 1, &dwBytes, &dwFlags, &pBuffer->ol, NULL) != NO_ERROR)
	{
		if(::WSAGetLastError() != WSA_IO_PENDING)
			return FALSE;
	}
	return TRUE;
}

//投递一个发送
BOOL PostSend(PBUFFER_OBJ pBuffer)
{
	// 设置I/O类型，增加套节字上的重叠I/O计数
	pBuffer->nOperation = OP_SEND;
	pBuffer->pSocket->nOutstandingOps ++;

	// 投递此重叠I/O
	DWORD dwBytes;
	DWORD dwFlags = 0;
	WSABUF buf;
	buf.buf = pBuffer->buff;
	buf.len = pBuffer->nLen;
	if(::WSASend(pBuffer->pSocket->s, 
			&buf, 1, &dwBytes, dwFlags, &pBuffer->ol, NULL) != NO_ERROR)
	{
		if(::WSAGetLastError() != WSA_IO_PENDING)
			return FALSE;
	}
	return TRUE;
}

BOOL HandleIO(PBUFFER_OBJ pBuffer)
{
	PSOCKET_OBJ pSocket = pBuffer->pSocket; // 从BUFFER_OBJ对象中提取SOCKET_OBJ对象指针，为的是方便引用
	pSocket->nOutstandingOps --;
	
	DWORD dwTrans;   //用于取得实际传输字节的数量
	DWORD dwFlags;   //用于取得完成状态
	// 获取重叠操作结果 true 重叠操作成功  false 套接字上有错误发生
	BOOL bRet = ::WSAGetOverlappedResult(pSocket->s, &pBuffer->ol, &dwTrans, FALSE, &dwFlags);
	if(!bRet)
	{
		// 在此套节字上有错误发生，因此，关闭套节字，移除此缓冲区对象。
		// 如果没有其它抛出的I/O请求了，释放此缓冲区对象，否则，等待此套节字上的其它I/O也完成
		if(pSocket->s != INVALID_SOCKET)
		{
			::closesocket(pSocket->s);
			pSocket->s = INVALID_SOCKET;
		}

		if(pSocket->nOutstandingOps == 0)
			FreeSocketObj(pSocket);	
		
		FreeBufferObj(pBuffer);
		return FALSE;
	}

	// 没有错误发生，处理已完成的I/O
	switch(pBuffer->nOperation)
	{
	case OP_ACCEPT:	// 接收到一个新的连接，并接收到了对方发来的第一个封包
		{
			// 为新客户创建一个SOCKET_OBJ对象
			PSOCKET_OBJ pClient = GetSocketObj(pBuffer->sAccept);

			// 为收发数据创建一个BUFFER_OBJ对象，这个对象会在套节字出错或者关闭时释放
			PBUFFER_OBJ pSend = GetBufferObj(pClient, BUFFER_SIZE);	
			if(pSend == NULL)
			{
				printf(" Too much connections! \n");
				FreeSocketObj(pClient);
				return FALSE;
			}

			SOCKADDR_IN LocalSockAddr;
			SOCKADDR_IN RemoteSockAddr;
			int nLocalLen, nRmoteLen;
			LPSOCKADDR pLocalAddr, pRemoteAddr;
			g_pfnGetAcceptExSockaddrs(
				pBuffer->buff,
				pBuffer->nLen - ((sizeof(sockaddr_in) + 16) * 2),
				sizeof(sockaddr_in) + 16,
				sizeof(sockaddr_in) + 16,
				(SOCKADDR **)&pLocalAddr,
				&nLocalLen,
				(SOCKADDR **)&pRemoteAddr,
				&nRmoteLen);

			memcpy(&LocalSockAddr, pLocalAddr, nLocalLen);
			memcpy(&RemoteSockAddr, pRemoteAddr, nRmoteLen);
			printf("Local Accepted client:%s:%d\n", inet_ntoa(LocalSockAddr.sin_addr), ntohs(LocalSockAddr.sin_port));
			printf("Remote Accepted client:%s:%d\n", inet_ntoa(RemoteSockAddr.sin_addr), ntohs(RemoteSockAddr.sin_port));

			RebuildArray();		
			
			// 将数据复制到发送缓冲区
			pSend->nLen = dwTrans;
			memcpy(pSend->buff, pBuffer->buff, dwTrans);

			// 投递此发送I/O（将数据回显给客户）
			if(!PostSend(pSend))
			{
				// 万一出错的话，释放上面刚申请的两个对象
				FreeSocketObj(pSocket);	
				FreeBufferObj(pSend);
				return FALSE;
			}
			// 继续投递接受I/O
			PostAccept(pBuffer);
		}
		break;
	case OP_RECV:	// 接收数据完成
		{
			if(dwTrans > 0)
			{
				// 创建一个缓冲区，以发送数据。这里就使用原来的缓冲区
				PBUFFER_OBJ pSend = pBuffer;
				printf("收到:%s\r\n", pBuffer->buff);
				pSend->nLen = dwTrans;
				
				// 投递发送I/O（将数据回显给客户）
				PostSend(pSend);
			}
			else	// 套节字关闭
			{
	
				// 必须先关闭套节字，以便在此套节字上投递的其它I/O也返回
				if(pSocket->s != INVALID_SOCKET)
				{
					::closesocket(pSocket->s);
					pSocket->s = INVALID_SOCKET;
				}

				if(pSocket->nOutstandingOps == 0)
					FreeSocketObj(pSocket);		
				
				FreeBufferObj(pBuffer);
				return FALSE;
			}
		}
		break;
	case OP_SEND:		// 发送数据完成
		{
			if(dwTrans > 0)
			{
				// 继续使用这个缓冲区投递接收数据的请求
				pBuffer->nLen = BUFFER_SIZE;
				PostRecv(pBuffer);
			}
			else	// 套节字关闭
			{
				// 同样，要先关闭套节字
				if(pSocket->s != INVALID_SOCKET)
				{
					::closesocket(pSocket->s);
					pSocket->s = INVALID_SOCKET;
				}

				if(pSocket->nOutstandingOps == 0)
					FreeSocketObj(pSocket);	

				FreeBufferObj(pBuffer);
				return FALSE;
			}
		}
		break;
	}
	return TRUE;
}
DWORD WINAPI WorkerThreadProc(LPVOID lpParam)
{

	return 1;
}

DWORD WINAPI AcceptThreadProc(LPVOID lpParam)
{


	return 1;
}

void main()
{
	// 创建监听套节字，绑定到本地端口，进入监听模式
	int nPort = 4567;
	SOCKET sListen = ::WSASocket(
		AF_INET, SOCK_STREAM, IPPROTO_TCP, //此三个参数与标准socket相同
		NULL, //指定下层服务提供者，可以是NULL
		0,    //保留
		WSA_FLAG_OVERLAPPED); //指定socket属性，要使用重叠I/O模型，必须指定WSA_FLAG_OVERLAPPED
	//如果使用socket则默认指定WSA_FLAG_OVERLAPPED
	//绑定并监听IP和端口
	SOCKADDR_IN si;
	si.sin_family = AF_INET;
	si.sin_port = ::ntohs(nPort);
	si.sin_addr.S_un.S_addr = INADDR_ANY;
	::bind(sListen, (sockaddr*)&si, sizeof(si));
	::listen(sListen, 200);

	// 为监听套节字创建一个SOCKET_OBJ对象 GetSocketObj函数仅仅开辟一个PSOCKET_OBJ内存并将套接字传入，
	PSOCKET_OBJ pListen = GetSocketObj(sListen);

	// 加载扩展函数AcceptEx
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes;
	//套接字选项和I/O控制命令  此处用来获取AcceptEx函数指针
	WSAIoctl(pListen->s, 
		SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&GuidAcceptEx, 
		sizeof(GuidAcceptEx),
		&pListen->lpfnAcceptEx, 
		sizeof(pListen->lpfnAcceptEx), 
		&dwBytes, 
		NULL, 
		NULL);

	g_pfnGetAcceptExSockaddrs = (LPFN_GETACCEPTEXSOCKADDRS)GetExtensionFuncPtr(pListen->s);
	// 创建用来重新建立g_events数组的事件对象
	g_events[0] = ::WSACreateEvent();

	// 在此可以投递多个接受I/O请求  投递5个接受连接请求
	for(int i=0; i<5; i++)
	{
		PostAccept(GetBufferObj(pListen, BUFFER_SIZE));
	}
	::WSASetEvent(g_events[0]);
	
	while(TRUE)
	{
		int nIndex = 
			::WSAWaitForMultipleEvents(g_nBufferCount + 1, g_events, FALSE, WSA_INFINITE, FALSE);
		if(nIndex == WSA_WAIT_FAILED)
		{
			printf("WSAWaitForMultipleEvents() failed \n");
			break;
		}
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		for(int i=0; i<=nIndex; i++)
		{
			int nRet = ::WSAWaitForMultipleEvents(1, &g_events[i], TRUE, 0, FALSE);
			if(nRet == WSA_WAIT_TIMEOUT)
				continue;
			else
			{
				::WSAResetEvent(g_events[i]);
				// 重新建立g_events数组
				if(i == 0)
				{
					RebuildArray();
					continue;
				}

				// 处理这个I/O
				PBUFFER_OBJ pBuffer = FindBufferObj(g_events[i]);
				if(pBuffer != NULL)
				{
					if(!HandleIO(pBuffer))
						RebuildArray();
				}
			}
		}
	}
}
