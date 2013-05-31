#ifndef _LUAFN_MESSION_H
#define _LUAFN_MESSION_H

#include "LuaDefine.h"
#include "MessagePool.h"
#include "Room.h"

#include "ISenceManager.h"

//�������
static int LuaFn_Print(lua_State* pState)
{
	char* pGlobal = (char* )lua_tostring(pState, -1);
	printf_s("[LuaFn_Mession_Print]Data=%s.\n", pGlobal);
	return 1;
}

//���뷿��(���)
static int LuaFn_Room_API_Join(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -4);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -3);
	char* pAPlayerName = (char*)lua_tostring(pState, -2);
	uint32 u4Money         = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->JoinRoom(u4RoomID, u4PlayerID, pAPlayerName, u4Money);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//���뷿��(�̶�λ��)
static int LuaFn_Room_API_JoinLocation(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -5);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -4);
	char* pAPlayerName = (char*)lua_tostring(pState, -3);
	uint32 u4Money         = (uint32)lua_tonumber(pState, -2);
	uint16 u2Index         = (uint16)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->JoinRoom(u4RoomID, u4PlayerID, pAPlayerName, u4Money, u2Index);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//�뿪����
static int LuaFn_Room_API_Exit(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->ExitRoom(u4RoomID, u4PlayerID);

	//�������������ϵĶ�ʱ��
	uint32 u4TimerID = App_RoomManager::instance()->GetRoomPlayerTimerID(u4RoomID, u4PlayerID);
	if(TIMER_ERROR_IN == u4TimerID)
	{
		lua_pushnumber(pState, (int)TIMER_ERROR_IN);
	}
	else
	{
		App_SenceManagerPtr::instance()->GetActiveTimer()->cancel(u4TimerID);
		App_RoomManager::instance()->SetRoomPlayerTimerID(u4RoomID, u4PlayerID, 0, TIMER_ERROR_IN, NULL);
		lua_pushnumber(pState, 0);
	}

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//��ʼ����
static int LuaFn_Room_API_Begin(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->BeginRoom(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//��������
static int LuaFn_Room_API_End(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->EndRoom(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//�õ�ָ��������������ǳ�
static int LuaFn_Room_API_GetRoomPlayerName(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4Index      = (uint32)lua_tonumber(pState, -1);

	char* pRet = App_RoomManager::instance()->GetRoomPlayerName(u4RoomID, u4Index);
	if(NULL != pRet)
	{
		lua_pushstring(pState, pRet);
	}
	else
	{
		lua_pushstring(pState, NULL);
	}

	return 1;
}

//������ҽ�Ǯ
static int LuaFn_Room_API_SetPlayerMoney(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -3);
	uint32 u4PlayerID   = (uint32)lua_tonumber(pState, -2);
	uint32 u4Money      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetPlayerMoney(u4RoomID, u4PlayerID, u4Money);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//���ָ����ҵ�TimerID
static int LuaFn_Room_API_GetPlayerTimerID(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4TimerID = App_RoomManager::instance()->GetRoomPlayerTimerID(u4RoomID, u4PlayerID);
	lua_pushnumber(pState, (int)u4TimerID);

	return 1;
}

//���ָ����ҵĽ�Ǯ
static int LuaFn_Room_API_GetPlayerMoney(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetPlayerMoney(u4RoomID, u4PlayerID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_SetPlayerCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -3);
	uint32 u4PlayerID   = (uint32)lua_tonumber(pState, -2);
	uint32 u4CardID     = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetPlayerCard(u4RoomID, u4PlayerID, u4CardID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//�õ�������ϵ�ָ������
static int LuaFn_Room_API_GetPlayerCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -3);
	uint32 u4PlayerID   = (uint32)lua_tonumber(pState, -2);
	uint32 u4Index      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetPlayerCard(u4RoomID, u4PlayerID, u4Index);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//ɾ���������ָ���Ŀ���
static int LuaFn_Room_API_DelPalyerCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -3);
	uint32 u4PlayerID   = (uint32)lua_tonumber(pState, -2);
	uint32 u4Index      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->DelPalyerCard(u4RoomID, u4PlayerID, u4Index);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//�õ���ǰ������ϵ���Ч��������
static int LuaFn_Room_API_GetPlayerCardCount(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetPlayerCardCount(u4RoomID, u4PlayerID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//���������ϵ�һ�ſ���
static int LuaFn_Room_API_SetRoomTableCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4CardID     = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomTableCard(u4RoomID, u4CardID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//�õ�������ָ��λ�õ�һ�ſ���
static int LuaFn_Room_API_GetRoomTableCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4Index      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomTableCard(u4RoomID, u4Index);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}


//ɾ��������ָ��λ�õ�һ�ſ���
static int LuaFn_Room_API_DelRoomTableCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4Index      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->DelRoomTableCard(u4RoomID, u4Index);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}


//�����ƿ��ϵ�һ�ſ���
static int LuaFn_Room_API_SetRoomBankCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4CardID     = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomBankCard(u4RoomID, u4CardID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//���ָ��λ�õ�һ���ƿ⿨��
static int LuaFn_Room_API_GetRoomBankCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4Index      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomBankCard(u4RoomID, u4Index);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//ɾ���ƿ���ָ��λ�õ�һ�ſ���
static int LuaFn_Room_API_DelRoomBankCard(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint32 u4Index      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->DelRoomBankCard(u4RoomID, u4Index);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

//���Player��ʱ��
static int LuaFn_PlayerTimer_API_Add(lua_State* pState)
{
	uint32 u4Seconds  = (uint32)lua_tonumber(pState, -4);
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -3);
	uint32 u4PlayerID = (uint32)lua_tonumber(pState, -2);
	uint32 u4EventID  = (uint32)lua_tonumber(pState, -1);

	_TimerInfo* pTimerInfo = new _TimerInfo();

	pTimerInfo->m_u4CommandID = COMMAND_TIMERPLAYER;
	pTimerInfo->m_u4RoomID    = u4RoomID;
	pTimerInfo->m_u4PlayerID  = u4PlayerID;
	pTimerInfo->m_u4Event     = u4EventID;

	uint32 u4TimerID = App_SenceManagerPtr::instance()->GetActiveTimer()->schedule(App_SenceManagerPtr::instance()->GetSenceManagerPtr(), (void* )pTimerInfo, ACE_OS::gettimeofday() + ACE_Time_Value(u4Seconds));
	if(0 == u4TimerID)
	{
		ACE_DEBUG((LM_ERROR, "CMessageService::StartTimer()--> Start thread time error.\n"));
		lua_pushnumber(pState, (int)TIMER_ERROR_IN);
		return 1;
	}
	else
	{
		App_RoomManager::instance()->SetRoomPlayerTimerID(u4RoomID, u4PlayerID, (uint16)u4Seconds, u4TimerID, pTimerInfo);
		lua_pushnumber(pState, (int)u4TimerID);
	}

	return 1;
}

//����Player��ʱ��
static int LuaFn_PlayerTimer_API_Del(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID = (uint32)lua_tonumber(pState, -1);

	_TimerInfo* pTimerInfo = new _TimerInfo();

	pTimerInfo->m_u4RoomID   = u4RoomID;
	pTimerInfo->m_u4PlayerID = u4PlayerID;

	uint32 u4TimerID = App_RoomManager::instance()->GetRoomPlayerTimerID(u4RoomID, u4PlayerID);
	if(TIMER_ERROR_IN == u4TimerID)
	{
		lua_pushnumber(pState, (int)TIMER_ERROR_IN);
	}
	else
	{
		App_SenceManagerPtr::instance()->GetActiveTimer()->cancel(u4TimerID);

		//�����������
		_TimerInfo* pTimerInfo = App_RoomManager::instance()->GetRoomPlayerTimerInfo(u4RoomID, u4PlayerID);
		SAFE_DELETE(pTimerInfo);

		App_RoomManager::instance()->SetRoomPlayerTimerID(u4RoomID, u4PlayerID, 0, TIMER_ERROR_IN, NULL);
		lua_pushnumber(pState, 0);
	}
	
	return 1;
}

//���Room��ʱ��
static int LuaFn_RoomTimer_API_Add(lua_State* pState)
{
	uint32 u4Seconds  = (uint32)lua_tonumber(pState, -4);
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -3);
	uint32 u4PlayerID = (uint32)lua_tonumber(pState, -2);
	uint32 u4EventID  = (uint32)lua_tonumber(pState, -1);

	_TimerInfo* pTimerInfo = new _TimerInfo();

	pTimerInfo->m_u4CommandID = COMMAND_TIMERROOM;
	pTimerInfo->m_u4RoomID    = u4RoomID;
	pTimerInfo->m_u4PlayerID  = u4PlayerID;
	pTimerInfo->m_u4Event     = u4EventID;

	uint32 u4TimerID = App_SenceManagerPtr::instance()->GetActiveTimer()->schedule(App_SenceManagerPtr::instance()->GetSenceManagerPtr(), (void* )pTimerInfo, ACE_OS::gettimeofday() + ACE_Time_Value(u4Seconds));
	if(0 == u4TimerID)
	{
		ACE_DEBUG((LM_ERROR, "CMessageService::StartTimer()--> Start thread time error.\n"));
		lua_pushnumber(pState, (int)TIMER_ERROR_IN);
		return 1;
	}
	else
	{
		App_RoomManager::instance()->SetRoomTimerID(u4RoomID, u4TimerID, (uint16)u4Seconds, pTimerInfo);
		lua_pushnumber(pState, (int)u4TimerID);
	}

	return 1;
}

//�õ������Ƿ��Ѿ����ڶ�ʱ��
static int LuaFn_RoomTimer_API_Check(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4TimerID = App_RoomManager::instance()->GetRoomTimerID(u4RoomID);
	if(TIMER_ERROR_IN == u4TimerID)
	{
		lua_pushboolean(pState, false);
	}
	else
	{
		lua_pushboolean(pState, true);
	}

	return 1;
}

//ɾ��Room��ʱ��
static int LuaFn_RoomTimer_API_Del(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID = (uint32)lua_tonumber(pState, -1);

	_TimerInfo* pTimerInfo = new _TimerInfo();

	pTimerInfo->m_u4RoomID   = u4RoomID;
	pTimerInfo->m_u4PlayerID = u4PlayerID;

	uint32 u4TimerID = App_RoomManager::instance()->GetRoomTimerID(u4RoomID);
	if(TIMER_ERROR_IN == u4TimerID)
	{
		lua_pushnumber(pState, (int)TIMER_ERROR_IN);
	}
	else
	{
		App_SenceManagerPtr::instance()->GetActiveTimer()->cancel(u4TimerID);

		//�����������
		_TimerInfo* pTimerInfo = App_RoomManager::instance()->GetRoomTimerInfo(u4RoomID);
		SAFE_DELETE(pTimerInfo);

		App_RoomManager::instance()->SetRoomTimerID(u4RoomID, 0, TIMER_ERROR_IN, NULL);
		lua_pushnumber(pState, 0);
	}

	return 1;
}

//���÷�������
static int LuaFn_RoomName_API_Set(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -2);
	char*  pRoomName  = (char* )lua_tostring(pState, -1);

	uint32 u4Return = App_RoomManager::instance()->SetRoomName(u4RoomID, pRoomName);
	lua_pushnumber(pState, (int)u4Return);

	return 1;
}

//��÷�������
static int LuaFn_RoomName_API_Get(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	const char* pRoomName = App_RoomManager::instance()->GetRoomName(u4RoomID);
	lua_pushstring(pState, pRoomName);

	return 1;
}

//���÷���������Ϣ
static int LuaFn_RoomDesc_API_Set(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -2);
	char*  pRoomName  = (char* )lua_tostring(pState, -1);

	uint32 u4Return = App_RoomManager::instance()->SetRoomDesc(u4RoomID, pRoomName);
	lua_pushnumber(pState, (int)u4Return);

	return 1;
}

//��÷���������Ϣ
static int LuaFn_RoomDesc_API_Get(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	const char* pRoomName = App_RoomManager::instance()->GetRoomDesc(u4RoomID);
	lua_pushstring(pState, pRoomName);

	return 1;
}

//���ָ������ڷ����е�ָ��λ��
static int LuaFn_RoomPlayerLocation_API_Get(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID = (uint32)lua_tonumber(pState, -1);

	uint32 u4Location = App_RoomManager::instance()->GetRoomPlayerLocation(u4RoomID, u4PlayerID);
	lua_pushnumber(pState, (int)u4Location);

	return 1;
}

//************************************
//���÷�����Ϸ��Ϣ���API
//************************************
static int LuaFn_Room_GameInfo_API_GetPostion(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomGame_Postion(u4RoomID);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_SetPostion(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -2);
	uint16 u4Postion  = (uint16)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomGame_Postion(u4RoomID, u4Postion);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_GetGameState(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomGame_GameState(u4RoomID);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_SetGameState(lua_State* pState)
{
	uint32 u4RoomID    = (uint32)lua_tonumber(pState, -2);
	uint16 u2GameState = (uint16)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomGame_GameState(u4RoomID, u2GameState);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_GetCurrentSmallBlind(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomGame_CurrentSmallBlind(u4RoomID);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_SetCurrentSmallBlind(lua_State* pState)
{
	uint32 u4RoomID            = (uint32)lua_tonumber(pState, -2);
	uint16 u2CurrentSmallBlind = (uint16)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomGame_CurrentSmallBlind(u4RoomID, u2CurrentSmallBlind);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_GetRoundCount(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomGame_RoundCount(u4RoomID);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_SetRoundCount(lua_State* pState)
{
	uint32 u4RoomID     = (uint32)lua_tonumber(pState, -2);
	uint16 u2RoundCount = (uint16)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomGame_RoundCount(u4RoomID, u2RoundCount);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_GetTempBet(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomGame_TempBet(u4RoomID);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_SetTempBet(lua_State* pState)
{
	uint32 u4RoomID  = (uint32)lua_tonumber(pState, -2);
	uint32 u4TempBet = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomGame_TempBet(u4RoomID, u4TempBet);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_GetMoney(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomGame_Money(u4RoomID);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_SetMoney(lua_State* pState)
{
	uint32 u4RoomID  = (uint32)lua_tonumber(pState, -2);
	uint32 u4Money   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomGame_Money(u4RoomID, u4Money);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_GetCurrPlayerID(lua_State* pState)
{
	uint32 u4RoomID   = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomGame_CurrPlayerID(u4RoomID);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_GameInfo_API_SetCurrPlayerID(lua_State* pState)
{
	uint32 u4RoomID       = (uint32)lua_tonumber(pState, -2);
	uint32 u4CurrPlayerID = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomGame_CurrPlayerID(u4RoomID, u4CurrPlayerID);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_PlayerSit(lua_State* pState)
{
	uint32 u4RoomID       = (uint32)lua_tonumber(pState, -3);
	uint32 u4CurrPlayerID = (uint32)lua_tonumber(pState, -2);
	int16  nLocation      = (int16)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomPlayerSitDown(u4RoomID, u4CurrPlayerID, nLocation);

	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_EnterRoom(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -4);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -3);
	char* pAPlayerName = (char*)lua_tostring(pState, -2);
	uint32 u4Money         = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->EnterRoom(u4RoomID, u4PlayerID, pAPlayerName, u4Money);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_SetRoomUpdateTime(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomUpdateTime(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_CheckRoomUpdateTime(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint32 u4MillSeconds   = (uint32)lua_tonumber(pState, -1);

	bool blRet = App_RoomManager::instance()->CheckRoomUpdateTime(u4RoomID, u4MillSeconds);
	lua_pushboolean(pState, blRet);

	return 1;
}

static int LuaFn_Room_API_SetRoomPlayerUpdateTime(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomPlayerUpdateTime(u4RoomID, u4PlayerID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_CheckRoomPlayerUpdateTime(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -3);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -2);
	uint32 u4MillSeconds   = (uint32)lua_tonumber(pState, -1);

	bool blRet = App_RoomManager::instance()->CheckRoomPlayerUpdateTime(u4RoomID, u4PlayerID, u4MillSeconds);
	lua_pushboolean(pState, blRet);

	return 1;
}

static int LuaFn_Room_API_GetPlayerCount(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomPlayerCount(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_GetVisitorPlayerCount(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomVisitorCount(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_GetRoomState(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomState(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_SetRoomState(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint32 u4State         = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomState(u4RoomID, u4State);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_DealCode(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -4);
	uint32 u4BottomMoney   = (uint32)lua_tonumber(pState, -3);
	int32  n4PlayerID      = (int32)lua_tonumber(pState, -2);
	int32  n4SmallBlind    = (int32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->DealCode(u4RoomID, u4BottomMoney, n4PlayerID, n4SmallBlind);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_GetRoomNextPlayerID(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomNextPlayer(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_GetRoomCurrPlayerID(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetCurrPlayerID(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_SetPlayerBet(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -3);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -2);
	uint32 u4Money         = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetBet(u4RoomID, u4PlayerID, u4Money);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_SetBenginePlayerID(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->SetRoomGame_BenginePlayerID(u4RoomID, u4PlayerID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_GetBenginePlayerID(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -1);

	uint32 u4Ret = App_RoomManager::instance()->GetRoomGame_BenginePlayerID(u4RoomID);
	lua_pushnumber(pState, (int)u4Ret);

	return 1;
}

static int LuaFn_Room_API_SendRoomState(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -1);

	bool blState = App_SenceManagerPtr::instance()->GetSenceManagerPtr()->SendRoomState(u4RoomID, u4PlayerID);
	lua_pushboolean(pState, blState);

	return 1;
}

static int LuaFn_Room_API_SendRoomSitdown(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -1);

	bool blState = App_SenceManagerPtr::instance()->GetSenceManagerPtr()->SendRoomState(u4RoomID, u4PlayerID);
	lua_pushboolean(pState, blState);

	return 1;
}

static int LuaFn_Room_API_SendRoomTimer(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -1);

	bool blState = App_SenceManagerPtr::instance()->GetSenceManagerPtr()->SendRoomTimer(u4RoomID, u4PlayerID);
	lua_pushboolean(pState, blState);

	return 1;
}

static int LuaFn_Room_API_SendRoomPlayerTimer(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint32 u4PlayerID      = (uint32)lua_tonumber(pState, -1);

	bool blState = App_SenceManagerPtr::instance()->GetSenceManagerPtr()->SendRoomPlayerTimer(u4RoomID, u4PlayerID);
	lua_pushboolean(pState, blState);

	return 1;
}

static int LuaFn_Room_API_SendRoomMoney(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -1);

	bool blState = App_SenceManagerPtr::instance()->GetSenceManagerPtr()->SendRoomMoney(u4RoomID);
	lua_pushboolean(pState, blState);

	return 1;
}

static int LuaFn_Room_API_SendRoomTableCode(lua_State* pState)
{
	uint32 u4RoomID        = (uint32)lua_tonumber(pState, -2);
	uint8  u1CardCount     = (uint8)lua_tonumber(pState, -1);

	bool blState = App_SenceManagerPtr::instance()->GetSenceManagerPtr()->SendRoomTableCode(u4RoomID, u1CardCount);
	lua_pushboolean(pState, blState);

	return 1;
}

#endif
