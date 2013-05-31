// IOCPMonitor.cpp: implementation of the IOCPMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/frame/netserver/IOCPMonitor.h"
#ifdef WIN32
#pragma comment ( lib, "mswsock.lib" )
#pragma comment ( lib, "ws2_32.lib" )
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace mdk
{

IOCPMonitor::IOCPMonitor()
:m_iocpDataPool( sizeof(IOCP_OVERLAPPED), 200 )
{
	m_nCPUCount = 0;
#ifdef WIN32
	SYSTEM_INFO sysInfo;
	::GetSystemInfo(&sysInfo);
	m_nCPUCount = sysInfo.dwNumberOfProcessors;
#endif
}

IOCPMonitor::~IOCPMonitor()
{

}

int IOCPMonitor::GetError(SOCKET sock, WSAOVERLAPPED* pWSAOVERLAPPED)  
{  
#ifdef WIN32
	DWORD dwTrans;  
	DWORD dwFlags;  
	if(FALSE == WSAGetOverlappedResult(sock, pWSAOVERLAPPED, &dwTrans, FALSE, &dwFlags))  
		return WSAGetLastError();  
	else  
		return ERROR_SUCCESS;  
#endif
	return 0;
}  

bool IOCPMonitor::Start( int nMaxMonitor )
{
#ifdef WIN32
	//����IOCP����
	//������ɶ˿�
	int NumberOfConcurrentThreads = 0;
	if ( 0 < m_nCPUCount ) NumberOfConcurrentThreads = m_nCPUCount * 2 + 2;
	m_hCompletPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 
		0, 0, NumberOfConcurrentThreads );
	if ( NULL == m_hCompletPort ) 
	{
		m_initError = "create iocp monitor faild";
		return false;
	}
	return true;

#endif
	return true;
}

bool IOCPMonitor::AddMonitor( SOCKET sock )
{
#ifdef WIN32
	//�������׽��ּ���IOCP�ж�
	if ( NULL == CreateIoCompletionPort( (HANDLE)sock, m_hCompletPort, 
		(DWORD)sock, 0 ) ) return false;
	return true;

#endif
	return true;
}

bool IOCPMonitor::WaitEvent( void *eventArray, int &count, bool block )
{
#ifdef WIN32
	IO_EVENT *events = (IO_EVENT*)eventArray;
	count = 0;
	/*
		��ɲ����ϵĴ��䳤��
		����accept�������ʱΪ0
		����recv/send�������ʱ����¼����/�����ֽ���
		���recv/send�����ϵĴ��䳤��С�ڵ���0��ʾ�׽����ѹر�
	*/
	DWORD dwIOSize;
	IOCP_OVERLAPPED *pOverlapped = NULL;//�����������
	if ( !::GetQueuedCompletionStatus( m_hCompletPort, &dwIOSize, 
						(LPDWORD)&events[count].sock, (OVERLAPPED**)&pOverlapped,
						INFINITE ) )
	{
		DWORD dwErrorCode = GetLastError();
		if ( ERROR_INVALID_HANDLE == dwErrorCode ) //����ʱ��δ֪ԭ��Ƿ�����������ǲ���ǰһ�γ���رպ�δ���������¼�
		{
			try
			{
				if ( NULL != pOverlapped )
				{
					m_iocpDataPool.Free(pOverlapped);
					pOverlapped = NULL;
					return true;
				}
			}catch(...)
			{
			}
			return true;
		}
		
		if ( ERROR_OPERATION_ABORTED == dwErrorCode ) 
		{
			if ( IOCPMonitor::connect == pOverlapped->completiontype ) 
			{
				m_iocpDataPool.Free(pOverlapped);
				pOverlapped = NULL;
				return false;
			}
		}
		if ( IOCPMonitor::connect == pOverlapped->completiontype ) //Accept�ϵ�socket�رգ�����Ͷ�ݼ���
		{
			AddAccept(events[count].sock);
		}
		else//�ͻ����쳣�Ͽ��������ߣ��ϵ磬��ֹ����
		{
			events[count].type = IOCPMonitor::close;
			count++;
		}
	}
	else if ( 0 == dwIOSize && IOCPMonitor::recv == pOverlapped->completiontype )
	{
		events[count].type = IOCPMonitor::close;
		count++;
	}
	else//io�¼�
	{
		//io�¼�
		events[count].type = pOverlapped->completiontype;
		events[count].client = pOverlapped->sock;
		events[count].pData = pOverlapped->m_wsaBuffer.buf;
		events[count].uDataSize = dwIOSize;
		count++;
	}
	m_iocpDataPool.Free(pOverlapped);
	pOverlapped = NULL;
				
	return true;
	
#endif
	return true;
}

