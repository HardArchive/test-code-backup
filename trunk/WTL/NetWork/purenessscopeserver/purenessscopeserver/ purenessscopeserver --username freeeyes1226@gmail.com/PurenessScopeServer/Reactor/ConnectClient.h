#ifndef _CONNECTCLIENT_H
#define _CONNECTCLIENT_H

//����ͻ�����Ϣ
//Reactor����ģʽ���������ݰ��������ݷ����߼��ദ��
//�������Proactor�µĿͻ��˷��ͣ���ʼ���reactor��ϵͳ��
//add by freeeyes
//2011-01-17

#include "define.h"

#include "ace/Reactor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "MainConfig.h"
#include "MessageBlockManager.h"
#include "ClientParsePool.h"
#include "ClientMessage.h"
#include "AceReactorManager.h"

class CConnectClient : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	CConnectClient(void);
	~CConnectClient(void);

	virtual int open(void*);
	virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);

	void SetClientMessage(IClientMessage* pClientMessage); //������Ϣ���մ�����
	void SetServerID(int nServerID);                       //���õ�ǰ��ServerID
	int  GetServerID();                                    //��õ�ǰServerID
	bool SendData(ACE_Message_Block* pmblk);
	bool Close();

	void ClinetClose();                                    //�����ر�
	bool CheckMessage();                                   //������յ�����Ϣ
	_ClientConnectInfo GetClientConnectInfo();             //�õ���ǰ������Ϣ

private:
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> Super;

public:
	char                       m_szError[MAX_BUFF_500];
	ACE_INET_Addr              m_addrRemote;
	int                        m_nIOCount;                     //��ǰIO�����ĸ���
	ACE_Recursive_Thread_Mutex m_ThreadLock;
	CClientParse*              m_pClientParse;
	IClientMessage*            m_pClientMessage;               //��Ϣ�������ָ��
	ACE_Message_Block*         m_pCurrMessage;                 //��ǰ��MB����
	int                        m_nServerID;                    //������ID
	uint32                     m_u4CurrSize;                   //��ǰ���յ����ֽ���
  uint32                     m_u4MaxPacketSize;              //�����հ���

	ACE_Time_Value              m_atvBegin;          //���ӽ���ʱ��
	uint32                      m_u4SendSize;        //�����ֽ���
	uint32                      m_u4SendCount;       //�������ݰ���  
	uint32                      m_u4RecvSize;        //�����ֽ���
	uint32                      m_u4RecvCount;       //�������ݰ���
	uint32                      m_u4CostTime;        //��Ϣ������ʱ��
};
#endif
