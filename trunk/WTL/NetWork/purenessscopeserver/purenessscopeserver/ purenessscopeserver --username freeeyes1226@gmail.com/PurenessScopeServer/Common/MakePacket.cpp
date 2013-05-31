#include "MakePacket.h"


CMakePacketPool::CMakePacketPool()
{
}

CMakePacketPool::~CMakePacketPool()
{
	OUR_DEBUG((LM_INFO, "[CMakePacketPool::~CMakePacketPool].\n"));
	Close();
}

void CMakePacketPool::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		_MakePacket* pPacket = new _MakePacket();
		if(NULL != pPacket)
		{
			//��ӵ�Free map����
			mapPacket::iterator f = m_mapPacketFree.find(pPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacket::value_type(pPacket, pPacket));
			}
		}
	}
}

void CMakePacketPool::Close()
{
	//���������Ѵ��ڵ�ָ��
	for(mapPacket::iterator itorFreeB = m_mapPacketFree.begin(); itorFreeB != m_mapPacketFree.end(); itorFreeB++)
	{
		_MakePacket* pPacket = (_MakePacket* )itorFreeB->second;
		SAFE_DELETE(pPacket);
	}

	for(mapPacket::iterator itorUsedB = m_mapPacketUsed.begin(); itorUsedB != m_mapPacketUsed.end(); itorUsedB++)
	{
		_MakePacket* pPacket = (_MakePacket* )itorUsedB->second;
		OUR_DEBUG((LM_ERROR, "[CMakePacketPool::Close]MakePacket has used!!memory address[0x%08x].\n", pPacket));
		SAFE_DELETE(pPacket);
	}

	m_mapPacketFree.clear();
	m_mapPacketUsed.clear();
}

int CMakePacketPool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapPacketUsed.size();
}

int CMakePacketPool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapPacketFree.size();
}

_MakePacket* CMakePacketPool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//���free�����Ѿ�û���ˣ�����ӵ�free���С�
	if(m_mapPacketFree.size() <= 0)
	{
		_MakePacket* pPacket = new _MakePacket();

		if(pPacket != NULL)
		{
			//��ӵ�Free map����
			mapPacket::iterator f = m_mapPacketFree.find(pPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacket::value_type(pPacket, pPacket));
			}
		}
		else
		{
			return NULL;
		}
	}

	//��free�����ó�һ��,���뵽used����
	mapPacket::iterator itorFreeB = m_mapPacketFree.begin();
	_MakePacket* pPacket = (_MakePacket* )itorFreeB->second;
	m_mapPacketFree.erase(itorFreeB);
	//��ӵ�used map����
	mapPacket::iterator f = m_mapPacketUsed.find(pPacket);
	if(f == m_mapPacketUsed.end())
	{
		m_mapPacketUsed.insert(mapPacket::value_type(pPacket, pPacket));
	}

	return (_MakePacket* )pPacket;
}

bool CMakePacketPool::Delete(_MakePacket* pBuffPacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	_MakePacket* pBuff = (_MakePacket* )pBuffPacket;
	if(NULL == pBuff)
	{
		return false;
	}

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


//*********************************************************************

CMakePacket::CMakePacket(void)
{
}

CMakePacket::~CMakePacket(void)
{
	OUR_DEBUG((LM_INFO, "[CMakePacket::~CMakePacket].\n"));
}

bool CMakePacket::Init()
{
	m_MakePacketPool.Init(MAX_PACKET_PARSE);
	return true;
}


bool CMakePacket::PutUDPMessageBlock(const ACE_INET_Addr& AddrRemote, uint8 u1Option, CPacketParse* pPacketParse)
{
	_MakePacket* pMakePacket = m_MakePacketPool.Create();
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutUDPMessageBlock] Get pMakePacket is NULL.\n"));
		return false;
	}

	pMakePacket->m_u4ConnectID       = UDP_HANDER_ID;
	pMakePacket->m_u1Option          = u1Option;
	pMakePacket->m_pPacketParse      = pPacketParse;
	pMakePacket->m_AddrRemote        = AddrRemote;
	pMakePacket->m_PacketType        = PACKET_UDP;

	ProcessMessageBlock(pMakePacket);

	return true;
}

bool CMakePacket::PutMessageBlock(uint32 u4ConnectID, uint8 u1Option, CPacketParse* pPacketParse)
{
	_MakePacket* pMakePacket = m_MakePacketPool.Create();
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutMessageBlock] Get pMakePacket is NULL.\n"));
		return false;
	}

	pMakePacket->m_u4ConnectID       = u4ConnectID;
	pMakePacket->m_u1Option          = u1Option;
	pMakePacket->m_pPacketParse      = pPacketParse;

	ProcessMessageBlock(pMakePacket);

	return true;
}