bool IOCPMonitor::AddAccept( SOCKET listenSocket )
{
#ifdef WIN32
	if ( SOCKET_ERROR == listenSocket ) return false;
	//��������
	IOCP_OVERLAPPED *pOverlapped = new (m_iocpDataPool.Alloc())IOCP_OVERLAPPED;
	if ( NULL == pOverlapped ) return false;
	memset( &pOverlapped->m_overlapped, 0, sizeof(OVERLAPPED) );
	pOverlapped->m_dwLocalAddressLength = sizeof(SOCKADDR_IN) + 16;//�ͻ��˾�����IP
	pOverlapped->m_dwRemoteAddressLength = sizeof(SOCKADDR_IN) + 16;//�ͻ�������IP
	memset( pOverlapped->m_outPutBuf, 0, sizeof(SOCKADDR_IN)*2+32 );
	Socket client;
	client.Init( Socket::tcp );
	pOverlapped->sock = client.GetSocket();
	pOverlapped->completiontype = IOCPMonitor::connect;
	//Ͷ�ݽ������Ӳ���
	if ( !AcceptEx( listenSocket,
		client.GetSocket(), 
		pOverlapped->m_outPutBuf, 0,
		pOverlapped->m_dwLocalAddressLength, 
		pOverlapped->m_dwRemoteAddressLength, 
		NULL, &pOverlapped->m_overlapped ) )
	{
		int nErrCode = WSAGetLastError();
		if ( ERROR_IO_PENDING != nErrCode ) 
		{
			m_iocpDataPool.Free(pOverlapped);
			pOverlapped = NULL;
			return false;
		}
	}
	
	return true;
	
#endif
	return true;
}

//����һ���������ݵĲ����������ݵ��WaitEvent�᷵��
bool IOCPMonitor::AddRecv( SOCKET socket, char* recvBuf, unsigned short bufSize )
{
#ifdef WIN32
	IOCP_OVERLAPPED *pOverlapped = new (m_iocpDataPool.Alloc())IOCP_OVERLAPPED;
	if ( NULL == pOverlapped )return false;
	memset( &pOverlapped->m_overlapped, 0, sizeof(OVERLAPPED) );
	pOverlapped->m_wsaBuffer.buf = recvBuf;
	pOverlapped->m_wsaBuffer.len = bufSize;
	pOverlapped->m_overlapped.Internal = 0;
	pOverlapped->sock = socket;
	pOverlapped->completiontype = IOCPMonitor::recv;
	
	DWORD dwRecv = 0;
	DWORD dwFlags = 0;
	//Ͷ�����ݽ��ղ���
	if ( ::WSARecv( socket, 
		&pOverlapped->m_wsaBuffer, 
		1, &dwRecv, &dwFlags, 
		&pOverlapped->m_overlapped, NULL ) )
	{
		int nErrCode = WSAGetLastError();
		if ( ERROR_IO_PENDING != nErrCode ) 
		{
			m_iocpDataPool.Free(pOverlapped);
			pOverlapped = NULL;
			return false;
		}
	}
	return true;
	
#endif
	return true;
}

//����һ���������ݵĲ�����������ɣ�WaitEvent�᷵��
bool IOCPMonitor::AddSend( SOCKET socket, char* dataBuf, unsigned short dataSize )
{
#ifdef WIN32
	IOCP_OVERLAPPED *pOverlapped = new (m_iocpDataPool.Alloc())IOCP_OVERLAPPED;
	if ( NULL == pOverlapped ) return false;
	memset( &pOverlapped->m_overlapped, 0, sizeof(OVERLAPPED) );
	pOverlapped->m_wsaBuffer.buf = dataBuf;
	pOverlapped->m_wsaBuffer.len = dataSize;
	pOverlapped->m_overlapped.Internal = 0;
	pOverlapped->sock = socket;
	pOverlapped->completiontype = IOCPMonitor::send;
	
	DWORD dwSend = 0;
	DWORD dwFlags = 0;
	//Ͷ�����ݽ��ղ���
	if ( ::WSASend( socket, 
		&pOverlapped->m_wsaBuffer, 
		1, &dwSend, dwFlags, 
		&pOverlapped->m_overlapped, NULL ) )
	{
		int nErrCode = WSAGetLastError();
		if ( ERROR_IO_PENDING != nErrCode ) 
		{
			m_iocpDataPool.Free(pOverlapped);
			pOverlapped = NULL;
			return false;
		}
	}
	return true;
	
#endif
	return true;
}

}//namespace mdk
