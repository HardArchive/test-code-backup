#ifndef _THREADINFO_H
#define _THREADINFO_H

#include "define.h"
#include <map>

using namespace std;

struct _ThreadInfo
{
	ACE_thread_t        m_u4ThreadID;         //�߳�ID 
	uint32              m_u4ThreadIndex;      //ϵͳ�߳�ID
	ACE_RW_Thread_Mutex m_rwMutex;
	ACE_Time_Value      m_tvUpdateTime;       //�߳���������ݵ�ʱ��
	uint32              m_u4State;            //0Ϊû���������ڴ���1Ϊ���ڴ���������ݡ�
	uint32              m_u4RecvPacketCount;  //���߳��½��հ��Ĵ������
	uint32              m_u4SendPacketCount;  //���߳��·��Ͱ��Ĵ������
	uint16              m_u2CommandID;        //��ǰ���������ID
	uint16              m_u2PacketTime;       //ƽ�����ݰ�����ʱ��
	uint32              m_u4CurrPacketCount;  //��λʱ���ڵİ���������

	_ThreadInfo()
	{
		m_u4State           = THREAD_INIT;
		m_tvUpdateTime      = ACE_OS::gettimeofday();
		m_u4RecvPacketCount = 0;
		m_u4SendPacketCount = 0;
		m_u2CommandID       = 0;
		m_u4ThreadIndex     = 0;
		m_u2PacketTime      = 0;
		m_u4CurrPacketCount = 0;
	}
};

typedef map<uint32, _ThreadInfo*> mapThreadInfo;

class CThreadInfo
{
public:
	CThreadInfo(void);
	~CThreadInfo(void);

	bool AddThreadInfo(uint32 u4ThreadID);
	int  GetThreadCount();
	_ThreadInfo* GetThreadInfo(uint32 u4ThreadID);
	bool CloseThread(uint32 u4ThreadID);
	void Close();

private:
	mapThreadInfo m_mapThreadInfo;
};
#endif