bool CMakePacket::ProcessMessageBlock(_MakePacket* pMakePacket)
{
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR,"[CMakePacket::ProcessMessageBlock] pMakePacket is NULL.\n"));
		return false;
	}

	//���ݲ���OP��������Ӧ�ķ�����
	CMessage* pMessage = NULL;
	if(pMakePacket->m_u1Option == PACKET_PARSE)
	{
		if(pMakePacket->m_PacketType == 0)
		{
			//TCP���ݰ�������
			pMessage = SetMessage(pMakePacket->m_pPacketParse, pMakePacket->m_u4ConnectID);
		}
		else
		{
			//UDP���ݰ�������
			pMessage = SetMessage(pMakePacket->m_pPacketParse, pMakePacket->m_AddrRemote);
		}		
	}
	else if(pMakePacket->m_u1Option == PACKET_CONNECT)
	{
		pMessage = SetMessageConnect(pMakePacket->m_u4ConnectID);
	}
	else if(pMakePacket->m_u1Option == PACKET_CDISCONNECT)
	{
		pMessage = SetMessageCDisConnect(pMakePacket->m_u4ConnectID);
	}
	else if(pMakePacket->m_u1Option == PACKET_SDISCONNECT)
	{
		pMessage = SetMessageSDisConnect(pMakePacket->m_u4ConnectID);
	}

	if(NULL != pMessage)
	{
		//��Ҫ�������Ϣ������Ϣ�����߳�
		if(false == App_MessageService::instance()->PutMessage(pMessage))
		{
			OUR_DEBUG((LM_ERROR, "[CMakePacket::ProcessMessageBlock] App_MessageService::instance()->PutMessage Error.\n"));
			App_MessagePool::instance()->Delete(pMessage);
			App_PacketParsePool::instance()->Delete(pMakePacket->m_pPacketParse);
			m_MakePacketPool.Delete(pMakePacket);
			return false;
		}
	}
	else
	{
			App_MessagePool::instance()->Delete(pMessage);
			App_PacketParsePool::instance()->Delete(pMakePacket->m_pPacketParse);
			m_MakePacketPool.Delete(pMakePacket);
			return false;		
	}

	if(NULL != pMakePacket->m_pPacketParse)
	{
		App_PacketParsePool::instance()->Delete(pMakePacket->m_pPacketParse);
	}

	m_MakePacketPool.Delete(pMakePacket);

	return true;
}

CMessage* CMakePacket::SetMessage(CPacketParse* pPacketParse, uint32 u4ConnectID)
{
	//�����µ�Message����
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//д����հ�����
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage is NULL.\n", u4ConnectID));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
		pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(pPacketParse->GetMessageHead());
		pMessage->SetPacketBody(pPacketParse->GetMessageBody());

		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
		return NULL;
	}
}

CMessage* CMakePacket::SetMessage(CPacketParse* pPacketParse, const ACE_INET_Addr& AddrRemote)
{
	//�����µ�Message����
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//д����հ�����
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] UDP ConnectID, pMessage is NULL.\n"));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = UDP_HANDER_ID;
		pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4Port        = (uint32)AddrRemote.get_port_number();
		pMessage->GetMessageBase()->m_u1PacketType  = PACKET_UDP;
		pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
		pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();
		sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "%s", AddrRemote.get_host_addr());
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(pPacketParse->GetMessageHead());
		pMessage->SetPacketBody(pPacketParse->GetMessageBody());
		
		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] UDP ConnectID, pMessage->GetMessageBase() is NULL.\n"));
		return NULL;
	}
}

CMessage* CMakePacket::SetMessageConnect(uint32 u4ConnectID)
{
	//�����µ�Message����
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//д����հ�����
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage is NULL.\n", u4ConnectID));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CONNECT;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);

		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
		return NULL;
	}
}

CMessage* CMakePacket::SetMessageCDisConnect(uint32 u4ConnectID)
{
	//�����µ�Message����
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//д����հ�����
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage is NULL.\n", u4ConnectID));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CDISCONNET;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);

		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
		return NULL;
	}
}

CMessage* CMakePacket::SetMessageSDisConnect(uint32 u4ConnectID)
{
	//�����µ�Message����
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//д����հ�����
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage is NULL.\n", u4ConnectID));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_SDISCONNET;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);

		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
		return NULL;
	}
}

