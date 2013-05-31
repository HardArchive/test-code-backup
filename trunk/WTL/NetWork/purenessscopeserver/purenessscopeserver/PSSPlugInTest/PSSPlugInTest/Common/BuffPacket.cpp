// BuffPacket.h
// ���ﶨ��������ݰ������ݻ���
// ʵ�ʵļ�֣�����������Ƿ�����Ҫ�ģ�ͬ���������Ƿ�����Ҫ�ġ�
// add by freeeyes
// 2009-01-02

#include "BuffPacket.h"

CBuffPacket::CBuffPacket(int nSize)
{
	Init(nSize);
}

CBuffPacket::~CBuffPacket(void)
{
	//OUR_DEBUG((LM_ERROR, "[CBuffPacket::~CBuffPacket].\n"));
	//Close();
}

bool CBuffPacket::Init(int nSize)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	try
	{
		m_szError[0] = '\0';
		if(nSize >= MAX_PACKET_SIZE)
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::Init] nSize [%d] is more than MAX_PACKET_SIZE.\n", nSize));
			char szError[MAX_BUFF_500] = {'\0'};
			sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::Init] nSize [%d] is more than MAX_PACKET_SIZE.", nSize);
			throw szError;
		}

		
		//��ʼ�������ݽṹ
		m_u4ReadPtr       = 0;
		m_u4WritePtr      = 0;
		m_u4PacketCount   = 0;
		m_u4PacketLen     = ((int)ceil((double)nSize/(double)DEFINE_PACKET_ADD))*DEFINE_PACKET_ADD;

		m_szData          = (char *)App_ACEMemory::instance()->malloc(m_u4PacketLen);

		//OUR_DEBUG((LM_ERROR, "[CBuffPacket::Init] nSize [%d], m_szData=[0x%08x].\n", m_u4PacketLen, m_szData));

		if(NULL == m_szData)
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::Init] nSize [%d] is new error.\n", m_u4PacketLen));
			char szError[MAX_BUFF_500] = {'\0'};
			sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::Init] nSize [%d] is new error..", m_u4PacketLen);
			throw szError;
		}

		return true;
	}
	catch(const char* szError)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
		return false;
	}
}

bool CBuffPacket::Close()
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	try
	{
		if(m_u4PacketLen >= MAX_PACKET_SIZE)
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::Close] nSize [%d] is more than MAX_PACKET_SIZE.\n", m_u4PacketLen));
			char szError[MAX_BUFF_500] = {'\0'};
			sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::Close] nSize [%d] is more than MAX_PACKET_SIZE.", m_u4PacketLen);
			throw szError;
		}

		//OUR_DEBUG((LM_ERROR, "[CBuffPacket::Close] nSize [%d], m_szData=[0x%08x].\n", m_u4PacketLen, m_szData));

		if(NULL != m_szData)
		{
			App_ACEMemory::instance()->free((void* )m_szData);
			//delete m_szData;
			m_szData          = NULL;
			m_u4ReadPtr       = 0;
			m_u4WritePtr      = 0;
			m_u4PacketLen     = 0;
		}

		return true;
	}
	catch(const char* szError)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
		return false;
	}
}

bool CBuffPacket::Clear()
{
	m_u4ReadPtr       = 0;
	m_u4WritePtr      = 0;
	m_u4PacketCount   = 0;

	return true;
}

