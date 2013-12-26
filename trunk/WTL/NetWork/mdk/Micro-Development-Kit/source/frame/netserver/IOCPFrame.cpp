// IOCPFrame.cpp: implementation of the IOCPFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/frame/netserver/IOCPMonitor.h"
#include "../../../include/frame/netserver/IOCPFrame.h"
#include "../../../include/frame/netserver/NetConnect.h"
#include "../../../include/mdk/atom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace mdk
{

IOCPFrame::IOCPFrame()
{
#ifdef WIN32
	m_pNetMonitor = new IOCPMonitor;
#endif
}

IOCPFrame::~IOCPFrame()
{
	if ( NULL != m_pNetMonitor ) 
	{
		delete m_pNetMonitor;
		m_pNetMonitor = NULL;
	}
}

void* IOCPFrame::NetMonitor( void* )
{
#ifdef WIN32
	IOCPMonitor::IO_EVENT e[1];
	int nCount = 1;
	while ( !m_stop )
	{
		if ( !m_pNetMonitor->WaitEvent( e, nCount, true ) ) return 0;
		if ( nCount <= 0 ) continue;
		switch( e->type )
		{
		case IOCPMonitor::connect :
			OnConnect( e->client, false );
			m_pNetMonitor->AddAccept( e->sock );
			break;
		case IOCPMonitor::recv :
			OnData( e->sock, e->pData, e->uDataSize );
			break;
		case IOCPMonitor::close :
			OnClose( e->sock );
			break;
		case IOCPMonitor::send :
			OnSend( e->sock, e->uDataSize );
			break;
		default:
			break;
		}
	}
#endif
	
	return NULL;
}

//�������ݣ�Ͷ��ʧ�ܱ�ʾ�����ѹرգ�����false, �������Ӧ�������Ҫ��������ʵ��
connectState IOCPFrame::RecvData( NetConnect *pConnect, char *pData, unsigned short uSize )
{
	pConnect->WriteFinished( uSize );
	if ( !m_pNetMonitor->AddRecv(  pConnect->GetSocket()->GetSocket(), 
		(char*)(pConnect->PrepareBuffer(BUFBLOCK_SIZE)), BUFBLOCK_SIZE ) )
	{
		return unconnect;
	}

	return ok;
}

connectState IOCPFrame::SendData(NetConnect *pConnect, unsigned short uSize)
{
	try
	{
		unsigned char buf[BUFBLOCK_SIZE];
		if ( uSize > 0 ) pConnect->m_sendBuffer.ReadData(buf, uSize);
		int nLength = pConnect->m_sendBuffer.GetLength();
		if ( 0 >= nLength ) 
		{
			pConnect->SendEnd();//���ͽ���
			nLength = pConnect->m_sendBuffer.GetLength();//�ڶ��μ�鷢�ͻ���
			if ( 0 >= nLength ) 
			{
				/*
					���1���ⲿ�����߳�δ��ɷ��ͻ���д��
						�ⲿ�߳����д��ʱ�������ڷ������̣����߳�SendStart()�ض��ɹ�
						���ۣ�����©����
					����������������������
				*/
				return ok;//û�д��������ݣ��˳������߳�
			}
			/*
				�ⲿ�����߳�����ɷ��ͻ���д��
				���̲߳���SendStart()��ֻ��һ���ɹ�
				���ۣ�������ֲ������ͣ�Ҳ����©����
			*/
			if ( !pConnect->SendStart() ) return ok;//�Ѿ��ڷ���
			//�������̿�ʼ
		}

		if ( nLength > BUFBLOCK_SIZE )
		{
			pConnect->m_sendBuffer.ReadData(buf, BUFBLOCK_SIZE, false);
			m_pNetMonitor->AddSend( pConnect->GetSocket()->GetSocket(), (char*)buf, BUFBLOCK_SIZE );
		}
		else
		{
			pConnect->m_sendBuffer.ReadData(buf, nLength, false);
			m_pNetMonitor->AddSend( pConnect->GetSocket()->GetSocket(), (char*)buf, nLength );
		}
	}
	catch(...)
	{
	}
	return ok;
}

SOCKET IOCPFrame::ListenPort(int port)
{
	Socket listenSock;//����socket
	if ( !listenSock.Init( Socket::tcp ) ) return INVALID_SOCKET;
	if ( !listenSock.StartServer( port ) ) 
	{
		listenSock.Close();
		return INVALID_SOCKET;
	}
	if ( !m_pNetMonitor->AddMonitor( listenSock.GetSocket() ) ) 
	{
		listenSock.Close();
		return INVALID_SOCKET;
	}
	int i = 0;
	for ( ; i < m_ioThreadCount; i++ ) 
	{
		if ( !m_pNetMonitor->AddAccept( listenSock.GetSocket() ) )
		{
			listenSock.Close();
			return INVALID_SOCKET;
		}
	}
	return listenSock.Detach();
}

//��������
bool IOCPFrame::MonitorConnect(NetConnect *pConnect)
{
	if ( !m_pNetMonitor->AddMonitor(pConnect->GetSocket()->GetSocket()) ) return false;
	return m_pNetMonitor->AddRecv( 
		pConnect->GetSocket()->GetSocket(), 
		(char*)(pConnect->PrepareBuffer(BUFBLOCK_SIZE)), 
		BUFBLOCK_SIZE );
}

}//namespace mdk
