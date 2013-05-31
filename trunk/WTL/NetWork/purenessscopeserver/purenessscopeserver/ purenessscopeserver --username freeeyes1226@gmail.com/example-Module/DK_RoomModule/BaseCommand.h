#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "SenceManager.h"

#include <string>

//����ͻ�������(TCP)
#define COMMAND_MODULE_LOGIN            0x3000    //ģ���ͨѶ����¼
#define COMMAND_MODULE_LOGOUT           0x3001    //ģ���ͨѶ���ǳ�

typedef map<uint32, uint32> mapConnectPlayer;  //������Һ�ConnectID�İ󶨹�ϵ

using namespace std;

class CBaseCommand : public CClientCommand, public ISendCommand
{
public:
	CBaseCommand(void);
	~CBaseCommand(void);

	bool Init(ActiveTimer* pActiveTimer);             //���Ӽ��س�ʼ������

	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);

	void SetServerObject(CServerObject* pServerObject);

	bool Module_Login(uint32 u4Pos, uint32 u4PlayerID, uint32 u4ConnectID);
	bool Module_Logout(uint32 u4PlayerID, uint32 u4ConnectID);

private:
	bool SendRoomState(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomInfo& objSendRoomInfo);
	bool SendRoomSitdown(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomPlayerInfo& objSendRoomPlayerInfo);
	bool SendRoomTimer(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomTimer& objSendRoomTimer);
	bool SendRoomPlayerTimer(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomPlayerTimer objSendRoomPlayerTimer);
	bool SendRoomPlayerMoney(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomMoney objSendRoomMoney);
	bool SendRoomTableCard(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomCodeInfo objSendRoomCodeInfo);

private:
	CServerObject*      m_pServerObject;
	CSenceManager       m_objSenceManager;
	mapConnectPlayer    m_mapOnlineUserPlayer;
};
