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

	int        m_nServerID;                        //服务器ID
	char       m_szServerName[MAX_BUFF_20];        //服务器名称
	char       m_szServerVersion[MAX_BUFF_20];     //服务器版本

	char       m_szPacketVersion[MAX_BUFF_20];     //数据解析包模块的版本号

	uint32     m_u4MsgHighMark;                    //消息的高水位阀值
	uint32     m_u4MsgLowMark;                     //消息的低水位阀值
	uint32     m_u4MsgThreadCount;                 //允许的工作线程个数
	uint32     m_u4MsgMaxQueue;                    //消息队列的最大个数

	char       m_szModulePath[MAX_BUFF_200];       //模块所在的路径
	char       m_szResourceName[MAX_BUFF_1024];    //资源模块名称

	int        m_nEncryptFlag;                     //0，加密方式关闭，1为加密方式开启
	char       m_szEncryptPass[MAX_BUFF_9];        //最长8位的加密密码，3DES算法
	int        m_nEncryptOutFlag;                  //回应数据包，0，为不加密，1为加密
	uint32     m_u4SendThresHold;                  //发送阀值(消息包的个数)
	uint32     m_u4RecvBuffSize;                   //接收数据缓冲池的大小
	uint16     m_u2SendQueueMax;                   //发送队列中最长的数据包个数
	uint16     m_u2ThreadTimuOut;                  //线程超时时间判定
	uint16     m_u2ThreadTimeCheck;                //线程自检时间
	uint16     m_u2PacketTimeOut;                  //处理数据超时时间
	uint16     m_u2SendAliveTime;                  //发送存活包的时间
	uint16     m_u2HandleCount;                    //handle对象池的个数
	uint16     m_u2MaxHanderCount;                 //最大同时处理Handler的数量
	uint16     m_u2MaxConnectTime;                 //最长等待连接链接时间（此时间内，如果接收和发送都没有发生，则由服务器关闭这个链接）

	uint16     m_u2ValidConnectCount;              //单位时间允许的最大链接次数
	uint8      m_u1Valid;                          //是否需要单位时间的链接阀值验证，0为允许，1为不允许
	uint32     m_u4ValidPacketCount;               //单位时间内允许的数据包数量
	uint32     m_u4ValidRecvSize;                  //单位时间允许的数据包发送数量
	uint16     m_u2ForbiddenTime;                  //链接封禁时间
	uint16     m_u2RecvQueueTimeout;               //接收队列处理超时时间限定
	uint16     m_u2SendQueueTimeout;               //发送队列处理超时时间限定
	uint16     m_u2SendQueueCount;                 //框架发送线程数
	uint8      m_u1CommandAccount;                 //是否需要统计命令出入服务器的信息，0是关闭，1是打开。打开后会生成相应的报表
	uint16     m_u2SendQueuePutTime;               //设置发送队列的入队超时时间
	uint16     m_u2WorkQueuePutTime;               //设置工作队列的入队超时时间

	uint8      m_u1ConsoleSupport;                 //是否支持Console服务，如果是1则是支持，0是不支持
	char       m_szConsoleIP[MAX_BUFF_100];        //Console服务器IP
	int        m_nConsolePort;                     //Console服务器的端口

	uint32     m_u4ReactorCount;                   //系统中遇到的反应器的个数
	uint32     m_u4ConnectServerTimerout;          //连接远程服务器间隔时间  
	uint16     m_u2ConnectServerCheck;             //服务器间连接单位检查时间

	typedef vector<_ServerInfo> vecServerInfo;
	vecServerInfo m_vecServerInfo;
	vecServerInfo m_vecUDPServerInfo;

	typedef vector<_ConsoleClientIP> vecConsoleClientIP;
	vecConsoleClientIP m_vecConsoleClientIP;                  //服务器后台允许的IP
	vecConsoleKey      m_vecConsoleKey;                       //服务器允许的key值
};

typedef ACE_Singleton<CMainConfig, ACE_Null_Mutex> App_MainConfig;

#endif
