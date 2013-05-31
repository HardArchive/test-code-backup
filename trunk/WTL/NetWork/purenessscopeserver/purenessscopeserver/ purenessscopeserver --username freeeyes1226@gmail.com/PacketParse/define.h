#ifndef _DEFINE_H
#define _DEFINE_H

// Define.h
// ���ﶨ�����з������õ���ͨ�õĶ��������������һЩace��ͷ��ͨ�õ�һЩ���ݷ���
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

//���ݲ�ͬ�Ĳ���ϵͳ�����岻ͬ��recv���ղ�������
#ifdef WIN32
	#define MSG_NOSIGNAL          0            //�ź���������WINDOWS��
#endif

#define SERVER_ACTOR_REACTOR  0
#define SERVER_ACTOR_PROACTOR 1

#define SERVER_ACTOR_TYPE     SERVER_ACTOR_PROACTOR         //��ǰ���������õļܹ����ͣ�������Proactor��Ҳ������Reactor

#define MAX_DEV_POLL_COUNT    5000         //Devpoll�������ӵ�������

#define CONNECT_CLOSE_TIME    2            //���ӻ���ر�ʱ���ӳ�

#define MAX_MSG_THREADCOUNT   1            //Ĭ�ϵ��߼��̴߳�����
#define MAX_MSG_THREADQUEUE   60000        //�����̵߳���Ϣ������
#define MAX_MSG_TIMEDELAYTIME 60           //�߼��߳��Լ�ʱ����
#define MAX_MSG_STARTTIME     1            //�߼��̴߳���ʼʱ��
#define MAX_MSG_MASK          64000        //�߼�Mark���߳���
#define MAX_MSG_PUTTIMEOUT    1000         //�����߼����ӳ�
#define MAX_MSG_SENDPACKET    10           //��໺�巢�Ͱ��ĸ���,����������ֱ�����������һ���������ݰ�
#define MAX_MSG_SNEDTHRESHOLD 10           //���ͷ�ֵ(��Ϣ���ĸ���)
#define MAX_MSG_SENDCHECKTIME 100          //ÿ�����ٺ������һ�η��͵ķ�ֵ
#define MAX_MSG_THREADTIMEOUT 30           //�߳��ж���ʱ
#define MAX_MSG_PACKETTIMEOUT 5            //�������ݰ���ʱʱ��
#define MAX_MSG_SOCKETBUFF    50*1024      //�������ݻ����С
#define MAX_MSG_SENDTIMEOUT   5            //���ͳ�ʱʱ��
#define MAX_MSG_HANDLETIME    120          //ͳ�Ƶ�ǰ�������ļ��
#define MAX_MSG_SENDALIVETIME 60           //���ʹ�����ʱ����
#define MAX_MSG_SENDALCOUNT   5            //�ж������Ƿ���ļ���
#define MAX_MSG_PACKETLENGTH  20*1024      //���һ���ݰ��������� 
#define MAX_MP_POOL           5000         //_MakePacket����������Ĵ�С
#define MAX_HANDLE_POOL       1000         //Ĭ��Handler����صĴ�С
#define MAX_ASYNCH_BACKLOG    100          //Ĭ�����õ�BACKLOG����

#define PACKET_PARSE          1
#define MAX_PACKET_PARSE      5000         //PACKETPARSE����ظ���
#define MAX_MESSAGE_POOL      5000         //Message����ظ���

#define PACKET_HEAD           4            //��ͷ����
#define BUFFPACKET_MAX_COUNT  2000         //��ʼ��BuffPacket������صĸ���

#define MAX_POSTMESSAGE_SIZE  65536        //����PostMessageѭ��

#define MAX_TIMERMANAGER_THREADCOUNT      1   //Timer������������߳���

#define PARM_CONNECTHANDLE_CHECK          2   //��ʱ�����Ͱ����
#define PARM_HANDLE_CLOSE                 1   //��ʱ������ر�

#define HANDLE_ISCLOSE_NO                 0
#define HANDLE_ISCLOSE_YES                1

#define MAX_PACKET_SIZE 1024*1024

//��Ӧ���ӵ�״̬��������������ʱ���״̬
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

//��Ӧ�����̵߳�״̬
enum
{
	THREAD_INIT      = 0,   //�̳߳�ʼ��
	THREAD_RUNBEGIN  = 1,   //��ʼ�����߳�
	THREAD_RUNEND    = 2,   //�������ݽ���
	THREAD_BLOCK     = 3,   //�߳�����
};

//��־�������
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

//�м��������ID
enum
{
	POSTSERVER_TEST    = 1,
};

//*****************************************************************
//����ͻ�������(TCP)
#define COMMAND_BASE            0x1000
#define COMMAND_RETURN_ALIVE    0xf000

//*****************************************************************
//����ͻ�������(UDP)
#define COMMAND_UDP_BASE  0xa000

//*****************************************************************

//����һ���ڴ���������
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

//��ʱ�����������ýṹ
struct _TimerCheckID 
{
	uint16 m_u2TimerCheckID;

	_TimerCheckID()
	{
		m_u2TimerCheckID = 0;
	}
};

//Message�������ݿ�ṹ��
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

//Ҫ���ӵķ�������Ϣ
struct _ServerConnectInfo
{
	uint32      m_u4ServerID;     //��������ID
	ACE_TString m_strServerName;  //������������
	ACE_TString m_strServerIP;    //��������IP
	uint32      m_u4ServerPort;   //�������Ķ˿�
	uint32      m_u4MaxConn;      //������������߳�������
	uint32      m_u4TimeOut;      //�����������ӳ�ʱʱ��

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

//����һ������������֧���ڴ�Խ����
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
