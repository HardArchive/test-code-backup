// Socket.h: interface for the Socket class.
//
//////////////////////////////////////////////////////////////////////
/***********************************************************************
��˵����	��װSocket
ʹ��˵����	�����windows���ڸ���Ķ������ڽ������߳���Ҫ����1�ξ�̬��Ա����SocketInit(NULL)
************************************************************************/

#ifndef MDK_SOCKET_H
#define MDK_SOCKET_H

#ifdef WIN32
#include <windows.h>
//#include <winsock2.h>  
//#include <mswsock.h>
//#define SOCK_STREAM 1
//#define SOCK_DGRAM 2
//#define SOL_SOCKET 0xffff
//#define SOMAXCONN       5

//typedef unsigned int SOCKET;
//struct sockaddr_in;
//{
//};
#define socklen_t int
#else
#include <errno.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
typedef int SOCKET;
#endif


#include <time.h>
#include <assert.h>
#include <string>

namespace mdk
{

class Socket
{
public:
	enum SocketError
	{
		seTimeOut = -2,
		seSocketClose = -1,
		seError = -3,
	};
	enum protocol 
	{
		tcp = SOCK_STREAM,
		udp = SOCK_DGRAM,
	};

	Socket();
	Socket( SOCKET hSocket, protocol nProtocolType );
	virtual ~Socket();
	
public:
	//ȡ���׽��־��
	SOCKET GetSocket();
	/*
		���ܣ�һ���Գ�ʼ����m_hSocket�������ⲿ�����������ڲ�����
		������
			nProtocolType	protocol		[In]	Socket::tcp��Socket::udp
		����ֵ���ɹ�����TRUE,ʧ�ܷ���FALSE
	*/
	bool Init(protocol nProtocolType);
	/**
		���ܣ��Ͽ�socket����
		����:
				������	����	[In/Out]	˵��   
		����ֵ:
		ע��:
				֧��ɵ��ʽ���ã��κ�ʱ�򶼿��Ե��ô˺����Ͽ����ӣ���Ϊ�Ͽ�״̬
	 */
	void Close();
	//�ǹر�״̬����true,���򷵻�false
	bool IsClosed();

	//////////////////////////////////////////////////////////////////////////
	//TCP�շ�
	/*
		���ܣ����ÿ⺯��send��������
		������
			lpBuf	const void*	[In]	���͵�����
			nBufLen	int		[In]	���ݳ���
			nFlags	int		[In]	An indicator specifying the way in which the call is made
		����ֵ���ɹ�ʵ�ʷ��͵��ֽ�����ʧ�ܷ���-1
	*/
	int Send( const void* lpBuf, int nBufLen, int nFlags = 0 );
	/*
		���ܣ���������
		������
			lpBuf		void*		[Out]	������յ�����
			nBufLen		int			[Out]	�յ����ݵĳ���
			lSecond		long		[In]	��ʱʱ����
			lMinSecond	long		[In]	��ʱʱ�����
		����ֵ��ʵ�ʽ��յ����ֽ�������ʱ����-1
	*/
	int Receive( void* lpBuf, int nBufLen, bool bCheckDataLength = false, long lSecond = 0, long lMinSecond = 0 );

	//////////////////////////////////////////////////////////////////////////
	//UDP�շ�
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
	int SendTo( const char *strIP, int nPort, const void* lpBuf, int nBufLen, int nFlags = 0 );
	/*
		���ܣ����ÿ⺯��recvFrom����UDP����
		������
			lpBuf		void*		[Out]	������յ�����
			nBufLen		int			[Out]	�յ����ݵĳ���
			strFromIP	string&		[Out]	���ͷ�IP
			nFromPort	int&		[Out]	���ͷ��˿�
			lSecond		long		[In]	��ʱʱ����
			lMinSecond	long		[In]	��ʱʱ�����
		����ֵ��ʵ�ʽ��յ����ֽ�������ʱ����-1
	*/
	int ReceiveFrom( char* lpBuf, int nBufLen, std::string &strFromIP, int &nFromPort, bool bCheckDataLength = false, long lSecond = 0, long lMinSecond = 0 );

	/*
		���ܣ��ͻ��˷��������ӷ�����
		������
			lpszHostAddress	LPCTSTR		[In]	�Է�Ip��ַ
			nHostPort		UINT		[In]	�Է������Ķ˿�
		����ֵ���ɹ�����TRUE,ʧ�ܷ���FALSE
	*/
	bool Connect( const char* lpszHostAddress, unsigned short nHostPort );

