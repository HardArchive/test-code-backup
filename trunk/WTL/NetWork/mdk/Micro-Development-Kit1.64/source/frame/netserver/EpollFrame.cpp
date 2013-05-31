// EpollFrame.cpp: implementation of the EpollFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/frame/netserver/EpollMonitor.h"
#include "../../../include/frame/netserver/EpollFrame.h"
#include "../../../include/frame/netserver/NetConnect.h"
#include "../../../include/mdk/atom.h"
#include "../../../include/mdk/Lock.h"
#include "../../../include/mdk/Socket.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace mdk
{

EpollFrame::EpollFrame()
{
#ifndef WIN32
	m_pNetMonitor = new EpollMonitor;
#endif
}

EpollFrame::~EpollFrame()
{
#ifndef WIN32
	if ( NULL != m_pNetMonitor ) 
	{
		delete m_pNetMonitor;
		m_pNetMonitor = NULL;
	}
#endif
}

void* EpollFrame::NetMonitor( void* )
{
#ifndef WIN32
	int nCount = MAXPOLLSIZE;
	EpollMonitor::IO_EVENT e[MAXPOLLSIZE];
	int i = 0;
	Socket sock;
	Socket sockClient;
	map<SOCKET,int> ioList;
	map<SOCKET,int>::iterator it;
	int eventType = 0;
	while ( !m_stop )
	{
		//û�п�io��socket��ȴ��¿�io��socket
		//�������Ƿ����µĿ�io��socket������ȡ�����뵽ioList�У�û��Ҳ���ȴ�
		//��������ioList�е�socket����io����
		if ( 0 >= ioList.size() ) m_pNetMonitor->WaitEvent( e, nCount, true );
		else m_pNetMonitor->WaitEvent( e, nCount, false );

		//���뵽ioList��
		for ( i = 0; i < nCount; i++ )
		{
			if ( EpollMonitor::epoll_accept == e[i].type )//��������ֱ��ִ��ҵ�� 
			{
				OnConnect(e[i].sock, false);
				continue;
			}
			
			//����recv send����뵽io�б�ͳһ����
			if ( EpollMonitor::epoll_in == e[i].type ) eventType = 1;//recv�¼�
			else eventType = 2;//send�¼�
			it = ioList.find(e[i].sock);
			if ( it == ioList.end() ) ioList.insert(map<SOCKET,int>::value_type(e[i].sock,eventType) );//���ӿ�io�Ķ���
			else it->second = it->second|eventType;//�������¼�
		}
		
		//����ioList��ִ��1��io
		
		for ( it = ioList.begin(); it != ioList.end(); it++ )
		{
			if ( 1&it->second ) //�ɶ�
			{
				if ( ok != OnData( it->first, 0, 0 ) ) //�����Ѷ���������ѶϿ�
				{
					it->second = it->second&~1;//����¼�
				}
			}
			if ( 2&it->second ) //��д
			{
				if ( ok != OnSend( it->first, 0 ) )//�����Ѿ������꣬��socket�Ѿ��Ͽ�����socket����д
				{
					it->second = it->second&~2;//����¼�
				}
			}
		}
		
		//������io��socket���
		it = ioList.begin();
		while (  it != ioList.end() ) 
		{
			if ( 0 == it->second ) 
			{
				ioList.erase(it);
				it = ioList.begin();
			}
			else it++;
		}
	}
#endif
				
	return NULL;
}

connectState EpollFrame::RecvData( NetConnect *pConnect, char *pData, unsigned short uSize )
{
#ifndef WIN32
	unsigned char* pWriteBuf = NULL;	
	int nRecvLen = 0;
	unsigned int nMaxRecvSize = 0;
	//������1M���ݣ��ø��������ӽ���io
	while ( nMaxRecvSize < 1048576 )
	{
		pWriteBuf = pConnect->PrepareBuffer(BUFBLOCK_SIZE);
		nRecvLen = pConnect->GetSocket()->Receive(pWriteBuf, BUFBLOCK_SIZE);
		if ( nRecvLen < 0 ) return unconnect;
		if ( 0 == nRecvLen ) 
		{
			if ( !m_pNetMonitor->AddRecv(pConnect->GetSocket()->GetSocket(), NULL, 0) ) return unconnect;
			return wait_recv;
		}
		nMaxRecvSize += nRecvLen;
		pConnect->WriteFinished( nRecvLen );
	}
#endif
	return ok;
}

SOCKET EpollFrame::ListenPort(int port)
{
#ifndef WIN32
	Socket listenSock;//����socket
	if ( !listenSock.Init( Socket::tcp ) ) return INVALID_SOCKET;
	listenSock.SetSockMode();
	if ( !listenSock.StartServer( port ) ) 
	{
		listenSock.Close();
		return INVALID_SOCKET;
	}
	m_pNetMonitor->AddMonitor( listenSock.GetSocket() );
	if ( !m_pNetMonitor->AddAccept( listenSock.GetSocket() ) ) 
	{
		listenSock.Close();
		return INVALID_SOCKET;
	}

	return listenSock.Detach();
#endif
	return INVALID_SOCKET;
}

bool EpollFrame::MonitorConnect(NetConnect *pConnect)
{
#ifndef WIN32
	pConnect->GetSocket()->SetSockMode();
	m_pNetMonitor->AddMonitor( pConnect->GetSocket()->GetSocket() );
	return m_pNetMonitor->AddRecv( pConnect->GetSocket()->GetSocket(), NULL, 0 );
#endif
	return false;
}

connectState EpollFrame::SendData(NetConnect *pConnect, unsigned short uSize)
{
#ifndef WIN32
	try
	{
		connectState cs = wait_send;//Ĭ��Ϊ�ȴ�״̬
		//////////////////////////////////////////////////////////////////////////
		//ִ�з���
		unsigned char buf[BUFBLOCK_SIZE];
		int nSize = 0;
		int nSendSize = 0;
		int nFinishedSize = 0;
		nSendSize = pConnect->m_sendBuffer.GetLength();
		if ( 0 < nSendSize )
		{
			nSize = 0;
			//һ�η���4096byte
			if ( BUFBLOCK_SIZE < nSendSize )//1�η����꣬����Ϊ����״̬
			{
				pConnect->m_sendBuffer.ReadData(buf, BUFBLOCK_SIZE, false);
				nSize += BUFBLOCK_SIZE;
				nSendSize -= BUFBLOCK_SIZE;
				cs = ok;
			}
			else//1�οɷ��꣬����Ϊ�ȴ�״̬
			{
				pConnect->m_sendBuffer.ReadData(buf, nSendSize, false);
				nSize += nSendSize;
				nSendSize = 0;
				cs = wait_send;
			}
			nFinishedSize = pConnect->GetSocket()->Send((char*)buf, nSize);//����
			if ( -1 == nFinishedSize ) cs = unconnect;
			else
			{
				pConnect->m_sendBuffer.ReadData(buf, nFinishedSize);//�����ͳɹ������ݴӻ������
				if ( nFinishedSize < nSize ) //sock��д��������Ϊ�ȴ�״̬
				{
					cs = wait_send;
				}
			}

		}
		if ( ok == cs || unconnect == cs ) return cs;//����״̬�����ӹر�ֱ�ӷ��أ����ӹرղ��ؽ����������̣�pNetConnect����ᱻ�ͷţ����������Զ�����

		//�ȴ�״̬���������η��ͣ��������·�������
		pConnect->SendEnd();//���ͽ���
		//////////////////////////////////////////////////////////////////////////
		//����Ƿ���Ҫ��ʼ�µķ�������
		if ( 0 >= pConnect->m_sendBuffer.GetLength() ) return cs;
		/*
			�ⲿ�����߳�����ɷ��ͻ���д��
			���̲߳���SendStart()��ֻ��һ���ɹ�
			���ۣ�������ֲ������ͣ�Ҳ����©����
		*/
		if ( !pConnect->SendStart() ) return cs;//�Ѿ��ڷ���
		//�������̿�ʼ
		m_pNetMonitor->AddSend( pConnect->GetSocket()->GetSocket(), NULL, 0 );
		return cs;
	}
	catch(...)
	{
	}
#endif
	return ok;
}

}//namespace mdk
