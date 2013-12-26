// NetEventMonitor.h: interface for the NetEventMonitor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_NETEVENTMONITOR_H
#define MDK_NETEVENTMONITOR_H

#include "../../../include/mdk/Socket.h"
#include <string>
#define MAXPOLLSIZE 20000 //���socket��

namespace mdk
{

class NetEventMonitor
{
public:
	NetEventMonitor();
	virtual ~NetEventMonitor();

public:
	//��ʼ����
	virtual bool Start( int nMaxMonitor ) = 0;
	//ֹͣ����
	virtual bool Stop();
	//����һ���������󵽼����б�
	virtual bool AddMonitor( SOCKET socket );
	//�ȴ��¼�����
	virtual bool WaitEvent( void *eventArray, int &count, bool block );

	//����һ���������ӵĲ����������ӽ�����WaitEvent�᷵��
	virtual bool AddAccept(SOCKET socket);
	//����һ���������ݵĲ����������ݵ��WaitEvent�᷵��
	virtual bool AddRecv( SOCKET socket, char* recvBuf, unsigned short bufSize );
	//����һ���������ݵĲ�����������ɣ�WaitEvent�᷵��
	virtual bool AddSend( SOCKET socket, char* dataBuf, unsigned short dataSize );

	//ɾ��һ����������Ӽ����б�
	virtual bool DelMonitor( SOCKET socket );

	//ȡ�����Ĵ���
	const char* GetInitError();

protected:
	std::string m_initError;//������Ϣ
};

}//namespace mdk

#endif // MDK_NETEVENTMONITOR_H
