#ifndef _MESSAGESERVICE_H
#define _MESSAGESERVICE_H

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "Message.h"
#include "MessageManager.h"
#include "LogManager.h"
#include "ThreadInfo.h"
#include "BuffPacket.h"
#include "MainConfig.h"
#include "TimerManager.h"

#ifdef WIN32
#include "ProConnectHandle.h"
#else
#include "ConnectHandler.h"
#endif

class CMessageService : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CMessageService(void);
	~CMessageService(void);

	virtual int open(void* args = 0);
	virtual int svc (void);
	int Close ();

	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

	void Init(uint32 u4ThreadCount = MAX_MSG_THREADCOUNT, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK);

	bool Start();

	bool PutMessage(CMessage* pMessage);

	CThreadInfo* GetThreadInfo();

private:
	bool StartTimer();
	bool KillTimer();
	bool ResumeThread(int nThreadCount);
	bool IsRun();
	bool ProcessMessage(CMessage* pMessage, uint32 u4ThreadID);
	int  SaveThreadInfoData(); 

private:
	ACE_Recursive_Thread_Mutex     m_RunMutex;            //线程锁级别
	ACE_RW_Thread_Mutex            m_rwMutex;
	uint32                         m_u4ThreadCount;       //处理的线程总数
	uint32                         m_u4ThreadNo;          //当前线程ID
	uint32                         m_u4MaxQueue;          //线程中最大消息对象个数
	uint32                         m_u4TimerID;           //定时器对象
	bool                           m_blRun;               //线程是否在运行
	uint32                         m_u4HighMask;
	uint32                         m_u4LowMask;
	uint16                         m_u2ThreadTimeOut;  
	uint16                         m_u2ThreadTimeCheck;
	uint64                         m_u8TimeCost;          //Put到队列信息的数据处理时间
	uint32                         m_u4Count;             //消息队列接受个数
	uint32                         m_u4WorkQueuePutTime;  //入队超时时间

	CThreadInfo                    m_ThreadInfo;
};

typedef ACE_Singleton<CMessageService,ACE_Null_Mutex> App_MessageService; 
#endif
