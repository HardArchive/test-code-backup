// ConnectHandle.h
// ����ͻ�������
// ���ò�˵��������һ�����Ŀ��飬���������������Ż���reactor�ܹ�����Ȼ������Ӧ����1024������������
// ����ACE�ܹ�����⣬ʹ�����µ���ʶ����Ȼ�����ڵ��ǣ��滻�ܹ��Ĳ��֣�ֻ�з��ͺͽ��ܲ��֡��������ֶ����Ա�����
// �Ѿ��Ļ��������Ķ�������Ȼ���Լ��񶨣���һ����ʹ��Ĺ��̣���Ȼ����������ĸ��ã����Ǳ���Ĵ��ۡ�
// �����Լ��ܹ����ĸ��ã��������⣬������⼴�ɡ�
// ���Ͱɣ����������ġ�
// add by freeeyes
// 2009-08-23

#ifndef _PROCONNECTHANDLE_H
#define _PROCONNECTHANDLE_H

#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Proactor.h"

#include "AceProactorManager.h"
#include "IConnectManager.h"
#include "TimerManager.h"
#include "MakePacket.h"
#include "PacketParsePool.h"
#include "BuffPacketManager.h"
#include "Fast_Asynch_Read_Stream.h"
#include "ForbiddenIP.h"
#include "IPAccount.h"
#include "SendMessage.h"
#include "CommandAccount.h"

#include <map>
#include <vector>

class CProConnectHandle : public ACE_Service_Handler
{
public:
	CProConnectHandle(void);
	~CProConnectHandle(void);

	//��д�̳з���
	virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                             //�û�����һ������
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);                   //������ܵ��û����ݰ���Ϣ�¼�
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);                 //�����͵��û�������ɵ��¼�
	virtual void addresses(const ACE_INET_Addr &remote_address, const ACE_INET_Addr &local_address); //��õ�ǰԶ�̿ͻ��˵�IP��ַ��Ϣ

	void Init(uint16 u2HandlerID);                                            //Connect Pool��ʼ�����õĺ���

	bool CheckAlive();                                                        //��⵱ǰ�����Ƿ�ʱ�ĺ���
	bool SendMessage(IBuffPacket* pBuffPacket, bool blState, uint8 u1SendType, uint32& u4PacketSize);   //���͸��ͻ������ݵĺ���
	bool Close(int nIOCount = 1);                                            
	bool ServerClose();                                                       //�������رտͻ������ӵĺ���

	const char*        GetError();                                            //�õ���ǰ���Ӵ�����Ϣ
	void               SetConnectID(uint32 u4ConnectID);                      //���õ�ǰ���ӵ�ID
	uint32             GetConnectID();                                        //��õ�ǰ���ӵ�ID
	uint8              GetConnectState();                                     //�õ�����״̬
	uint8              GetSendBuffState();                                    //�õ�����״̬    
	bool               GetIsClosing();                                        //�����Ƿ�Ӧ�ùر�
	_ClientConnectInfo GetClientInfo();                                       //�õ��ͻ�����Ϣ
	_ClientIPInfo      GetClientIPInfo();                                     //�õ��ͻ���IP��Ϣ

	bool SetRecvQueueTimeCost(uint32 u4TimeCost);                             //��¼��ǰ�������ݵ�ģ�鴦����ɵľ���ʱ������
	bool SetSendQueueTimeCost(uint32 u4TimeCost);                             //��¼��ǰ�ӷ��Ͷ��е����ݷ�����ɵľ���ʱ������

private:
	bool RecvClinetPacket(uint32 u4PackeLen);                                 //�������ݰ�
	bool CheckMessage();                                                      //������յ�����
	bool PutSendPacket(ACE_Message_Block* pMbData);                           //���������ݷ������

private:
	char             m_szError[MAX_BUFF_500];          //������Ϣ��������
	ACE_INET_Addr    m_addrRemote;                     //Զ�����ӿͻ��˵�ַ
	ACE_Time_Value   m_atvConnect;                     //��ǰ���ӽ���ʱ��
	ACE_Time_Value   m_atvInput;                       //���һ�ν�������ʱ��
	ACE_Time_Value   m_atvOutput;                      //���һ�η�������ʱ��
	ACE_Time_Value   m_atvSendAlive;                   //���Ӵ��ʱ��

	CBuffPacket        m_AlivePacket;                  //�����������
	uint8              m_u1ConnectState;               //Ŀǰ���Ӵ���״̬
	uint8              m_u1SendBuffState;              //Ŀǰ�������Ƿ��еȴ����͵�����
	uint16             m_u2SendQueueMax;               //���Ͷ�����󳤶�
	uint16             m_u2MaxConnectTime;             //�������ʱ���ж�
	uint32             m_u4MaxPacketSize;              //�������ݰ�����󳤶�
	uint64             m_u8RecvQueueTimeCost;          //�ɹ��������ݵ����ݴ�����ɣ�δ���ͣ����ѵ�ʱ���ܺ�
	uint32             m_u4RecvQueueCount;             //��ǰ���ӱ���������ݰ���
	uint64             m_u8SendQueueTimeCost;          //�ɹ��������ݵ����ݴ�����ɣ�ֻ���ͣ����ѵ�ʱ���ܺ�
	uint16             m_u2SendQueueTimeout;           //���ͳ�ʱʱ��,�������ʱ��Ķ��ᱻ��¼����־��
	uint16             m_u2RecvQueueTimeout;           //���ܳ�ʱʱ�䣬�������ʱ��Ķ��ᱻ��¼����־��
	uint32             m_u4HandlerID;                  //��Hander����ʱ��ID
	uint32             m_u4ConnectID;                  //��ǰConnect����ˮ��
	uint32             m_u4AllRecvCount;               //��ǰ���ӽ������ݰ��ĸ���
	uint32             m_u4AllSendCount;               //��ǰ���ӷ������ݰ��ĸ���
	uint32             m_u4AllRecvSize;                //��ǰ���ӽ����ֽ�����
	uint32             m_u4AllSendSize;                //��ǰ���ӷ����ֽ����� 
	CPacketParse*      m_pPacketParse;                 //���ݰ�������

	ACE_Recursive_Thread_Mutex m_ThreadWriteLock;

	uint32              m_u4SendThresHold;              //���ͷ�ֵ(��Ϣ���ĸ���)
	uint32              m_u4SendCheckTime;              //���ͼ��ʱ��ķ�ֵ
	bool                m_blCanWrite;                   //��һ�����ݰ��Ƿ��ͽ���
	bool                m_blTimeClose;                  //�Ƿ����ڹر�

	int                 m_u4RecvPacketCount;            //���ܰ��ĸ���
	int                 m_nIOCount;                     //��ǰIO�����ĸ���
	_TimeConnectInfo    m_TimeConnectInfo;              //���ӽ��������
	ACE_Message_Block*  m_pBlockMessage;                //��ǰ���ͻ���ȴ����ݿ�

	Fast_Asynch_Read_Stream  m_Reader;
	Fast_Asynch_Write_Stream m_Writer;
};

