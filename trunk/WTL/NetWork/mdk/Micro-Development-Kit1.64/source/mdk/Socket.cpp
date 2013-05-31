// Socket.cpp: implementation of the Socket class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/mdk/Socket.h"

#ifdef WIN32
#pragma comment ( lib, "ws2_32.lib" )
#endif
#include <stdio.h>
using namespace std;
namespace mdk
{

Socket::Socket()
{
	m_hSocket = INVALID_SOCKET;
	m_bBlock = true;
	m_bOpened = false;//δ��״̬
}

Socket::Socket( SOCKET hSocket, protocol nProtocolType )
{
	m_hSocket = hSocket;
	m_bBlock = true;
	m_bOpened = false;//δ��״̬
	Init(nProtocolType);
	InitPeerAddress();
	InitLocalAddress();
}

Socket::~Socket()
{

}

/*
	���ܣ�Socket��ʼ��(windowsר��)
	������
		lpwsaData	WSADATA*	[In]	A pointer to a WSADATA structure. If lpwsaData is not equal to NULL, then the address of the WSADATA structure is filled by the call to ::WSAStartup. This function also ensures that ::WSACleanup is called for you before the application terminates.

	����ֵ����ʱ����TRUE�����򷵻�FALSE
*/
bool Socket::SocketInit(void *lpVoid)
{
#ifdef WIN32
	// initialize Winsock library
	WSADATA *lpwsaData = (WSADATA *)lpVoid;
	WSADATA wsaData;
	if (lpwsaData == NULL)
		lpwsaData = &wsaData;
	
	WORD wVersionRequested = MAKEWORD(1, 1);
	__int32 nResult = WSAStartup(wVersionRequested, lpwsaData);
	if (nResult != 0)
		return false;
	
	if (LOBYTE(lpwsaData->wVersion) != 1 || HIBYTE(lpwsaData->wVersion) != 1)
	{
		WSACleanup();
		return false;
	}
#endif
	return true;
}

void Socket::SocketDestory()
{
#ifdef WIN32
	WSACleanup();
#endif
}

void Socket::GetLastErrorMsg( string &strError )
{
	char strErr[1024];
#ifdef WIN32
	LPSTR lpBuffer;
	DWORD dwErrorCode = WSAGetLastError();
	
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwErrorCode,
		LANG_NEUTRAL,
		(LPTSTR)&lpBuffer,
		0,
		NULL );
	sprintf( strErr, "Socket Error(%ld):%s", dwErrorCode, lpBuffer );
	strError = strErr;
	LocalFree(lpBuffer);

#else
	sprintf( strErr, "socket errno(%d):%s\n", errno, strerror(errno) );
	strError = strErr;
#endif
}

bool Socket::InitForIOCP( SOCKET hSocket )
{
#ifdef WIN32
	return 0 == setsockopt( hSocket,
		SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
		(char *)&(hSocket), sizeof(hSocket) );
#else
	return true;
#endif
}


//ȡ���׽��־��
SOCKET Socket::GetSocket()
{
	return m_hSocket;
}

void Socket::GetPeerAddress( string& strWanIP, int& nWanPort )
{ 
	nWanPort = m_nWanPort;
	strWanIP = m_strWanIP;
	return;
}

bool Socket::InitPeerAddress()
{
	assert( INVALID_SOCKET != m_hSocket );

	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	socklen_t nSockAddrLen = sizeof(sockAddr);
	if ( SOCKET_ERROR == getpeername( m_hSocket, 
		(sockaddr*)&sockAddr, &nSockAddrLen ) ) return false;
	m_nWanPort = ntohs(sockAddr.sin_port);
	m_strWanIP = inet_ntoa(sockAddr.sin_addr);
	
	return true;
}

void Socket::GetLocalAddress( string& strWanIP, int& nWanPort )
{ 
	nWanPort = m_nLocalPort;
	strWanIP = m_strLocalIP;
	return;
}

