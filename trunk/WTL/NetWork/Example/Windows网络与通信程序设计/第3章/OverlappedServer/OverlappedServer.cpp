///////////////////////////////////////////////////////
// OverlappedServer.cpp�ļ�

#include "../common/initsock.h"

#include <Mswsock.h>
#include <stdio.h>
#include <windows.h>

CInitSocket clsInitSock;
LPFN_GETACCEPTEXSOCKADDRS g_pfnGetAcceptExSockaddrs = NULL; //��ȡ����addr
#define BUFFER_SIZE 1024

typedef struct _SOCKET_OBJ
{
	SOCKET s;						// �׽��־��
	int nOutstandingOps;			// ��¼���׽����ϵ��ص�I/O����
	LPFN_ACCEPTEX lpfnAcceptEx;		// ��չ����AcceptEx��ָ�루���Լ����׽��ֶ��ԣ�
} SOCKET_OBJ, *PSOCKET_OBJ;

typedef struct _BUFFER_OBJ
{	
	OVERLAPPED ol;			// �ص��ṹ
	char *buff;				// send/recv/AcceptEx��ʹ�õĻ�����
	int nLen;				// buff�ĳ���
	PSOCKET_OBJ pSocket;	// ��I/O�������׽��ֶ���

	int nOperation;			// �ύ�Ĳ�������
#define OP_ACCEPT	1
#define OP_RECV		2
#define OP_SEND	    3

	SOCKET sAccept;			// ��������AcceptEx���ܵĿͻ��׽��֣����Լ����׽��ֶ��ԣ�
	_BUFFER_OBJ *pNext;
} BUFFER_OBJ, *PBUFFER_OBJ;

HANDLE g_events[WSA_MAXIMUM_WAIT_EVENTS];	// I/O�¼��������  64 ��0��Ϊ���ؼ����¼����
int g_nBufferCount;							// ����������Ч�������
PBUFFER_OBJ g_pBufferHead, g_pBufferTail;	// ��¼������������ɵ������ͷ��ַ

