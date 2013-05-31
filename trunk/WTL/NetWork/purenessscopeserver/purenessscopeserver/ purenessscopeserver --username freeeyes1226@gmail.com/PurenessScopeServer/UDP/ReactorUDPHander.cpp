#include "ReactorUDPHander.h"

CReactorUDPHander::CReactorUDPHander(void)
{
	m_pPacketParse      = NULL;
	m_u4RecvPacketCount = 0;
	m_u4SendPacketCount = 0;
	m_u4RecvSize        = 0;
	m_u4SendSize        = 0;
}

CReactorUDPHander::~CReactorUDPHander(void)
{
}

int CReactorUDPHander::OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Reactor* pReactor)
{
	if(m_skRemote.open(AddrRemote) == -1)
	{
		ACE_DEBUG((LM_ERROR, "[CReactorUDPHander::OpenAddress]Open error(%d).\n", errno));
		return -1;
	}

	reactor(pReactor);

	m_pPacketParse = App_PacketParsePool::instance()->Create();

	if(-1 == this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK))
	{
		ACE_DEBUG((LM_ERROR, "[CReactorUDPHander::OpenAddress] Addr is register_handler error(%d).\n", errno));
		return -1;
	}
	return 0;
}

void CReactorUDPHander::Close()
{
	ACE_Reactor_Mask close_mask = ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
	this->reactor()->remove_handler(this, close_mask);
	m_skRemote.close();
}

ACE_HANDLE CReactorUDPHander::get_handle(void) const
{
	return m_skRemote.get_handle();
}

int CReactorUDPHander::handle_input(ACE_HANDLE fd)
{
	if(fd == ACE_INVALID_HANDLE)
	{
		ACE_DEBUG((LM_ERROR, "[CReactorUDPHander::handle_input]fd is ACE_INVALID_HANDLE.\n"));
		return -1;
	}	
	
	char szBuff[MAX_UDP_PACKET_LEN] = {'\0'};

	int nDataLen = m_skRemote.recv(szBuff, MAX_UDP_PACKET_LEN, m_addrRemote);
	if(nDataLen > 0)
	{
		CheckMessage(szBuff, (uint32)nDataLen);

		m_pPacketParse = App_PacketParsePool::instance()->Create();
	}

	return 0;
}

int CReactorUDPHander::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	if(handle == ACE_INVALID_HANDLE)
	{
		ACE_DEBUG((LM_ERROR, "[CReactorUDPHander::handle_close]close_mask = %d.\n", (uint32)close_mask));
	}	
	
	Close();
	return 0;
}

bool CReactorUDPHander::SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead, uint16 u2CommandID)
{
	ACE_hrtime_t m_tvBegin = ACE_OS::gethrtime();

	ACE_INET_Addr AddrRemote;
	int nErr = AddrRemote.set(nPort, szIP);
	if(nErr != 0)
	{
		OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::SendMessage]set_address error[%d].\n", errno));
		SAFE_DELETE_ARRAY(pMessage);
		return false;
	}

	//�����Ҫƴ�Ӱ�ͷ����ƴ�Ӱ�ͷ
	if(blHead == true)
	{
		CPacketParse PacketParse;

		ACE_Message_Block* pMbData = NULL;
		uint32 u4SendLength = PacketParse.MakePacketLength(u4Len);
		pMbData = App_MessageBlockManager::instance()->Create(u4SendLength);
		if(NULL == pMbData)
		{
			SAFE_DELETE(pMessage);
			return false;
		}

		PacketParse.MakePacket(pMessage, u4Len, pMbData);

		int nSize = (int)m_skRemote.send(pMbData->rd_ptr(), pMbData->length(), AddrRemote);
		if((uint32)nSize == u4Len)
		{
			m_atvOutput = ACE_OS::gettimeofday();
			m_u4SendSize += u4Len;
			m_u4SendPacketCount++;
			SAFE_DELETE_ARRAY(pMessage);

			//ͳ�Ʒ�����Ϣ
			uint32 u4Cost = (uint32)(ACE_OS::gethrtime() - m_tvBegin);
			App_CommandAccount::instance()->SaveCommandData(u2CommandID, u4Cost, PACKET_UDP, (uint32)pMbData->length(), u4Len, COMMAND_TYPE_OUT);

			//�ͷŷ�����
			pMbData->release();

			return true;
		}
		else
		{
			ACE_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
			SAFE_DELETE_ARRAY(pMessage);

			//�ͷŷ�����
			pMbData->release();

			return false;
		}
	}
	else
	{
		int nSize = (int)m_skRemote.send(pMessage, u4Len, AddrRemote);
		if((uint32)nSize == u4Len)
		{
			m_atvOutput = ACE_OS::gettimeofday();
			m_u4SendSize += u4Len;
			m_u4SendPacketCount++;
			SAFE_DELETE_ARRAY(pMessage);

			//ͳ�Ʒ�����Ϣ
			uint32 u4Cost = (uint32)(ACE_OS::gethrtime() - m_tvBegin);
			App_CommandAccount::instance()->SaveCommandData(u2CommandID, u4Cost, PACKET_UDP, u4Len, u4Len, COMMAND_TYPE_OUT);

			return true;
		}
		else
		{
			ACE_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
			SAFE_DELETE_ARRAY(pMessage);
			return false;
		}
	}

}

