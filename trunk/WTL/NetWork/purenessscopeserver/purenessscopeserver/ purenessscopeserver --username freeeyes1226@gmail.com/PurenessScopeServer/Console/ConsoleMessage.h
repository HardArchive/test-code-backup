#ifndef _CONSOLEMESSAGE_H
#define _CONSOLEMESSAGE_H

#include <ace/OS_NS_sys_resource.h>

#include "define.h"
#include "IBuffPacket.h"
#include "LoadModule.h"
#include "MessageManager.h"
#include "MessageService.h"
#include "MakePacket.h"
#include "ForbiddenIP.h"
#include "ace/Message_Block.h"
#include "IPAccount.h"

#ifdef WIN32
#include "ProConnectHandle.h"
#include "ClientProConnectManager.h"
#include "ProUDPManager.h"
#include "WindowsCPU.h"
#else
#include "ConnectHandler.h"
#include "ClientReConnectManager.h"
#include "ReUDPManager.h"
#include "LinuxCPU.h"
#endif

//�������ֵ���Ͷ���
enum
{
	CONSOLE_MESSAGE_SUCCESS = 0,
	CONSOLE_MESSAGE_FAIL    = -1,
};

//Ŀǰ֧�ֵ�����
#define CONSOLEMESSAHE_LOADMOUDLE         "LoadModule"          //����ģ��
#define CONSOLEMESSAHE_UNLOADMOUDLE       "UnLoadModule"        //ж��ģ��
#define CONSOLEMESSAHE_RELOADMOUDLE       "ReLoadModule"        //���¼���ģ��
#define CONSOLEMESSAHE_SHOWMOUDLE         "ShowModule"          //��ʾ���������Ѿ����ص�ģ��
#define CONSOLEMESSAHE_CLIENTCOUNT        "ClientCount"         //��ǰ�ͻ���������
#define CONSOLEMESSAHE_COMMANDINFO        "CommandInfo"         //��ǰĳһ�������״̬��Ϣ
#define CONSOLEMESSAHE_COMMANDTIMEOUT     "CommandTimeout"      //���г�ʱ�������б�
#define CONSOLEMESSAHE_COMMANDTIMEOUTCLR  "CommandTimeoutclr"   //�����ʱ�������б�
#define CONSOLEMESSAGE_COMMANDDATALOG     "CommandDataLog"      //�洢CommandDataLog
#define CONSOLEMESSAHE_THREADINFO         "WorkThreadState"     //��ǰ�����̺߳͹����߳�״̬
#define CONSOLEMESSAHE_CLIENTINFO         "ConnectClient"       //��ǰ�ͻ������ӵ���Ϣ
#define CONSOLEMESSAHE_UDPCONNECTINFO     "UDPConnectClient"    //��ǰUDP�ͻ��˵�������Ϣ
#define CONSOLEMESSAHE_COLSECLIENT        "CloseClient"         //�رտͻ���
#define CONSOLEMESSAHE_FORBIDDENIP        "ForbiddenIP"         //��ֹIP����
#define CONSOLEMESSAHE_FORBIDDENIPSHOW    "ShowForbiddenIP"     //�鿴��ֹ����IP�б�
#define CONSOLEMESSAHE_LIFTED             "LiftedIP"            //���ĳIP
#define CONSOLEMESSAHE_SERVERCONNECT_TCP  "ServerConnectTCP"    //��������ͨѶ(TCP)
#define CONSOLEMESSAHE_SERVERCONNECT_UDP  "ServerConnectUDP"    //��������ͨѶ(UDP)
#define CONSOLEMESSAGE_PROCESSINFO        "ShowCurrProcessInfo" //�鿴��ǰ������������״̬
#define CONSOLEMESSAGE_CLIENTHISTORY      "ShowConnectHistory"  //�鿴��������ʷ����״̬
#define CONSOLEMESSAGE_ALLCOMMANDINFO     "ShowAllCommand"      //�鿴����������ע��ģ��������Ϣ
#define CONSOLEMESSAGE_SERVERINFO         "ShowServerInfo"      //�鿴������������Ϣ
#define CONSOLEMESSAGE_SERVERRECONNECT    "ReConnectServer"     //Զ�˿�������ĳһ��Զ�˷�����

//��������
struct _CommandInfo
{
	char m_szCommandTitle[MAX_BUFF_100];  //��������ͷ
	char m_szCommandExp[MAX_BUFF_100];    //����������չ����

	_CommandInfo()
	{
		m_szCommandTitle[0] = '\0';
		m_szCommandExp[0]   = '\0';
	}
};

//�ļ����ṹ
struct _FileInfo
{
	char m_szFilePath[MAX_BUFF_100];
	char m_szFileName[MAX_BUFF_100];

	_FileInfo()
	{
		m_szFilePath[0] = '\0';
		m_szFileName[0] = '\0';
	}
};

class CConsoleMessage
{
public:
	CConsoleMessage();
	~CConsoleMessage();

	int Dispose(ACE_Message_Block* pmb, IBuffPacket* pBuffPacket);             //Ҫ����������ֽ���, pBuffPacketΪ����Ҫ���͸��ͻ��˵�����

	//��ʼ������
	bool SetConsoleKey(vecConsoleKey* pvecConsoleKey);       //�����֤�����keyֵ

	//�������ݲ���
private:
	int ParseCommand(const char* pCommand, IBuffPacket* pBuffPacket);          //ִ������
	bool GetCommandInfo(const char* pCommand, _CommandInfo& CommandInfo);      //�������и��Ӧ���е����ݸ�ʽ
	bool GetFileInfo(const char* pFile, _FileInfo& FileInfo);                  //��һ��ȫ·���зֳ��ļ���
	bool GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);      //�õ���ֹ��IP�б�
	bool GetConnectServerID(const char* pCommand, int& nServerID);             //�õ�һ��ָ���ķ�����ID
	bool CheckConsoleKey(const char* pKey);                                    //��֤key

	//�������ʵ�ֲ���
private:
	bool DoMessage_LoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_UnLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ReLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ClientMessageCount(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_CommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_WorkThreadState(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessgae_CloseClient(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ForbiddenIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowForbiddenList(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_LifedIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_UDPClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ServerConnectTCP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ServerConnectUDP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowProcessInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowClientHisTory(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowAllCommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowServerInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ReConnectServer(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_CommandTimeout(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_CommandTimeoutclr(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_CommandDataLog(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);

private:
	vecConsoleKey* m_pvecConsoleKey;
};

#endif
