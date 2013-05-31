
// ConnectHandle.h
// ����ͻ�������
// �ܶ�ʱ�䣬�����������������ˣ������������������������ˡ�û��ʲô��ںý���
// ������2009��Ĵ����һ�����ټ������������˼ά����������������ϵĶ����������������ļ̳�������
// �����ң��������Щ�ѣ������Ҳ����ں�����Ϊ��֪��������ĵ�·�Ӳ�ƽ̹���������¿�ʼ����Ϊ���ܸе�������ҵ�������
// ��Ӷ��������������ݰ����Ĺܿء�
// add by freeeyes
// 2008-12-22

#ifndef _CONNECTHANDLE_H
#define _CONNECTHANDLE_H

#include "define.h"

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "AceReactorManager.h"
#include "MessageService.h"
#include "IConnectManager.h"
#include "MakePacket.h"
#include "MessageBlockManager.h"
#include "PacketParsePool.h"
#include "BuffPacketManager.h"
#include "ForbiddenIP.h"
#include "IPAccount.h"
#include "TimerManager.h"
#include "SendMessage.h"
#include "CommandAccount.h"

#include <map>

using namespace std;

class CConnectHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
	CConnectHandler(void);
	~CConnectHandler(void);

	//��д�̳з���
	virtual int open(void*);                                                 //�û�����һ������
	virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);            //���ܿͻ����յ������ݿ�
	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);           //���ӹر��¼�

	void Init(uint16 u2HandlerID);                                           //Connect Pool��ʼ������ʱ����õķ���

	bool SendMessage(IBuffPacket* pBuffPacket, bool blState, uint8 u1SendType, uint32& u4PacketSize);  //���͵�ǰ����

	bool SetRecvQueueTimeCost(uint32 u4TimeCost);                            //��¼��ǰ�������ݵ�ģ�鴦����ɵľ���ʱ������
	bool SetSendQueueTimeCost(uint32 u4TimeCost);                            //��¼��ǰ�ӷ��Ͷ��е����ݷ�����ɵľ���ʱ������

	bool Close(int nIOCount = 1);
	bool ServerClose();                                                      //�������رտͻ������ӷ���

	const char* GetError();                                                  //�õ���ǰ������Ϣ
	void        SetConnectID(uint32 u4ConnectID);                            //���õ�ǰ����ID
	uint32      GetConnectID();                                              //�õ���ǰ����ID
	uint8       GetConnectState();                                           //�õ�����״̬
	uint8       GetSendBuffState();                                          //�õ�����״̬    
	uint8       GetIsClosing();                                              //�����Ƿ�Ӧ�ùر�
	bool        CheckAlive();
	_ClientConnectInfo GetClientInfo();                                      //�õ��ͻ�����Ϣ 
	_ClientIPInfo      GetClientIPInfo();                                    //�õ��ͻ���IP��Ϣ

private:
	bool CheckMessage();                                                     //������յ�����
	bool PutSendPacket(ACE_Message_Block* pMbData);                          //��������

private:
	char                       m_szError[MAX_BUFF_500];        //������Ϣ��������
	ACE_INET_Addr              m_addrRemote;                   //Զ�����ӿͻ��˵�ַ
	ACE_Time_Value             m_atvConnect;                   //��ǰ���ӽ���ʱ��
	ACE_Time_Value             m_atvInput;                     //���һ�ν�������ʱ��
	ACE_Time_Value             m_atvOutput;                    //���һ�η�������ʱ��
	ACE_Time_Value             m_atvSendAlive;                 //���Ӵ��ʱ��

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
	uint32                     m_u4MaxPacketSize;              //�������ݰ�����󳤶�
	uint64                     m_u8RecvQueueTimeCost;          //�ɹ��������ݵ����ݴ�����ɣ�δ���ͣ����ѵ�ʱ���ܺ�
	uint32                     m_u4RecvQueueCount;             //��ǰ���ӱ���������ݰ���
	uint64                     m_u8SendQueueTimeCost;          //�ɹ��������ݵ����ݴ�����ɣ�ֻ���ͣ����ѵ�ʱ���ܺ�
	uint64                     m_u8SendQueueTimeout;           //���ͳ�ʱʱ�䣬�������ʱ��Ķ��ᱻ��¼����־��
	uint64                     m_u8RecvQueueTimeout;           //���ܳ�ʱʱ�䣬�������ʱ��Ķ��ᱻ��¼����־��

	ACE_Recursive_Thread_Mutex m_ThreadLock;

	uint32                     m_u4SendThresHold;              //���ͷ�ֵ(��Ϣ���ĸ���)
	uint32                     m_u4SendCheckTime;              //���ͼ��ʱ��ķ�ֵ
	int                        m_nIOCount;                     //��ǰIO�����ĸ���
	bool                       m_blBlockState;                 //�Ƿ�������״̬ falseΪ��������״̬��trueΪ������״̬
	int                        m_nBlockCount;                  //���������Ĵ���
	int                        m_nBlockMaxCount;               //������������������
	int                        m_nBlockSize;                   //��������ʱ�������Ĵ�С
	uint32                     m_u4BlockTimerID;               //������ʱ����ID

	_TimerCheckID*             m_pTCClose;                     //��ʱ������Ӵ��״̬�Ķ�ʱ��

	CBuffPacket                m_AlivePacket;                  //�����������
	CPacketParse*              m_pPacketParse;                 //���ݰ�������
	ACE_Message_Block*         m_pCurrMessage;                 //��ǰ��MB����
	ACE_Message_Block*         m_pBlockMessage;                //��ǰ���ͻ���ȴ����ݿ�
	uint32                     m_u4CurrSize;                   //��ǰMB�����ַ�����
	_TimeConnectInfo           m_TimeConnectInfo;              //���ӽ��������
};