//��ȡ��չ����ָ��
PVOID GetExtensionFuncPtr(SOCKET sock)
{
	DWORD dwBytes;
	PVOID pfn = NULL;
	GUID guid = WSAID_GETACCEPTEXSOCKADDRS;  //GetAcceptExSockAddrs

	//I/O��������winsock1����ioctlsocket��winsock2����WSAIoctl,��һ���׽��ֽ�������
	//ͨ��WSAIoctl() (ѡ�����ΪSIO_GET_EXTENSION_FUNCTION_POINTER)��̬��ú�����ָ��
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
// �����׽��ֶ�����ͷ��׽��ֶ���ĺ���
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

		// ���µ�BUFFER_OBJ��ӵ��б���
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
	// ���б����Ƴ�BUFFER_OBJ����
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
	// �ͷ���ռ�õ��ڴ�ռ�
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

//�ؽ�������������IO�¼��������Ķ�Ӧ��ϵ
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

//Ͷ��һ��������������
BOOL PostAccept(PBUFFER_OBJ pBuffer)
{
	PSOCKET_OBJ pSocket = pBuffer->pSocket;
	

	if(pSocket->lpfnAcceptEx != NULL)
	{	
		// ����I/O���ͣ������׽����ϵ��ص�I/O����
		pBuffer->nOperation = OP_ACCEPT;
		pSocket->nOutstandingOps ++;

		// Ͷ�ݴ��ص�I/O  
		DWORD dwBytes;
		pBuffer->sAccept = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);  //����socket
		//��ʼ����
		BOOL b = pSocket->lpfnAcceptEx(pSocket->s,   //�����׽���
			pBuffer->sAccept,   //�����׽���
			pBuffer->buff, 
			BUFFER_SIZE - ((sizeof(sockaddr_in) + 16) * 2), //buf��С
			sizeof(sockaddr_in) + 16,  //��������Ϊ���ص�ַԤ���ĳ��ȣ���������רַ���ȶ�16
			sizeof(sockaddr_in) + 16,  //��������ΪԶ�̵�ַԤ���ĳ��ȣ���������רַ���ȶ�16
			&dwBytes,                  //�������յ������ݳ���
			&pBuffer->ol);             //��������������ص��ṹ(OVERLAPPED)
		if(!b)
		{
			if(::WSAGetLastError() != WSA_IO_PENDING)
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
};

//Ͷ��һ������
BOOL PostRecv(PBUFFER_OBJ pBuffer)
{	
	// ����I/O���ͣ������׽����ϵ��ص�I/O����
	pBuffer->nOperation = OP_RECV;
	pBuffer->pSocket->nOutstandingOps ++;

	// Ͷ�ݴ��ص�I/O
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

//Ͷ��һ������
BOOL PostSend(PBUFFER_OBJ pBuffer)
{
	// ����I/O���ͣ������׽����ϵ��ص�I/O����
	pBuffer->nOperation = OP_SEND;
	pBuffer->pSocket->nOutstandingOps ++;

	// Ͷ�ݴ��ص�I/O
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
	PSOCKET_OBJ pSocket = pBuffer->pSocket; // ��BUFFER_OBJ��������ȡSOCKET_OBJ����ָ�룬Ϊ���Ƿ�������
	pSocket->nOutstandingOps --;
	
	DWORD dwTrans;   //����ȡ��ʵ�ʴ����ֽڵ�����
	DWORD dwFlags;   //����ȡ�����״̬
	// ��ȡ�ص�������� true �ص������ɹ�  false �׽������д�����
	BOOL bRet = ::WSAGetOverlappedResult(pSocket->s, &pBuffer->ol, &dwTrans, FALSE, &dwFlags);
	if(!bRet)
	{
		// �ڴ��׽������д���������ˣ��ر��׽��֣��Ƴ��˻���������
		// ���û�������׳���I/O�����ˣ��ͷŴ˻��������󣬷��򣬵ȴ����׽����ϵ�����I/OҲ���
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

	// û�д���������������ɵ�I/O
	switch(pBuffer->nOperation)
	{
	case OP_ACCEPT:	// ���յ�һ���µ����ӣ������յ��˶Է������ĵ�һ�����
		{
			// Ϊ�¿ͻ�����һ��SOCKET_OBJ����
			PSOCKET_OBJ pClient = GetSocketObj(pBuffer->sAccept);

			// Ϊ�շ����ݴ���һ��BUFFER_OBJ���������������׽��ֳ�����߹ر�ʱ�ͷ�
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
			
			// �����ݸ��Ƶ����ͻ�����
			pSend->nLen = dwTrans;
			memcpy(pSend->buff, pBuffer->buff, dwTrans);

			// Ͷ�ݴ˷���I/O�������ݻ��Ը��ͻ���
			if(!PostSend(pSend))
			{
				// ��һ����Ļ����ͷ�������������������
				FreeSocketObj(pSocket);	
				FreeBufferObj(pSend);
				return FALSE;
			}
			// ����Ͷ�ݽ���I/O
			PostAccept(pBuffer);
		}
		break;
	case OP_RECV:	// �����������
		{
			if(dwTrans > 0)
			{
				// ����һ�����������Է������ݡ������ʹ��ԭ���Ļ�����
				PBUFFER_OBJ pSend = pBuffer;
				printf("�յ�:%s\r\n", pBuffer->buff);
				pSend->nLen = dwTrans;
				
				// Ͷ�ݷ���I/O�������ݻ��Ը��ͻ���
				PostSend(pSend);
			}
			else	// �׽��ֹر�
			{
	
				// �����ȹر��׽��֣��Ա��ڴ��׽�����Ͷ�ݵ�����I/OҲ����
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
	case OP_SEND:		// �����������
		{
			if(dwTrans > 0)
			{
				// ����ʹ�����������Ͷ�ݽ������ݵ�����
				pBuffer->nLen = BUFFER_SIZE;
				PostRecv(pBuffer);
			}
			else	// �׽��ֹر�
			{
				// ͬ����Ҫ�ȹر��׽���
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
	// ���������׽��֣��󶨵����ض˿ڣ��������ģʽ
	int nPort = 4567;
	SOCKET sListen = ::WSASocket(
		AF_INET, SOCK_STREAM, IPPROTO_TCP, //�������������׼socket��ͬ
		NULL, //ָ���²�����ṩ�ߣ�������NULL
		0,    //����
		WSA_FLAG_OVERLAPPED); //ָ��socket���ԣ�Ҫʹ���ص�I/Oģ�ͣ�����ָ��WSA_FLAG_OVERLAPPED
	//���ʹ��socket��Ĭ��ָ��WSA_FLAG_OVERLAPPED
	//�󶨲�����IP�Ͷ˿�
	SOCKADDR_IN si;
	si.sin_family = AF_INET;
	si.sin_port = ::ntohs(nPort);
	si.sin_addr.S_un.S_addr = INADDR_ANY;
	::bind(sListen, (sockaddr*)&si, sizeof(si));
	::listen(sListen, 200);

	// Ϊ�����׽��ִ���һ��SOCKET_OBJ���� GetSocketObj������������һ��PSOCKET_OBJ�ڴ沢���׽��ִ��룬
	PSOCKET_OBJ pListen = GetSocketObj(sListen);

	// ������չ����AcceptEx
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes;
	//�׽���ѡ���I/O��������  �˴�������ȡAcceptEx����ָ��
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
	// �����������½���g_events������¼�����
	g_events[0] = ::WSACreateEvent();

	// �ڴ˿���Ͷ�ݶ������I/O����  Ͷ��5��������������
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
				// ���½���g_events����
				if(i == 0)
				{
					RebuildArray();
					continue;
				}

				// �������I/O
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
