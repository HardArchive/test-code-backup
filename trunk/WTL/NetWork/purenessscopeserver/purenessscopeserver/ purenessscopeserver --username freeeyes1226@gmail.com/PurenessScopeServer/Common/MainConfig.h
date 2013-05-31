#ifndef _MAINCONFIG_H
#define _MAINCONFIG_H

#include "define.h"
#include "XmlOpeation.h"

#include "ace/Singleton.h"
#include <vector>

#include "PacketParse.h"

struct _ServerInfo
{
	char m_szServerIP[MAX_BUFF_20];
	int  m_nPort;

	_ServerInfo()
	{
		m_szServerIP[0] = '\0';
		m_nPort         = 0;
	}
};

struct _ConsoleKey
{
	char m_szKey[MAX_BUFF_100];

	_ConsoleKey()
	{
		m_szKey[0] = '\0';
	}
};

typedef vector<_ConsoleKey> vecConsoleKey;

struct _ConsoleClientIP
{
	char m_szServerIP[MAX_BUFF_20];

	_ConsoleClientIP()
	{
		m_szServerIP[0] =  '\0';
	}
};

using namespace std;

class CMainConfig
{
public:
	CMainConfig(void);
	~CMainConfig(void);

	bool Init(const char* szConfigPath);
	void Display();
	const char* GetError();

	const char* GetServerName();
	const char* GetServerVersion();
	const char* GetPacketVersion();
	uint16 GetServerID();
	uint16 GetServerPortCount();
	_ServerInfo* GetServerPort(int nIndex);

	uint32 GetMgsHighMark();
	uint32 GetMsgLowMark();
	uint32 GetThreadCount();
	uint32 GetMsgMaxQueue();
	uint16 GetHandleCount();

	const char* GetModulePath();
	const char* GetModuleString();

	int GetEncryptFlag();
	const char* GetEncryptPass();
	int GetEncryptOutFlag();

	uint32 GetSendThresHold();

	uint32 GetRecvBuffSize();
	uint16 GetSendQueueMax();
	uint16 GetThreadTimuOut();
	uint16 GetThreadTimeCheck();
	uint16 GetPacketTimeOut();
	uint16 GetSendAliveTime();
	uint16 GetMaxHandlerCount();
	uint16 GetMaxConnectTime();
	uint8  GetConsoleSupport();
	int    GetConsolePort();
	const char* GetConsoleIP();
	vecConsoleKey* GetConsoleKey();
	uint16 GetRecvQueueTimeout();
	uint16 GetSendQueueTimeout();
	uint16 GetSendQueueCount();

	bool CompareConsoleClinetIP(const char* pConsoleClientIP);

	uint16 GetUDPServerPortCount();
	_ServerInfo* GetUDPServerPort(int nIndex);

	uint32 GetReactorCount();

	uint16 GetValidConnectCount();
	uint8  GetValid();
	uint32 GetValidPacketCount();
	uint32 GetValidRecvSize();
	uint16 GetForbiddenTime();
	uint8 GetCommandAccount();
	uint32 GetConnectServerTimeout();
	uint16 GetConnectServerCheck();
	uint16 GetSendQueuePutTime();
	uint16 GetWorkQueuePutTime();

private:
	CXmlOpeation m_MainConfig;
	char       m_szError[MAX_BUFF_500];

	int        m_nServerID;                        //������ID
	char       m_szServerName[MAX_BUFF_20];        //����������
	char       m_szServerVersion[MAX_BUFF_20];     //�������汾

	char       m_szPacketVersion[MAX_BUFF_20];     //���ݽ�����ģ��İ汾��

	uint32     m_u4MsgHighMark;                    //��Ϣ�ĸ�ˮλ��ֵ
	uint32     m_u4MsgLowMark;                     //��Ϣ�ĵ�ˮλ��ֵ
	uint32     m_u4MsgThreadCount;                 //����Ĺ����̸߳���
	uint32     m_u4MsgMaxQueue;                    //��Ϣ���е�������

