#ifndef _MESSAGEPOOL_H
#define _MESSAGEPOOL_H

//���ݻ������ݳ�
//add by freeeyes

#include <map>
#include "ace/Malloc.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Message_Block.h"
#include "ace/Guard_T.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"

#include "define.h"

using namespace std;

#define MAX_BUFF_SIZE 1024   //��󻺳�鳤��

//����ṹ��Ϣ
struct _CommandInfo
{
	uint32 u4OpreationID;   //����ID
	uint32 u4RoomID;        //����ID
	uint32 u4PlayerID;      //���ID
	uint32 u4CardID;        //����ID
	uint32 u4Data;          //��������

	_CommandInfo()
	{
		Clear();
	}

	_CommandInfo& operator = (const _CommandInfo& ar)
	{
		this->u4OpreationID    = ar.u4OpreationID;
		this->u4RoomID         = ar.u4RoomID;
		this->u4PlayerID       = ar.u4PlayerID;
		this->u4CardID         = ar.u4CardID;
		this->u4Data           = ar.u4Data;

		return *this;
	}

	void Clear()
	{
		u4OpreationID = 0;
		u4RoomID      = 0;
		u4PlayerID    = 0;
		u4CardID      = 0;
		u4Data        = 0;
	}
};

//�����е���Ϣ��Ϣ�ṹ��
struct _QueueMessage
{
	uint32       m_u4CommandID;        //��������
	_CommandInfo m_objData;            //����������

	_QueueMessage()
	{
		m_u4CommandID = 0;
	}

	void Clear()
	{
		m_u4CommandID = 0;
		m_objData.Clear();
	}
};

//Message�����
class CMessagePool
{
public:
	CMessagePool();
	~CMessagePool();

	void Init(uint32 u4PacketCount);
	void Close();

	_QueueMessage* Create();
	bool Delete(_QueueMessage* pMakePacket);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<_QueueMessage*, _QueueMessage*> mapMessage;
	mapMessage                  m_mapMessageUsed;                      //��ʹ�õ�
	mapMessage                  m_mapMessageFree;                      //û��ʹ�õ�
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //���ƶ��߳���
};


//Message�����
class CMessageBlockPool
{
public:
	CMessageBlockPool();
	~CMessageBlockPool();

	void Close();

	ACE_Message_Block* Create(uint32 u4Size);
	bool Delete(ACE_Message_Block* pMakePacket);

private:
	ACE_Allocator * m_pmsgallocator;
	ACE_Allocator * m_pdata_allocator;
	ACE_Allocator * m_pbuff_allocator;

	//ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //���ƶ��߳���
};

//����ṹ��
struct _BufferInfo
{
	uint32  m_u4BufferID;
	char    m_szBuff[MAX_BUFF_SIZE];    //�����
	uint32  m_u4WritePos;               //д���

	_BufferInfo()
	{
		Clear();
	}

	bool SetBuffer(uint32 u4Data)
	{
		if(m_u4WritePos + sizeof(int) >= MAX_BUFF_SIZE)
		{
			return false;
		}
		else
		{
			ACE_OS::memcpy(&m_szBuff + m_u4WritePos, &u4Data, sizeof(int));
			m_u4WritePos += sizeof(int);
			return true;
		}
	}

	bool SetBuffer(const char* pData, uint32 u4Len)
	{
		if(m_u4WritePos + u4Len + sizeof(int) >= MAX_BUFF_SIZE)
		{
			return false;
		}
		else
		{
			ACE_OS::memcpy(&m_szBuff + m_u4WritePos, &u4Len, sizeof(int));
			m_u4WritePos += sizeof(int);
			ACE_OS::memcpy(&m_szBuff + m_u4WritePos, pData, u4Len);
			m_u4WritePos += u4Len;
			return true;
		}
	}

	void Clear()
	{
		m_u4WritePos  = 0;
		m_szBuff[0]   = '\0';
	}
};

//�������ݻ����
class CBufferInfoPool
{
public:
	CBufferInfoPool();
	~CBufferInfoPool();

	void Init(uint32 u4Count);
	void Close();
	_BufferInfo* Create();
	int Delete(uint32 u4BufferID);

	int GetUsedCount();
	int GetFreeCount();

	bool SetBufferInfo(uint32 u4BufferID, uint32 u4Data);
	bool SetBufferInfo(uint32 u4BufferID, const char* pData, uint32 u4Len);

private:
	typedef map<uint32, _BufferInfo*> mapMessage;
	mapMessage                  m_mapMessageUsed;                      //��ʹ�õ�
	mapMessage                  m_mapMessageFree;                      //û��ʹ�õ�
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //���ƶ��߳���
};

typedef ACE_Singleton<CBufferInfoPool, ACE_Null_Mutex> App_BufferInfoPool;

#endif
