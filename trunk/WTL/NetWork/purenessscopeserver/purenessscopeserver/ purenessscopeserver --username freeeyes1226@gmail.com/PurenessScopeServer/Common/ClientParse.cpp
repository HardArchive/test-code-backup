#include "ClientParse.h"

CClientParse::CClientParse(void)
{
}

CClientParse::~CClientParse(void)
{
	OUR_DEBUG((LM_INFO, "[CClientParse::~CClientParse].\n"));
}

void CClientParse::Init()
{
	m_u4PacketHead      = MAX_PACKET_HEAD;
	m_u2PacketCommandID = 0;
	m_u4PacketData      = 0;

	m_blIsHead          = false;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

uint32 CClientParse::GetPacketHeadLen()
{
	return m_u4PacketHead;
}

uint32 CClientParse::GetPacketDataLen()
{
	return m_u4PacketData;
}

uint16 CClientParse::GetPacketCommandID()
{
	return m_u2PacketCommandID;
}

bool CClientParse::GetIsHead()
{
	return m_blIsHead;
}

bool CClientParse::SetPacketHead(char* pData, uint32 u4Len)
{
	//这里添加自己对包头的分析，主要分析出包长度。
	if(u4Len == sizeof(uint32))
	{
		ACE_OS::memcpy(&m_u4PacketData, &pData[0], sizeof(uint32));
		m_blIsHead = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool CClientParse::SetPacketData(char* pData, uint32 u4Len)
{
	//这里分析出包体内的一些数据，如果包头包含了CommandID，那么包体就不必做解析。
	if(u4Len >= sizeof(uint16))
	{
		//得到命令头
		ACE_OS::memcpy(&m_u2PacketCommandID, &pData[0], sizeof(uint16));
		m_blIsHead = false;
		return true;
	}
	else
	{
		m_blIsHead = false;
		return false;
	}
}

bool CClientParse::SetMessageHead(ACE_Message_Block* pmbHead)
{
	m_pmbHead = pmbHead;
	return true;
}

bool CClientParse::SetMessageBody(ACE_Message_Block* pmbBody)
{
	m_pmbBody = pmbBody;
	return true;
}

ACE_Message_Block* CClientParse::GetMessageHead()
{
	return m_pmbHead;
}

ACE_Message_Block* CClientParse::GetMessageBody()
{
	return m_pmbBody;
}

uint32 CClientParse::MakePacketLength(uint32 u4DataLen)
{
	return u4DataLen + sizeof(uint32);
}

bool CClientParse::MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData)
{
	if(pMbData == NULL)
	{
		return false;
	}

	//拼装数据包
	ACE_OS::memcpy(pMbData->wr_ptr(), (const void*)&u4Len, sizeof(uint32));
	ACE_OS::memcpy(pMbData->wr_ptr() + sizeof(uint32), (const void*)pData, u4Len);
	pMbData->wr_ptr(u4Len + sizeof(uint32));

	return true;
}

void CClientParse::Close()
{
	if(m_pmbHead != NULL)
	{
		m_pmbHead->release();
		m_pmbHead = NULL;
	}

	if(m_pmbBody != NULL)
	{
		m_pmbBody->release();
		m_pmbBody = NULL;
	}

	m_blIsHead = false;
}
