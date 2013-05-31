
#include "../../../include/frame/netserver/STIocp.h"
#include "../../../include/frame/netserver/STEpoll.h"
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#define strnicmp strncasecmp
#endif

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <time.h>

#include "../../../include/mdk/Socket.h"
#include "../../../include/mdk/atom.h"
#include "../../../include/mdk/MemoryPool.h"
#include "../../../include/mdk/mapi.h"

#include "../../../include/frame/netserver/STNetEngine.h"
#include "../../../include/frame/netserver/STNetConnect.h"
#include "../../../include/frame/netserver/STNetServer.h"

using namespace std;
namespace mdk
{
	
STNetEngine::STNetEngine()
{
	Socket::SocketInit();
	m_pConnectPool = NULL;
	m_stop = true;//ֹͣ��־
	m_startError = "";
	m_nHeartTime = 0;//�������(S)��Ĭ�ϲ����
	m_nReconnectTime = 0;//Ĭ�ϲ��Զ�����
#ifdef WIN32
	m_pNetMonitor = new STIocp;
#else
	m_pNetMonitor = new STEpoll;
#endif
	m_pNetServer = NULL;
	m_averageConnectCount = 5000;
}

STNetEngine::~STNetEngine()
{
	Stop();
	if ( NULL != m_pConnectPool )
	{
		delete m_pConnectPool;
		m_pConnectPool = NULL;
	}
	Socket::SocketDestory();
}

//����ƽ��������
void STNetEngine::SetAverageConnectCount(int count)
{
	m_averageConnectCount = count;
}

//��������ʱ��
void STNetEngine::SetHeartTime( int nSecond )
{
	m_nHeartTime = nSecond;
}

//�����Զ�����ʱ��,С�ڵ���0��ʾ���Զ�����
void STNetEngine::SetReconnectTime( int nSecond )
{
	m_nReconnectTime = nSecond;
}

/**
 * ��ʼ����
 * �ɹ�����true��ʧ�ܷ���false
 */
bool STNetEngine::Start()
{
	if ( !m_stop ) return true;
	m_stop = false;	
	
	int memoryCount = 2;
	for ( memoryCount = 2; memoryCount * memoryCount < m_averageConnectCount * 2; memoryCount++ );
	if ( memoryCount < 200 ) memoryCount = 200;
	if ( NULL != m_pConnectPool )//֮ǰStop()��,����������
	{
		delete m_pConnectPool;
		m_pConnectPool = NULL;
	}
	m_pConnectPool = new MemoryPool( sizeof(STNetConnect), memoryCount );
	if ( NULL == m_pConnectPool )
	{
		m_startError = "�ڴ治�㣬�޷�����NetConnect�ڴ��";
		Stop();
		return false;
	}
	if ( !m_pNetMonitor->Start( MAXPOLLSIZE ) ) 
	{
		m_startError = m_pNetMonitor->GetInitError();
		Stop();
		return false;
	}
	if ( !ListenAll() )
	{
		Stop();
		return false;
	}
	ConnectAll();
	return m_mainThread.Run( Executor::Bind(&STNetEngine::Main), this, 0 );
}

bool STNetEngine::WINIO(int timeout)
{
#ifdef WIN32
	STIocp::IO_EVENT e;
	if ( !m_pNetMonitor->WaitEvent( e, timeout ) ) return false;
	switch( e.type )
	{
	case STIocp::timeout :
		break;
	case STIocp::stop :
		return false;
		break;
	case STIocp::connect :
		OnConnect( e.client, false );
		m_pNetMonitor->AddAccept( e.sock );
		break;
	case STIocp::recv :
		OnData( e.sock, e.pData, e.uDataSize );
		break;
	case STIocp::close :
		OnClose( e.sock );
		break;
	case STIocp::send :
		OnSend( e.sock, e.uDataSize );
		break;
	default:
		break;
	}
	return true;
#endif
	return false;
}

bool STNetEngine::LinuxIO( int timeout )
{
#ifndef WIN32
	int nCount = 0;
	int eventType = 0;
	int i = 0;
	Socket sockListen;
	Socket sockClient;
	SOCKET sock;
	map<SOCKET,int>::iterator it;
	pair<map<SOCKET,int>::iterator,bool> ret;
	
	//û�п�io��socket��ȴ��¿�io��socket
	//�������Ƿ����µĿ�io��socket������ȡ�����뵽m_ioList�У�û��Ҳ���ȴ�
	//��������m_ioList�е�socket����io����
	if ( 0 >= m_ioList.size() ) nCount = m_pNetMonitor->WaitEvent( timeout );
	else nCount = m_pNetMonitor->WaitEvent( 0 );
	if ( 0 > nCount ) return false;
	//���뵽m_ioList��
	for ( i = 0; i < nCount; i++ )
	{
		sock = m_pNetMonitor->GetSocket(i);
		if ( INVALID_SOCKET == sock ) return false;//STEpoll�ѹر�
		if ( m_pNetMonitor->IsAcceptAble(i) )//��������ֱ��ִ��ҵ�� 
		{
			while ( true )
			{
				sockListen.Detach();
				sockListen.Attach(sock);
				sockListen.Accept( sockClient );
				if ( INVALID_SOCKET == sockClient.GetSocket() ) break;
				sockClient.SetSockMode();
				OnConnect(sockClient.Detach(), false);
			}
			continue;
		}
		//���Ǽ���socketһ����io�¼�
		//���뵽io�б�ͳһ����
		if ( m_pNetMonitor->IsWriteAble(i) ) eventType = 1|2;//recv+send�¼�
		else eventType = 1;//recv�¼�
		ret = m_ioList.insert(map<SOCKET,int>::value_type(sock,eventType) );//���ӿ�io�Ķ���
		if ( !ret.second ) ret.first->second = ret.first->second|eventType;//�������¼�
	}
	//����m_ioList��ִ��1��io
	for ( it = m_ioList.begin(); it != m_ioList.end(); it++ )
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
	it = m_ioList.begin();
	while (  it != m_ioList.end() ) 
	{
		if ( 0 == it->second ) 
		{
			m_ioList.erase(it);
			it = m_ioList.begin();
			continue;
		}
		it++;
	}
	return true;
#endif
	return false;
}

//�ȴ�ֹͣ
void STNetEngine::WaitStop()
{
	m_mainThread.WaitStop();
}

//ֹͣ����
void STNetEngine::Stop()
{
	if ( m_stop ) return;
	m_stop = true;
	m_pNetMonitor->Stop();
	m_mainThread.Stop(3000);
#ifndef WIN32
	m_ioList.clear();
#endif
}

//���߳�
void* STNetEngine::Main(void*)
{
	time_t lastConnect = time(NULL);
	time_t curTime = time(NULL);
	
	while ( !m_stop ) 
	{
#ifdef WIN32
		if ( !WINIO( 10000 ) ) break;
#else
		if ( !LinuxIO( 10000 ) ) break;
#endif
		curTime = time(NULL);
		if ( 10000 <= curTime - lastConnect ) continue;
		lastConnect = curTime;
		HeartMonitor();
		ReConnectAll();
	}
	return NULL;
}

//�����߳�
void STNetEngine::HeartMonitor()
{
	if ( 0 >= m_nHeartTime ) return;
	//////////////////////////////////////////////////////////////////////////
	//�ر�������������
	ConnectList::iterator it;
	STNetConnect *pConnect;
	time_t tCurTime = 0;
	/*	
		����һ����ʱ���ͷ��б���Ҫ�ͷŵĶ��󣬵ȱ�������1���Ե���ȴ��ͷŶ����б�
		������ѭ������Ϊ�ظ���Ϊ�ȴ��ͷ��б�ķ��ʶ���������
	 */
	tCurTime = time( NULL );
	time_t tLastHeart;
	AutoLock lock( &m_connectsMutex );
	for ( it = m_connectList.begin(); it != m_connectList.end(); )//����ʱ��<=0����������,��������
	{
		pConnect = it->second;
		if ( pConnect->m_host.IsServer() ) //�������� �����������
		{
			it++;
			continue;
		}
		//�������
		tLastHeart = pConnect->GetLastHeart();
		if ( tCurTime < tLastHeart || tCurTime - tLastHeart < m_nHeartTime )//������
		{
			it++;
			continue;
		}
		//������/�����ѶϿ���ǿ�ƶϿ����ӣ������ͷ��б�
		CloseConnect( it );
		it = m_connectList.begin();
	}
	lock.Unlock();
}

void STNetEngine::ReConnectAll()
{
	if ( 0 >= m_nReconnectTime ) return;//����������
	static time_t lastConnect = time(NULL);
	time_t curTime = time(NULL);
	if ( m_nReconnectTime > curTime - lastConnect ) return;
	lastConnect = curTime;
	ConnectAll();
}

//�ر�һ������
void STNetEngine::CloseConnect( ConnectList::iterator it )
{
	/*
	   ������ɾ���ٹرգ�˳���ܻ���
	   ����رպ�eraseǰ��������client���ӽ�����
	   ϵͳ���̾ͰѸ����ӷ������clientʹ�ã������client�ڲ���m_connectListʱʧ��
	*/
	STNetConnect *pConnect = it->second;
	m_connectList.erase( it );//֮�󲻿�����MsgWorker()��������ΪOnData�����Ѿ��Ҳ���������
	AtomDec(&pConnect->m_useCount, 1);//m_connectList�������
	pConnect->GetSocket()->Close();
	pConnect->m_bConnect = false;
	if ( 0 == AtomAdd(&pConnect->m_nReadCount, 1) ) NotifyOnClose(pConnect);
	pConnect->Release();//���ӶϿ��ͷŹ������
	return;
}

void STNetEngine::NotifyOnClose(STNetConnect *pConnect)
{
	if ( 0 == AtomAdd(&pConnect->m_nDoCloseWorkCount, 1) )//ֻ��1���߳�ִ��OnClose���ҽ�ִ��1��
	{
		SetServerClose(pConnect);//���ӵķ���Ͽ�
		m_pNetServer->OnCloseConnect( pConnect->m_host );
	}
}

int g_c = 0;
bool STNetEngine::OnConnect( SOCKET sock, bool isConnectServer )
{
	AtomAdd(&g_c, 1);
	STNetConnect *pConnect = new (m_pConnectPool->Alloc())STNetConnect(sock, isConnectServer, m_pNetMonitor, this, m_pConnectPool);
	if ( NULL == pConnect ) 
	{
		closesocket(sock);
		return false;
	}
	//��������б�
	pConnect->RefreshHeart();
	AtomAdd(&pConnect->m_useCount, 1);//��m_connectList����
	AutoLock lock( &m_connectsMutex );
	pair<ConnectList::iterator, bool> ret = m_connectList.insert( ConnectList::value_type(pConnect->GetSocket()->GetSocket(),pConnect) );
	lock.Unlock();
	//ִ��ҵ��
	STNetHost accessHost = pConnect->m_host;//��������ʣ��ֲ������뿪ʱ�����������Զ��ͷŷ���
	m_pNetServer->OnConnect( pConnect->m_host );
	/*
		��������
		�����OnConnect��ɣ��ſ��Կ�ʼ���������ϵ�IO�¼�
		���򣬿���ҵ�����δ������ӳ�ʼ�����������յ�OnMsg֪ͨ��
		����ҵ��㲻֪������δ�����Ϣ
	 */
	bool bMonitor = true;
	if ( !m_pNetMonitor->AddMonitor(sock) ) return false;
#ifdef WIN32
	bMonitor = m_pNetMonitor->AddRecv( 
		sock, 
		(char*)(pConnect->PrepareBuffer(BUFBLOCK_SIZE)), 
		BUFBLOCK_SIZE );
#else
	bMonitor = m_pNetMonitor->AddIO( sock, true, false );
#endif
	if ( !bMonitor ) CloseConnect(pConnect->GetSocket()->GetSocket());
	return true;
}

void STNetEngine::OnClose( SOCKET sock )
{
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find(sock);
	if ( itNetConnect == m_connectList.end() )return;//�ײ��Ѿ������Ͽ�
	CloseConnect( itNetConnect );
}

connectState STNetEngine::OnData( SOCKET sock, char *pData, unsigned short uSize )
{
	connectState cs = unconnect;
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find(sock);//client�б������
	if ( itNetConnect == m_connectList.end() ) return cs;//�ײ��Ѿ��Ͽ�
	STNetConnect *pConnect = itNetConnect->second;
	STNetHost accessHost = pConnect->m_host;//��������ʣ��ֲ������뿪ʱ�����������Զ��ͷŷ���
	lock.Unlock();

	pConnect->RefreshHeart();
	try
	{
		cs = RecvData( pConnect, pData, uSize );
		if ( unconnect == cs )
		{
			OnClose( sock );
			return cs;
		}
		if ( 0 != AtomAdd(&pConnect->m_nReadCount, 1) ) return cs;
		//ִ��ҵ��STNetServer::OnMsg();
		MsgWorker(pConnect);
	}catch( ... ){}
	return cs;
}

void* STNetEngine::MsgWorker( STNetConnect *pConnect )
{
	for ( ; !m_stop; )
	{
		m_pNetServer->OnMsg( pConnect->m_host );//�޷���ֵ���������߼������ڿͻ�ʵ��
		if ( !pConnect->m_bConnect ) break;
		if ( !pConnect->IsReadAble() ) break;
	}
	AtomDec(&pConnect->m_nReadCount,1);
	//ȷ��NetServer::OnClose()һ��������NetServer::OnMsg()���֮��
	if ( !pConnect->m_bConnect ) NotifyOnClose(pConnect);
	return 0;
}

connectState STNetEngine::RecvData( STNetConnect *pConnect, char *pData, unsigned short uSize )
{
#ifdef WIN32
	pConnect->WriteFinished( uSize );
	if ( !m_pNetMonitor->AddRecv(  pConnect->GetSocket()->GetSocket(), 
		(char*)(pConnect->PrepareBuffer(BUFBLOCK_SIZE)), BUFBLOCK_SIZE ) )
	{
		return unconnect;
	}
#else
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
			if ( !m_pNetMonitor->AddIO(pConnect->GetSocket()->GetSocket(), true, false) ) return unconnect;
			return wait_recv;
		}
		nMaxRecvSize += nRecvLen;
		pConnect->WriteFinished( nRecvLen );
	}