//���������Ѿ�����������
class CConnectManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CConnectManager(void);
	~CConnectManager(void);

	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);   //��ʱ�����

	virtual int open(void* args = 0);
	virtual int svc (void);
	virtual int close (u_long);

	void CloseAll();
	bool AddConnect(uint32 u4ConnectID, CConnectHandler* pConnectHandler);
	bool SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket,  uint16 u2CommandID, bool blSendState, uint8 u1SendType, ACE_hrtime_t& tvSendBegin);              //ͬ������                                                                     //���ͻ�������
	bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);           //�첽����
	bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);                            //�첽Ⱥ��
	bool Close(uint32 u4ConnectID);                                                                          //�ͻ����ر�
	bool CloseConnect(uint32 u4ConnectID);                                                                   //�������ر�
	void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);                                         //���ص�ǰ������ӵ���Ϣ
	void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost);                                        //��¼ָ���������ݴ���ʱ��

	_ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                                       //�õ�ָ��������Ϣ

	bool StartTimer();                                                                                       //������ʱ��
	bool KillTimer();                                                                                        //�رն�ʱ��

	int         GetCount();
	const char* GetError();

private:
	bool IsRun();
	void CloseQueue();

private:
	typedef map<uint32, CConnectHandler*> mapConnectManager;
	typedef vector<uint32> vecConnectManager;
	mapConnectManager           m_mapConnectManager;     //��ǰ�Ѿ����ڲ��Ҽ��������map
	char                        m_szError[MAX_BUFF_500]; //������Ϣ����
	uint32                      m_u4ConnectCurrID;       //������ConnectID
	uint32                      m_u4TimeCheckID;         //��ʱ������TimerID
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;       //����ѭ����غͶϿ�����ʱ���������
	_TimerCheckID*              m_pTCTimeSendCheck;      //��ʱ���Ĳ����ṹ�壬����һ����ʱ��ִ�в�ͬ���¼�
	ACE_Time_Value              m_tvCheckConnect;        //��ʱ����һ�μ������ʱ��
	bool                        m_blRun;                 //�߳��Ƿ�������
	CSendMessagePool            m_SendMessagePool;       //������Ϣ��
	uint32                      m_u4SendQueuePutTime;    //���Ͷ�����ӳ�ʱʱ��
};

//����ConnectHandler�ڴ��
class CConnectHandlerPool
{
public:
	CConnectHandlerPool(void);
	~CConnectHandlerPool(void);

	void Init(int nObjcetCount);
	void Close();

	CConnectHandler* Create();
	bool Delete(CConnectHandler* pObject);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<CConnectHandler*, CConnectHandler*> mapHandle;
	mapHandle                   m_mapMessageUsed;                      //��ʹ�õ�
	mapHandle                   m_mapMessageFree;                      //û��ʹ�õ�
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //���ƶ��߳���
	uint32                      m_u4CurrMaxCount;
};

//����˼������ѷ��Ͷ�����ڼ����߳���ȥ����������ܡ������ﳢ��һ�¡�(���߳�ģʽ��һ���߳�һ�����У��������ֲ�������)
class CConnectManagerGroup : public IConnectManager
{
public:
	CConnectManagerGroup();
	~CConnectManagerGroup();

	void Init(uint16 u2SendQueueCount);

	bool AddConnect(CConnectHandler* pConnectHandler);
	bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);                      //�첽����
	bool PostMessage(uint32 u4ConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);            //�첽����
	bool PostMessage(vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);             //�첽Ⱥ��ָ����ID
	bool PostMessage(vector<uint32> vecConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);   //�첽Ⱥ��ָ����ID
	bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);
	bool PostMessageAll(const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);
	bool CloseConnect(uint32 u4ConnectID);                                                                   //�������ر�
	_ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                                       //�õ�ָ��������Ϣ
	void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);                                         //���ص�ǰ������ӵ���Ϣ
	void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost);                                        //��¼ָ���������ݴ���ʱ��

	int  GetCount();
	void CloseAll();
	bool Close(uint32 u4ConnectID);                                                                          //�ͻ����ر�

	bool StartTimer();                                                                                       //������ʱ��
	const char* GetError();                                                                                      

private:
	uint32 GetGroupIndex();                                                                                  //�õ���ǰ���ӵ�ID������

private:
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                                                           //���ƶ��߳���
	typedef map<int, CConnectManager*> mapConnectManager;                                                    //�������ӹ�����
	uint16            m_u2ThreadQueueCount;                                                                  //��ǰ�����̶߳��и���
	mapConnectManager m_mapConnectManager;                                                                   //����ǰ���з����̳߳ض���
	uint32            m_u4CurrMaxCount;                                                                      //��ǰ����������
};


typedef ACE_Singleton<CConnectManagerGroup, ACE_Recursive_Thread_Mutex> App_ConnectManager;
typedef ACE_Singleton<CConnectHandlerPool, ACE_Null_Mutex> App_ConnectHandlerPool;

#endif