bool Socket::InitLocalAddress()
{
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	socklen_t nSockAddrLen = sizeof(sockAddr);
	if ( SOCKET_ERROR == getsockname( m_hSocket, 
		(sockaddr*)&sockAddr, &nSockAddrLen )) return false;
	m_nLocalPort = ntohs(sockAddr.sin_port);
	m_strLocalIP = inet_ntoa(sockAddr.sin_addr);

	return true;
}

/*
	���ܣ�һ���Գ�ʼ����m_hSocket�������ⲿ�����������ڲ�����
	������
		nProtocolType	__int32		[In]	A particular protocol to be used with the socket that is specific to the indicated address family.
	����ֵ���ɹ�����TRUE,ʧ�ܷ���FALSE
*/
bool Socket::Init(protocol nProtocolType)
{
	if ( m_bOpened ) return true;
	if ( m_hSocket == INVALID_SOCKET )
	{
		m_hSocket = socket( PF_INET, nProtocolType, 0 );
		if ( m_hSocket == INVALID_SOCKET ) return false;
	}
	m_bOpened = true;
	
	return m_bOpened;
}

/*
	���ܣ��ͻ��˺���������TCP����
	������
		lpszHostAddress	LPCTSTR		[In]	�Է�Ip��ַ
		nHostPort		UINT		[In]	�Է������Ķ˿�
	����ֵ���ɹ�����TRUE,ʧ�ܷ���FALSE
*/
bool Socket::Connect( const char *lpszHostAddress, unsigned short nHostPort)
{
	assert( NULL != lpszHostAddress );

	sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszHostAddress);
	sockAddr.sin_port = htons( nHostPort );

	if ( SOCKET_ERROR != connect(m_hSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) )
	{
		InitPeerAddress();
		InitLocalAddress();
		return true;
	}

	return false;
}

//��ʼ����Service
bool Socket::StartServer( int nPort )
{
	if ( !this->Bind( nPort ) ) return false;
	return this->Listen();
}

//�ǹر�״̬����true,���򷵻�false
bool Socket::IsClosed()
{
	return !m_bOpened;
}
/*
	���ܣ��Ͽ�socket����
	�߼���֧��ɵ��ʽ���ã��κ�ʱ�򶼿��Ե��ô˺����Ͽ����ӣ���Ϊ�Ͽ�״̬
	����ֵ����Զ����TRUE
*/
void Socket::Close()
{
	if ( INVALID_SOCKET == m_hSocket ) return;
	if ( m_bOpened ) 
	{
		closesocket(m_hSocket);
		m_bOpened = false;
	}
	m_hSocket = INVALID_SOCKET;
	return;
}

/*
	���ܣ���һ��socket������ø���������������Ͻ��в���
	�߼���
		ɵ��ʽ�󶨣����������֮ǰ�Ƿ��Ѿ����������׽��֣����Ȼ����ɵ��ʽ����close�ر����ӣ�Ȼ���ڰ��µ��׽��֣�
		���û��ʵ�ֵ���Detach����ɰ󶨣���ô�ɵİ�sock����ʧ
	������
		hSocket	SOCKET	[In]	Ҫ��sock���
*/
void Socket::Attach(SOCKET hSocket)
{
	m_hSocket = hSocket;
	m_bBlock = true;
	m_bOpened = true;//δ��״̬
	InitPeerAddress();
	InitLocalAddress();
}

/*
	���ܣ�����󶨣����ذ󶨵�socket���
	����ֵ���Ѱ󶨵�socket�����������һ��INVALID_SOCKET��˵��֮ǰû���κΰ�
*/
SOCKET Socket::Detach()
{
	SOCKET hSocket = m_hSocket;
	m_hSocket = INVALID_SOCKET;
	m_bBlock = true;
	m_bOpened = false;//δ��״̬
	return hSocket;
}

