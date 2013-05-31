#include "PlayerPoolCommand.h"

CPlayerPoolCommand::CPlayerPoolCommand(void)
:m_MapReadData(MAX_READ_OBJECT),m_MapOnlineData(MAX_ONLINE_OBJECT)
{
	Init();
	m_pServerObject = NULL;
}

CPlayerPoolCommand::~CPlayerPoolCommand(void)
{
}

void CPlayerPoolCommand::Init()
{
	SMKey key;
#ifdef WIN32	
	key = 1111;
#else
	key = ftok("/home/freeeyes/SMData/", 0);
#endif

	printf("[Test]%d.\n", key);
	m_UserPool.Init(key, MAX_READ_OBJECT + MAX_ONLINE_OBJECT);

	//���������ݴ���mapָ��
	for(int i = 0; i < m_UserPool.GetUsedObjectCount(); i++)
	{
		CPlayerData* pData = m_UserPool.GetUsedObject(i);
		if(pData->GetOnline() == true)
		{
			//����������û�������ӵ���½��
			m_MapOnlineData.AddMapData((string)pData->GetKey(), pData, false);
		}
		else
		{
			//��ӵ���ȡ��
			m_MapReadData.AddMapData((string)pData->GetKey(), pData, false);
		}
	}
}

int CPlayerPoolCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
	uint16 u2CommandID = 0;
	IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
	if(NULL == pBodyPacket)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
		return -1;
	}

	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);

	pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

	(*pBodyPacket) >> u2CommandID;

	//����ID�������ݴ���(����������)
	if(u2CommandID == COMMAND_PLAYINSERT)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		CPlayerData* pData = Do_PlayerInsert(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		if(NULL != pData)
		{
			//��������
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			VCHARM_STR strMData;
			strMData.u2Len = sizeof(CPlayerData);
			strMData.text  = (const char* )pData;

			(*pResponsesPacket) << (uint16)COMMAND_PLAYINSERT;
			(*pResponsesPacket) << (uint8)1;         //1��ʾ������
			(*pResponsesPacket) << strMData;

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
		else
		{
			//��������
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

			(*pResponsesPacket) << (uint16)COMMAND_PLAYINSERT;
			(*pResponsesPacket) << (uint8)0;         //0��ʾû������

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}

	//(������ݸ���)
	if(u2CommandID == COMMAND_PLAYUPDATE)
	{
		VCHARM_STR strMName;
		(*pBodyPacket) >> strMName;
		CPlayerData* pData = (CPlayerData* )strMName.text;
		Do_PlayerUpdate(pData);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
	}

	//(�������ɾ��)
	if(u2CommandID == COMMAND_PLAYDELETE)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		Do_PlayerDelete(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
	}

	//(������ݲ�ѯ)
	if(u2CommandID == COMMAND_PLAYSEACH)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		CPlayerData* pData = Do_PlayerSearch(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		if(pData != NULL)
		{
			//��������
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			VCHARM_STR strMData;
			strMData.u2Len = sizeof(CPlayerData);
			strMData.text  = (const char* )pData;

			(*pResponsesPacket) << (uint16)COMMAND_PLAYINSERT;
			(*pResponsesPacket) << (uint8)1;             //1��ʾ������
			(*pResponsesPacket) << strMData;

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
		else
		{
			//��������
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

			(*pResponsesPacket) << (uint16)COMMAND_PLAYINSERT;
			(*pResponsesPacket) << (uint8)0;             //0��ʾû������

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}

	}

	//(������ݵ�½)
	if(u2CommandID == COMMAND_PLAYLOGIN)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		CPlayerData* pData = Do_PlayerLogin(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		if(pData != NULL)
		{
			//��������
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			VCHARM_STR strMData;
			strMData.u2Len = sizeof(CPlayerData);
			strMData.text  = (const char* )pData;

			(*pResponsesPacket) << (uint16)COMMAND_PLAYLOGIN;
			(*pResponsesPacket) << (uint8)1;             //1��ʾ������
			(*pResponsesPacket) << strMData;

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
		else
		{
			//��������
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

			(*pResponsesPacket) << (uint16)COMMAND_PLAYLOGIN;
			(*pResponsesPacket) << (uint8)0;             //0��ʾû������

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}


	//(��������뿪)
	if(u2CommandID == COMMAND_PLAYLOGOFF)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		bool blState = Do_PlayerLogOff(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		//��������
		IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

		(*pResponsesPacket) << (uint16)COMMAND_PLAYLOGOFF;
		if(true == blState)
		{
			(*pResponsesPacket) << (uint8)1;             //1��ʾ�뿪�ɹ�
		}
		else
		{
			(*pResponsesPacket) << (uint8)0;             //1��ʾ�뿪ʧ��
		}

		if(NULL != m_pServerObject->GetConnectManager())
		{
			m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
		}
		else
		{
			OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
		}
	}

	return 0;
}

CPlayerData* CPlayerPoolCommand::Do_PlayerInsert(const char* pPlayerNick)
{
	string strPlayerNick = pPlayerNick;
	
	//�����е��б��в��ң��ɶ��ĺ����ߵ�map�в��ң�
	CPlayerData* pPlayerDataRead = m_MapReadData.SearchMapData(strPlayerNick);
	if(NULL != pPlayerDataRead)
	{
		return NULL;
	}

	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL != pPlayerDataOnline)
	{
		return NULL;
	}

	//��IO�����
	CPlayerData* pPlayerIO = m_IOData.GetPlayer(pPlayerNick);
	if(NULL != pPlayerIO)
	{
		//������ݴ��ڣ��򷵻ش���ʧ�ܣ�pPlayerIO��Ҫ�Լ�����
		SAFE_DELETE(pPlayerIO);
		return NULL;
	}

	//����½���Ƿ����㹻�Ŀռ䡣
	if((int)m_MapOnlineData.GetSize() >= MAX_ONLINE_OBJECT)
	{
		//��½�����ˣ��������½��
		return NULL;
	}

	//�鿴������Ƿ��Ѿ�ȫ��������ǣ���Ҫ����һ������õĶ�ȡ���û�
	if((int)m_MapOnlineData.GetSize() + (int)m_MapOnlineData.GetSize() >= MAX_ONLINE_OBJECT + MAX_READ_OBJECT)
	{
		//�Ӷ�ȡ����ɾ��һ������õ�����
		CPlayerData* pPlayerDataRead = m_MapReadData.GetDeleteKey();
		if(pPlayerDataRead != NULL)
		{
			//��Ҫ�����ȡ���ݣ��Ƿ���ҪIO�洢
			const time_t ttHeadTimeStamp = m_UserPool.GetObjectHeadTimeStamp(pPlayerDataRead);
			if(ttHeadTimeStamp < pPlayerDataRead->GetTimeStamp())
			{
				//DS��δд������ݣ�����������Ҫд��ģ���������һ��IOд�롣
				m_IOData.SavePlayer(pPlayerDataRead);
			}

			//ɾ�������ڴ浥Ԫ����ִ�е�½��->��ȡ�أ���������
			m_UserPool.DeleteObject(pPlayerDataRead);
			m_MapReadData.DelMapData(pPlayerDataRead->GetKey(), false);
		}
	}

	//�����µĽ�ɫ
	CPlayerData* pData = m_UserPool.NewObject();
	if(NULL == pData)
	{
		return NULL;
	}

	pData->Create(pPlayerNick);
	pData->SetOnline(true);
	m_MapOnlineData.AddMapData(pPlayerNick, pData, false);
	return pData;
}

bool CPlayerPoolCommand::Do_PlayerUpdate(CPlayerData* pPlayerData)
{
	if(NULL == pPlayerData)
	{
		return false;
	}

	string strPlayerNick = pPlayerData->GetKey();
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		return false;
	}
	else
	{
		memcpy(pPlayerDataOnline, pPlayerData, sizeof(CPlayerData));

		//����ʱ���
		pPlayerDataOnline->SetTimeStamp();
		return true;
	}
}

bool CPlayerPoolCommand::Do_PlayerDelete(const char* pPlayerNick)
{
	//�û�����Online���ߵ�ʱ�����ɾ��
	string strPlayerNick = pPlayerNick;
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		return false;
	}

	//ɾ��IO����
	m_IOData.DeletePlayer(pPlayerNick);

	//��������ڴ��е�����
	m_UserPool.DeleteObject(pPlayerDataOnline);
	m_MapOnlineData.DelMapData(pPlayerNick, false);

	return true;
}

CPlayerData* CPlayerPoolCommand::Do_PlayerSearch(const char* pPlayerNick)
{
	string strPlayerNick = pPlayerNick;
	//���������û��в��ң����û�У����ڶ�ȡ������ȥ����
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		CPlayerData* pPlayerDataRead = m_MapReadData.SearchMapData(strPlayerNick);
		if(NULL == pPlayerDataRead)
		{
			//��������û�����û�У���ȡ����Ҳû�У���ȥIOȥ��
			//IO���Ҷ���������ҵ��ˣ���ӵ���ȡ����
			CPlayerData* pPlayerDataIO = m_IOData.GetPlayer(pPlayerNick);
			if(NULL == pPlayerDataIO)
			{
				return NULL;
			}
			else
			{
				//�ҵ��ˣ���ӵ���ȡ��
				CPlayerData* pPlayerDataDelete = m_MapReadData.GetDeleteKey();
				if(NULL != pPlayerDataDelete)
				{
					m_UserPool.DeleteObject(pPlayerDataDelete);
					m_MapReadData.DelMapData(pPlayerNick, false);
				}

				//����µĹ����ڴ�ڵ�
				CPlayerData* pPlayerDataNew = m_UserPool.NewObject();
				if(NULL != pPlayerDataNew)
				{
					SAFE_DELETE(pPlayerDataDelete);
					return NULL;
				}

				ACE_OS::memcpy(pPlayerDataNew, pPlayerDataIO, sizeof(CPlayerData));
				m_MapReadData.AddMapData(pPlayerDataNew->GetKey(), pPlayerDataNew, false);
				SAFE_DELETE(pPlayerDataDelete);
				return pPlayerDataNew;
			}
		}
		else
		{
			return pPlayerDataRead;
		}
	}
	else
	{
		return pPlayerDataOnline;
	}
}

