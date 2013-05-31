#ifndef _PACKETPARSEPOOL_H
#define _PACKETPARSEPOOL_H

#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"

#include "PacketParse.h"

#include <map>

using namespace std;

//CPacketParse�����
class CPacketParsePool
{
public:
	CPacketParsePool();
	~CPacketParsePool();

	void Init(uint32 u4PacketParsePoolCount);
	void Close();

	CPacketParse* Create();
	bool Delete(CPacketParse* pPacketParse);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<CPacketParse*, CPacketParse*> mapPacketParse;
	mapPacketParse                  m_mapPacketUsed;                       //��ʹ�õ�
	mapPacketParse                  m_mapPacketFree;                       //û��ʹ�õ�
	ACE_Recursive_Thread_Mutex      m_ThreadWriteLock;                     //���ƶ��߳���
};

typedef ACE_Singleton<CPacketParsePool, ACE_Null_Mutex> App_PacketParsePool; 

#endif