#endif
	return ok;
}

//�ر�һ������
void STNetEngine::CloseConnect( SOCKET sock )
{
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find( sock );
	if ( itNetConnect == m_connectList.end() ) return;//�ײ��Ѿ������Ͽ�
	CloseConnect( itNetConnect );
}

//��Ӧ��������¼�
connectState STNetEngine::OnSend( SOCKET sock, unsigned short uSize )
{
	connectState cs = unconnect;
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find(sock);
	if ( itNetConnect == m_connectList.end() )return cs;//�ײ��Ѿ������Ͽ�
	STNetConnect *pConnect = itNetConnect->second;
	STNetHost accessHost = pConnect->m_host;//��������ʣ��ֲ������뿪ʱ�����������Զ��ͷŷ���
	lock.Unlock();
	try
	{
		if ( pConnect->m_bConnect ) cs = SendData(pConnect, uSize);
	}
	catch(...)
	{
	}
	return cs;
	
}

connectState STNetEngine::SendData(STNetConnect *pConnect, unsigned short uSize)
{
#ifdef WIN32
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
#else
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
		return cs;
	}
	catch(...)
	{
	}
#endif
	return ok;
}

bool STNetEngine::Listen(int port)
{
	pair<map<int,SOCKET>::iterator,bool> ret 
		= m_serverPorts.insert(map<int,SOCKET>::value_type(port,INVALID_SOCKET));
	map<int,SOCKET>::iterator it = ret.first;
	if ( !ret.second && INVALID_SOCKET != it->second ) return true;
	if ( m_stop ) return true;

	it->second = ListenPort(port);
	if ( INVALID_SOCKET == it->second ) return false;
	return true;
}

