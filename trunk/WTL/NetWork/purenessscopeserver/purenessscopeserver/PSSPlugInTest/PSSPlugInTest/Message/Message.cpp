// BuffPacket.h
// ������Ϣ���࣬���ڽ���Ϣ�ַ�����Ҫ���������
// ѧϰ�Ǳ�Ҫ�ģ�������Ҫ�������ҵ�ʵ������ƣ�������ǣ���Ҫ����սʤ����
// add by freeeyes
// 2009-01-10

#include "Message.h"

CMessage::CMessage(void)
{
	m_pMessageBase  = NULL;
	m_pmbHead       = NULL;
	m_pmbBody       = NULL;
	m_szError[0]    = '\0';

	m_pMessageBase = new _MessageBase();
}

CMessage::~CMessage(void)
{
	//OUR_DEBUG((LM_INFO, "[CMessage::~CMessage].\n"));
	Close();
	//OUR_DEBUG((LM_INFO, "[CMessage::~CMessage]End.\n"));
}

const char* CMessage::GetError()
{
	return m_szError;
}

void CMessage::SetMessageBase(_MessageBase* pMessageBase)
{
	if(NULL != m_pMessageBase)
	{
		delete m_pMessageBase;
		m_pMessageBase = NULL;
	}

	m_pMessageBase = pMessageBase;
}

_MessageBase* CMessage::GetMessageBase()
{
	return m_pMessageBase;
}

bool CMessage::GetPacketHead(_PacketInfo& PacketInfo)
{
	if(m_pmbHead != NULL)
	{
		PacketInfo.m_pData    = m_pmbHead->rd_ptr();
		PacketInfo.m_nDataLen = (int)m_pmbHead->length();
		return true;
	}
	else
	{
		return false;
	}
}

bool CMessage::GetPacketBody(_PacketInfo& PacketInfo)
{
	if(m_pmbBody != NULL)
	{
		PacketInfo.m_pData    = m_pmbBody->rd_ptr();
		PacketInfo.m_nDataLen = (int)m_pmbBody->length();
		return true;
	}
	else
	{
		return false;
	}
}

bool CMessage::SetPacketHead(ACE_Message_Block* pmbHead)
{
	m_pmbHead = pmbHead;
	return true;
}

bool CMessage::SetPacketBody(ACE_Message_Block* pmbBody)
{
	m_pmbBody = pmbBody;
	return true;
}

void CMessage::Close()
{
	if(NULL != m_pMessageBase)
	{
		delete m_pMessageBase;
		m_pMessageBase = NULL;
	}

	if(NULL != m_pmbHead)
	{
		m_pmbHead->release();
		m_pmbHead = NULL;
	}

	if(NULL != m_pmbBody)
	{
		m_pmbBody->release();
		m_pmbBody = NULL;
	}
}

void CMessage::Clear()
{
	if(NULL != m_pmbHead)
	{
		App_MessageBlockManager::instance()->Close(m_pmbHead);
		m_pmbHead = NULL;
	}

	if(NULL != m_pmbBody)
	{
		App_MessageBlockManager::instance()->Close(m_pmbBody);
		m_pmbBody = NULL;
	}
}

CMessagePool::CMessagePool()
{
}

CMessagePool::~CMessagePool()
{
	OUR_DEBUG((LM_INFO, "[CMessagePool::~CMessagePool].\n"));
	Close();
	OUR_DEBUG((LM_INFO, "[CMessagePool::~CMessagePool]End.\n"));
}

void CMessagePool::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		CMessage* pPacket = new CMessage();
		if(NULL != pPacket)
		{
			//��ӵ�Free map����
			mapMessage::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapMessage::value_type(pPacket, pPacket));
			}
		}
	}
}

void CMessagePool::Close()
{
	//���������Ѵ��ڵ�ָ��
	for(mapMessage::iterator itorFreeB = m_mapMessageFree.begin(); itorFreeB != m_mapMessageFree.end(); itorFreeB++)
	{
		CMessage* pPacket = (CMessage* )itorFreeB->second;
		SAFE_DELETE(pPacket);
	}

	for(mapMessage::iterator itorUsedB = m_mapMessageUsed.begin(); itorUsedB != m_mapMessageUsed.end(); itorUsedB++)
	{
		CMessage* pPacket = (CMessage* )itorUsedB->second;
		SAFE_DELETE(pPacket);
	}

	m_mapMessageFree.clear();
	m_mapMessageUsed.clear();
}

int CMessagePool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageUsed.size();
}

int CMessagePool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageFree.size();
}

CMessage* CMessagePool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//���free�����Ѿ�û���ˣ�����ӵ�free���С�
	if(m_mapMessageFree.size() <= 0)
	{
		CMessage* pPacket = new CMessage();

		if(pPacket != NULL)
		{
			//��ӵ�Free map����
			mapMessage::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapMessage::value_type(pPacket, pPacket));
			}
		}
		else
		{
			return NULL;
		}
	}

	//��free�����ó�һ��,���뵽used����
	mapMessage::iterator itorFreeB = m_mapMessageFree.begin();
	CMessage* pPacket = (CMessage* )itorFreeB->second;
	m_mapMessageFree.erase(itorFreeB);
	//��ӵ�used map����
	mapMessage::iterator f = m_mapMessageUsed.find(pPacket);
	if(f == m_mapMessageUsed.end())
	{
		m_mapMessageUsed.insert(mapMessage::value_type(pPacket, pPacket));
	}

	return (CMessage* )pPacket;
}

bool CMessagePool::Delete(CMessage* pBuffPacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	CMessage* pBuff = (CMessage* )pBuffPacket;
	if(NULL == pBuff)
	{
		return false;
	}

	pBuffPacket->Clear();

	mapMessage::iterator f = m_mapMessageUsed.find(pBuff);
	if(f != m_mapMessageUsed.end())
	{
		m_mapMessageUsed.erase(f);

		//��ӵ�Free map����
		mapMessage::iterator f = m_mapMessageFree.find(pBuff);
		if(f == m_mapMessageFree.end())
		{
			m_mapMessageFree.insert(mapMessage::value_type(pBuff, pBuff));
		}
	}

	return true;
}



