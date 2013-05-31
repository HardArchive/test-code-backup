#include "ServerParse.h"

CServerParse::CServerParse()
{
	//���������ð�ͷ�ĳ��ȣ���Ϊ�󲿷ֵİ�ͷ�����ǹ̶��ġ����峤���ǿɱ�ġ�
	m_u4PacketHead      = PACKET_HEAD;
	m_u4PacketCommandID = 0;
	m_u4PacketData      = 0;

	m_blIsHead          = false;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

CServerParse::~CServerParse()
{
	OUR_DEBUG((LM_INFO, "[CServerParse::~CServerParse].\n"));
}

void CServerParse::Init()
{
	m_u4PacketHead      = PACKET_HEAD;
	m_u4PacketCommandID = 0;
	m_u4PacketData      = 0;

	m_blIsHead          = false;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

uint32 CServerParse::GetPacketHeadLen()
{
	return m_u4PacketHead;
}

uint32 CServerParse::GetPacketDataLen()
{
	return m_u4PacketData;
}

uint16 CServerParse::GetPacketCommandID()
{
	return m_u4PacketCommandID;
}

bool CServerParse::GetIsHead()
{
	return m_blIsHead;
}

bool CServerParse::SetPacketHead(char* pData, uint32 u4Len)
{
	//��������Լ��԰�ͷ�ķ�������Ҫ�����������ȡ�
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

bool CServerParse::SetPacketData(char* pData, uint32 u4Len)
{
	//��������������ڵ�һЩ���ݣ������ͷ������CommandID����ô����Ͳ�����������
	if(u4Len >= sizeof(uint16))
	{
		ACE_OS::memcpy(&m_u4PacketCommandID, pData, sizeof(uint16));
		m_blIsHead = false;
		return true;
	}
	else
	{
		m_blIsHead = false;
		return false;
	}
}

bool CServerParse::SetMessageHead(ACE_Message_Block* pmbHead)
{
	m_pmbHead = pmbHead;
	return true;
}

bool CServerParse::SetMessageBody(ACE_Message_Block* pmbBody)
{
	m_pmbBody = pmbBody;
	return true;
}

ACE_Message_Block* CServerParse::GetMessageHead()
{
	return m_pmbHead;
}

ACE_Message_Block* CServerParse::GetMessageBody()
{
	return m_pmbBody;
}

uint32 CServerParse::MakePacketLength(uint32 u4DataLen)
{
	return u4DataLen + sizeof(uint32);
}

bool CServerParse::MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData)
{
	if(pMbData == NULL)
	{
		return false;
	}

	//ƴװ���ݰ�
	ACE_OS::memcpy(pMbData->wr_ptr(), (const void*)&u4Len, sizeof(uint32));
	ACE_OS::memcpy(pMbData->wr_ptr() + sizeof(uint32), (const void*)pData, u4Len);
	pMbData->wr_ptr(u4Len + sizeof(uint32));

	return true;
}

void CServerParse::Close()
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