_ClientConnectInfo CReactorUDPHander::GetClientConnectInfo()
{
	_ClientConnectInfo ClientConnectInfo;
	ClientConnectInfo.m_blValid       = true;
	ClientConnectInfo.m_u4ConnectID   = 0;
	ClientConnectInfo.m_u4AliveTime   = 0;
	ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvInput.sec();
	ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
	ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
	ClientConnectInfo.m_u4RecvCount   = m_u4RecvPacketCount;
	ClientConnectInfo.m_u4SendCount   = m_u4SendPacketCount;
	return ClientConnectInfo;
}

bool CReactorUDPHander::CheckMessage(const char* pData, uint32 u4Len)
{
	if(NULL == m_pPacketParse || NULL == pData)
	{
		return false;
	}

	if(u4Len <= m_pPacketParse->GetPacketHeadLen())
	{
		return false;
	}

	//�����������ݰ�ת��ΪPacketParse����
	ACE_Message_Block* pMBHead = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketHeadLen());
	ACE_OS::memcpy(pMBHead->wr_ptr(), (const void*)pData, m_pPacketParse->GetPacketHeadLen());
	pMBHead->wr_ptr(m_pPacketParse->GetPacketHeadLen());
	m_pPacketParse->SetPacketHead(pMBHead->rd_ptr(), (uint32)pMBHead->length());

	if(u4Len != m_pPacketParse->GetPacketHeadLen() + m_pPacketParse->GetPacketDataLen())
	{
		pMBHead->release();
		return false;
	}
	m_pPacketParse->SetMessageHead(pMBHead);

	char* pBody = (char* )(&pData[0] + m_pPacketParse->GetPacketHeadLen());
	ACE_Message_Block* pMBBody = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketDataLen());
	ACE_OS::memcpy(pMBBody->wr_ptr(), (const void*)pBody, m_pPacketParse->GetPacketDataLen());
	pMBBody->wr_ptr(m_pPacketParse->GetPacketDataLen());
	m_pPacketParse->SetPacketData(pMBBody->rd_ptr(), (uint32)pMBBody->length());
	m_pPacketParse->SetMessageBody(pMBBody);

	//UDP��Ϊ�����������ӵģ���������ConnectID���ó�-1
	if(false == App_MakePacket::instance()->PutUDPMessageBlock(m_addrRemote, PACKET_PARSE, m_pPacketParse))
	{
		App_PacketParsePool::instance()->Delete(m_pPacketParse);
		OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]PutMessageBlock is error.\n"));
		return false;
	}

	m_atvInput = ACE_OS::gettimeofday();
	m_u4RecvSize += u4Len;
	m_u4RecvPacketCount++;

	return true;
}
