#ifndef _CONSOLEHANDLE_H
#define _CONSOLEHANDLE_H

//�����ڶԷ��������Ƶ�ģ�飬����Զ�̿����������
//������汾��һֱ��ӣ���������ſ�ʼ�㣬��Ҫ���Լ�������ξ������ٸ��Լ�������ɡ�
//add by freeeyes
#include "define.h"

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "AceReactorManager.h"
#include "MessageService.h"
#include "IConnectManager.h"
#include "ConsoleMessage.h"
#include "MessageBlockManager.h"
#include "PacketParsePool.h"
#include "BuffPacketManager.h"
#include "MessageBlockManager.h"

class CConsoleHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
	CConsoleHandler(void);
	~CConsoleHandler(void);

	//��д�̳з���
	virtual int open(void*);                                                //�û�����һ������
	virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);

	bool SendMessage(IBuffPacket* pBuffPacket);

	bool Close(int nIOCount = 1);
	bool ServerClose();

	const char* GetError();
	void        SetConnectID(uint32 u4ConnectID);
	uint32      GetConnectID();
	uint8       GetConnectState();                                           //�õ�����״̬
	uint8       GetSendBuffState();                                          //�õ�����״̬    
	uint8       GetIsClosing();                                              //�����Ƿ�Ӧ�ùر�

private:
	bool CheckMessage();                                                      //������յ�����
	bool PutSendPacket(ACE_Message_Block* pMbData);                           //��������

private:
	char                       m_szError[MAX_BUFF_500];
	ACE_INET_Addr              m_addrRemote;
	ACE_Time_Value             m_atvConnect;
	ACE_Time_Value             m_atvInput;
	ACE_Time_Value             m_atvOutput;
	ACE_Time_Value             m_atvSendAlive;

	uint8                      m_u1ConnectState;               //Ŀǰ���Ӵ���״̬
	uint8                      m_u1SendBuffState;              //Ŀǰ�������Ƿ��еȴ����͵�����
	uint8                      m_u1IsClosing;                  //�Ƿ�Ӧ�ùر� 0Ϊ��1Ϊ��
	uint16                     m_u2SendQueueMax;               //���Ͷ�����󳤶�
	uint16                     m_u2SendCount;                  //��ǰ���ݰ��ĸ���
	uint32                     m_u4HandlerID;                  //��Hander����ʱ��ID
	uint32                     m_u4ConnectID;                  //���ӵ�ID
	uint32                     m_u4AllRecvCount;               //��ǰ���ӽ������ݰ��ĸ���
	uint32                     m_u4AllSendCount;               //��ǰ���ӷ������ݰ��ĸ���
	uint32                     m_u4AllRecvSize;                //��ǰ���ӽ����ֽ�����
	uint32                     m_u4AllSendSize;                //��ǰ���ӷ����ֽ�����
	uint16                     m_u2MaxConnectTime;             //���ʱ�������ж�

	ACE_Recursive_Thread_Mutex m_ThreadLock;

	uint32                     m_u4SendThresHold;              //���ͷ�ֵ(��Ϣ���ĸ���)
	uint32                     m_u4SendCheckTime;              //���ͼ��ʱ��ķ�ֵ
	int                        m_nIOCount;                     //��ǰIO�����ĸ���

	_TimerCheckID*             m_pTCClose;

	CBuffPacket                m_AlivePacket;                  //�����������
	CPacketParse*              m_pPacketParse;                 //���ݰ�������
	ACE_Message_Block*         m_pCurrMessage;                 //��ǰ��MB����
	uint32                     m_u4CurrSize;                   //��ǰMB�����ַ�����

	CConsoleMessage          m_ConsoleMessage;                 //������Ϣ������
};

#endif
