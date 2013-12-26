// STIocp.cpp: implementation of the STIocp class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/frame/netserver/STIocp.h"
#ifdef WIN32
#pragma comment ( lib, "mswsock.lib" )
#pragma comment ( lib, "ws2_32.lib" )
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace mdk
{

STIocp::STIocp()
:m_iocpDataPool( sizeof(IOCP_OVERLAPPED), 200 )
{
	
}

STIocp::~STIocp()
{

}

int STIocp::GetError(SOCKET sock, WSAOVERLAPPED* pWSAOVERLAPPED)  
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

bool STIocp::Start( int nMaxMonitor )
{
#ifdef WIN32
	//����IOCP����
	//������ɶ˿�
	m_hCompletPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 
		0, 0, m_nCPUCount * 2 );
	if ( NULL == m_hCompletPort ) 
	{
		m_initError = "create iocp monitor faild";
		return false;
	}
	return true;

#endif
	return true;
}

bool STIocp::Stop()
{
#ifdef WIN32
	m_olExit.completiontype = STIocp::stop;
	return 0 != PostQueuedCompletionStatus( m_hCompletPort, 
		0, NULL, &m_olExit.m_overlapped );
#endif
	return false;
}

bool STIocp::AddMonitor( SOCKET sock )
{
#ifdef WIN32
	//�������׽��ּ���IOCP�ж�
	if ( NULL == CreateIoCompletionPort( (HANDLE)sock, m_hCompletPort, 
		(DWORD)sock, 0 ) ) return false;
	return true;

#endif
	return true;
}

bool STIocp::WaitEvent( IO_EVENT &sockEvent, int timeout )
{
#ifdef WIN32
	sockEvent.type = STIocp::timeout;
	/*
		��ɲ����ϵĴ��䳤��
		����accept�������ʱΪ0
		����recv/send�������ʱ����¼����/�����ֽ���
		���recv/send�����ϵĴ��䳤��С�ڵ���0��ʾ�׽����ѹر�
	*/
	DWORD dwIOSize;
	IOCP_OVERLAPPED *pOverlapped = NULL;//�����������
	if ( !::GetQueuedCompletionStatus( m_hCompletPort, &dwIOSize, 
						(LPDWORD)&sockEvent.sock, (OVERLAPPED**)&pOverlapped,
						timeout ) )
	{
		if ( NULL == pOverlapped ) return true;
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
			if ( STIocp::connect == pOverlapped->completiontype ) 
			{
				m_iocpDataPool.Free(pOverlapped);
				pOverlapped = NULL;
				return false;
			}
		}
		if ( STIocp::connect == pOverlapped->completiontype ) //Accept�ϵ�socket�رգ�����Ͷ�ݼ���
		{
			AddAccept(sockEvent.sock);
		}
		else//�ͻ����쳣�Ͽ��������ߣ��ϵ磬��ֹ����
		{
			sockEvent.type = STIocp::close;
		}
	}
	else if ( STIocp::stop == pOverlapped->completiontype )
	{
		sockEvent.type = STIocp::stop;
		return true;
	}
	else if ( 0 == dwIOSize && STIocp::recv == pOverlapped->completiontype )
	{
		sockEvent.type = STIocp::close;
	}
	else//io�¼�
	{
		//io�¼�
		sockEvent.type = pOverlapped->completiontype;
		sockEvent.client = pOverlapped->sock;
		sockEvent.pData = pOverlapped->m_wsaBuffer.buf;
		sockEvent.uDataSize = dwIOSize;
	}
	m_iocpDataPool.Free(pOverlapped);
	pOverlapped = NULL;
				
	return true;
	
#endif
	return true;
}

bool STIocp::AddAccept( SOCKET listenSocket )
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
	pOverlapped->completiontype = STIocp::connect;
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
bool STIocp::AddRecv( SOCKET socket, char* recvBuf, unsigned short bufSize )
{
#ifdef WIN32
	IOCP_OVERLAPPED *pOverlapped = new (m_iocpDataPool.Alloc())IOCP_OVERLAPPED;
	if ( NULL == pOverlapped )return false;
	memset( &pOverlapped->m_overlapped, 0, sizeof(OVERLAPPED) );
	pOverlapped->m_wsaBuffer.buf = recvBuf;
	pOverlapped->m_wsaBuffer.len = bufSize;
	pOverlapped->m_overlapped.Internal = 0;
	pOverlapped->sock = socket;
	pOverlapped->completiontype = STIocp::recv;
	
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
bool STIocp::AddSend( SOCKET socket, char* dataBuf, unsigned short dataSize )
{
#ifdef WIN32
	IOCP_OVERLAPPED *pOverlapped = new (m_iocpDataPool.Alloc())IOCP_OVERLAPPED;
	if ( NULL == pOverlapped ) return false;
	memset( &pOverlapped->m_overlapped, 0, sizeof(OVERLAPPED) );
	pOverlapped->m_wsaBuffer.buf = dataBuf;
	pOverlapped->m_wsaBuffer.len = dataSize;
	pOverlapped->m_overlapped.Internal = 0;
	pOverlapped->sock = socket;
	pOverlapped->completiontype = STIocp::send;
	
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
