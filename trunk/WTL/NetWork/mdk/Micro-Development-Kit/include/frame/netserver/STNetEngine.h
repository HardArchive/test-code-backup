#ifndef MDK_ST_NET_ENGINE_H
#define MDK_ST_NET_ENGINE_H

#include "../../../include/mdk/Socket.h"
#include "../../../include/mdk/FixLengthInt.h"
#include "../../../include/mdk/MemoryPool.h"
#include "../../../include/mdk/Thread.h"
#include "../../../include/mdk/Lock.h"

#include <map>
#include <vector>
#include <string>

namespace mdk
{
void XXSleep( long lSecond );
class STNetConnect;
class NetHost;
class NetEventMonitor;
class STIocp;
class STEpoll;
class STNetServer;
class MemoryPool;
typedef std::map<SOCKET,STNetConnect*> ConnectList;
	
/**
 * ������ͨ��������(���̰߳�)
 * ͨ�Ų��������
 * ʹ��һ��ͨ�Ų��ԣ�IOCP��EPoll��ͳ��select�ȣ�����ͨ�ſ���
 * 
 * ��Ա
 * �������������
 * �ͻ����������ӳ���
 * �ӿ�
 * ����
 * ֹͣ
 * 
 * ����
 * ����������
 * �Ͽ�����
 * ��Ϣ����
 * 
 */
enum connectState
{
	ok = 0,
	unconnect = 1,
	wait_recv = 2,
	wait_send = 3,
};
class STNetEngine
{
	friend class STNetServer;
protected:
	std::string m_startError;//����ʧ��ԭ��
	MemoryPool *m_pConnectPool;//STNetConnect�����
	int m_averageConnectCount;//ƽ��������
	bool m_stop;//ֹͣ��־
	/**
		���ӱ�
		map<unsigned long,STNetConnect*>
		��ʱ�����б����������з���������
		��û�����������ӶϿ�
	*/
	ConnectList m_connectList;
	Mutex m_connectsMutex;//�����б���ʿ���
	int m_nHeartTime;//�������(S)
	int m_nReconnectTime;//�Զ�����ʱ��(S)
	Thread m_mainThread;
#ifdef WIN32
	STIocp *m_pNetMonitor;
#else
	STEpoll *m_pNetMonitor;
	std::map<SOCKET,int> m_ioList;//δ���io������socket�б�
#endif
	STNetServer *m_pNetServer;
	std::map<int,SOCKET> m_serverPorts;//�ṩ����Ķ˿�,key�˿ڣ�value״̬��������˿ڵ��׽���
	std::map<uint64,SOCKET> m_serIPList;//���ӵ��ⲿ�����ַ�б�
protected:
	//win������io����
	bool WINIO(int timeout);
	//linux������io����
	bool LinuxIO(int timeout);
	//��Ӧ�����¼�,sockΪ�����ӵ��׽���
	bool OnConnect( SOCKET sock, bool isConnectServer );
	//��Ӧ�ر��¼���sockΪ�رյ��׽���
	void OnClose( SOCKET sock );
	void NotifyOnClose(STNetConnect *pConnect);//����OnClose֪ͨ
	//��Ӧ���ݵ����¼���sockΪ�����ݵ�����׽���
	connectState OnData( SOCKET sock, char *pData, unsigned short uSize );
	/*
		��������
		��������״̬
	*/
	connectState RecvData( STNetConnect *pConnect, char *pData, unsigned short uSize );
	void* MsgWorker( STNetConnect *pConnect );//ҵ��㴦����Ϣ
	connectState OnSend( SOCKET sock, unsigned short uSize );//��Ӧ�����¼�
	virtual connectState SendData(STNetConnect *pConnect, unsigned short uSize);//��������
	virtual SOCKET ListenPort(int port);//����һ���˿�,���ش������׽���
	//��ĳ�����ӹ㲥��Ϣ(ҵ���ӿ�)
	void BroadcastMsg( int *recvGroupIDs, int recvCount, char *msg, int msgsize, int *filterGroupIDs, int filterCount );
	void SendMsg( int hostID, char *msg, int msgsize );//��ĳ����������Ϣ(ҵ���ӿ�)
private:
	//���߳�
	void* RemoteCall Main(void*);
	//�����߳�
	void HeartMonitor();
	//��������
	void ReConnectAll();
	//�ر�һ�����ӣ���socket�Ӽ�������ɾ��
	void CloseConnect( ConnectList::iterator it );

	//////////////////////////////////////////////////////////////////////////
	//����˿�
	bool ListenAll();//��������ע��Ķ˿�
	//////////////////////////////////////////////////////////////////////////
	//����������������
	SOCKET ConnectOtherServer(const char* ip, int port);//����һ������,��������׽���
	bool ConnectAll();//��������ע��ķ��������ӵĻ��Զ�����
	void SetServerClose(STNetConnect *pConnect);//���������ӵķ���Ϊ�ر�״̬
	const char* GetInitError();//ȡ������������Ϣ
public:
	/**
	 * ���캯��,�󶨷�������ͨ�Ų���
	 * 
	 */
	STNetEngine();
	virtual ~STNetEngine();

	//����ƽ��������
	void SetAverageConnectCount(int count);
	//��������ʱ��
	void SetHeartTime( int nSecond );
	//�����Զ�����ʱ��
	void SetReconnectTime( int nSecond );
	/**
	 * ��ʼ
	 * �ɹ�����true��ʧ�ܷ���false
	 */
	bool Start();
	//ֹͣ
	void Stop();
	//�ȴ�ֹͣ
	void WaitStop();
	//�ر�һ���������,ͨ�Ų㷢���������ر�����ʱ����������ýӿ�
	void CloseConnect( SOCKET sock );
	//����һ���˿�
	bool Listen( int port );
	/*
		����һ������
		heartMsg�Ǹ÷�����Ҫ�������������
		����Է�û��Ҫ����������len����Ϊ0��
	*/
	bool Connect(const char* ip, int port);
};

}  // namespace mdk
#endif //MDK_ST_NET_ENGINE_H
