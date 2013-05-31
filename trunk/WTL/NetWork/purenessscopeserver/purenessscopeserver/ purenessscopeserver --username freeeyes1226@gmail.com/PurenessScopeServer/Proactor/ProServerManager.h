#ifndef _PROSERVERMANAGER_H
#define _PROSERVERMANAGER_H

#include "define.h"
#include "MainConfig.h"
#include "ForbiddenIP.h"
#include "ProConnectAccept.h"
#include "AceProactorManager.h"
#include "MessageService.h"
#include "LoadModule.h"
#include "LogManager.h"
#include "FileLogger.h"
#include "IObject.h"
#include "ClientProConnectManager.h"
#include "ProUDPManager.h"
#include "ModuleMessageManager.h"

//��ӶԷ��������Ƶ�֧�֣�Consoleģ������֧�������Է������Ŀ���
//add by freeeyes

class CProServerManager
{
public:
	CProServerManager(void);
	~CProServerManager(void);

	bool Init();
	bool Start();
	bool Close();

private:
	CProConnectAcceptManager   m_ConnectAcceptorManager;         //���ڹ����ͻ�������
	CProConsoleConnectAcceptor m_ProConsoleConnectAcceptor;      //���ڹ�������������
};

typedef ACE_Singleton<CProServerManager, ACE_Null_Mutex> App_ProServerManager;
#endif
