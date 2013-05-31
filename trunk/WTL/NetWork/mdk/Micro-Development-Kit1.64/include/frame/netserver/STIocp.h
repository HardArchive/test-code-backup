// STIocp.h: interface for the STIocp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_IOCPMONITOR_H
#define MDK_IOCPMONITOR_H

#ifdef WIN32
#include <winsock2.h>  
#include <mswsock.h>
#include <windows.h>
#else
typedef int SOCKADDR_IN;
typedef int WSAOVERLAPPED;
typedef int WSABUF;
typedef int HANDLE;
typedef int OVERLAPPED;
#endif

#include "NetEventMonitor.h"
#include "../../../include/mdk/MemoryPool.h"
#include "../../../include/mdk/Lock.h"

/*
 *	IOCP��װ(���߳�)
 *	��Ҫ��WaitEvent�ṩ��ʱ��������ռ���߳�
*/
namespace mdk
{
	//�׽�������
	enum socketType
	{
		stListen = 0,			//�����׽���,ֻ�ᷢ��opAccept
			stCommunication = 1,	//ͨ���׽���,ֻ����opRecv,opSend
	};
	
		
class STIocp : public NetEventMonitor  
{
public:
	enum EventType
	{
		unknow = 0,
			connect = 1,
			close = 2,
			recv = 3,
			send = 4,
			timeout = 5,
			stop = 6,
	};
	typedef struct IO_EVENT
	{
		SOCKET sock;
		EventType type;
		SOCKET client;
		char *pData;
		unsigned short uDataSize;
	}IO_EVENT;
public:
	MemoryPool m_iocpDataPool;//iocpͶ�ݲ�����
	typedef struct IOCP_OVERLAPPED
	{
		/**
		 * OVERLAPPED����ָ��
		 * ָ����ɲ�������
		 * ���ݸ�AcceptEx()�����һ������
		 * ���ݸ�WSARecv()�ĵ�6������
		 * GetQueuedCompletionStatus()���صĵ�4������
		 */
		OVERLAPPED m_overlapped;
		/**
		 * ָ��������ӽ����Ŀͻ��˾�������������ַ���ڴ�
		 * ����ʹ�ö�̬������ڴ��
		 * ���ݸ�AcceptEx()�ĵ�3������
		 * 
		 */
		char m_outPutBuf[sizeof(SOCKADDR_IN)*2+32];
		/**
		 * �ͻ��˾�����IP��Ϣ����
		 * ���ݸ�AcceptEx()�ĵ�5������
		 */
		unsigned long m_dwLocalAddressLength;
		/**
		 * �ͻ�������IP��Ϣ����
		 * ���ݸ�AcceptEx()�ĵ�6������
		 */
		unsigned long m_dwRemoteAddressLength;
		WSABUF m_wsaBuffer;//WSARecv���ջ�������,���ݸ�WSARecv()�ĵ�2������
		SOCKET sock;
		EventType completiontype;//�������1recv 2send
	}IOCP_OVERLAPPED;
public:
	STIocp();
	virtual ~STIocp();
public:
	//��ʼ����
	bool Start( int nMaxMonitor );
	//ֹͣ����
	bool Stop();
	//����һ����������
	bool AddMonitor( SOCKET socket );
	//�ȴ��¼���ʧ�ܷ���false,��ʱsockEvent.type = STIocp::timeout,�ⲿ������Stop()��sockEvent.type = STIocp::stop
	//timeout��ʱʱ�䣺INFINITE����ʱ��0���ȴ���>0��ʱʱ�䵥λ(ǧ��֮һ��)
	bool WaitEvent( IO_EVENT &sockEvent, int timeout );
	//����һ���������ӵĲ����������ӽ�����WaitEvent�᷵��
	bool AddAccept(SOCKET listenSocket);
	//����һ���������ݵĲ����������ݵ��WaitEvent�᷵��
	bool AddRecv( SOCKET socket, char* recvBuf, unsigned short bufSize );
	//����һ���������ݵĲ�����������ɣ�WaitEvent�᷵��
	bool AddSend( SOCKET socket, char* dataBuf, unsigned short dataSize );
protected:
	int GetError(SOCKET sock, WSAOVERLAPPED* pWSAOVERLAPPED)  ;
		
private:
	SOCKET m_listenSocket;
	HANDLE m_hCompletPort;//��ɶ˿ھ��
	int m_nCPUCount;
	IOCP_OVERLAPPED m_olExit;
};

}//namespace mdk


#endif // MDK_IOCPMONITOR_H