CPlayerData* CPlayerPoolCommand::Do_PlayerLogin(const char* pPlayerNick)
{
	string strPlayerNick = pPlayerNick;
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		//������ڵ�½���У�ȥ��ȡ�ز���
		CPlayerData* pPlayerDataRead = m_MapReadData.SearchMapData(pPlayerNick);
		if(NULL == pPlayerDataRead)
		{
			//��IO����ȥ����
			CPlayerData* pPlayerDataIO = m_IOData.GetPlayer(pPlayerNick);
			if(NULL == pPlayerDataIO)
			{
				return NULL;
			}
			else
			{
				//�ҵ��ˣ���ӵ���½����ȥ
				if(m_MapOnlineData.GetSize() >= MAX_ONLINE_OBJECT)
				{
					//��½�����ˣ�����������ˡ�
					SAFE_DELETE(pPlayerDataIO);
					return NULL;
				}
				else
				{
					//����µĹ����ڴ�ڵ�
					CPlayerData* pPlayerDataNew = m_UserPool.NewObject();
					if(NULL != pPlayerDataNew)
					{
						SAFE_DELETE(pPlayerDataIO);
						return NULL;
					}

					ACE_OS::memcpy(pPlayerDataNew, pPlayerDataIO, sizeof(CPlayerData));
					pPlayerDataNew->SetOnline(true);
					m_MapOnlineData.AddMapData(pPlayerDataNew->GetKey(), pPlayerDataNew, false);
					SAFE_DELETE(pPlayerDataIO);
					return pPlayerDataNew;
				}
			}
		}
		else
		{
			//����ȡ���е�����Ǩ�Ƶ���½����
			if((int)m_MapOnlineData.GetSize() >= MAX_ONLINE_OBJECT)
			{
				//��½���Ѿ����ˣ������ڵ�½��
				return NULL;
			}
			else
			{
				//��ȡ�� -> ��½��
				pPlayerDataRead->SetOnline(true);
				m_MapReadData.DelMapData(pPlayerNick, false);
				m_MapOnlineData.AddMapData(strPlayerNick, pPlayerDataRead, false);
				return pPlayerDataRead;
			}
		}
	}
	else
	{
		return pPlayerDataOnline;
	}
}