bool CBuffPacket::AddBuffPacket(uint32 u4Size)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	try
	{
		char* szNewData = NULL;

		if(u4Size + m_u4PacketLen >= MAX_PACKET_SIZE)
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::AddBuff] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", u4Size, m_u4PacketLen));
			char szError[MAX_BUFF_500] = {'\0'};
			sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::AddBuff] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", u4Size, m_u4PacketLen);
			throw szError;
		}

		//���浱ǰ����
		uint32 u4OldPacketLen = m_u4PacketLen;

		//��ʽ���´������ڴ��С
		u4Size = (uint32)(((int)ceil((double)u4Size/(double)DEFINE_PACKET_ADD))*DEFINE_PACKET_ADD);
		m_u4PacketLen += u4Size;

		szNewData = (char *)App_ACEMemory::instance()->malloc(m_u4PacketLen);

		if(NULL == szNewData)
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::AddBuff] nSize [%d] is new error.\n", m_u4PacketLen));
			char szError[MAX_BUFF_500] = {'\0'};
			sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::AddBuff] nSize [%d] is new error..", m_u4PacketLen);
			throw szError;
		}

		if(NULL == m_szData)
		{
			m_szData = szNewData;
		}
		else
		{
			//�ƶ��ڴ�
			ACE_OS::memmove(szNewData, m_szData, u4OldPacketLen);

			//ɾ���Ѿ����õ��ڴ�
			App_ACEMemory::instance()->free(m_szData);
			//delete m_szData;
			m_szData = szNewData;
		}


		return true;
	}
	catch(const char* szError)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
		return false;
	}
}


bool CBuffPacket::AddBuff(uint32 u4Size)
{
	try
	{
		char* szNewData = NULL;

		if(u4Size + m_u4PacketLen >= MAX_PACKET_SIZE)
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::AddBuff] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", u4Size, m_u4PacketLen));
			char szError[MAX_BUFF_500] = {'\0'};
			sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::AddBuff] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", u4Size, m_u4PacketLen);
			throw szError;
		}

		//���浱ǰ����
		uint32 u4OldPacketLen = m_u4PacketLen;

		//��ʽ���´������ڴ��С
		u4Size = (uint32)(((int)ceil((double)u4Size/(double)DEFINE_PACKET_ADD))*DEFINE_PACKET_ADD);
		m_u4PacketLen += u4Size;

		szNewData = (char *)App_ACEMemory::instance()->malloc(m_u4PacketLen);

		if(NULL == szNewData)
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::AddBuff] nSize [%d] is new error.\n", m_u4PacketLen));
			char szError[MAX_BUFF_500] = {'\0'};
			sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::AddBuff] nSize [%d] is new error..", m_u4PacketLen);
			throw szError;
		}

		if(NULL == m_szData)
		{
			m_szData = szNewData;
		}
		else
		{
			//�ƶ��ڴ�
			ACE_OS::memmove(szNewData, m_szData, u4OldPacketLen);

			//ɾ���Ѿ����õ��ڴ�
			App_ACEMemory::instance()->free(m_szData);
			//delete m_szData;
			m_szData = szNewData;
		}


		return true;
	}
	catch(const char* szError)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
		return false;
	}
}

const char* CBuffPacket::GetData()
{
	return m_szData;
}

uint32 CBuffPacket::GetPacketSize()
{
	return m_u4PacketLen;
}

uint32 CBuffPacket::GetPacketLen()
{
	return m_u4WritePtr;
}

uint32 CBuffPacket::GetReadLen()
{
	return m_u4ReadPtr;
}

uint32 CBuffPacket::GetWriteLen()
{
	return m_u4WritePtr;
}

uint32 CBuffPacket::GetHeadLen()
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if(m_u4PacketLen - m_u4ReadPtr > sizeof(uint32))
	{
		return *(uint32 *)ReadPtr();
	}
	else
	{
		return (uint32)0;
	}
}

void CBuffPacket::ReadBuffPtr(uint32 u4Size)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	ReadPtr(u4Size);
}

void CBuffPacket::ReadPtr(uint32 u4Size)
{
	if(m_u4ReadPtr + u4Size < m_u4PacketLen)
	{
		m_u4ReadPtr += u4Size;
	}
}

void CBuffPacket::WriteBuffPtr(uint32 u4Size)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	WritePtr(u4Size);
}

void CBuffPacket::WritePtr(uint32 u4Size)
{
	if(m_u4WritePtr + u4Size <= m_u4PacketLen)
	{
		m_u4WritePtr += u4Size;
	}
}

