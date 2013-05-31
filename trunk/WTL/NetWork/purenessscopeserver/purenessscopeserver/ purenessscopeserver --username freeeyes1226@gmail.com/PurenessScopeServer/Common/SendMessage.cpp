#include "SendMessage.h"

CSendMessagePool::CSendMessagePool(void)
{
	m_u4CurrMaxCount = 0;
}

CSendMessagePool::~CSendMessagePool(void)
{
	OUR_DEBUG((LM_INFO, "[CSendMessagePool::~CSendMessagePool].\n"));
	Close();
	OUR_DEBUG((LM_INFO, "[CSendMessagePool::~CSendMessagePool] End.\n"));
}

void CSendMessagePool::Init(int nObjcetCount)
{
	Close();

	for(int i = 0; i < nObjcetCount; i++)
	{
		_SendMessage* pPacket = new _SendMessage();
		if(NULL != pPacket)
		{
			//��ӵ�Free map����
			mapSendMessage::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapSendMessage::value_type(pPacket, pPacket));
				m_u4CurrMaxCount++;
			}
		}
	}
}

void CSendMessagePool::Close()
{
	//���������Ѵ��ڵ�ָ��
	for(mapSendMessage::iterator itorFreeB = m_mapMessageFree.begin(); itorFreeB != m_mapMessageFree.end(); itorFreeB++)
	{
		_SendMessage* pObject = (_SendMessage* )itorFreeB->second;
		SAFE_DELETE(pObject);
	}
	
	for(mapSendMessage::iterator itorUsedB = m_mapMessageUsed.begin(); itorUsedB != m_mapMessageUsed.end(); itorUsedB++)
	{
		_SendMessage* pPacket = (_SendMessage* )itorUsedB->second;
		SAFE_DELETE(pPacket);
	}

	m_u4CurrMaxCount = 0;
	m_mapMessageFree.clear();
	m_mapMessageUsed.clear();
}

_SendMessage* CSendMessagePool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//���free�����Ѿ�û���ˣ�����ӵ�free���С�
	if(m_mapMessageFree.size() <= 0)
	{
		_SendMessage* pPacket = new _SendMessage();

		if(pPacket != NULL)
		{
			//��ӵ�Free map����
			mapSendMessage::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapSendMessage::value_type(pPacket, pPacket));
				m_u4CurrMaxCount++;
			}
		}
		else
		{
			return NULL;
		}
	}

	//��free�����ó�һ��,���뵽used����
	mapSendMessage::iterator itorFreeB = m_mapMessageFree.begin();
	_SendMessage* pPacket = (_SendMessage* )itorFreeB->second;
	m_mapMessageFree.erase(itorFreeB);

	//��ӵ�used map����
	mapSendMessage::iterator f = m_mapMessageUsed.find(pPacket);
	if(f == m_mapMessageUsed.end())
	{
		m_mapMessageUsed.insert(mapSendMessage::value_type(pPacket, pPacket));
	}

	return (_SendMessage* )pPacket;
}

bool CSendMessagePool::Delete(_SendMessage* pObject)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	if(NULL == pObject)
	{
		return false;
	}

	mapSendMessage::iterator f = m_mapMessageUsed.find(pObject);
	if(f != m_mapMessageUsed.end())
	{
		m_mapMessageUsed.erase(f);

		//��ӵ�Free map����
		mapSendMessage::iterator f = m_mapMessageFree.find(pObject);
		if(f == m_mapMessageFree.end())
		{
			m_mapMessageFree.insert(mapSendMessage::value_type(pObject, pObject));
		}
	}

	return true;
}

int CSendMessagePool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageUsed.size();
}

int CSendMessagePool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageFree.size();
}


