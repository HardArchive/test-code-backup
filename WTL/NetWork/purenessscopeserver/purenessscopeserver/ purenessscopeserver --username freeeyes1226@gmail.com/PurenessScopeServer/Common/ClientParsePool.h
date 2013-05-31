#ifndef _CLIENTPARSEPOOL_H
#define _CLIENTPARSEPOOL_H

//数据接收包分析类缓冲池
//add by freeeyes 2010-12-24

#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"

#include "ClientParse.h"

#include <map>

using namespace std;

class CClientParsePool
{
public:
	CClientParsePool(void);
	~CClientParsePool(void);

	void Init(uint32 u4Count);
	void Close();

	CClientParse* Create();
	bool Delete(CClientParse* pPacketParse);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<CClientParse*, CClientParse*> mapClientParse;
	mapClientParse                  m_mapPacketUsed;                       //已使用的
	mapClientParse                  m_mapPacketFree;                       //没有使用的
	ACE_Recursive_Thread_Mutex      m_ThreadWriteLock;                     //控制多线程锁
};

typedef ACE_Singleton<CClientParsePool, ACE_Null_Mutex> App_ClientParsePool; 
#endif
