#ifndef MDK_C_NET_SERVER_H
#define MDK_C_NET_SERVER_H

#include "../../../include/mdk/Thread.h"
#include "STNetHost.h"

namespace mdk
{
	class STNetEngine;
	class STNetHost;
/**
 * �������������(���̰߳�)
 * ������Ϣ��ִ��ҵ����
 * 
 */
class STNetServer
{
	friend class STNetEngine;
private:
	/*
	 * ���������������������ִ���ض�������ͨ�Ų���(IOCP��EPoll��ͳ��select)
	 * Ҫ����������ԣ�ֻҪ������������༴��
	 * 
	 */
	STNetEngine* m_pNetCard;
	//���߳�
	Thread m_mainThread;
	bool m_bStop;
	
protected:
public:
	void* RemoteCall TMain(void* pParam);
	/*
		��̨ҵ���̣߳��ص�����
		
			������ҵ���̲߳����κ����飬ֱ�ӵ��ô˷������˷����˳����������ҵ���߳��˳�
		�����߳��˳�������ʾ������ֹͣ����ֻ��ҵ���߼��̣߳�
			��������ȫ����û�г��������ں�̨��ҵ���߼���ֻ����������Ϣ

	 	����ʱ��������������

		�˳�ʱ����
			Stop()�����ú�3s�ڲ��Լ��˳���ǿ��ɱ��
				IFҵ���д���ѭ��������ʹ��IsOK()����Ƿ���Stop()������
				IFҵ���д����̹߳���������Ҫ��Stop()����ǰ���з����źŻ����߳���������
			
	 	�û�Ҳ���Ժ��Դ˷������Լ��������߳�
	 */
	virtual void* Main(void* pParam){ return 0; }
	
	/**
		�������ӽ�����ҵ����ص�����
		������
			host ���ӽ���������
			��������io��һЩ������������������ο�STNetHost��
	 */
	virtual void OnConnect(STNetHost &host){}
	/**
		�����ӶϿ���ҵ����ص�����
		������
			host ���ӶϿ�������
			���ڵ���ID()��������ʶ�Ͽ����󣬲���Close()�������Ѿ�Close()���ˣ�
			������������������ο�STNetHost��
	*/
	virtual void OnCloseConnect(STNetHost &host){}
	/**
		�����ݿɶ���ҵ����ص�����
		������
			host �����ݿɶ�������
			���ڵ���ID()��������ʶ�Ͽ����󣬲���Close()�������Ѿ�Close()���ˣ�
			������������������ο�STNetHost��
	*/
	virtual void OnMsg(STNetHost &host){}

	/*
		������״̬��飬����Ϊmain()��������Ϊѭ���˳�����ʹ��
		������Start()�󷵻�true��Stop()�󷵻�false
	*/
	bool IsOk();
 
public:
	STNetServer();
	virtual ~STNetServer();
	/**
	 * ���з�����
	 * �ɹ�����NULL
	 * ʧ�ܷ���ʧ��ԭ��
	 */
	const char* Start();
	/**
	 * �رշ�����
	 */
	void Stop();
	/*
	 *	�ȴ�������ֹͣ
	 */
	void WaitStop();

	//���õ������������̿��ܳ��ص�ƽ����������Ĭ��5000
	void SetAverageConnectCount(int count);
	//�����Զ�����ʱ��,��С10s���������򣬻�����С�ڵ���0��������������
	//ʹ��Connect()�������ӵĵ�ַ�Ͽ�ʱ��ϵͳ�ᶨʱ������������
	void SetReconnectTime( int nSecond );
	//��������ʱ��,��С10s���������򣬻�����С�ڵ���0�����������������
	void SetHeartTime( int nSecond );
	//����ĳ���˿ڣ��ɶ�ε��ü�������˿�
	bool Listen(int port);
	//�����ⲿ���������ɶ�ε������Ӷ���ⲿ������
	//����Ҫ��������������δ���˲��ԣ����ܳ���bug
	bool Connect(const char *ip, int port);
	/*
		�㲥��Ϣ
		������recvGroupIDs������һ�飬ͬʱ���˵�����filterGroupIDs������һ���������������Ϣ
		���STNetHost::InGroup(),STNetHost::OutGroup()ʹ��

		������
			recvGroupIDs		Ҫ���ո���Ϣ�ķ����б�
			recvCount		recvGroupIDs�з�����
			msg				��Ϣ
			msgsize			��Ϣ����
			filterGroupIDs		���ܽ��ո���Ϣ�ķ����б�
			filterCount		filterGroupIDs�з�����

		Ӧ�ó�����
			��Ϸ�кܶ��ͼ����ͼ��ΨһID����Ϊ����ID
			NetHost1->InGroup(��ͼID1) ��ҽ����ͼ1
			NetHost2->InGroup(��ͼID2) ��ҽ����ͼ2
			BroadcastMsg({��ͼID1, ��ͼID2,}, 2,��)���ͼ1���ͼ2�е�������ҷ�����Ϣ


		���磺
			A B C3������
			A���� 1 2 4
			B���� 2 3 5
				C���� 1 3 5
				D���� 2 3
				E���� 3 5
				BroadcastMsg( {1,3}, 2, msg, len, {5}, 1 );
				�����ڷ���1�����ڷ���3,ͬʱ�����ڷ���5������������Ϣ����AD�����յ���Ϣ��BCE������

		�û�Ҳ���Բ����÷������Լ������������
  
	 */
	void BroadcastMsg( int *recvGroupIDs, int recvCount, char *msg, int msgsize, int *filterGroupIDs, int filterCount );
	/*
		��ĳ����������Ϣ

					������
						hosteID	���շ�id
						msg		��Ϣ
						msgsize	��Ϣ����

		��STNetHost::Send()������
		SendMsg()�ڲ����������б��У��������Ҷ���Ȼ���ǵ���STNetHost::Send()������Ϣ
		���Ѿ��õ�STNetHost���������£�ֱ��STNetHost::Send()Ч����ߣ��Ҳ�������������
	 */
	void SendMsg( int hostID, char *msg, int msgsize );
	/*
	 	�ر�������������
	 */
	void CloseConnect( int hostID );
};

}  // namespace mdk
#endif //MDK_C_NET_SERVER_H
