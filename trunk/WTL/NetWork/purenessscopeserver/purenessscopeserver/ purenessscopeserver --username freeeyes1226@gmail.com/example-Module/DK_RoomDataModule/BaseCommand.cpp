#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
	m_pServerObject = NULL;
	m_nCount        = 0;
}

CBaseCommand::~CBaseCommand(void)
{
}

bool CBaseCommand::Init()
{
	m_objSMDataManager.Init();
	m_objSMDataManager.ReadSMPool();

	return true;
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{	
	if(m_pServerObject == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL.\n"));
		return -1;
	}

	if(pMessage == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
		return -1;
	}

	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
	{
		mapLoginUserInfo::iterator f = m_mapLoginUserInfo.find(pMessage->GetMessageBase()->m_u4ConnectID);
		if(f != m_mapLoginUserInfo.end())
		{
			//�û����ӶϿ��������û���¼��Ϣ
			_LoginUserInfo objLoginUserInfo = (_LoginUserInfo)f->second;

			//���͸���Ϸģ���û��˳��ɹ���Ϣ
			IBuffPacket* pModuleBuffPacket = m_pServerObject->GetPacketManager()->Create();
			(*pModuleBuffPacket) << (uint32)objLoginUserInfo.m_u4PlayerID;
			(*pModuleBuffPacket) << (uint32)pMessage->GetMessageBase()->m_u4ConnectID;
			m_pServerObject->GetModuleMessageManager()->SendModuleMessage("DKPoker-GameCenter", COMMAND_MODULE_LOGOUT, pModuleBuffPacket, NULL);

			m_mapLoginUserInfo.erase(f);

			//����������ʱ����
			m_pServerObject->GetPacketManager()->Delete(pModuleBuffPacket);
		}
	}
	//�����û���¼��Ϣ
	else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_USERIN)
	{
		uint16     u2CommandID  = 0;
		VCHARS_STR strUserName;
		char       szPlayerNick[MAX_BUFF_20] = {'\0'};
		
		//OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));
		
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
		(*pBodyPacket) >> strUserName;
		
		//������ô����߼�
		if(strUserName.u1Len >= MAX_BUFF_20)
		{
			strUserName.u1Len = MAX_BUFF_20;
		}
		ACE_OS::memcpy(szPlayerNick, strUserName.text, strUserName.u1Len);
		int32 n4Pos = m_objSMDataManager.ReadPlayerSMPos(szPlayerNick);

		_UserPlayer* pUserPlayer = m_objSMDataManager.GetUserPlayer(n4Pos);

		//��������(TCP)
		if(NULL != pUserPlayer)
		{
			//�����¼�ɹ�������ӵ���Ӧ�ı���
			mapLoginUserInfo::iterator f = m_mapLoginUserInfo.find(pMessage->GetMessageBase()->m_u4ConnectID);
			if(f == m_mapLoginUserInfo.end())
			{
				_LoginUserInfo objLoginUserInfo;
				objLoginUserInfo.m_u4PlayerID = pUserPlayer->m_u4PlayerID;
				sprintf_safe(objLoginUserInfo.m_szPlayerNick, MAX_BUFF_20, "%s", szPlayerNick);
				m_mapLoginUserInfo.insert(mapLoginUserInfo::value_type(pMessage->GetMessageBase()->m_u4ConnectID, objLoginUserInfo));
			}

			//���͸���Ϸģ���û���¼�ɹ���Ϣ
			IBuffPacket* pModuleBuffPacket = m_pServerObject->GetPacketManager()->Create();
			(*pModuleBuffPacket) << (uint32)n4Pos;
			(*pModuleBuffPacket) << (uint32)pUserPlayer->m_u4PlayerID;
			(*pModuleBuffPacket) << (uint32)pMessage->GetMessageBase()->m_u4ConnectID;
			m_pServerObject->GetModuleMessageManager()->SendModuleMessage("DKPoker-GameCenter", COMMAND_MODULE_LOGIN, pModuleBuffPacket, NULL);

			//����������ʱ����
			m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
			m_pServerObject->GetPacketManager()->Delete(pModuleBuffPacket);


			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			uint16 u2PostCommandID = COMMAND_RETURN_USERIN;
			(*pResponsesPacket) << u2PostCommandID;
			(*pResponsesPacket) << pUserPlayer->m_u4PlayerID;
			if(NULL != m_pServerObject->GetConnectManager())
			{
				//��������
				m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
		else
		{
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			uint16 u2PostCommandID = COMMAND_RETURN_USERIN;
			(*pResponsesPacket) << u2PostCommandID;
			(*pResponsesPacket) << (uint32)PLAYER_INVALID_ID;
			if(NULL != m_pServerObject->GetConnectManager())
			{
				//��������
				m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}
	else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_USEROUT)
	{
		uint16     u2CommandID  = 0;
		uint32     u4PlayerID   = PLAYER_INVALID_ID;

		//OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));

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
		(*pBodyPacket) >> u4PlayerID;

		mapLoginUserInfo::iterator f = m_mapLoginUserInfo.find(pMessage->GetMessageBase()->m_u4ConnectID);
		if(f != m_mapLoginUserInfo.end())
		{
			//�����ڴ���뿪
			_LoginUserInfo objLoginInfo = (_LoginUserInfo)f->second;
			m_objSMDataManager.LeaveDBData(objLoginInfo.m_szPlayerNick);
			m_mapLoginUserInfo.erase(f);
		}

		//���͸���Ϸģ���û��˳��ɹ���Ϣ
		IBuffPacket* pModuleBuffPacket = m_pServerObject->GetPacketManager()->Create();
		(*pModuleBuffPacket) << (uint32)u4PlayerID;
		(*pModuleBuffPacket) << (uint32)pMessage->GetMessageBase()->m_u4ConnectID;
		m_pServerObject->GetModuleMessageManager()->SendModuleMessage("DKPoker-GameCenter", COMMAND_MODULE_LOGOUT, pModuleBuffPacket, NULL);

		//����������ʱ����
		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
		m_pServerObject->GetPacketManager()->Delete(pModuleBuffPacket);

		//��������(TCP)
		IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
		uint16 u2PostCommandID = COMMAND_RETURN_USEROUT;
		(*pResponsesPacket) << u2PostCommandID;
		(*pResponsesPacket) << (uint32)0;
		if(NULL != m_pServerObject->GetConnectManager())
		{
			//��������
			m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
		}
		else
		{
			OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
		}		
	}


	return 0;
}
