#include "ProactorUDPClient.h"

CProactorUDPClient::CProactorUDPClient(void)
{
	m_pClientUDPMessage = NULL;
	m_u4RecvPacketCount = 0;
	m_u4SendPacketCount = 0;
	m_u4RecvSize        = 0;
	m_u4SendSize        = 0;
}

CProactorUDPClient::~CProactorUDPClient(void)
{
}

int CProactorUDPClient::OpenAddress(const ACE_INET_Addr& AddrLocal, ACE_Proactor* pProactor, IClientUDPMessage* pClientUDPMessage)
{
	if(m_skRemote.open(AddrLocal) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::OpenAddress]Open error(%d).\n", errno));
		return -1;
	}

	char* pCompletionKey = NULL;
	if(m_Read.open(*this, m_skRemote.get_handle(), pCompletionKey, pProactor) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::OpenAddress]m_Read error.\n"));
		return -1;
	}

	if(m_Write.open(*this, m_skRemote.get_handle(), pCompletionKey, pProactor) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::OpenAddress]m_Write error.\n"));
		return -1;
	}

	char* pAct = NULL;
	ACE_Message_Block* pMBBuff = App_MessageBlockManager::instance()->Create(MAX_UDP_PACKET_LEN);
	if(NULL == pMBBuff)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::OpenAddress]pMBBuff is NULL.\n"));
		return -1;
	}

	m_pClientUDPMessage = pClientUDPMessage;
	size_t stRecvLen = MAX_UDP_PACKET_LEN;
	int nRecvSize = m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, pAct);   
	return nRecvSize;   
}  

void CProactorUDPClient::Close()
{
	m_Read.cancel();
	m_Write.cancel();
	m_skRemote.close();
}

void CProactorUDPClient::handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result)
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

		pMBBuff = App_MessageBlockManager::instance()->Create(MAX_UDP_PACKET_LEN);
	}
	else
	{
		pMBBuff = pMb;
	}

	char* pAct = NULL;
	
	if(NULL == pMBBuff)
	{
		OUR_DEBUG((LM_INFO, "[CProactorUDPClient::handle_read_dgram]pMBBuff is NULL.\n"));
		return;
	}
	else
	{
		size_t stRecvLen = MAX_UDP_PACKET_LEN;
		m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, pAct);  
	}
}

bool CProactorUDPClient::SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort)
{
	ACE_INET_Addr AddrRemote;
	int nErr = AddrRemote.set(nPort, szIP);
	if(nErr != 0)
	{
		OUR_DEBUG((LM_INFO, "[CProactorUDPClient::SendMessage]set_address error[%d].\n", errno));
		SAFE_DELETE_ARRAY(pMessage);
		return false;
	}

	int nSize = (int)m_skRemote.send(pMessage, u4Len, AddrRemote);
	if((uint32)nSize == u4Len)
	{
		m_atvOutput = ACE_OS::gettimeofday();
		m_u4SendSize += u4Len;
		m_u4SendPacketCount++;
		SAFE_DELETE_ARRAY(pMessage);
		return true;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::SendMessage]send error(%d).\n", errno));
		SAFE_DELETE_ARRAY(pMessage);
		return false;
	}
}

_ClientConnectInfo CProactorUDPClient::GetClientConnectInfo()
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

bool CProactorUDPClient::CheckMessage(ACE_Message_Block* pMbData, uint32 u4Len)
{
	if(NULL == m_pClientUDPMessage || NULL == pMbData)
	{
		return false;
	}

	m_pClientUDPMessage->RecvData(pMbData->rd_ptr(), u4Len);

	m_atvInput = ACE_OS::gettimeofday();
	m_u4RecvSize += u4Len;
	m_u4RecvPacketCount++;

	return true;
}

