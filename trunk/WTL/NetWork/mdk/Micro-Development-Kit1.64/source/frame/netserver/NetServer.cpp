#include <assert.h>

#include "../../../include/frame/netserver/NetServer.h"
#include "../../../include/frame/netserver/NetEngine.h"
#include "../../../include/frame/netserver/IOCPFrame.h"
#include "../../../include/frame/netserver/EpollFrame.h"


namespace mdk
{

NetServer::NetServer()
{
#ifdef WIN32
	m_pNetCard = new mdk::IOCPFrame;
#else
	m_pNetCard = new mdk::EpollFrame;
#endif
	m_pNetCard->m_pNetServer = this;
	m_bStop = true;
}
 
NetServer::~NetServer()
{
	delete m_pNetCard;
}

void* NetServer::TMain(void* pParam)
{
	Main(pParam);
	return 0;
}

/**
 * �ͻ��ӿڣ������пɼ�
 * ���з�����
 * Ĭ���߼���һ��˳������ģ��
 * Ӳ�̡��Կ���CPU������
 * 
 */
const char* NetServer::Start()
{
	if ( NULL == this->m_pNetCard ) return "no class NetEngine object";
	if ( !m_pNetCard->Start() ) return m_pNetCard->GetInitError();
	m_bStop = false;
	m_mainThread.Run(Executor::Bind(&NetServer::TMain), this, NULL);
	return NULL;
}

void NetServer::WaitStop()
{
	if ( NULL == this->m_pNetCard ) return;
	m_pNetCard->WaitStop();
	m_mainThread.WaitStop();
}

/**
 * �ͻ��ӿڣ������пɼ�
 * �رշ�����
 */
void NetServer::Stop()
{
	m_bStop = true;
	m_mainThread.Stop( 3000 );
	if ( NULL != this->m_pNetCard ) m_pNetCard->Stop();
}

//��������������true�����򷵻�false
bool NetServer::IsOk()
{
	return !m_bStop;
}

//����ƽ��������
void NetServer::SetAverageConnectCount(int count)
{
	m_pNetCard->SetAverageConnectCount(count);
}

//�����Զ�����ʱ��
void NetServer::SetReconnectTime( int nSecond )
{
	m_pNetCard->SetReconnectTime(nSecond);
}

//��������ʱ��
void NetServer::SetHeartTime( int nSecond )
{
	m_pNetCard->SetHeartTime(nSecond);
}

//��������IO�߳�����
void NetServer::SetIOThreadCount(int nCount)
{
	m_pNetCard->SetIOThreadCount(nCount);
}

//���ù����߳���
void NetServer::SetWorkThreadCount(int nCount)
{
	m_pNetCard->SetWorkThreadCount(nCount);
}

//�����˿�
bool NetServer::Listen(int port)
{
	m_pNetCard->Listen(port);
	return true;
}

//��������IP
bool NetServer::Connect(const char *ip, int port)
{
	m_pNetCard->Connect(ip, port);
	return true;
}

//��ĳ�����ӹ㲥��Ϣ
void NetServer::BroadcastMsg( int *recvGroupIDs, int recvCount, char *msg, int msgsize, int *filterGroupIDs, int filterCount )
{
	m_pNetCard->BroadcastMsg( recvGroupIDs, recvCount, msg, msgsize, filterGroupIDs, filterCount );
}

//��ĳ����������Ϣ
void NetServer::SendMsg( int hostID, char *msg, int msgsize )
{
	m_pNetCard->SendMsg(hostID, msg, msgsize);
}

/*
	�ر�������������
 */
void NetServer::CloseConnect( int hostID )
{
	m_pNetCard->CloseConnect( hostID );
}

}  // namespace mdk
