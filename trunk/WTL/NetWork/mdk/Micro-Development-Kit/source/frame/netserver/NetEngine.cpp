
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

#include "../../../include/mdk/mapi.h"
#include "../../../include/mdk/Socket.h"

#include "../../../include/frame/netserver/NetEngine.h"
#include "../../../include/frame/netserver/NetConnect.h"
#include "../../../include/frame/netserver/NetEventMonitor.h"
#include "../../../include/frame/netserver/NetServer.h"
#include "../../../include/mdk/atom.h"
#include "../../../include/mdk/MemoryPool.h"

using namespace std;
namespace mdk
{

NetEngine::NetEngine()
{
	Socket::SocketInit();
	m_pConnectPool = NULL;
	m_stop = true;//ֹͣ��־
	m_startError = "";
	m_nHeartTime = 0;//�������(S)��Ĭ�ϲ����
	m_nReconnectTime = 0;//Ĭ�ϲ��Զ�����
	m_pNetMonitor = NULL;
	m_ioThreadCount = 16;//����io�߳�����
	m_workThreadCount = 16;//�����߳�����
	m_pNetServer = NULL;
	m_averageConnectCount = 5000;
}

NetEngine::~NetEngine()
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
void NetEngine::SetAverageConnectCount(int count)
{
	m_averageConnectCount = count;
}

//��������ʱ��
void NetEngine::SetHeartTime( int nSecond )
{
	m_nHeartTime = nSecond;
}

//�����Զ�����ʱ��,С�ڵ���0��ʾ���Զ�����
void NetEngine::SetReconnectTime( int nSecond )
{
	m_nReconnectTime = nSecond;
}

//��������IO�߳�����
void NetEngine::SetIOThreadCount(int nCount)
{
	m_ioThreadCount = nCount;//����io�߳�����
}

//���ù����߳���
void NetEngine::SetWorkThreadCount(int nCount)
{
	m_workThreadCount = nCount;//�����߳�����
}

/**
 * ��ʼ����
 * �ɹ�����true��ʧ�ܷ���false
 */
bool NetEngine::Start()
{
	if ( !m_stop ) return true;
	m_stop = false;	
	int memoryCount = 2;
	for ( memoryCount = 2; memoryCount * memoryCount < m_averageConnectCount * 2; memoryCount++ );
	if ( memoryCount < 200 ) memoryCount = 200;
	if ( NULL != m_pConnectPool )//֮ǰStop��������Start
	{
		delete m_pConnectPool;
		m_pConnectPool = NULL;
	}
	m_pConnectPool = new MemoryPool( sizeof(NetConnect), memoryCount );
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
	m_workThreads.Start( m_workThreadCount );
	int i = 0;
	for ( ; i < m_ioThreadCount; i++ ) m_ioThreads.Accept( Executor::Bind(&NetEngine::NetMonitorTask), this, NULL );
	m_ioThreads.Start( m_ioThreadCount );
	if ( !ListenAll() )
	{
		Stop();
		return false;
	}
	ConnectAll();
	return m_mainThread.Run( Executor::Bind(&NetEngine::Main), this, 0 );
}

void* NetEngine::NetMonitorTask( void* pParam)
{
	return NetMonitor( pParam );
}

//�ȴ�ֹͣ
void NetEngine::WaitStop()
{
	m_mainThread.WaitStop();
}

//ֹͣ����
void NetEngine::Stop()
{
	if ( m_stop ) return;
	m_stop = true;
	m_pNetMonitor->Stop();
	m_sigStop.Notify();
	m_mainThread.Stop( 3000 );
	m_ioThreads.Stop();
	m_workThreads.Stop();
}

//���߳�
void* NetEngine::Main(void*)
{
	while ( !m_stop ) 
	{
		if ( m_sigStop.Wait( 10000 ) ) break;
		HeartMonitor();
		ReConnectAll();
	}
	return NULL;
}

//�����߳�
void NetEngine::HeartMonitor()
{
	if ( 0 >= m_nHeartTime ) return;//����������
	//////////////////////////////////////////////////////////////////////////
	//�ر�������������
	ConnectList::iterator it;
	NetConnect *pConnect;
	time_t tCurTime = 0;
	tCurTime = time( NULL );
	time_t tLastHeart;
	AutoLock lock( &m_connectsMutex );
	for ( it = m_connectList.begin();  it != m_connectList.end(); )
	{
		pConnect = it->second;
		if ( pConnect->m_host.IsServer() ) //�������ӣ����������
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
		//������/�����ѶϿ���ǿ�ƶϿ�����
		CloseConnect( it );
		it = m_connectList.begin();
	}
	lock.Unlock();
}

void NetEngine::ReConnectAll()
{
	if ( 0 >= m_nReconnectTime ) return;//����������
	static time_t lastConnect = time(NULL);
	time_t curTime = time(NULL);
	if ( m_nReconnectTime > curTime - lastConnect ) return;
	lastConnect = curTime;
	ConnectAll();
}

//�ر�һ������
void NetEngine::CloseConnect( ConnectList::iterator it )
{
	/*
	   ������ɾ���ٹرգ�˳���ܻ���
	   ����رպ�eraseǰ��������client���ӽ�����
	   ϵͳ���̾ͰѸ����ӷ������clientʹ�ã������client�ڲ���m_connectListʱʧ��
	*/
	NetConnect *pConnect = it->second;
	m_connectList.erase( it );//֮�󲻿�����MsgWorker()��������ΪOnData�����Ѿ��Ҳ���������
	/*
		pConnect->GetSocket()->Close();
		���ϲ�����V1.51���У����Ӵ˴��ƶ���CloseWorker()��
		��m_pNetServer->OnCloseConnect()֮��ִ��

		A.�����Ƴ�Close��Ŀ��
			��OnCloseConnect()���ǰ��Ҳ����ҵ���������ӶϿ�ҵ��ǰ
			����ϵͳ����socket�ľ����������
			
			�Ա��ⷢ�����������
				�����û���ҵ���(NetServer������)�д���map<int,NetHost>����host�б�
				��NetServer::OnConnect()ʱ����
				��NetServer::OnClose())ʱɾ��

				����������ִ�йر�socket��������Ϊ100��

				ҵ���NetServer::OnClose����֮��õ�֪ͨ��
				�����ʱ���������ӽ�������ϵͳ�����ʹ��100��Ϊ�������������ӡ�
				�����Ƕ��̲߳��������Կ�����NetServer::OnClose֮ǰ����ִ��NetServer::OnConnect()
				����NetServer::OnClose��û����ɣ�100���key���ɴ������û�������map�У�
				����NetServer::OnConnect()�еĲ������ʧ��
				
		  ��ˣ��û���Ҫ׼��һ��wait_insert�жӣ���OnConnect()��insertʧ��ʱ��
		  ��Ҫ�����󱣴浽wait_insert�жӣ�����ֹOnConnect()ҵ���߼�

		  ��OnClose��ɾ��������ö����key��wait_insert�ж��м�飬
		  �ҵ�ƥ��Ķ�����insert��Ȼ�����ִ��OnConnect�ĺ���ҵ��
		  OnConnectҵ���߼��������
		  
		  1.�����Ϸǳ��鷳
		  2.�ƻ��˹����ھۣ�OnConnect()��OnClose()�߼����������һ��

		B.�ٷ����Ƴ�Close��û������������
		����1����������û�йرգ���server������closeʱ������״̬ʵ�ʻ��������ģ�
		���client��ͣ�ط������ݣ��᲻�ᵼ��OnMsg�߳�һֱ���ղ������ݣ�
		��OnCloseû����ִ�У�
		
		  �𣺲��ᣬ��Ϊm_bConnect��־������Ϊfalse�ˣ���OnMsg����MsgWorker()�б�ѭ�����ã�
		ÿ��ѭ��������m_bConnect��־�����Լ�ʹ�������ݿɽ��գ�OnMsgҲ�ᱻ��ֹ
	 */
//	pConnect->GetSocket()->Close();

	pConnect->m_bConnect = false;
	/*
		ִ��ҵ��NetServer::OnClose();
		������δ���MsgWorker������(MsgWorker�ڲ�ѭ������OnMsg())��Ҳ���Ǳ�����OnMsg����

		��MsgWorker�Ĳ����������
		���1��MsgWorker�Ѿ�return
			��ôAtomAdd����0��ִ��NotifyOnClose()�������ܷ�����OnMsg֮ǰ��
			֮��Ҳ������OnMsg��ǰ���Ѿ�˵��MsgWorker()�������ٷ���
		���2��MsgWorkerδ���أ���2�����
			���1��������AtomAdd
				��Ȼ���ط�0����Ϊû�з�����AtomDec
				��ִ��OnClose
				��©OnClose��
				���ᣡ��ô��MsgWorker()��AtomAdd���ط�0������AtomDec��Ȼ����>1��
				MsgWorker()����ѭ��һ��OnMsg�����OnMsg��û�����ݵģ����û�û��Ӱ��
				OnMsg�������㹻���ݺ���������
				Ȼ���˳�ѭ��������m_bConnect=false������NotifyOnClose()����OnClose֪ͨ
				OnClose֪ͨû�б���©
			���2��MsgWorker��AtomDec
				��Ȼ����1����ΪMsgWorkerѭ������������1�����м���û��AtomAdd����
				MsgWorker�˳�ѭ��
				����m_bConnect=false������NotifyOnClose()����OnClose֪ͨ
				Ȼ������AtomAdd��Ȼ����0��ҲNotifyOnClose()����OnClose֪ͨ
				�ظ�֪ͨ��
				���ᣬNotifyOnClose()��֤�˶��̲߳��������£�ֻ��֪ͨ1��

		��OnData�Ĳ����������
			���1��OnData��AtomAdd
				��֤��MsgWorker��ִ��
				AtomAdd���ط�0������NotifyOnClose
				MsgWorkerһ����NotifyOnClose
			���2��������AtomAdd
				OnData��AtomAddʱ��Ȼ����>0��OnData����MsgWorker
				��©OnMsg��Ӧ�������������ݣ���������©
				��3�ֶϿ����
				1.server��������û���ˣ�����close���Ǿ�������ԭ��ǿ�ƶϿ�������ν���ݶ�ʧ
				2.client��server���������ҵ��ϣ�������Ͽ�
					�Ǿ�Ӧ�ð���ͨ����ҵ���Ӱ�ȫ�Ͽ���ԭ���ý��շ�����Close
					�����ܷ��ͷ�����Close,���Բ�������©����
					������ͷ�����close�����������������ƣ���û�취��֤�յ������������
	 */
	if ( 0 == AtomAdd(&pConnect->m_nReadCount, 1) ) NotifyOnClose(pConnect);
	pConnect->Release();//���ӶϿ��ͷŹ������
	return;
}

void NetEngine::NotifyOnClose(NetConnect *pConnect)
{
	if ( 0 == AtomAdd(&pConnect->m_nDoCloseWorkCount, 1) )//ֻ��1���߳�ִ��OnClose���ҽ�ִ��1��
	{
		AtomAdd(&pConnect->m_useCount, 1);//ҵ����Ȼ�ȡ����
		m_workThreads.Accept( Executor::Bind(&NetEngine::CloseWorker), this, pConnect);
	}
}

bool NetEngine::OnConnect( SOCKET sock, bool isConnectServer )
{
	NetConnect *pConnect = new (m_pConnectPool->Alloc())NetConnect(sock, isConnectServer, m_pNetMonitor, this, m_pConnectPool);
	if ( NULL == pConnect ) 
	{
		closesocket(sock);
		return false;
	}
	pConnect->GetSocket()->SetSockMode();
	//��������б�
	AutoLock lock( &m_connectsMutex );
	pConnect->RefreshHeart();
	pair<ConnectList::iterator, bool> ret = m_connectList.insert( ConnectList::value_type(pConnect->GetSocket()->GetSocket(),pConnect) );
	AtomAdd(&pConnect->m_useCount, 1);//ҵ����Ȼ�ȡ����
	lock.Unlock();
	//ִ��ҵ��
	m_workThreads.Accept( Executor::Bind(&NetEngine::ConnectWorker), this, pConnect );
	return true;
}

void* NetEngine::ConnectWorker( NetConnect *pConnect )
{
	m_pNetServer->OnConnect( pConnect->m_host );
	/*
		��������
		�������OnConnectҵ����ɣ��ſ��Կ�ʼ���������ϵ�IO�¼�
		���򣬿���ҵ�����δ������ӳ�ʼ�����������յ�OnMsg֪ͨ��
		����ҵ��㲻֪������δ�����Ϣ
		
		����δ���������pConnect���󲻴��ڲ����̷߳���
		���OnConnectҵ���У�û�йر����ӣ����ܼ������

		����������m_bConnect����MonitorConnect�п��ܳɹ�������OnData�л��ᴥ����
		��ΪCloseConnect����ֻ�������˹ر����ӵı�־������NetConnect�������б�ɾ����
		��û����Ĺر�socket��
		����Ϊ�˱�֤socket�����NetServer::OnCloseҵ�����ǰ������ϵͳ�ظ�ʹ�ã�

		�����ر�����NetEngine::CloseWorker()��������һ���߳��ˡ�
		�������OnConnectҵ���е����˹رգ�����CloseWorker�߳�ִ��ǰ��
		��������Ȼ�п����ȱ�ִ�У������ɹ�������������ǲ�ϣ��������
	*/
	if ( pConnect->m_bConnect )
	{
		if ( !MonitorConnect(pConnect) )
		{
			CloseConnect(pConnect->GetSocket()->GetSocket());
		}
	}
	pConnect->Release();//ҵ���ʹ�����,�ͷŹ������
	
	return 0;
}

void NetEngine::OnClose( SOCKET sock )
{
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find(sock);
	if ( itNetConnect == m_connectList.end() )return;//�ײ��Ѿ������Ͽ�
	CloseConnect( itNetConnect );
	lock.Unlock();
}

void* NetEngine::CloseWorker( NetConnect *pConnect )
{
	SetServerClose(pConnect);//���ӵķ���Ͽ�
	m_pNetServer->OnCloseConnect( pConnect->m_host );
	/*
		����pConnect->GetSocket()->Close();����
		��V1.51���У���CloseConnect( ConnectList::iterator it )���ƶ�����
		�Ƴ�ִ��close

		ȷ��ҵ������closeҵ���ϵͳ�ſ���������socket���
		��ϸԭ�򣬲ο�CloseConnect( ConnectList::iterator it )��ע��
	*/
	pConnect->GetSocket()->Close();
	pConnect->Release();//ʹ������ͷŹ������
	return 0;
}

connectState NetEngine::OnData( SOCKET sock, char *pData, unsigned short uSize )
{
	connectState cs = unconnect;
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find(sock);//client�б������
	if ( itNetConnect == m_connectList.end() ) return cs;//�ײ��Ѿ��Ͽ�

	NetConnect *pConnect = itNetConnect->second;
	pConnect->RefreshHeart();
	AtomAdd(&pConnect->m_useCount, 1);//ҵ����Ȼ�ȡ����
	lock.Unlock();//ȷ��ҵ���ռ�ж����HeartMonitor()���л�����pConnect��״̬
	try
	{
		cs = RecvData( pConnect, pData, uSize );//������ʵ��
		if ( unconnect == cs )
		{
			pConnect->Release();//ʹ������ͷŹ������
			OnClose( sock );
			return cs;
		}
		/*
			���Ⲣ��MsgWorker��Ҳ���Ǳ��Ⲣ����

			��MsgWorker�Ĳ����������
			���1��MsgWorker�Ѿ�return
				��ôAtomAdd����0�������µ�MsgWorker��δ����

			���2��MsgWorkerδ��ɣ���2�����
				���1��������AtomAdd
				��Ȼ���ط�0����Ϊû�з�����AtomDec
				��������MsgWorker
				��©OnMsg��
				���ᣡ��ô��MsgWorker()��AtomAdd���ط�0������AtomDec��Ȼ����>1��
				MsgWorker()����ѭ��һ��OnMsg
				û����©OnMsg���޲���
			���2��MsgWorker��AtomDec
				��Ȼ����1����ΪMsgWorkerѭ������������1�����м���û��AtomAdd����
				MsgWorker�˳�ѭ��
				Ȼ������AtomAdd����Ȼ����0�������µ�MsgWorker��δ����
		 */
		if ( 0 < AtomAdd(&pConnect->m_nReadCount, 1) ) 
		{
			pConnect->Release();//ʹ������ͷŹ������
			return cs;
		}
		//ִ��ҵ��NetServer::OnMsg();
		m_workThreads.Accept( Executor::Bind(&NetEngine::MsgWorker), this, pConnect);
	}catch( ... ){}
	return cs;
}

void* NetEngine::MsgWorker( NetConnect *pConnect )
{
	for ( ; !m_stop; )
	{
		pConnect->m_nReadCount = 1;
		m_pNetServer->OnMsg( pConnect->m_host );//�޷���ֵ���������߼������ڿͻ�ʵ��
		if ( !pConnect->m_bConnect ) break;
		if ( pConnect->IsReadAble() ) continue;
		if ( 1 == AtomDec(&pConnect->m_nReadCount,1) ) break;//����©����
	}
	//����OnClose(),ȷ��NetServer::OnClose()һ��������NetServer::OnMsg()���֮��
	if ( !pConnect->m_bConnect ) NotifyOnClose(pConnect);
	pConnect->Release();//ʹ������ͷŹ������
	return 0;
}

connectState NetEngine::RecvData( NetConnect *pConnect, char *pData, unsigned short uSize )
{
	return unconnect;
}

//�ر�һ������
void NetEngine::CloseConnect( SOCKET sock )
{
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find( sock );
	if ( itNetConnect == m_connectList.end() ) return;//�ײ��Ѿ������Ͽ�
	CloseConnect( itNetConnect );
}

//��Ӧ��������¼�
connectState NetEngine::OnSend( SOCKET sock, unsigned short uSize )
{
	connectState cs = unconnect;
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find(sock);
	if ( itNetConnect == m_connectList.end() )return cs;//�ײ��Ѿ������Ͽ�
	NetConnect *pConnect = itNetConnect->second;
	AtomAdd(&pConnect->m_useCount, 1);//ҵ����Ȼ�ȡ����
	lock.Unlock();//ȷ��ҵ���ռ�ж����HeartMonitor()���л�����pConnect��״̬
	try
	{
		if ( pConnect->m_bConnect ) cs = SendData(pConnect, uSize);
	}
	catch(...)
	{
	}
	pConnect->Release();//ʹ������ͷŹ������
	return cs;
	
}

connectState NetEngine::SendData(NetConnect *pConnect, unsigned short uSize)
{
	return unconnect;
}

bool NetEngine::Listen(int port)
{
	AutoLock lock(&m_listenMutex);
	pair<map<int,SOCKET>::iterator,bool> ret 
		= m_serverPorts.insert(map<int,SOCKET>::value_type(port,INVALID_SOCKET));
	map<int,SOCKET>::iterator it = ret.first;
	if ( !ret.second && INVALID_SOCKET != it->second ) return true;
	if ( m_stop ) return true;

	it->second = ListenPort(port);
	if ( INVALID_SOCKET == it->second ) return false;
	return true;
}

SOCKET NetEngine::ListenPort(int port)
{
	return INVALID_SOCKET;
}

bool NetEngine::ListenAll()
{
	bool ret = true;
	AutoLock lock(&m_listenMutex);
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

bool NetEngine::Connect(const char* ip, int port)
{
	uint64 addr64 = 0;
	if ( !addrToI64(addr64, ip, port) ) return false;

	AutoLock lock(&m_serListMutex);
	pair<map<uint64,SOCKET>::iterator,bool> ret 
		= m_serIPList.insert(map<uint64,SOCKET>::value_type(addr64,INVALID_SOCKET));
	map<uint64,SOCKET>::iterator it = ret.first;
	if ( !ret.second && INVALID_SOCKET != it->second ) return true;
	if ( m_stop ) return true;

	it->second = ConnectOtherServer(ip, port);
	if ( INVALID_SOCKET == it->second ) return false;
	lock.Unlock();

	if ( !OnConnect(it->second, true) )	it->second = INVALID_SOCKET;
	
	return true;
}

SOCKET NetEngine::ConnectOtherServer(const char* ip, int port)
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

bool NetEngine::ConnectAll()
{
	if ( m_stop ) return false;
	bool ret = true;
	AutoLock lock(&m_serListMutex);
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

void NetEngine::SetServerClose(NetConnect *pConnect)
{
	if ( !pConnect->m_host.IsServer() ) return;
	
	SOCKET sock = pConnect->GetID();
	AutoLock lock(&m_serListMutex);
	map<uint64,SOCKET>::iterator it = m_serIPList.begin();
	for ( ; it != m_serIPList.end(); it++ )
	{
		if ( sock != it->second ) continue;
		it->second = INVALID_SOCKET;
		break;
	}
}

//��������
bool NetEngine::MonitorConnect(NetConnect *pConnect)
{
	return false;
}

//��ĳ�����ӹ㲥��Ϣ(ҵ���ӿ�)
void NetEngine::BroadcastMsg( int *recvGroupIDs, int recvCount, char *msg, int msgsize, int *filterGroupIDs, int filterCount )
{
	//////////////////////////////////////////////////////////////////////////
	//�ر�������������
	ConnectList::iterator it;
	NetConnect *pConnect;
	vector<NetConnect*> recverList;
	//���������й㲥�������Ӹ��Ƶ�һ��������
	AutoLock lock( &m_connectsMutex );
	for ( it = m_connectList.begin(); m_nHeartTime > 0 && it != m_connectList.end(); it++ )
	{
		pConnect = it->second;
		if ( !pConnect->IsInGroups(recvGroupIDs, recvCount) 
			|| pConnect->IsInGroups(filterGroupIDs, filterCount) ) continue;
		recverList.push_back(pConnect);
		AtomAdd(&pConnect->m_useCount, 1);//ҵ����Ȼ�ȡ����
	}
	lock.Unlock();
	
	//������е����ӿ�ʼ�㲥
	vector<NetConnect*>::iterator itv = recverList.begin();
	for ( ; itv != recverList.end(); itv++ )
	{
		pConnect = *itv;
		if ( pConnect->m_bConnect ) pConnect->SendData((const unsigned char*)msg,msgsize);
		pConnect->Release();//ʹ������ͷŹ������
	}
}

//��ĳ����������Ϣ(ҵ���ӿ�)
void NetEngine::SendMsg( int hostID, char *msg, int msgsize )
{
	AutoLock lock( &m_connectsMutex );
	ConnectList::iterator itNetConnect = m_connectList.find(hostID);
	if ( itNetConnect == m_connectList.end() ) return;//�ײ��Ѿ������Ͽ�
	NetConnect *pConnect = itNetConnect->second;
	AtomAdd(&pConnect->m_useCount, 1);//ҵ����Ȼ�ȡ����
	lock.Unlock();
	if ( pConnect->m_bConnect ) pConnect->SendData((const unsigned char*)msg,msgsize);
	pConnect->Release();//ʹ������ͷŹ������

	return;
}

const char* NetEngine::GetInitError()//ȡ������������Ϣ
{
	return m_startError.c_str();
}

}
// namespace mdk

