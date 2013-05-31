// ConnectHandle.h
// ����������Ӧ���Ķ�ʱ���кܴ�������ԣ�����Reactor��Proactor��ʹ�ö�ʱ����ʱ���ǳ��鷳��
// ��Ȼ��ϵͳ֧������ģʽ�����Ըĵĺ�һЩ������ʱ������������������������з��������õ��Ķ�ʱ��������
// add by freeeyes
// 2009-08-25

#ifndef _TIMEMANAGER_H
#define _TIMEMANAGER_H

#include "define.h"
#include "ITimerManager.h"

//freeeyes�Լ�д�Ķ�ʱ��ʵ�֣�Ŀǰ���ã�

class CTimerManager
{
public:
	CTimerManager(void);
	~CTimerManager(void);

	bool Init();
	void Close();

	void wait_for_event(void);

	long schedule(ACE_Event_Handler *cb, void *arg, const ACE_Time_Value &abs_time, const ACE_Time_Value &interval = ACE_Time_Value::zero);

	int cancel(int nTimerID);

	int reset_interval(long timer_id, const ACE_Time_Value &interval);

private:
	ACE_Timer_Queue*  m_pTimerQueue;
	ACE_Event         m_EvTimer;
};

class CTimerManagerTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CTimerManagerTask();
	~CTimerManagerTask();

	bool Start();
	bool Stop();

	virtual int open(void *args = 0);
	virtual int svc(void);

	long schedule(ACE_Event_Handler *cb, void *arg, const ACE_Time_Value &abs_time, const ACE_Time_Value &interval = ACE_Time_Value::zero);
	int cancel(int nTimerID);

private:
	CTimerManager m_TimerManager;
};

typedef ACE_Singleton<ActiveTimer, ACE_Null_Mutex> App_TimerManager;
#endif
