#include "CommandAccount.h"

CCommandAccount::CCommandAccount()
{
	m_u1CommandAccount = 0;
}

CCommandAccount::~CCommandAccount()
{
	OUR_DEBUG((LM_ERROR, "CCommandAccount::~CCommandAccount].\n"));
	Close();
}

void CCommandAccount::Init(uint8 u1CommandAccount)
{
	m_u1CommandAccount = u1CommandAccount;
}

void CCommandAccount::Close()
{
	for(mapCommandDataList::iterator itorFreeB = m_mapCommandDataList.begin(); itorFreeB != m_mapCommandDataList.end(); itorFreeB++)
	{
		_CommandData* pCommandData = (_CommandData* )itorFreeB->second;
		SAFE_DELETE(pCommandData);
	}

	m_mapCommandDataList.clear();
	m_u1CommandAccount = 0;
}

bool CCommandAccount::SaveCommandData(uint16 u2CommandID, uint64 u8CommandCost, uint8 u1PacketType, uint32 u4PacketSize, uint32 u4CommandSize, uint8 u1CommandType, ACE_Time_Value tvTime)
{
	//���ͳ�ƿ��ش򿪣��ſ�ʼ��¼ͳ����Ϣ
	if(m_u1CommandAccount == 0)
	{
		return true;
	}

	//���Ҳ����
	mapCommandDataList::iterator f = m_mapCommandDataList.find(u2CommandID);
	if(f != m_mapCommandDataList.end())
	{
		//����Ѿ����ڣ���ֱ�����
		_CommandData* pCommandData = (_CommandData* )f->second;
		if(pCommandData != NULL)
		{
			pCommandData->m_u4CommandCount++;
			pCommandData->m_u8CommandCost += u8CommandCost;
			pCommandData->m_u1PacketType  = u1PacketType;
			pCommandData->m_u4PacketSize  += u4PacketSize;
			pCommandData->m_u4CommandSize += u4CommandSize;
			pCommandData->m_tvCommandTime = tvTime;
		}
		else
		{
			return false;
		}
	}
	else
	{
		//����µ�����ͳ����Ϣ
		_CommandData* pCommandData =  new _CommandData();
		if(pCommandData != NULL)
		{
			pCommandData->m_u2CommandID   = u2CommandID;
			pCommandData->m_u1CommandType = u1CommandType;
			pCommandData->m_u8CommandCost = u8CommandCost;
			pCommandData->m_u1PacketType  = u1PacketType;
			pCommandData->m_u4PacketSize  += u4PacketSize;
			pCommandData->m_u4CommandSize += u4CommandSize;
			pCommandData->m_tvCommandTime = tvTime;

			m_mapCommandDataList.insert(mapCommandDataList::value_type(u2CommandID, pCommandData));
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CCommandAccount::SaveCommandDataLog()
{

	return true;
}
