#ifndef _DEFINE_H
#define _DEFINE_H

// Define.h
// 这里定义所有服务器用到的通用的东西，在这里加载一些ace的头和通用的一些数据方法
// add by freeeyes
// 2008-12-22

#include "ace/SString.h"
#include "ace/Malloc.h"
#include "ace/Malloc_T.h"
#include "ace/Task_T.h"
#include "ace/Local_Memory_Pool.h"
#include "ace/Time_Value.h"
#include "ace/OS_main.h"

#ifndef NULL
#define NULL 0
#endif

#define MAINCONFIG            "main.conf"
#define SERVER_CONNECT_FILE   "ConnectServer.conf"

#define MAX_BUFF_9    9
#define MAX_BUFF_20   20
#define MAX_BUFF_100  100
#define MAX_BUFF_200  200
#define MAX_BUFF_500  500
#define MAX_BUFF_1000 1000
#define MAX_BUFF_1024 1024

//根据不同的操作系统，定义不同的recv接收参数类型
#ifdef WIN32
	#define MSG_NOSIGNAL          0            //信号量参数（WINDOWS）
#endif

#define SERVER_ACTOR_REACTOR  0
#define SERVER_ACTOR_PROACTOR 1

#define SERVER_ACTOR_TYPE     SERVER_ACTOR_PROACTOR         //当前服务器所用的架构类型，可以是Proactor，也可以是Reactor

#define MAX_DEV_POLL_COUNT    5000         //Devpoll处理链接的最大个数

#define CONNECT_CLOSE_TIME    2            //链接缓冲关闭时间延迟

#define MAX_MSG_THREADCOUNT   1            //默认的逻辑线程处理数
#define MAX_MSG_THREADQUEUE   60000        //处理线程的消息队列数
#define MAX_MSG_TIMEDELAYTIME 60           //逻辑线程自检时间间隔
#define MAX_MSG_STARTTIME     1            //逻辑线程处理开始时间
#define MAX_MSG_MASK          64000        //逻辑Mark的线程数
#define MAX_MSG_PUTTIMEOUT    1000         //放入逻辑的延迟
#define MAX_MSG_SENDPACKET    10           //最多缓冲发送包的个数,多于这个数字报警并丢弃下一个发送数据包
#define MAX_MSG_SNEDTHRESHOLD 10           //发送阀值(消息包的个数)
#define MAX_MSG_SENDCHECKTIME 100          //每隔多少毫秒进行一次发送的阀值
#define MAX_MSG_THREADTIMEOUT 30           //线程判定超时
#define MAX_MSG_PACKETTIMEOUT 5            //处理数据包超时时间
#define MAX_MSG_SOCKETBUFF    50*1024      //接收数据缓存大小
#define MAX_MSG_SENDTIMEOUT   5            //发送超时时间
#define MAX_MSG_HANDLETIME    120          //统计当前连接数的间隔
#define MAX_MSG_SENDALIVETIME 60           //发送存活包的时间间隔
#define MAX_MSG_SENDALCOUNT   5            //判定链接是否存活的极限
#define MAX_MSG_PACKETLENGTH  20*1024      //最大单一数据包长度限制 
#define MAX_MP_POOL           5000         //_MakePacket交换池里面的大小
#define MAX_HANDLE_POOL       1000         //默认Handler对象池的大小
#define MAX_ASYNCH_BACKLOG    100          //默认设置的BACKLOG数量

#define PACKET_PARSE          1
#define MAX_PACKET_PARSE      5000         //PACKETPARSE对象池个数
#define MAX_MESSAGE_POOL      5000         //Message对象池个数

#define PACKET_HEAD           4            //包头长度
#define BUFFPACKET_MAX_COUNT  2000         //初始化BuffPacket包缓冲池的个数

#define MAX_POSTMESSAGE_SIZE  65536        //最大的PostMessage循环

#define MAX_TIMERMANAGER_THREADCOUNT      1   //Timer管理器的最大线程数

#define PARM_CONNECTHANDLE_CHECK          2   //定时器发送包检测
#define PARM_HANDLE_CLOSE                 1   //定时器句柄关闭

#define HANDLE_ISCLOSE_NO                 0
#define HANDLE_ISCLOSE_YES                1

#define MAX_PACKET_SIZE 1024*1024