//���������Ѿ�����������
class CProConnectManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CProConnectManager(void);
	~CProConnectManager(void);

	virtual int open(void* args = 0);
	virtual int svc (void);
	virtual int close (u_long);
	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

	void CloseAll();                                                                                         //�ر�����������Ϣ
	bool AddConnect(uint32 u4ConnectID, CProConnectHandle* pConnectHandler);                                 //���һ���µ�������Ϣ
	bool SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint16 u2CommandID, bool blSendState, uint8 u1SendType, ACE_hrtime_t& tvSendBegin);               //��������
	bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);           //�첽����
	bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);                            //�첽Ⱥ��
	bool Close(uint32 u4ConnectID);                                                                          //�ͻ��˹ر�
	bool CloseConnect(uint32 u4ConnectID);                                                                   //�������ر�
	void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);                                         //���ص�ǰ������ӵ���Ϣ
	void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost);                                        //��¼ָ���������ݴ���ʱ��

	_ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                 //�õ�ָ��������Ϣ

	bool StartTimer();
	bool KillTimer();

	int         GetCount();
	const char* GetError();

private:
	bool IsRun();

private:
	typedef map<uint32, CProConnectHandle*> mapConnectManager;
	typedef vector<uint32> vecConnectManager;
	mapConnectManager           m_mapConnectManager;     //��ǰ�Ѿ����ڲ��Ҽ��������map
	char                        m_szError[MAX_BUFF_500]; //������Ϣ����
	uint32                      m_u4ConnectCurrID;       //������ConnectID
	uint32                      m_u4TimeCheckID;         //��ʱ������TimerID
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;       //����ѭ����غͶϿ�����ʱ���������
	ACE_Time_Value              m_tvCheckConnect;        //��ʱ����һ�μ������ʱ��
	bool                        m_blRun;                 //�߳��Ƿ�������
	CSendMessagePool            m_SendMessagePool;       //���Ͷ����
	uint32                      m_u4SendQueuePutTime;    //���Ͷ�����ӳ�ʱʱ��
};

//����ConnectHandler�ڴ��
class CProConnectHandlerPool
{
public:
	CProConnectHandlerPool(void);
	~CProConnectHandlerPool(void);

	void Init(int nObjcetCount);
	void Close();

	CProConnectHandle* Create();
	bool Delete(CProConnectHandle* pObject);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<CProConnectHandle*, CProConnectHandle*> mapHandle;
	mapHandle                   m_mapMessageUsed;                      //��ʹ�õ�
	mapHandle                   m_mapMessageFree;                      //û��ʹ�õ�
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //���ƶ��߳���
	uint32                      m_u4CurrMaxCount;
};

//����˼������ѷ��Ͷ�����ڼ����߳���ȥ����������ܡ������ﳢ��һ�¡�(���߳�ģʽ��һ���߳�һ�����У��������ֲ�������)
class CProConnectManagerGroup : public IConnectManager
{
public:
	CProConnectManagerGroup();
	~CProConnectManagerGroup();

	void Init(uint16 u2SendQueueCount);

	bool AddConnect(CProConnectHandle* pConnectHandler);
	bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true);           //�첽����
	bool PostMessage(uint32 u4ConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true); //�첽����
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
	typedef map<int, CProConnectManager*> mapConnectManager;                                                 //�������ӹ�����
	uint16            m_u2ThreadQueueCount;                                                                  //��ǰ�����̶߳��и���
	mapConnectManager m_mapConnectManager;                                                                   //����ǰ���з����̳߳ض���
	uint32            m_u4CurrMaxCount;                                                                      //��ǰ����������
};


typedef ACE_Singleton<CProConnectManagerGroup, ACE_Null_Mutex> App_ProConnectManager; 
typedef ACE_Singleton<CProConnectHandlerPool, ACE_Null_Mutex> App_ProConnectHandlerPool;

#endif