SOCKET STNetEngine::ListenPort(int port)
{
	Socket listenSock;//����socket
	if ( !listenSock.Init( Socket::tcp ) ) return INVALID_SOCKET;
	listenSock.SetSockMode();
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
	if ( !m_pNetMonitor->AddAccept( listenSock.GetSocket() ) )
	{
		listenSock.Close();
		return INVALID_SOCKET;
	}

	return listenSock.Detach();
}


bool STNetEngine::ListenAll()
{
	bool ret = true;
	map<int,SOCKET>::iterator it = m_serverPorts.begin();
	char strPort[256];
	string strFaild;
	for ( ; it != m_serverPorts.end(); it++ )
	{
		if ( INVALID_SOCKET != it->second ) continue;
		it->second = ListenPort(it->first);
		if ( INVALID_SOCKET == it->second ) 
		{
			sprintf( strPort, "%d", it->first );
			strFaild += strPort;
			strFaild += " ";
			ret = false;
		}
	}
	if ( !ret ) m_startError += "listen port:" + strFaild + "faild";
	return ret;
}


bool STNetEngine::Connect(const char* ip, int port)
{
	uint64 addr64 = 0;
	if ( !addrToI64(addr64, ip, port) ) return false;

	pair<map<uint64,SOCKET>::iterator,bool> ret 
		= m_serIPList.insert(map<uint64,SOCKET>::value_type(addr64,INVALID_SOCKET));
	map<uint64,SOCKET>::iterator it = ret.first;
	if ( !ret.second && INVALID_SOCKET != it->second ) return true;
	if ( m_stop ) return true;

	it->second = ConnectOtherServer(ip, port);
	if ( INVALID_SOCKET == it->second ) return false;

	if ( !OnConnect(it->second, true) )	it->second = INVALID_SOCKET;
	
	return true;
}