char* CBuffPacket::ReadPtr()
{
	return m_szData + m_u4ReadPtr;
}

void CBuffPacket::SetReadPtr(uint32 u4Pos)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if(u4Pos < m_u4PacketLen)
	{
		m_u4ReadPtr = u4Pos;
	}
}

char* CBuffPacket::WritePtr()
{
	return m_szData + m_u4WritePtr;
}

bool CBuffPacket::RollBack(uint32 u4Len)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	uint32 u4NewLen = 0;
	if(u4Len >= m_u4WritePtr)
	{
		if(u4Len > m_u4WritePtr)
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::RollBack] Error u4Len(%d) m_u4WritePtr(%d).\n", u4Len, m_u4WritePtr));
			return false;
		}
		m_u4WritePtr = 0;
		m_u4ReadPtr  = 0;
		return true;
	}
	else
	{
		u4NewLen = m_u4WritePtr - u4Len;
		ACE_OS::memmove(m_szData, m_szData + u4Len, u4NewLen);

		m_u4ReadPtr  = 0;
		m_u4WritePtr = u4NewLen;
		return true;
	}
}

bool CBuffPacket::WriteStream(const char* szData, uint32 u4Len)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if(u4Len + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::WriteStream] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", u4Len, m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::WriteStream] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", u4Len, m_u4PacketLen);
		return false;
	}

	if(m_u4WritePtr + u4Len > m_u4PacketLen)
	{
		if(false == AddBuff(u4Len))
		{
			OUR_DEBUG((LM_ERROR, "[CBuffPacket::WriteStream] nSize = [%d] m_u4PacketLen = [%d] AddBuff false.\n", u4Len, m_u4PacketLen));
			return false;
		}

		//д���ı�����
		ACE_OS::memcpy(WritePtr(), szData, u4Len);
		WritePtr(u4Len);
		return true;
	}
	else
	{
		//д���ı�����
		ACE_OS::memcpy(WritePtr(), szData, u4Len);
		WritePtr(u4Len);
		return true;
	}
}

bool CBuffPacket::ReadStream(char*& pData, uint32 u4MaxLen, uint32 u4Len)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if(pData == NULL || u4MaxLen > m_u4PacketLen)
	{
		return false;
	}

	memcpy(pData, m_szData, u4Len);
	return true;
}

void CBuffPacket::SetPacketCount(uint32 u4PacketCount)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	m_u4PacketCount = u4PacketCount;
}

uint32 CBuffPacket::GetPacketCount()
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	return m_u4PacketCount;
}

