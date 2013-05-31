#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "SMPool.h"
#include "IOData.h"
#include "MapTemplate.h"

#include <string>
#include <map>

//��½�أ�ָ�����û�������Ҫ�޸ģ�DS��ϵ����ݼ��ϡ��ⲿ�����ݵ���Ա�󣬲������������㷨��
//��ȡ�أ�ָ�����û����ݲ���Ҫ�޸ģ�ֻ����ʱʹ�õ����ݡ��ⲿ��������Ա�󣬻����������㷨��

using namespace std;

#define COMMAND_PLAYINSERT 0x1010   //�û����ݴ���
#define COMMAND_PLAYUPDATE 0x1011   //�û����ݸ���
#define COMMAND_PLAYDELETE 0x1012   //�û�����ɾ��
#define COMMAND_PLAYSEACH  0x1013   //�û���ѯ
#define COMMAND_PLAYLOGIN  0x1014   //�û���½
#define COMMAND_PLAYLOGOFF 0x1015   //�û��뿪

#define MAX_READ_OBJECT    1000     //�����ڴ�������û����������
#define MAX_ONLINE_OBJECT  500      //�����ڴ�������û����ߵ��������

class CPlayerPoolCommand : public CClientCommand
{
public:
	CPlayerPoolCommand(void);
	~CPlayerPoolCommand(void);

	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
	void SetServerObject(CServerObject* pServerObject);

private:
	void Init();
	CPlayerData* Do_PlayerInsert(const char* pPlayerNick);
	bool         Do_PlayerUpdate(CPlayerData* pPlayerData);
	bool         Do_PlayerDelete(const char* pPlayerNick);
	CPlayerData* Do_PlayerSearch(const char* pPlayerNick);
	CPlayerData* Do_PlayerLogin(const char* pPlayerNick);
	bool         Do_PlayerLogOff(const char* pPlayerNick);


private:
	CServerObject*       m_pServerObject;
	int                  m_nCount;
	CSMPool<CPlayerData> m_UserPool;
	typedef CMapTemplate<string, CPlayerData> MapMRU;
	CIOData m_IOData;                    //IO���ݽӿ�
	MapMRU  m_MapReadData;               //��ǰ����Ķ�ȡ�û���
	MapMRU  m_MapOnlineData;             //��ǰ����������û���
};
