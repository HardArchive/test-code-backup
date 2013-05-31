// NetConnect.cpp: implementation of the NetConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/frame/netserver/NetConnect.h"
#include "../../../include/frame/netserver/NetEventMonitor.h"
#include "../../../include/frame/netserver/NetEngine.h"
#include "../../../include/mdk/atom.h"
#include "../../../include/mdk/MemoryPool.h"
using namespace std;

namespace mdk
{

NetConnect::NetConnect(SOCKET sock, bool bIsServer, NetEventMonitor *pNetMonitor, NetEngine *pEngine, MemoryPool *pMemoryPool)
:m_socket(sock,Socket::tcp)
{
	m_pMemoryPool = pMemoryPool;
	m_useCount = 1;
	m_pEngine = pEngine;
	m_pNetMonitor = pNetMonitor;
	m_id = m_socket.GetSocket();
	m_host.m_pConnect = this;
	m_nReadCount = 0;
	m_bReadAble = false;

	m_nSendCount = 0;//���ڽ��з��͵��߳���
	m_bSendAble = false;//io��������������Ҫ����
	m_bConnect = true;//ֻ�з������ӲŴ����������Զ��󴴽�����һ��������״̬
	m_nDoCloseWorkCount = 0;//û��ִ�й�NetServer::OnClose()
	m_bIsServer = bIsServer;
#ifdef WIN32
	Socket::InitForIOCP(sock);	
#endif
	m_socket.InitPeerAddress();
	m_socket.InitLocalAddress();
}

NetConnect::~NetConnect()
{

}

void NetConnect::Release()
{
	if ( 1 == AtomDec(&m_useCount, 1) )
	{
		m_host.m_pConnect = NULL;
		if ( NULL == m_pMemoryPool ) 
		{
			delete this;
			return;
		}
		this->~NetConnect();
		m_pMemoryPool->Free(this);
	}
}

void NetConnect::RefreshHeart()
{
	m_tLastHeart = time( NULL );
}

time_t NetConnect::GetLastHeart()
{
	return m_tLastHeart;
}

unsigned char* NetConnect::PrepareBuffer(unsigned short uRecvSize)
{
	return m_recvBuffer.PrepareBuffer( uRecvSize );
}

void NetConnect::WriteFinished(unsigned short uLength)
{
	m_recvBuffer.WriteFinished( uLength );
}

bool NetConnect::IsReadAble()
{
	return m_bReadAble && 0 < m_recvBuffer.GetLength();
}

uint32 NetConnect::GetLength()
{
	return m_recvBuffer.GetLength();
}

bool NetConnect::ReadData( unsigned char* pMsg, unsigned short uLength, bool bClearCache )
{
	m_bReadAble = m_recvBuffer.ReadData( pMsg, uLength, bClearCache );
	if ( !m_bReadAble ) uLength = 0;
	
	return m_bReadAble;
}

bool NetConnect::SendData( const unsigned char* pMsg, unsigned short uLength )
{
	try
	{
		unsigned char *ioBuf = NULL;
		int nSendSize = 0;
		AutoLock lock(&m_sendMutex);//�ظ�������֪ͨ���ڲ���send
		if ( 0 >= m_sendBuffer.GetLength() )//û�еȴ����͵����ݣ���ֱ�ӷ���
		{
			nSendSize = m_socket.Send( pMsg, uLength );
		}
		if ( -1 == nSendSize ) return false;//�����������ӿ����ѶϿ�
		if ( uLength == nSendSize ) return true;//���������ѷ��ͣ����سɹ�
		
		//���ݼ��뷢�ͻ��壬�����ײ�ȥ����
		uLength -= nSendSize;
		while ( true )
		{
			if ( uLength > BUFBLOCK_SIZE )
			{
				ioBuf = m_sendBuffer.PrepareBuffer( BUFBLOCK_SIZE );
				memcpy( ioBuf, &pMsg[nSendSize], BUFBLOCK_SIZE );
				m_sendBuffer.WriteFinished( BUFBLOCK_SIZE );
				nSendSize += BUFBLOCK_SIZE;
				uLength -= BUFBLOCK_SIZE;
			}
			else
			{
				ioBuf = m_sendBuffer.PrepareBuffer( uLength );
				memcpy( ioBuf, &pMsg[nSendSize], uLength );
				m_sendBuffer.WriteFinished( uLength );
				break;
			}
		}
		if ( !SendStart() ) return true;//�Ѿ��ڷ���
		//�������̿�ʼ
		m_pNetMonitor->AddSend( m_socket.GetSocket(), NULL, 0 );
	}
	catch(...){}
	return true;
}

Socket* NetConnect::GetSocket()
{
	return &m_socket;
}

int NetConnect::GetID()
{
	return m_id;
}

//��ʼ��������
bool NetConnect::SendStart()
{
	if ( 0 != AtomAdd(&m_nSendCount,1) ) return false;//ֻ�������һ����������
	return true;
}

//������������
void NetConnect::SendEnd()
{
	m_nSendCount = 0;
}

void NetConnect::Close()
{
	m_pEngine->CloseConnect(m_id);
}

bool NetConnect::IsServer()
{
	return m_bIsServer;
}

void NetConnect::InGroup( int groupID )
{
	m_groups.insert(map<int,int>::value_type(groupID,groupID));
}

void NetConnect::OutGroup( int groupID )
{
	map<int,int>::iterator it;
	it = m_groups.find(groupID);
	if ( it == m_groups.end() ) return;
	m_groups.erase(it);
}

bool NetConnect::IsInGroups( int *groups, int count )
{
	int i = 0;
	for ( i = 0; i < count; i++ )
	{
		if ( m_groups.end() != m_groups.find(groups[i]) ) return true;
	}
	
	return false;
}

void NetConnect::GetServerAddress( string &ip, int &port )
{
	if ( this->m_bIsServer ) m_socket.GetPeerAddress( ip, port );
	else m_socket.GetLocalAddress( ip, port );
	return;
}

void NetConnect::GetAddress( string &ip, int &port )
{
	if ( !this->m_bIsServer ) m_socket.GetPeerAddress( ip, port );
	else m_socket.GetLocalAddress( ip, port );
	return;
}

}//namespace mdk

