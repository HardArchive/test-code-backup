#include "PacketParse.h"

CPacketParse::CPacketParse(void)
{
	//在这里设置包头的长度，因为大部分的包头长度是固定的。包体长度是可变的。
	m_u4PacketHead      = PACKET_HEAD;
	m_u2PacketCommandID = 0;
	m_u4PacketData      = 0;
	m_u4HeadSrcSize     = 0;
	m_u4BodySrcSize     = 0;

  //这里修改属于你的包解析版本号
  sprintf_safe(m_szPacketVersion, MAX_BUFF_20, "0.90");

	m_blIsHead          = false;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

CPacketParse::~CPacketParse(void)
{
}

void CPacketParse::Init()
{
	m_u4PacketHead      = PACKET_HEAD;
	m_u2PacketCommandID = 0;
	m_u4PacketData      = 0;
	m_u4HeadSrcSize     = 0;
	m_u4BodySrcSize     = 0;

	m_blIsHead          = false;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

uint32 CPacketParse::GetPacketHeadLen()
{
	return m_u4PacketHead;
}

uint32 CPacketParse::GetPacketDataLen()
{
	return m_u4PacketData;
}

uint16 CPacketParse::GetPacketCommandID()
{
	return m_u2PacketCommandID;
}

bool CPacketParse::GetIsHead()
{
	return m_blIsHead;
}

bool CPacketParse::SetPacketHead(char* pData, uint32 u4Len)
{
	//这里添加自己对包头的分析，主要分析出包长度。
	m_u4HeadSrcSize = u4Len;
	if(u4Len == sizeof(uint32))
	{
		ACE_OS::memcpy(&m_u4PacketData, pData, sizeof(uint32));
		m_blIsHead = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool CPacketParse::SetPacketData(char* pData, uint32 u4Len)
{
	//这里分析出包体内的一些数据，如果包头包含了CommandID，那么包体就不必做解析。
	m_u4BodySrcSize = u4Len;
	if(u4Len >= sizeof(uint16))
	{
		ACE_OS::memcpy(&m_u2PacketCommandID, pData, sizeof(uint16));
		m_blIsHead = false;
		return true;
	}
	else
	{
		m_blIsHead = false;
		return false;
	}
}

uint32 CPacketParse::GetPacketHeadSrcLen()
{
	return m_u4HeadSrcSize;
}

uint32 CPacketParse::GetPacketBodySrcLen()
{
	return m_u4BodySrcSize;
}

bool CPacketParse::SetMessageHead(ACE_Message_Block* pmbHead)
{
	m_pmbHead = pmbHead;
	return true;
}

bool CPacketParse::SetMessageBody(ACE_Message_Block* pmbBody)
{
	m_pmbBody = pmbBody;
	return true;
}

ACE_Message_Block* CPacketParse::GetMessageHead()
{
	return m_pmbHead;
}

ACE_Message_Block* CPacketParse::GetMessageBody()
{
	return m_pmbBody;
}

uint32 CPacketParse::MakePacketLength(uint32 u4DataLen)
{
	return u4DataLen + sizeof(uint32);
}

bool CPacketParse::MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData)
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

void CPacketParse::Close()
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

const char* CPacketParse::GetPacketVersion()
{
  return m_szPacketVersion;
}
