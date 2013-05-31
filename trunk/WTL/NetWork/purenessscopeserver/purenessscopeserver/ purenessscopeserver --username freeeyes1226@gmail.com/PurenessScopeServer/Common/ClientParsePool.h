#ifndef _CLIENTPARSEPOOL_H
#define _CLIENTPARSEPOOL_H

//���ݽ��հ������໺���
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
	mapClientParse                  m_mapPacketUsed;                       //��ʹ�õ�
	mapClientParse                  m_mapPacketFree;                       //û��ʹ�õ�
	ACE_Recursive_Thread_Mutex      m_ThreadWriteLock;                     //���ƶ��߳���
};

typedef ACE_Singleton<CClientParsePool, ACE_Null_Mutex> App_ClientParsePool; 
#endif