CBuffPacket& CBuffPacket::operator >> (uint8& u1Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	u1Data = 0;
	if(m_u4ReadPtr <= m_u4WritePtr - sizeof(u1Data))
	{
		u1Data = *(uint8 *)ReadPtr();
		ReadPtr((uint32)sizeof(u1Data));
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator >> (uint16& u2Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	u2Data = 0;
	if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(u2Data))
	{
		u2Data = *(uint16 *)ReadPtr();
		ReadPtr((uint32)sizeof(u2Data));
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator >> (uint32& u4Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	u4Data = 0;
	if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(u4Data))
	{
		u4Data = *(uint32 *)ReadPtr();
		ReadPtr((uint32)sizeof(u4Data));
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator >> (uint64 &u8Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	u8Data = 0;
	if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(u8Data))
	{
		u8Data = *(uint64 *)ReadPtr();
		ReadPtr((uint32)sizeof(u8Data));
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator >> (float32& f4Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	f4Data = 0;
	if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(f4Data))
	{
		f4Data = *(float32 *)ReadPtr();
		ReadPtr((uint32)sizeof(f4Data));
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator >> (float64& f8Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	f8Data = 0;
	if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(f8Data))
	{
		f8Data = *(float64 *)ReadPtr();
		ReadPtr((uint32)sizeof(f8Data));
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator >> (VCHARS_STR& str)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	uint8 u1Len = 0;
	operator >> (u1Len);

	if(u1Len && m_u4ReadPtr <= m_u4WritePtr - u1Len)
	{
		str.text = (const char *)ReadPtr();
		str.u1Len = u1Len;
		ReadPtr((uint32)u1Len);
	}
	else
	{
		str.text = "";
		str.u1Len = 0;
	}
	return *this;
}

CBuffPacket& CBuffPacket::operator >> (VCHARM_STR& str)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	uint16 u2Len = 0;
	operator >> (u2Len);

	if(u2Len && m_u4ReadPtr <= m_u4WritePtr - u2Len)
	{
		str.text = (const char *)ReadPtr();
		str.u2Len = u2Len;
		ReadPtr((uint32)u2Len);
	}
	else
	{
		str.text = "";
		str.u2Len = 0;
	}
	return *this;
}

CBuffPacket& CBuffPacket::operator >> (VCHARB_STR& str)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	uint32 u4Len = 0;
	operator >> (u4Len);

	if(u4Len && m_u4ReadPtr <= m_u4WritePtr - u4Len)
	{
		str.text = (const char *)ReadPtr();
		str.u4Len = u4Len;
		ReadPtr((uint32)u4Len);
	}
	else
	{
		str.text = "";
		str.u4Len = 0;
	}
	return *this;
}

CBuffPacket& CBuffPacket::operator << (uint8 u1Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if( (uint32)sizeof(u1Data) + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(u1Data), m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(u1Data), m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(u1Data) >= GetPacketSize())
	{
		if(false == AddBuff(DEFINE_PACKET_ADD))
		{
			return *this;
		}
		else
		{
			*(uint8 *)WritePtr() = u1Data;
			WritePtr((uint32)sizeof(u1Data));
			return *this;
		}
	}
	else
	{
		*(uint8 *)WritePtr() = u1Data;
		WritePtr((uint32)sizeof(u1Data));
		return *this;
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator << (uint16 u2Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if( (uint32)sizeof(u2Data) + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(u2Data), m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(u2Data), m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(u2Data) >= GetPacketSize())
	{
		if(false == AddBuff(DEFINE_PACKET_ADD))
		{
			return *this;
		}
		else
		{
			*(uint16 *)WritePtr() = u2Data;
			WritePtr((uint32)sizeof(u2Data));
			return *this;
		}
	}
	else
	{
		*(uint16 *)WritePtr() = u2Data;
		WritePtr((uint32)sizeof(u2Data));
		return *this;
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator << (uint32 u4Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if( (uint32)sizeof(u4Data) + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(u4Data), m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(u4Data), m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(u4Data) >= GetPacketSize())
	{
		if(false == AddBuff(DEFINE_PACKET_ADD))
		{
			return *this;
		}
		else
		{
			*(uint32 *)WritePtr() = u4Data;
			WritePtr((uint32)sizeof(u4Data));
			return *this;
		}
	}
	else
	{
		*(uint32 *)WritePtr() = u4Data;
		WritePtr((uint32)sizeof(u4Data));
		return *this;
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator << (uint64 u8Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if( (uint32)sizeof(u8Data) + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(u8Data), m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(u8Data), m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(u8Data) >= GetPacketSize())
	{
		if(false == AddBuff(DEFINE_PACKET_ADD))
		{
			return *this;
		}
		else
		{
			*(uint64 *)WritePtr() = u8Data;
			WritePtr((uint32)sizeof(u8Data));
			return *this;
		}
	}
	else
	{
		*(uint64 *)WritePtr() = u8Data;
		WritePtr((uint32)sizeof(u8Data));
		return *this;
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator << (float32 f4Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if( (uint32)sizeof(f4Data) + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(f4Data), m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(f4Data), m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(f4Data) >= GetPacketSize())
	{
		if(false == AddBuff(DEFINE_PACKET_ADD))
		{
			return *this;
		}
		else
		{
			*(float32 *)WritePtr() = f4Data;
			WritePtr((uint32)sizeof(f4Data));
			return *this;
		}
	}
	else
	{
		*(float32 *)WritePtr() = f4Data;
		WritePtr((uint32)sizeof(f4Data));
		return *this;
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator << (float64 f8Data)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if( (uint32)sizeof(f8Data) + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(f8Data), m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(f8Data), m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(f8Data) >= GetPacketSize())
	{
		if(false == AddBuff(DEFINE_PACKET_ADD))
		{
			return *this;
		}
		else
		{
			*(float64 *)WritePtr() = f8Data;
			WritePtr((uint32)sizeof(f8Data));
			return *this;
		}
	}
	else
	{
		*(float64 *)WritePtr() = f8Data;
		WritePtr((uint32)sizeof(f8Data));
		return *this;
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator << (VCHARS_STR &str)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if( (uint32)sizeof(str.u1Len) + str.u1Len + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(str.u1Len) + str.u1Len, m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(str.u1Len) + str.u1Len, m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(str.u1Len) + str.u1Len >= GetPacketSize())
	{
		if(false == AddBuff((uint32)sizeof(str.u1Len) + str.u1Len))
		{
			return *this;
		}
		else
		{
			//д���ַ�������
			*this << str.u1Len;

			//д���ı�����
			ACE_OS::memcpy(WritePtr(), str.text, (int)str.u1Len);
			WritePtr(str.u1Len);
			return *this;
		}
	}
	else
	{
		//д���ַ�������
		*this << str.u1Len;

		//д���ı�����
		ACE_OS::memcpy(WritePtr(), str.text, (int)str.u1Len);
		WritePtr(str.u1Len);
		return *this;
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator << (VCHARM_STR &str)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if( (uint32)sizeof(str.u2Len) + str.u2Len + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(str.u2Len) + str.u2Len, m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(str.u2Len) + str.u2Len, m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(str.u2Len) + str.u2Len >= GetPacketSize())
	{
		if(false == AddBuff((uint32)sizeof(str.u2Len) + str.u2Len))
		{
			return *this;
		}
		else
		{
			//д���ַ�������
			*this << str.u2Len;

			//д���ı�����
			ACE_OS::memcpy(WritePtr(), str.text, (int)str.u2Len);
			WritePtr(str.u2Len);
			return *this;
		}
	}
	else
	{
		//д���ַ�������
		*this << str.u2Len;

		//д���ı�����
		ACE_OS::memcpy(WritePtr(), str.text, (int)str.u2Len);
		WritePtr(str.u2Len);
		return *this;
	}

	return *this;
}

CBuffPacket& CBuffPacket::operator << (VCHARB_STR &str)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if((uint32)sizeof(str.u4Len) + str.u4Len + m_u4PacketLen >= MAX_PACKET_SIZE)
	{
		OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(str.u4Len) + str.u4Len, m_u4PacketLen));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than MAX_PACKET_SIZE.\n", (uint32)sizeof(str.u4Len) + str.u4Len, m_u4PacketLen);
		return *this;
	}

	if(m_u4WritePtr + (uint32)sizeof(str.u4Len) + str.u4Len >= GetPacketSize())
	{
		if(false == AddBuff((uint32)sizeof(str.u4Len) + str.u4Len))
		{
			return *this;
		}
		else
		{
			//д���ַ�������
			*this << str.u4Len;

			//д���ı�����
			ACE_OS::memcpy(WritePtr(), str.text, (int)str.u4Len);
			WritePtr(str.u4Len);
			return *this;
		}
	}
	else
	{
		//д���ַ�������
		*this << str.u4Len;

		//д���ı�����
		ACE_OS::memcpy(WritePtr(), str.text, (int)str.u4Len);
		WritePtr(str.u4Len);
		return *this;
	}

	return *this;
}