SOCKET STNetEngine::ConnectOtherServer(const char* ip, int port)
{
	Socket sock;//����socket
	if ( !sock.Init( Socket::tcp ) ) return INVALID_SOCKET;
	if ( !sock.Connect(ip, port) ) 
	{
		sock.Close();
		return INVALID_SOCKET;
	}
	Socket::InitForIOCP(sock.GetSocket());
	
	return sock.Detach();
}

bool STNetEngine::ConnectAll()
{
	if ( m_stop ) return false;
	bool ret = true;
	map<uint64,SOCKET>::iterator it = m_serIPList.begin();
	char ip[24];
	int port;
	for ( ; it != m_serIPList.end(); it++ )
	{
		if ( INVALID_SOCKET != it->second ) continue;
		i64ToAddr(ip, port, it->first);
		it->second = ConnectOtherServer(ip, port);
		if ( INVALID_SOCKET == it->second ) 
		{
			ret = false;
			continue;
		}
		if ( !OnConnect(it->second, true) )	it->second = INVALID_SOCKET;
	}
	
	return ret;
}

void STNetEngine::SetServerClose(STNetConnect *pConnect)
{
	if ( !pConnect->m_host.IsServer() ) return;
	
	SOCKET sock = pConnect->GetID();
	map<uint64,SOCKET>::iterator it = m_serIPList.begin();
	for ( ; it != m_serIPList.end(); it++ )
	{
		if ( sock != it->second ) continue;
		it->second = INVALID_SOCKET;
		break;
	}
}