bool CPlayerPoolCommand::Do_PlayerLogOff(const char* pPlayerNick) 
{
	string strPlayerNick = pPlayerNick;
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		return false;
	}
	else
	{
		//������һ��С���ɣ��ȴӶ�ȡ���л��һ�����滻���ݵ�ָ��
		//�ж����ָ���ǲ�����Ҫд��IO�������Ҫ��д�룬����Ҫ��ֱ��ɾ��
		CPlayerData* pPlayerDataRead = m_MapReadData.GetDeleteKey();
		if(pPlayerDataRead == NULL)
		{
			//û����Ҫ�Ƴ������ݣ���ֱ�ӷ����ȡ��
			pPlayerDataOnline->SetOnline(false);
			m_MapOnlineData.DelMapData(pPlayerNick, false);
			m_MapReadData.AddMapData(pPlayerNick, pPlayerDataOnline, false);
		}
		else
		{
			//��Ҫ�����ȡ���ݣ��Ƿ���ҪIO�洢
			const time_t ttHeadTimeStamp = m_UserPool.GetObjectHeadTimeStamp(pPlayerDataRead);
			if(ttHeadTimeStamp < pPlayerDataRead->GetTimeStamp())
			{
				//DS��δд������ݣ�����������Ҫд��ģ���������һ��IOд�롣
				m_IOData.SavePlayer(pPlayerDataRead);
			}

			//ɾ�������ڴ浥Ԫ����ִ�е�½��->��ȡ�أ���������
			m_UserPool.DeleteObject(pPlayerDataRead);
			m_MapReadData.DelMapData(pPlayerDataRead->GetKey(), false);
			m_MapOnlineData.DelMapData(pPlayerNick, false);
			m_MapReadData.AddMapData(pPlayerNick, pPlayerDataOnline, false);
		}

		return true;
	}
}

void CPlayerPoolCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}