/*
	���ܣ���������
	������
		lpBuf		void*		[Out]	������յ�����
		nBufLen		__int32		[Out]	�յ����ݵĳ���
		lSecond		long		[In]	��ʱʱ����
		lMinSecond	long		[In]	��ʱʱ�����
	����ֵ��ʵ�ʽ��յ����ֽ�������ʱ����-2���Ͽ����ӷ���-1���������󷵻�-3
*/
int Socket::Receive( void* lpBuf, int nBufLen, bool bCheckDataLength, long lSecond, long lMinSecond )
{
	if ( TimeOut( lSecond, lMinSecond ) ) return seTimeOut;//��ʱ
	int nResult;
	int nFlag = 0;
	if ( bCheckDataLength ) nFlag = MSG_PEEK;
	nResult = recv(m_hSocket, (char*)lpBuf, nBufLen, nFlag);
	if ( 0 == nResult ) return seSocketClose;//�Ͽ�����
	if ( SOCKET_ERROR != nResult ) return nResult;//���쳣����
	
	//socket�����쳣
#ifdef WIN32
		int nError = GetLastError();
		if ( WSAEWOULDBLOCK == nError ) return 0;//������recv���أ������ݿɽ���
		return seError;
#else
		if ( EAGAIN == errno ) return 0;//������recv���أ������ݿɽ���
		return seError;
#endif
}

/*
	���ܣ����ÿ⺯��send��������
	������
		lpBuf	const void*	[In]	���͵�����
		nBufLen	int		[In]	���ݳ���
		nFlags	int		[In]	An indicator specifying the way in which the call is made
	����ֵ���ɹ�ʵ�ʷ��͵��ֽ�����ʧ�ܷ���С��0
*/
int Socket::Send( const void* lpBuf, int nBufLen, int nFlags )
{
	int nSendSize = send(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
	if ( 0 > nSendSize ) 
	{
#ifdef WIN32
		int nError = GetLastError();
		if ( WSAEWOULDBLOCK == nError ) return 0;//����������������,����WSAEWOULDBLOCK���η��ͻ���ᱻϵͳ��գ���1byteҲ���ᱻ���ͣ����Է���ʵ�ʷ���0byte
		return seError;
#else
		if ( EAGAIN == errno ) return 0;//������������������ͬ��
		return seError;
#endif
	}
	if ( nSendSize <= nBufLen ) 
	{
		return nSendSize;
	}
	return seError;
}

/*
	���ܣ�����˺������󶨼����Ķ˿���IP
	������
		nSocketPort			UINT		[In]	�����Ķ˿�
		lpszSocketAddress	LPCTSTR		[In]	IP
	����ֵ���ɹ�����TRUE�����򷵻�FALSE
*/
bool Socket::Bind( unsigned short nPort, char *strIP )
{
	memset(&m_sockAddr,0,sizeof(m_sockAddr));
	m_sockAddr.sin_family = AF_INET;
	if ( NULL == strIP ) m_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		unsigned long lResult = inet_addr( strIP );
		if ( lResult == INADDR_NONE ) return false;
		m_sockAddr.sin_addr.s_addr = lResult;
	}
	m_sockAddr.sin_port = htons((unsigned short)nPort);

	return (SOCKET_ERROR != bind(m_hSocket, (sockaddr*)&m_sockAddr, sizeof(m_sockAddr)));
}

/*
	���ܣ�����˺�������ʼ����
	������
		nConnectionBacklog	__int32	[In]	���������
	����ֵ���ɹ�����TRUE�����򷵻�FALSE
*/
bool Socket::Listen( int nConnectionBacklog )
{ 
	return (SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog)); 
}

/*
	���ܣ�����˺��������տͻ�������
	������
	rConnectedSocket	Socket	[In]	client socket����
	����ֵ���ɹ�����TRUE�����򷵻�FALSE
	��ע������TRUE����ʾrConnectedSocket�ȷ��ز���ֵ��Ч����Ϊ����Ƿ�����ģʽ��
	�����ӵ���ú���һ������TRUE������ʱrConnectedSocket��������ָ��INVALID_SOCKET
*/
bool Socket::Accept(Socket& rConnectedSocket)
{
	assert( INVALID_SOCKET == rConnectedSocket.m_hSocket );
	rConnectedSocket.m_hSocket = accept(m_hSocket, NULL, NULL);
	if ( INVALID_SOCKET == rConnectedSocket.m_hSocket )
	{
#ifdef WIN32
		if ( WSAEWOULDBLOCK == GetLastError() ) return true;//���������أ����������󵽴�
#else
		if ( EAGAIN == errno ) return true;//���������أ����������󵽴�
#endif
		return false;//socket�쳣
	}
	rConnectedSocket.m_bOpened = true;
	rConnectedSocket.InitPeerAddress();
	rConnectedSocket.InitLocalAddress();
	return true;
}

