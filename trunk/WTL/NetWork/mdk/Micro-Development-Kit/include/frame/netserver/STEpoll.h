// STEpoll.h: interface for the STEpoll class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_EPOLLMONITOR_H
#define MDK_EPOLLMONITOR_H

#include "NetEventMonitor.h"
#include <map>
#ifndef WIN32
#include <sys/epoll.h>
#define INFINITE -1
#endif
/*
 *	Epoll��װ(���߳�)
 *	��Ҫ��WaitEvent�ṩ��ʱ��������ռ���߳�
 */
namespace mdk
{

	class STEpoll : public NetEventMonitor
{
public:
	STEpoll();
	virtual ~STEpoll();

public:
	//��ʼ����
	bool Start( int nMaxMonitor );
	//ֹͣ����
	bool Stop();
	//�ȴ��¼����������¼���socket������-1��ʾʧ��
	//timeout��ʱʱ�䣺INFINITE����ʱ��0���ȴ���>0��ʱʱ�䵥λ(ǧ��֮һ��)
	//�����ɶ��̵߳��ã�m_events���ݻᱻ����
	int WaitEvent( int timeout );
	//�õ���i���¼��ľ��,����epoll�˳��źţ�����INVALID_SOCKET
	int GetSocket( int i );
	//��i��socket�н��տ�����
	bool IsAcceptAble( int i );
	//��i��socket�ɶ�
	bool IsReadAble( int i );
	//��i��socket��д
	bool IsWriteAble( int i );

	//����һ��Accept�������������Ӳ�����WaitEvent�᷵��
	bool AddAccept( SOCKET sock );
	bool AddMonitor( SOCKET sock );
	//����һ��IO��������IOʱWaitEvent�᷵��
	bool AddIO( SOCKET sock, bool read, bool write );
	//�ȴ��¼�����
	//ɾ��һ����������Ӽ����б�
	bool DelMonitor( SOCKET sock );
protected:
	void SheildSigPipe();//����SIGPIPE�źţ�������̱����źŹر�
		
	
private:
	bool m_bStop;
	int m_nMaxMonitor;//������socket����
	SOCKET m_epollExit;//epoll�˳�����sock
	
	int m_hEpoll;//EPOLLIN������ epoll���
	std::map<SOCKET,SOCKET> m_listenSockets;
#ifndef WIN32
	epoll_event *m_events;	//epoll�¼�
#endif
};

}//namespace mdk

#endif // MDK_EPOLLMONITOR_H