	char       m_szModulePath[MAX_BUFF_200];       //ģ�����ڵ�·��
	char       m_szResourceName[MAX_BUFF_1024];    //��Դģ������

	int        m_nEncryptFlag;                     //0�����ܷ�ʽ�رգ�1Ϊ���ܷ�ʽ����
	char       m_szEncryptPass[MAX_BUFF_9];        //�8λ�ļ������룬3DES�㷨
	int        m_nEncryptOutFlag;                  //��Ӧ���ݰ���0��Ϊ�����ܣ�1Ϊ����
	uint32     m_u4SendThresHold;                  //���ͷ�ֵ(��Ϣ���ĸ���)
	uint32     m_u4RecvBuffSize;                   //�������ݻ���صĴ�С
	uint16     m_u2SendQueueMax;                   //���Ͷ�����������ݰ�����
	uint16     m_u2ThreadTimuOut;                  //�̳߳�ʱʱ���ж�
	uint16     m_u2ThreadTimeCheck;                //�߳��Լ�ʱ��
	uint16     m_u2PacketTimeOut;                  //�������ݳ�ʱʱ��
	uint16     m_u2SendAliveTime;                  //���ʹ�����ʱ��
	uint16     m_u2HandleCount;                    //handle����صĸ���
	uint16     m_u2MaxHanderCount;                 //���ͬʱ����Handler������
	uint16     m_u2MaxConnectTime;                 //��ȴ���������ʱ�䣨��ʱ���ڣ�������պͷ��Ͷ�û�з��������ɷ������ر�������ӣ�

	uint16     m_u2ValidConnectCount;              //��λʱ�������������Ӵ���
	uint8      m_u1Valid;                          //�Ƿ���Ҫ��λʱ������ӷ�ֵ��֤��0Ϊ����1Ϊ������
	uint32     m_u4ValidPacketCount;               //��λʱ������������ݰ�����
	uint32     m_u4ValidRecvSize;                  //��λʱ����������ݰ���������
	uint16     m_u2ForbiddenTime;                  //���ӷ��ʱ��
	uint16     m_u2RecvQueueTimeout;               //���ն��д���ʱʱ���޶�
	uint16     m_u2SendQueueTimeout;               //���Ͷ��д���ʱʱ���޶�
	uint16     m_u2SendQueueCount;                 //��ܷ����߳���
	uint8      m_u1CommandAccount;                 //�Ƿ���Ҫͳ������������������Ϣ��0�ǹرգ�1�Ǵ򿪡��򿪺��������Ӧ�ı���
	uint16     m_u2SendQueuePutTime;               //���÷��Ͷ��е���ӳ�ʱʱ��
	uint16     m_u2WorkQueuePutTime;               //���ù������е���ӳ�ʱʱ��

	uint8      m_u1ConsoleSupport;                 //�Ƿ�֧��Console���������1����֧�֣�0�ǲ�֧��
	char       m_szConsoleIP[MAX_BUFF_100];        //Console������IP
	int        m_nConsolePort;                     //Console�������Ķ˿�

	uint32     m_u4ReactorCount;                   //ϵͳ�������ķ�Ӧ���ĸ���
	uint32     m_u4ConnectServerTimerout;          //����Զ�̷��������ʱ��  
	uint16     m_u2ConnectServerCheck;             //�����������ӵ�λ���ʱ��

	typedef vector<_ServerInfo> vecServerInfo;
	vecServerInfo m_vecServerInfo;
	vecServerInfo m_vecUDPServerInfo;

	typedef vector<_ConsoleClientIP> vecConsoleClientIP;
	vecConsoleClientIP m_vecConsoleClientIP;                  //��������̨�����IP
	vecConsoleKey      m_vecConsoleKey;                       //�����������keyֵ
};

typedef ACE_Singleton<CMainConfig, ACE_Null_Mutex> App_MainConfig;

#endif
