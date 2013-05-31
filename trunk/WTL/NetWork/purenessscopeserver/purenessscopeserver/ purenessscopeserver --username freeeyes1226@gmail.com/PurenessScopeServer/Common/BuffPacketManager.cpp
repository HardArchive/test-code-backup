#include "BuffPacketManager.h"

CBuffPacketManager::CBuffPacketManager(void)
{

}

CBuffPacketManager::~CBuffPacketManager(void)
{
	OUR_DEBUG((LM_ERROR, "[CBuffPacketManager::~CBuffPacketManager].\n"));
	//Close();
}

IBuffPacket* CBuffPacketManager::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//���free�����Ѿ�û���ˣ�����ӵ�free���С�
	if(m_mapPacketFree.size() <= 0)
	{
		CBuffPacket* pBuffPacket = new CBuffPacket();

		if(pBuffPacket != NULL)
		{
			//��ӵ�Free map����
			mapPacket::iterator f = m_mapPacketFree.find(pBuffPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacket::value_type(pBuffPacket, pBuffPacket));
			}
		}
		else
		{
			return NULL;
		}
	}

	//��free�����ó�һ��,���뵽used����
	mapPacket::iterator itorFreeB = m_mapPacketFree.begin();
	CBuffPacket* pBuffPacket = (CBuffPacket* )itorFreeB->second;
	m_mapPacketFree.erase(itorFreeB);
	//��ӵ�used map����
	mapPacket::iterator f = m_mapPacketUsed.find(pBuffPacket);
	if(f == m_mapPacketUsed.end())
	{
		m_mapPacketUsed.insert(mapPacket::value_type(pBuffPacket, pBuffPacket));
	}

	return (IBuffPacket* )pBuffPacket;
}

bool CBuffPacketManager::Delete(IBuffPacket* pBuffPacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	CBuffPacket* pBuff = (CBuffPacket* )pBuffPacket;
	if(NULL == pBuff)
	{
		return false;
	}

	pBuffPacket->Clear();

	mapPacket::iterator f = m_mapPacketUsed.find(pBuff);
	if(f != m_mapPacketUsed.end())
	{
		m_mapPacketUsed.erase(f);

		//��ӵ�Free map����
		mapPacket::iterator f = m_mapPacketFree.find(pBuff);
		if(f == m_mapPacketFree.end())
		{
			m_mapPacketFree.insert(mapPacket::value_type(pBuff, pBuff));
		}
	}

	return true;
}

void CBuffPacketManager::Close()
{
	//���������Ѵ��ڵ�ָ��
	for(mapPacket::iterator itorFreeB = m_mapPacketFree.begin(); itorFreeB != m_mapPacketFree.end(); itorFreeB++)
	{
		CBuffPacket* pBuffPacket = (CBuffPacket* )itorFreeB->second;
		pBuffPacket->Close();
		SAFE_DELETE(pBuffPacket);
	}

	for(mapPacket::iterator itorUsedB = m_mapPacketUsed.begin(); itorUsedB != m_mapPacketUsed.end(); itorUsedB++)
	{
		CBuffPacket* pBuffPacket = (CBuffPacket* )itorUsedB->second;
		pBuffPacket->Close();
		OUR_DEBUG((LM_ERROR, "[CBuffPacketManager::Close]CBuffPacket has used!!memory address[0x%08x].\n", pBuffPacket));
		SAFE_DELETE(pBuffPacket);
	}

	m_mapPacketFree.clear();
	m_mapPacketUsed.clear();
}

void CBuffPacketManager::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		CBuffPacket* pBuffPacket = new CBuffPacket();
		if(NULL != pBuffPacket)
		{
			//��ӵ�Free map����
			mapPacket::iterator f = m_mapPacketFree.find(pBuffPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacket::value_type(pBuffPacket, pBuffPacket));
			}
		}
	}

}

uint32 CBuffPacketManager::GetBuffPacketUsedCount()
{
	return (uint32)m_mapPacketUsed.size();
}

uint32 CBuffPacketManager::GetBuffPacketFreeCount()
{
	return (uint32)m_mapPacketFree.size();
}
