// EpollMonitor.h: interface for the EpollMonitor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_EPOLLMONITOR_H
#define MDK_EPOLLMONITOR_H

#include "../../../include/mdk/Thread.h"
#include "../../../include/mdk/Task.h"
#include "../../../include/mdk/Queue.h"
#include "../../../include/mdk/Signal.h"
#include "NetEventMonitor.h"
#include <map>
#include <vector>

namespace mdk
{

class EpollMonitor : public NetEventMonitor  
{
public:
	enum EventType
	{
		epoll_accept = 0,
		epoll_in = 1,
		epoll_out = 2,
	};
	typedef struct IO_EVENT
	{
		SOCKET sock;
		EventType type;
	}IO_EVENT;
public:
	EpollMonitor();
	virtual ~EpollMonitor();

public:
	//��ʼ����
	bool Start( int nMaxMonitor );
	//ֹͣ����
	bool Stop();
	//����һ��Accept�������������Ӳ�����WaitEvent�᷵��
	bool AddAccept( SOCKET sock );
	//����һ���������ݵĲ����������ݵ��WaitEvent�᷵��
	bool AddRecv( SOCKET sock, char* recvBuf, unsigned short bufSize );
	//����һ���������ݵĲ�����������ɣ�WaitEvent�᷵��
	bool AddSend( SOCKET sock, char* dataBuf, unsigned short dataSize );
	//�ȴ��¼�����
	bool WaitEvent( void *eventArray, int &count, bool block );
	//ɾ��һ����������Ӽ����б�
	bool DelMonitor( SOCKET sock );
	bool AddMonitor( SOCKET sock );
	
protected:
	void SheildSigPipe();//����SIGPIPE�źţ�������̱����źŹر�
	void* RemoteCall WaitAcceptEvent( void *pData );
	void* RemoteCall WaitInEvent( void *pData );
	bool DelMonitorIn( SOCKET sock );
	void* RemoteCall WaitOutEvent( void *pData );
	bool DelMonitorOut( SOCKET sock );
		
	
private:
	bool m_bStop;
	int m_nMaxMonitor;//������socket����
	SOCKET m_epollExit;//epoll�˳�����sock
	Signal m_ioSignal;//���¼��ź�
	
	int m_hEPollAccept;//�������¼�������epoll���
	Thread m_acceptThread;//���߳�accept��accept recv����
	Queue* m_acceptEvents;//������-�����ߣ�accept����¼�����
	Signal m_sigWaitAcceptSpace;//accept�����пռ��ź�
	
	int m_hEPollIn;//EPOLLIN������ epoll���
	Thread m_pollinThread;//EPOLLIN�����̣߳���Ϊepoll_wait�Ǿ�Ⱥ��
	Queue* m_inEvents;//������-�����ߣ�EPOLLIN�¼�����
	Signal m_sigWaitInSpace;//EPOLLIN�����пռ��ź�
	
	int m_hEPollOut;//EPOLLOUT������ epoll���
	Thread m_polloutThread;//EPOLLOUT�����̣߳���Ϊepoll_wait�Ǿ�Ⱥ��
	Queue* m_outEvents;//������-�����ߣ�EPOLLOUT�¼�����
	Signal m_sigWaitOutSpace;//EPOLLOUT�����пռ��ź�
	
};

}//namespace mdk

#endif // MDK_EPOLLMONITOR_H
