#ifndef _ISECNCEMANAGER_H
#define _ISECNCEMANAGER_H

#include "ITimerManager.h"

#include "ace/Task.h"
#include "ace/Thread_Mutex.h"
#include "ace/OS_main.h"
#include "ace/Synch.h"

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"



//��¼ISenceManager��ָ��
//add by freeeyes

class ISenceManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
	ISenceManager() {};
	virtual ~ISenceManager() {};

	virtual ActiveTimer* GetActiveTimer() = 0;

	virtual bool SendRoomState(uint32 u4RoomID, uint32 u4PlayerID)        = 0;
	virtual bool SendRoomSitdown(uint32 u4RoomID, uint32 u4PlayerID)      = 0;
	virtual bool SendRoomTimer(uint32 u4RoomID, uint32 u4PlayerID)        = 0;
	virtual bool SendRoomPlayerTimer(uint32 u4RoomID, uint32 u4PlayerID)  = 0;
	virtual bool SendRoomMoney(uint32 u4RoomID)                           = 0;
	virtual bool SendRoomTableCode(uint32 u4RoomID, uint8 u1CodseCount)   = 0;
};

//����CSenceManager��Զ��ָ�룬����ܶ��ģ�Ϊ�˽��ѭ�����ã�����������
//ѭ��������ʱ���������������ʱ������鷳������ôд���Ժ����Ҹ��õķ�����
//add by freeeyes
class CSenceManagerPtr
{
public: 
	CSenceManagerPtr() { m_pSenceManager = NULL; };
	~CSenceManagerPtr() {};

	void SetSenceManagerPtr(ISenceManager* pSenceManager)
	{
		m_pSenceManager = pSenceManager;
	}

	ISenceManager* GetSenceManagerPtr()
	{
		return m_pSenceManager;
	}

	ActiveTimer* GetActiveTimer() { return m_pSenceManager->GetActiveTimer(); };       //�õ���ǰ��ʱ������

private:
	ISenceManager* m_pSenceManager;
};

typedef ACE_Singleton<CSenceManagerPtr, ACE_Null_Mutex> App_SenceManagerPtr;

#endif
