// Base.cpp : һ��TCP�Ĳ�����
//����PurenessScopeServer�Ĳ��Ժ�ʹ��
//add by freeeyes
//2011-09-20

#include "BaseCommand.h"
#include "IObject.h"

static const char *g_szDesc      = "DKPoker-DataCenter";          //ģ�����������
static const char *g_szName      = "DKPoker-DataCenter";          //ģ�������
static const char *g_szModuleKey = "DKPoker-DataCenter";          //ģ���Key

#ifdef WIN32
#ifdef DK_ROOMDATAMODULE_BUILD_DLL
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#else
#define DECLDIR
#endif

extern "C"
{
	DECLDIR int LoadModuleData(CServerObject* pServerObject);
	DECLDIR int UnLoadModuleData();
	DECLDIR const char* GetDesc();
	DECLDIR const char* GetName();
	DECLDIR const char* GetModuleKey();
	DECLDIR int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
}

static CBaseCommand g_BaseCommand;
CServerObject*      g_pServerObject = NULL;

int LoadModuleData(CServerObject* pServerObject)
{
	g_pServerObject = pServerObject;
	OUR_DEBUG((LM_INFO, "[Base LoadModuleData] Begin.\n"));
	if(g_pServerObject != NULL)
	{
		g_BaseCommand.SetServerObject(pServerObject);	
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pServerObject is NULL.\n"));
	}

	IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();
	if(NULL != pMessageManager)
	{
		pMessageManager->DelClientCommand(CLIENT_LINK_CDISCONNET, &g_BaseCommand);
		pMessageManager->AddClientCommand(COMMAND_USERIN, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_USEROUT, &g_BaseCommand, g_szName);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
	}		

	OUR_DEBUG((LM_INFO, "[Base LoadModuleData] End.\n"));

	//����Command��ʼ��
	g_BaseCommand.Init();

	return 0;
}

int UnLoadModuleData()
{
	OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] Begin.\n"));
	if(g_pServerObject != NULL)
	{
		IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();
		if(NULL != pMessageManager)
		{
			pMessageManager->DelClientCommand(CLIENT_LINK_CDISCONNET, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_USERIN, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_USEROUT, &g_BaseCommand);
			pMessageManager = NULL;
		}
	}
	OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] End.\n"));
	return 0;
}

const char* GetDesc()
{
	return g_szDesc;
}

const char* GetName()
{
	return g_szName;
}

const char* GetModuleKey()
{
	return g_szModuleKey;
}

//����ģ���ĵ��ýӿ�
int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
	return 0;
}