//对应链接的状态，用于设置链接时候的状态
enum
{
	CONNECT_INIT       = 0,
	CONNECT_OPEN       = 1,
	CONNECT_RECVGEGIN  = 2,
	CONNECT_RECVGEND   = 3,
	CONNECT_SENDBEGIN  = 4,
	CONNECT_SENDEND    = 5,
	CONNECT_CLOSEBEGIN = 6,
	CONNECT_CLOSEEND   = 7,
	CONNECT_RECVERROR  = 8,
	CONNECT_SENDERROR  = 9,
	CONNECT_SENDBUFF   = 10,
	CONNECT_SENDNON    = 11,
};

//对应处理线程的状态
enum
{
	THREAD_INIT      = 0,   //线程初始化
	THREAD_RUNBEGIN  = 1,   //开始处理线程
	THREAD_RUNEND    = 2,   //处理数据结束
	THREAD_BLOCK     = 3,   //线程阻塞
};

//日志编号声明
#define LOG_SYSTEM              1000
#define LOG_SYSTEM_ERROR        1001
#define LOG_SYSTEM_CONNECT      1002
#define LOG_SYSTEM_WORKTHREAD   1003
#define LOG_SYSTEM_POSTTHREAD   1004
#define LOG_SYSTEM_UDPTHREAD    1005
#define LOG_SYSTEM_POSTCONNECT  1006
#define LOG_SYSTEM_PACKETTIME   1007
#define LOG_SYSTEM_PACKETTHREAD 1008

#define OUR_DEBUG(X)  ACE_DEBUG((LM_INFO, "[%t]")); ACE_DEBUG(X)

enum
{
	REACTOR_CLIENTDEFINE  = 0,
	REACTOR_POSTDEFINE    = 1,
	REACTOR_UDPDEFINE     = 2,
};

//中间服务器的ID
enum
{
	POSTSERVER_TEST    = 1,
};

//*****************************************************************
//定义客户端信令(TCP)
#define COMMAND_BASE            0x1000
#define COMMAND_RETURN_ALIVE    0xf000

//*****************************************************************
//定义客户端信令(UDP)
#define COMMAND_UDP_BASE  0xa000

//*****************************************************************

//定义一个内存管理分配器
typedef  ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX> MUTEX_MALLOC;
typedef ACE_Allocator_Adapter<MUTEX_MALLOC> Mutex_Allocator;

#ifndef uint8
typedef ACE_UINT8 uint8;
#endif

#ifndef uint16
typedef ACE_UINT16 uint16;
#endif

#ifndef uint32
typedef ACE_UINT32 uint32;
#endif

#ifndef uint64
typedef ACE_UINT64 uint64;
#endif

#ifndef int8
typedef char int8;
#endif

#ifndef int16
typedef short int16;
#endif

#ifndef int32
typedef int int32;
#endif

#ifndef float32
typedef float float32;
#endif

#ifndef float64
typedef double float64;
#endif

#ifndef VCHARS_STR
typedef  struct _VCHARS_STR {
	const char *text;
	uint8 u1Len;
}VCHARS_STR;
#endif

#ifndef VCHARM_STR
typedef  struct _VCHARM_STR {
	const char *text;
	uint16 u2Len;
}VCHARM_STR;
#endif

#ifndef VCHARB_STR
typedef  struct _VCHARB_STR {
	const char *text;
	uint32 u4Len;
}VCHARB_STR;
#endif

//定时器参数的设置结构
struct _TimerCheckID 
{
	uint16 m_u2TimerCheckID;

	_TimerCheckID()
	{
		m_u2TimerCheckID = 0;
	}
};

//Message里面数据块结构体
struct _PacketInfo
{
	char* m_pData;
	int   m_nDataLen;

	_PacketInfo()
	{
		m_pData    = NULL;
		m_nDataLen = 0;
	}
};

//要连接的服务器信息
struct _ServerConnectInfo
{
	uint32      m_u4ServerID;     //服务器的ID
	ACE_TString m_strServerName;  //服务器的名称
	ACE_TString m_strServerIP;    //服务器的IP
	uint32      m_u4ServerPort;   //服务器的端口
	uint32      m_u4MaxConn;      //服务器的最大线程连接数
	uint32      m_u4TimeOut;      //服务器的链接超时时间

	_ServerConnectInfo()
	{
		m_u4ServerID   = 0;
		m_u4ServerPort = 0;
		m_u4MaxConn    = 0;
		m_u4TimeOut    = 0;
	}
};

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if( (x) != NULL ) { delete (x); (x) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if( (x) != NULL ) { delete[] (x); (x) = NULL; }
#endif

//定义一个函数，可以支持内存越界检查
inline void sprintf_safe(char* szText, int nLen, char* fmt ...)
{
	if(szText == NULL)
	{
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	ACE_OS::vsnprintf(szText, nLen, fmt, ap);

	va_end(ap);
}



#endif