	/*
		���ܣ�����˷�������ʼ�������
		������
			rConnectedSocket	Socket	[In]	client socket����
		����ֵ���ɹ�����TRUE�����򷵻�FALSE
		��ע������TRUE����ʾrConnectedSocket�ȷ��ز���ֵ��Ч����Ϊ����Ƿ�����ģʽ��
		�����ӵ���ú���һ������TRUE������ʱrConnectedSocket��������ָ��INVALID_SOCKET
	*/
	bool StartServer( int nPort );
	/*
		���ܣ�����˷��������տͻ�������
		������
			rConnectedSocket	Socket	[In]	client socket����
		����ֵ���ɹ�����TRUE�����򷵻�FALSE
		��ע������TRUE����ʾrConnectedSocket�ȷ��ز���ֵ��Ч����Ϊ����Ƿ�����ģʽ��
		�����ӵ���ú���һ������TRUE������ʱrConnectedSocket��������ָ��INVALID_SOCKET
	*/
	bool Accept(Socket& rConnectedSocket);
	//ȡ��������ַ
	void GetWanAddress( std::string& strWanIP, int& nWanPort );
	//ȡ��������ַ
	void GetLocalAddress( std::string& strWanIP, int& nWanPort );
	/*
		���ܣ�������ʽ����
		������
			bWait		bool	[In]	TRUE������FALSE������
		����ֵ����ʱ����TRUE�����򷵻�FALSE
	*/
	bool SetSockMode( bool bWait = false );
	/*
		���ܣ��׽������ã��˿����õ�
		������
			nOptionName		__int32		[In]	The socket option for which the value is to be set
			lpOptionValue	const void*	[In]	A pointer to the buffer in which the value for the requested option is supplied
			nOptionLen		__int32		[In]	lpOptionValue�Ĵ�С
			nLevel			__int32		[In]	The level at which the option is defined; the supported levels include SOL_SOCKET and IPPROTO_TCP. See the Windows Sockets 2 Protocol-Specific Annex (a separate document included with the Platform SDK) for more information on protocol-specific levels
		����ֵ���ɹ�����TRUE�����򷵻�FALSE
	*/
	bool SetSockOpt( int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET );

	/*
		���ܣ�Socket��ʼ��
		������
			lpwsaData	WSADATA*	[In]	A pointer to a WSADATA structure. If lpwsaData is not equal to NULL, then the address of the WSADATA structure is filled by the call to ::WSAStartup. This function also ensures that ::WSACleanup is called for you before the application terminates.

		����ֵ����ʱ����TRUE�����򷵻�FALSE
	*/
	static bool SocketInit(void *lpVoid = NULL);
	static void SocketDestory();
	//�������Ĵ�����Ϣ
	void GetLastErrorMsg( std::string &strError );
	
	//���IOCP����ʹ��GetPeerNameȡ��ַ��Ϣ
	//��ֻ����Connect֮�����
	static bool InitForIOCP( SOCKET hSocket );

	/*
		���ܣ���һ��socket������ø���������������Ͻ��в���
		�߼���
			ɵ��ʽ�󶨣����������֮ǰ�Ƿ��Ѿ����������׽��֣����Ȼ����ɵ��ʽ����close�ر����ӣ�Ȼ���ڰ��µ��׽��֣�
			���û��ʵ�ֵ���Detach����ɰ󶨣���ô�ɵİ�sock����ʧ
		������
			hSocket	SOCKET	[In]	Ҫ��sock���
	*/
	void Attach( SOCKET hSocket );
	/*
		���ܣ�����󶨣����ذ󶨵�socket���
		����ֵ���Ѱ󶨵�socket�����������һ��INVALID_SOCKET��˵��֮ǰû���κΰ�
	*/
	SOCKET Detach();

	//��ʼ��������ַ
	//��ֻ����Connect֮�����
	bool InitWanAddress();
	//��ʼ��������ַ
	//��ֻ����Connect֮�����
	bool InitLocalAddress();
	
protected:
	/*
		���ܣ���ʱ����
		������
			lSecond		long	[In]	��ʱ������
			lMinSecond	long	[In]	��ʱ���ú���
		����ֵ����ʱ����TRUE�����򷵻�FALSE
	*/
	bool TimeOut( long lSecond, long lMinSecond );
	//���ܣ��ȴ�����
	bool WaitData();
	/*
		���ܣ���sockaddr�ṹת���ɳ������ͱ�ʾ�ĵ�ַ
		������
		sockAddr	sockaddr_in		[In]	��ַ��Ϣ
		strIP		string			[Out]	ip
		nPort		int				[Out]	�˿�
	*/
	void GetAddress( const sockaddr_in &sockAddr, std::string &strIP, int &nPort );
	/*
		���ܣ�����˺������󶨼����Ķ˿���IP
		������
			nSocketPort			UINT		[In]	�����Ķ˿�
			lpszSocketAddress	LPCTSTR		[In]	IP
		����ֵ���ɹ�����TRUE�����򷵻�FALSE
	*/
	bool Bind( unsigned short nPort, char *strIP = NULL );
	/*
		���ܣ�����˺�������ʼ����
		������
			nConnectionBacklog	__int32	[In]	���������
		����ֵ���ɹ�����TRUE�����򷵻�FALSE
	*/
	bool Listen( int nConnectionBacklog = SOMAXCONN );
		
public:
private:
	SOCKET m_hSocket;//sock���
	bool m_bBlock;//�������
	bool m_bOpened;//��״̬
	sockaddr_in m_sockAddr;
	std::string m_strWanIP;
	int m_nWanPort;
	std::string m_strLocalIP;
	int m_nLocalPort;
};

}//namespace mdk

#endif // MDK_SOCKET_H