/*
	���ܣ��׽������ã��˿����õ�
	������
		nOptionName		__int32		[In]	The socket option for which the value is to be set
		lpOptionValue	const void*	[In]	A pointer to the buffer in which the value for the requested option is supplied
		nOptionLen		__int32		[In]	lpOptionValue�Ĵ�С
		nLevel			__int32		[In]	The level at which the option is defined; the supported levels include SOL_SOCKET and IPPROTO_TCP. See the Windows Sockets 2 Protocol-Specific Annex (a separate document included with the Platform SDK) for more information on protocol-specific levels
	����ֵ���ɹ�����TRUE�����򷵻�FALSE
*/
bool Socket::SetSockOpt(
						   int nOptionName, 
						   const void* lpOptionValue, 
						   int nOptionLen, 
						   int nLevel)
{ 
	return ( SOCKET_ERROR != setsockopt( 
		m_hSocket, 
		nLevel, 
		nOptionName, 
		(char *)lpOptionValue, 
		nOptionLen)); 
}

/*
	���ܣ���ʱ����
	������
		lSecond		long	[In]	��ʱ������
		lMinSecond	long	[In]	��ʱ���ú���
	����ֵ����ʱ����TRUE�����򷵻�FALSE
*/
bool Socket::TimeOut( long lSecond, long lMinSecond )
{
	if ( lSecond <= 0 && lMinSecond <= 0 ) return false;
	//���ճ�ʱ����
	timeval outtime;//��ʱ�ṹ
	outtime.tv_sec = lSecond;
	outtime.tv_usec =lMinSecond;
	int nSelectRet;
#ifdef WIN32
	FD_SET readfds = { 1, m_hSocket };
	nSelectRet=::select( 0, &readfds, NULL, NULL, &outtime ); //���ɶ�״̬
#else
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(m_hSocket, &readfds);
	nSelectRet=::select(m_hSocket+1, &readfds, NULL, NULL, &outtime); //���ɶ�״̬
#endif

	if ( SOCKET_ERROR == nSelectRet ) 
	{
		return true;
	}
	if ( 0 == nSelectRet ) //��ʱ�������޿ɶ����� 
	{
		return true;
	}

	return false;
}

//���ܣ��ȴ�����
bool Socket::WaitData()
{
	int nSelectRet;
#ifdef WIN32
	FD_SET readfds = { 1, m_hSocket };
	nSelectRet=::select( 0, &readfds, NULL, NULL, NULL ); //���ɶ�״̬
#else
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(m_hSocket, &readfds);
	nSelectRet=::select(m_hSocket+1, &readfds, NULL, NULL, NULL); //���ɶ�״̬
#endif
	if ( SOCKET_ERROR == nSelectRet ) 
	{
		return false;
	}
	if ( 0 == nSelectRet ) //��ʱ�������޿ɶ����� 
	{
		return false;
	}
	return true;
}

/*
	���ܣ�������ʽ����
	������
		bWait		bool	[In]	TRUE������FALSE������
	����ֵ���ɹ�����TRUE�����򷵻�FALSE
*/
bool Socket::SetSockMode( bool bWait )
{
#ifdef WIN32
	m_bBlock = bWait;
	unsigned long ul = 1;
	if ( m_bBlock ) ul = 0;
	else ul = 1;
	int ret = ioctlsocket( m_hSocket, FIONBIO, (unsigned long*)&ul );
	if ( ret == SOCKET_ERROR )
	{
		return false;
	} 
#else
	m_bBlock = bWait;
	int flags = fcntl( m_hSocket, F_GETFL, 0 ); //ȡ�õ�ǰ״̬����
	if ( !m_bBlock ) 
		fcntl( m_hSocket, F_SETFL, flags|O_NONBLOCK );//׷�ӷ�������־
	else 
		fcntl( m_hSocket, F_SETFL, flags&(~O_NONBLOCK&0xffffffff) );//ȥ����������־������״̬
#endif
	return true;
}

