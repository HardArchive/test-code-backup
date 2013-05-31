#ifndef _SENCEMANAGER_H
#define _SENCEMANAGER_H

#include "MessagePool.h"
#include "ISenceManager.h"

#include "LuaFn.h"
#include "SMDataManager.h"
#include "ISendCommand.h"

//�������г�����һ�鳡��һ���߳�
//create by freeeyes

//Ĭ�ϵĲ�����Ϣ
#define WATER_MASK    64000    //ˮλ���
#define TIMER_CHECK   5        //��ʱ��ʱ����
#define QUEUE_COUNT   10000    //�����е�����������
#define BUFFER_COUNT  10000    //Buff����ص�������
#define LUA_ROOM_PATH "./luascript/RoomLogic.lua"

typedef map<uint32, _UserPlayer*> mapOnSencePlayer;   //�ڵ�ǰ�������û��б�

class CSenceManager : public ISenceManager
{
public:
	CSenceManager(void);
	virtual ~CSenceManager(void);

	virtual int open(void* args = 0);
	virtual int svc (void);

	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

	void Init(ActiveTimer* pActiveTimer, ISendCommand* pSendCommand);
	int  Close();
	bool KillTimer();
	bool PutMessage(uint32 u4CommandID, _CommandInfo& objCommandInfo);

	bool AddSencePlayer(uint32 u4PlayerID, uint32 u4Pos);
	bool DelSencePlayer(uint32 u4PlayerID);

	ActiveTimer* GetActiveTimer();

	bool StartTimer();

public:
	bool SendRoomState(uint32 u4RoomID, uint32 u4PlayerID);
	bool SendRoomSitdown(uint32 u4RoomID, uint32 u4PlayerID);
	bool SendRoomTimer(uint32 u4RoomID, uint32 u4PlayerID);
	bool SendRoomPlayerTimer(uint32 u4RoomID, uint32 u4PlayerID);
	bool SendRoomMoney(uint32 u4RoomID);
	bool SendRoomTableCode(uint32 u4RoomID, uint8 u1CodseCount);

private:
	bool ProcessMessage(_QueueMessage* pQueueMessage);
	bool IsRun();

private:
	ACE_RW_Thread_Mutex            m_rwMutex;
	bool                           m_blRun;
	int                            m_nTimerID;
	uint32                         m_u4ProCount;         //���������
	uint32                         m_u4TimeCost;         //�������ʱ��������ʱ��
	CMessagePool                   m_objMessagePool;
	CMessageBlockPool              m_objMessageBlockPool;
	CLuaFn                         m_objLuaFn;
	CSMDataManager                 m_objSMDataManager;
	ActiveTimer*                   m_pActiveTimer;       //��ʱ�����̳п�ܵĶ�ʱ��
	mapOnSencePlayer               m_mapOnSencePlayer;
	ISendCommand*                  m_pSendCommand;       //��������ָ��   
};

typedef ACE_Singleton<CSenceManager, ACE_Null_Mutex> App_SenceManager;

#endif
