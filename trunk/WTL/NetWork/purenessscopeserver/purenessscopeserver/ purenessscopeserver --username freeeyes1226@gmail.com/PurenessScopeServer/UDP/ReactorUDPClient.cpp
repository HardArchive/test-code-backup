#include "ReactorUDPClient.h"

CReactorUDPClient::CReactorUDPClient(void)
{
	m_pClientUDPMessage = NULL;
	m_u4RecvPacketCount = 0;
	m_u4SendPacketCount = 0;
	m_u4RecvSize        = 0;
	m_u4SendSize        = 0;
}

CReactorUDPClient::~CReactorUDPClient(void)
{
}

int CReactorUDPClient::OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Reactor* pReactor, IClientUDPMessage* pClientUDPMessage)
{
	if(m_skRemote.open(AddrRemote) == -1)
	{
		ACE_DEBUG((LM_ERROR, "[CReactorUDPClient::OpenAddress]Open error(%d).\n", errno));
		return -1;
	}

	reactor(pReactor);

	m_pClientUDPMessage = pClientUDPMessage; 

	if(-1 == this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK))
	{
		ACE_DEBUG((LM_ERROR, "[CReactorUDPClient::OpenAddress] Addr is register_handler error(%d).\n", errno));
		return -1;
	}
	return 0;
}

void CReactorUDPClient::Close()
{
	ACE_Reactor_Mask close_mask = ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
	this->reactor()->remove_handler(this, close_mask);
	m_skRemote.close();
}

ACE_HANDLE CReactorUDPClient::get_handle(void) const
{
	return m_skRemote.get_handle();
}

int CReactorUDPClient::handle_input(ACE_HANDLE fd)
{
	if(fd == ACE_INVALID_HANDLE)
	{
		ACE_DEBUG((LM_ERROR, "[CReactorUDPClient::handle_input]fd is ACE_INVALID_HANDLE.\n"));
		return -1;
	}
	
	char szBuff[MAX_UDP_PACKET_LEN] = {'\0'};

	int nDataLen = m_skRemote.recv(szBuff, MAX_UDP_PACKET_LEN, m_addrRemote);
	if(nDataLen > 0)
	{
		CheckMessage(szBuff, (uint32)nDataLen);
	}

	return 0;
}

int CReactorUDPClient::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	if(handle == ACE_INVALID_HANDLE)
	{
		ACE_DEBUG((LM_ERROR, "[CReactorUDPClient::handle_close]close_mask = %d.\n", (uint32)close_mask));
	}
	
	Close();
	return 0;
}

bool CReactorUDPClient::SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort)
{
	ACE_INET_Addr AddrRemote;
	int nErr = AddrRemote.set(nPort, szIP);
	if(nErr != 0)
	{
		OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::SendMessage]set_address error[%d].\n", errno));
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
		ACE_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
		SAFE_DELETE_ARRAY(pMessage);
		return false;
	}
}

_ClientConnectInfo CReactorUDPClient::GetClientConnectInfo()
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

bool CReactorUDPClient::CheckMessage(const char* pData, uint32 u4Len)
{
	if(NULL == m_pClientUDPMessage || NULL == pData)
	{
		return false;
	}

	m_pClientUDPMessage->RecvData(pData, u4Len);

	m_atvInput = ACE_OS::gettimeofday();
	m_u4RecvSize += u4Len;
	m_u4RecvPacketCount++;

	return true;
}
