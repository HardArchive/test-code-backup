// Define.h
// ���ﶨ��Server����Ҫ�ӿڣ����������˿ڵȵȣ�
// ��Ȼ�Եã���ʹʲô��������Ҳ��һ�����ˡ�
// add by freeeyes
// 2008-12-23


#ifndef _SERVERMANAGER_H
#define _SERVERMANAGER_H

#include "define.h"
#include "MainConfig.h"
#include "ForbiddenIP.h"
#include "ConnectAccept.h"
#include "AceReactorManager.h"
#include "MessageService.h"
#include "SigHandle.h"
#include "LoadModule.h"
#include "LogManager.h"
#include "FileLogger.h"
#include "IObject.h"
#include "BuffPacketManager.h"
#include "ClientReConnectManager.h"
#include "ReUDPManager.h"
#include "CommandAccount.h"
#include "ModuleMessageManager.h"

class CServerManager
{
public:
	CServerManager(void);
	~CServerManager(void);

	bool Init();
	bool Start();
	bool Close();

private:
	CConnectAcceptorManager m_ConnectAcceptorManager;    //��ͨ�ͻ�������
	ConnectConsoleAcceptor  m_ConnectConsoleAcceptor;    //��̨��������
	CFileLogger*            m_pFileLogger;               //��־����
};

typedef ACE_Singleton<CServerManager, ACE_Null_Mutex> App_ServerManager;

#endif
