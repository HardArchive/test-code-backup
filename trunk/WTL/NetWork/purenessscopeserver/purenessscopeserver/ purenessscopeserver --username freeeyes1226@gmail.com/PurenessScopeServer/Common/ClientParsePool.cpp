#include "ClientParsePool.h"

CClientParsePool::CClientParsePool(void)
{
}

CClientParsePool::~CClientParsePool(void)
{
	OUR_DEBUG((LM_INFO, "[CClientParsePool::~CClientParsePool].\n"));
	Close();
	OUR_DEBUG((LM_INFO, "[CClientParsePool::~CClientParsePool]End.\n"));
}

void CClientParsePool::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		CClientParse* pPacket = new CClientParse();
		if(NULL != pPacket)
		{
			//��ӵ�Free map����
			mapClientParse::iterator f = m_mapPacketFree.find(pPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapClientParse::value_type(pPacket, pPacket));
			}
		}
	}
}

void CClientParsePool::Close()
{
	//���������Ѵ��ڵ�ָ��
	for(mapClientParse::iterator itorFreeB = m_mapPacketFree.begin(); itorFreeB !=  m_mapPacketFree.end(); itorFreeB++)
	{
		CClientParse* pPacket = (CClientParse* )itorFreeB->second;
		SAFE_DELETE(pPacket);
	}

	for(mapClientParse::iterator itorUsedB = m_mapPacketUsed.begin(); itorUsedB != m_mapPacketUsed.end(); itorUsedB++)
	{
		CClientParse* pPacket = (CClientParse* )itorUsedB->second;
		SAFE_DELETE(pPacket);
	}

	m_mapPacketFree.clear();
	m_mapPacketUsed.clear();
}

int CClientParsePool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapPacketUsed.size();
}

int CClientParsePool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapPacketFree.size();
}

CClientParse* CClientParsePool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//���free�����Ѿ�û���ˣ�����ӵ�free���С�
	if(m_mapPacketFree.size() <= 0)
	{
		CClientParse* pPacket = new CClientParse();

		if(pPacket != NULL)
		{
			//��ӵ�Free map����
			mapClientParse::iterator f = m_mapPacketFree.find(pPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapClientParse::value_type(pPacket, pPacket));
			}
		}
		else
		{
			return NULL;
		}
	}

	//��free�����ó�һ��,���뵽used����
	mapClientParse::iterator itorFreeB = m_mapPacketFree.begin();
	CClientParse* pPacket = (CClientParse* )itorFreeB->second;
	m_mapPacketFree.erase(itorFreeB);
	//��ӵ�used map����
	mapClientParse::iterator f = m_mapPacketUsed.find(pPacket);
	if(f == m_mapPacketUsed.end())
	{
		m_mapPacketUsed.insert(mapClientParse::value_type(pPacket, pPacket));
	}

	pPacket->Init();
	return (CClientParse* )pPacket;
}

bool CClientParsePool::Delete(CClientParse* pPacketParse)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	CClientParse* pBuff = (CClientParse* )pPacketParse;
	if(NULL == pBuff)
	{
		return false;
	}

	pPacketParse->SetMessageHead(NULL);
	pPacketParse->SetMessageBody(NULL);

	mapClientParse::iterator f = m_mapPacketUsed.find(pBuff);
	if(f != m_mapPacketUsed.end())
	{
		m_mapPacketUsed.erase(f);

		//��ӵ�Free map����
		mapClientParse::iterator f = m_mapPacketFree.find(pBuff);
		if(f == m_mapPacketFree.end())
		{
			m_mapPacketFree.insert(mapClientParse::value_type(pBuff, pBuff));
		}
	}

	return true;
}