/*
	���ܣ����ÿ⺯��sendto����UDP����
	������
	    strIP	const char*	[In]	���շ�IP
		nPort	int		[In]	���շ��˿�
		lpBuf	const void*	[In]	���͵�����
		nBufLen	int		[In]	���ݳ���
		nFlags	int		[In]	An indicator specifying the way in which the call is made
	����ֵ���ɹ�����ʵ�ʷ����ֽ���������С�������͵ĳ��ȣ�ʧ�ܷ��س���SOCKET_ERROR,����WSAGetLastError�����ɻ�ȡ������Ϣ
*/
int Socket::SendTo( const char *strIP, int nPort, const void* lpBuf, int nBufLen, int nFlags )
{
	sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(nPort);
	sockAddr.sin_addr.s_addr = inet_addr(strIP);

	int ret = sendto( m_hSocket, (const char*)lpBuf, nBufLen, nFlags, 
		(sockaddr*)&sockAddr, sizeof(sockaddr));
	if (ret < 0) ret = -1;
	return ret;
}

/*
	���ܣ����ÿ⺯��recvFrom����UDP����
	������
		lpBuf		void*		[Out]	������յ�����
		nBufLen		int			[Out]	�յ����ݵĳ���
		strFromIP	string&		[Out]	���ͷ�IP
		nFromPort	int&		[Out]	���ͷ��˿�
		lSecond		long		[In]	��ʱʱ����
		lMinSecond	long		[In]	��ʱʱ�����
		����ֵ��ʵ�ʽ��յ����ֽ�������ʱ����0
*/
int Socket::ReceiveFrom( char* lpBuf, int nBufLen, string &strFromIP, int &nFromPort, bool bCheckDataLength, long lSecond, long lMinSecond )
{
	strFromIP = "";
	nFromPort = -1;
	if ( 0 >= nBufLen ) return 0;
	sockaddr_in sockAddr;
	socklen_t nAddrLen = sizeof(sockaddr);
	/* waiting for receive data */
	int nResult;
	int nFlag = 0;
	while ( true )
	{
		if ( TimeOut( lSecond, lMinSecond ) ) return seTimeOut;
		if ( bCheckDataLength )nFlag = MSG_PEEK;
		nResult = recvfrom(m_hSocket, lpBuf, nBufLen, nFlag, (sockaddr*)&sockAddr, &nAddrLen);
		if ( nAddrLen > 0 ) GetAddress(sockAddr, strFromIP, nFromPort);
		if ( SOCKET_ERROR == nResult ) //socket�����쳣
		{
#ifndef WIN32
			if ( EAGAIN == errno ) return 0;//������recv���أ������ݿɽ���
			return seError;
#else
			int nError = GetLastError();
			if ( 0 == nError )//֮ǰ����ʧ�ܵ�udp����
			{
				if ( MSG_PEEK == nFlag )//û��ɾ�����ջ��壬�ӽ��ջ��彫��Ϣɾ��
				{
					recvfrom(m_hSocket, lpBuf, nBufLen, 0, (sockaddr*)&sockAddr, &nAddrLen);
				}
				continue;
			}
			if ( WSAEWOULDBLOCK == nError ) return 0;//������recv���أ������ݿɽ���
			return seError;
#endif
		}		
		break;
	}
	return nResult;
}

void Socket::GetAddress( const sockaddr_in &sockAddr, string &strIP, int &nPort )
{
	nPort = ntohs(sockAddr.sin_port);
	strIP = inet_ntoa(sockAddr.sin_addr);
}

}//namespace mdk
