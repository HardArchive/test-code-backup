#include "ProactorUDPHandler.h"

CProactorUDPHandler::CProactorUDPHandler(void)
{
	m_pPacketParse      = NULL;
	m_u4RecvPacketCount = 0;
	m_u4SendPacketCount = 0;
	m_u4RecvSize        = 0;
	m_u4SendSize        = 0;
}

CProactorUDPHandler::~CProactorUDPHandler(void)
{
}

int CProactorUDPHandler::OpenAddress(const ACE_INET_Addr& AddrLocal, ACE_Proactor* pProactor)
{
	if(m_skRemote.open(AddrLocal) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]Open error(%d).\n", errno));
		return -1;
	}

	sprintf_safe(m_szCompletionkey, MAX_BUFF_20, "CompUDP");
	sprintf_safe(m_szAct, MAX_BUFF_20, "ActUDP");

	if(m_Read.open(*this, m_skRemote.get_handle(), m_szCompletionkey, pProactor) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]m_Read error.\n"));
		return -1;
	}

	if(m_Write.open(*this, m_skRemote.get_handle(), m_szCompletionkey, pProactor) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]m_Write error.\n"));
		return -1;
	}

	ACE_Message_Block* pMBBuff = App_MessageBlockManager::instance()->Create(MAX_UDP_PACKET_LEN);
	if(NULL == pMBBuff)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]pMBBuff is NULL.\n"));
		return -1;
	}

	//初始化检查器
	m_TimeConnectInfo.Init(App_MainConfig::instance()->GetValid(), App_MainConfig::instance()->GetValidPacketCount(), App_MainConfig::instance()->GetValidRecvSize());

	m_pPacketParse = App_PacketParsePool::instance()->Create();

	size_t stRecvLen = MAX_UDP_PACKET_LEN;
	//OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::OpenAddress]pMBBuff=0x%08x.\n", pMBBuff));
	int nRecvSize = m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, m_szAct);   
	return nRecvSize;   
}  

void CProactorUDPHandler::Close()
{
	App_PacketParsePool::instance()->Delete(m_pPacketParse);
	m_Read.cancel();
	m_Write.cancel();
	m_skRemote.close();
}

void CProactorUDPHandler::handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result)
{
	//因为UDP是一次性发送完整的数据包，所以不必做粘包处理
	ACE_Message_Block* pMBBuff = NULL;

	ACE_Message_Block* pMb = result.message_block();
	int nPacketLen = (int)result.bytes_transferred();
	int nTran = (int)result.bytes_transferred();

	result.remote_address(m_addrRemote);

	if(nPacketLen != 0 && nTran != 0)
	{
		//处理数据
		CheckMessage(pMb, (uint32)nTran);

		pMb->release();
		m_pPacketParse = App_PacketParsePool::instance()->Create();

		pMBBuff = App_MessageBlockManager::instance()->Create(MAX_UDP_PACKET_LEN);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]error=%d.\n", errno));
		//由于连接找不到远程对象，内存块不用释放，直接滚入下次IO
		pMBBuff = pMb;
	}

	if(NULL == pMBBuff)
	{
		OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]pMBBuff is NULL.\n"));
		return;
	}
	else
	{
		size_t stRecvLen = MAX_UDP_PACKET_LEN;
		//OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]pMBBuff=0x%08x.\n", pMBBuff));
		m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, m_szAct);  
	}
}

bool CProactorUDPHandler::SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead, uint16 u2CommandID)
{
	ACE_hrtime_t m_tvBegin = ACE_OS::gethrtime();

	ACE_INET_Addr AddrRemote;
	int nErr = AddrRemote.set(nPort, szIP);
	if(nErr != 0)
	{
		OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::SendMessage]set_address error[%d].\n", errno));
		SAFE_DELETE(pMessage);
		return false;
	}

	//如果需要拼接包头，则拼接包头
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

		uint32 u4DataLen = (uint32)pMbData->length();
		int nSize = (int)m_skRemote.send(pMbData->rd_ptr(), u4DataLen, AddrRemote);
		if((uint32)nSize == u4DataLen)
		{
			m_atvOutput = ACE_OS::gettimeofday();
			m_u4SendSize += u4Len;
			m_u4SendPacketCount++;
			SAFE_DELETE_ARRAY(pMessage);

			//统计发送信息
			uint32 u4Cost = (uint32)(ACE_OS::gethrtime() - m_tvBegin);
			App_CommandAccount::instance()->SaveCommandData(u2CommandID, u4Cost, PACKET_UDP, u4DataLen, u4Len, COMMAND_TYPE_OUT);

			//释放发送体
			pMbData->release();

			return true;
		}
		else
		{
			OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
			SAFE_DELETE_ARRAY(pMessage);

			//释放发送体
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

			//统计发送信息
			uint32 u4Cost = (uint32)(ACE_OS::gethrtime() - m_tvBegin);
			App_CommandAccount::instance()->SaveCommandData(u2CommandID, u4Cost, PACKET_UDP, u4Len, u4Len, COMMAND_TYPE_OUT);

			return true;
		}
		else
		{
			OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
			SAFE_DELETE_ARRAY(pMessage);
			return false;
		}
	}
}

_ClientConnectInfo CProactorUDPHandler::GetClientConnectInfo()
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

bool CProactorUDPHandler::CheckMessage(ACE_Message_Block* pMbData, uint32 u4Len)
{
	if(NULL == m_pPacketParse || NULL == pMbData)
	{
		return false;
	}

	if(u4Len <= m_pPacketParse->GetPacketHeadLen())
	{
		return false;
	}

	//将完整的数据包转换为PacketParse对象
	ACE_Message_Block* pMBHead = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketHeadLen());
	ACE_OS::memcpy(pMBHead->wr_ptr(), (const void*)pMbData->rd_ptr(), m_pPacketParse->GetPacketHeadLen());
	pMBHead->wr_ptr(m_pPacketParse->GetPacketHeadLen());

	m_pPacketParse->SetPacketHead(pMBHead->rd_ptr(), (uint32)pMBHead->length());
	if(u4Len != m_pPacketParse->GetPacketHeadLen() + m_pPacketParse->GetPacketDataLen())
	{
		return false;
	}

	m_pPacketParse->SetMessageHead(pMBHead);
	pMbData->rd_ptr(m_pPacketParse->GetPacketHeadLen());


	ACE_Message_Block* pMBBody = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketDataLen());
	ACE_OS::memcpy(pMBBody->wr_ptr(), (const void*)pMbData->rd_ptr(), m_pPacketParse->GetPacketDataLen());
	pMBBody->wr_ptr(m_pPacketParse->GetPacketDataLen());
	m_pPacketParse->SetPacketData(pMBBody->rd_ptr(), (uint32)pMBBody->length());
	m_pPacketParse->SetMessageBody(pMBBody);

	//UDP因为不是面向链接的
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