//��ĳ�����ӹ㲥��Ϣ(ҵ���ӿ�)
void STNetEngine::BroadcastMsg( int *recvGroupIDs, int recvCount, char *msg, int msgsize, int *filterGroupIDs, int filterCount )
{
	//////////////////////////////////////////////////////////////////////////
	//�ر�������������
	ConnectList::iterator it;
	STNetConnect *pConnect;
	vector<STNetConnect*> recverList;
	STNetHost accessHost;
	//���������й㲥�������Ӹ��Ƶ�һ��������
	AutoLock lock( &m_connectsMutex );
	for ( it = m_connectList.begin(); m_nHeartTime > 0 && it != m_connectList.end(); it++ )
	{
		pConnect = it->second;
		if ( !pConnect->IsInGroups(recvGroupIDs, recvCount) 
			|| pConnect->IsInGroups(filterGroupIDs, filterCount) ) continue;
		recverList.push_back(pConnect);
		accessHost = pConnect->m_host;//���û����ʣ��ֲ������뿪ʱ�����������Զ��ͷŷ���
	}
	lock.Unlock();
	
	//������е����ӿ�ʼ�㲥
	vector<STNetConnect*>::iterator itv = recverList.begin();
	for ( ; itv != recverList.end(); itv++ )
	{
		pConnect = *itv;
		if ( pConnect->m_bConnect ) pConnect->SendData((const unsigned char*)msg,msgsize);
	}
}

//��ĳ����������Ϣ(ҵ���ӿ�)
void STNetEngine::SendMsg( int hostID, char *msg, int msgsize )
{
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find(hostID);
	if ( itNetConnect == m_connectList.end() ) return;//�ײ��Ѿ������Ͽ�
	STNetConnect *pConnect = itNetConnect->second;
	STNetHost accessHost = pConnect->m_host;//���û����ʣ��ֲ������뿪ʱ�����������Զ��ͷŷ���
	lock.Unlock();
	if ( pConnect->m_bConnect ) pConnect->SendData((const unsigned char*)msg,msgsize);

	return;
}

const char* STNetEngine::GetInitError()//ȡ������������Ϣ
{
	return m_startError.c_str();
}

}
// namespace mdk

