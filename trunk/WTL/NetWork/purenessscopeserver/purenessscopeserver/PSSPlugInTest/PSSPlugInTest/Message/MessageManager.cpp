// MessageService.h
// ������Ϣ������Ϣ���ɸ�������߼�������ȥִ��
// ���쵽�˹���ͼ��ݣ��о��¹���ĺ����ɣ�������д���������˼��
// add by freeeyes
// 2009-01-29

#include "MessageManager.h"

bool Delete_CommandInfo(_ClientCommandInfo* pClientCommandInfo)
{
	return pClientCommandInfo->m_u4CurrUsedCount == 0;
}

CMessageManager::CMessageManager(void)
{
}

CMessageManager::~CMessageManager(void)
{
	OUR_DEBUG((LM_INFO, "[CMessageManager::~CMessageManager].\n"));
	Close();
}

bool CMessageManager::DoMessage(IMessage* pMessage, uint16& u2CommandID)
{
	if(NULL == pMessage)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage] pMessage is NULL.\n"));
		return false;
	}

	//�Ÿ���Ҫ�̳е�ClientCommand��ȥ����
	bool bDeleteFlag = true;         //���ݰ��Ƿ������ɾ��
	//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d.\n", u2CommandID));

	CClientCommandList* pClientCommandList = GetClientCommandList(u2CommandID);
	if(pClientCommandList != NULL)
	{
		int nCount = pClientCommandList->GetCount();
		for(int i = 0; i < nCount; i++)
		{
			_ClientCommandInfo* pClientCommandInfo = pClientCommandList->GetClientCommandIndex(i);
			if(NULL != pClientCommandInfo && pClientCommandInfo->m_u1State == 0)
			{
				m_ThreadWriteLock.acquire();
				pClientCommandInfo->m_u4CurrUsedCount++;
				m_ThreadWriteLock.release();

				//��ð���
				_PacketInfo PacketInfoHead;
				_PacketInfo PacketInfoBody;

				pMessage->GetPacketHead(PacketInfoHead);
				pMessage->GetPacketBody(PacketInfoBody);

				//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d Begin.\n", u2CommandID));
				pClientCommandInfo->m_pClientCommand->DoMessage(pMessage, bDeleteFlag);
				//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d End.\n", u2CommandID));
				m_ThreadWriteLock.acquire();
				uint32 u4Cost = (uint32)pMessage->GetMessageBase()->m_ProfileTime.Stop();

				//���ͳ����Ϣ
				AppCommandAccount::instance()->SaveCommandData(u2CommandID, (uint64)u4Cost, 
					                                           pMessage->GetMessageBase()->m_u1PacketType, 
															   pMessage->GetMessageBase()->m_u4HeadSrcSize + pMessage->GetMessageBase()->m_u4BodySrcSize, 
															   (uint32)(PacketInfoHead.m_nDataLen + PacketInfoBody.m_nDataLen), 
															   COMMAND_TYPE_IN);

				if(pClientCommandInfo->m_u4CurrUsedCount > 0)
				{
					pClientCommandInfo->m_u4CurrUsedCount--;

					//�����һ���жϣ��ж��Ƿ���Ҫ�ر�
					if(pClientCommandInfo->m_u1State == 1)
					{
						CloseCommandInfo(pClientCommandInfo->m_szModuleName);
					}
				}
				m_ThreadWriteLock.release();
			}
		}		
	}

	if(true == bDeleteFlag)
	{
		App_MessagePool::instance()->Delete((CMessage* )pMessage);
	}

	return true;
}

CClientCommandList* CMessageManager::GetClientCommandList(uint16 u2CommandID)
{
	mapClientCommand::iterator f = m_mapClientCommand.find(u2CommandID);
	if(f != m_mapClientCommand.end())
	{
		CClientCommandList* pClientCommandList = (CClientCommandList* )f->second;
		return pClientCommandList;
	}
	else
	{
		//OUR_DEBUG((LM_ERROR, "[CMessageManager::GetClientCommand] u2CommandID = %d 0x%08x Add OK.\n", u2CommandID, &m_mapClientCommand));
		return NULL;
	}
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName)
{
	if(NULL == pClientCommand)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d pClientCommand is NULL.\n", u2CommandID));
		return false;
	}

	mapClientCommand::iterator f = m_mapClientCommand.find(u2CommandID);
	if(f != m_mapClientCommand.end())
	{
		CClientCommandList* pClientCommandList = (CClientCommandList* )f->second;
		if(NULL != pClientCommandList)
		{
			_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName);
			if(NULL != pClientCommandInfo) 
			{
				//���������ID
				pClientCommandInfo->m_u2CommandID = u2CommandID;
				
				//��ӵ�ģ������
				string strModule = pModuleName;
				mapModuleClient::iterator f = m_mapModuleClient.find(strModule);
				if(f == m_mapModuleClient.end())
				{
					//�Ҳ����������µ�ģ����Ϣ
					_ModuleClient* pModuleClient = new _ModuleClient();
					if(NULL != pModuleClient)
					{
						pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
						m_mapModuleClient.insert(mapModuleClient::value_type(strModule, pModuleClient));
					}
				}
				else
				{
					//�ҵ��ˣ���ӽ�ȥ
					_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
					if(NULL != pModuleClient)
					{
						pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

	}

	CClientCommandList* pClientCommandList = new CClientCommandList();
	if(NULL == pClientCommandList)
	{
		return false;
	}
	else
	{
		_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName);
		if(NULL != pClientCommandInfo)
		{
			//���������ID
			pClientCommandInfo->m_u2CommandID = u2CommandID;			
			
			//��ӵ�ģ������
			string strModule = pModuleName;
			mapModuleClient::iterator f = m_mapModuleClient.find(strModule);
			if(f == m_mapModuleClient.end())
			{
				//�Ҳ����������µ�ģ����Ϣ
				_ModuleClient* pModuleClient = new _ModuleClient();
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					m_mapModuleClient.insert(mapModuleClient::value_type(strModule, pModuleClient));
				}
			}
			else
			{
				//�ҵ��ˣ���ӽ�ȥ
				_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
				}
			}
		}

		m_mapClientCommand.insert(mapClientCommand::value_type(u2CommandID, pClientCommandList));
		OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
		return true;
	}
}

bool CMessageManager::DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand)
{
	mapClientCommand::iterator f = m_mapClientCommand.find(u2CommandID);
	if(f != m_mapClientCommand.end())
	{
		CClientCommandList* pClientCommandList = (CClientCommandList* )f->second;
		if(NULL != pClientCommandList)
		{
			if(true == pClientCommandList->DelClientCommand(pClientCommand))
			{
				SAFE_DELETE(pClientCommandList);
				m_mapClientCommand.erase(f);
			}
		}

		OUR_DEBUG((LM_ERROR, "[CMessageManager::DelClientCommand] u2CommandID = %d Del OK.\n", u2CommandID));
		return true;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::DelClientCommand] u2CommandID = %d is not exist.\n", u2CommandID));
		return false;
	}
}

bool CMessageManager::UnloadModuleCommand(const char* pModuleName, uint8 u1State)
{
	string strModuleName = pModuleName;
	mapModuleClient::iterator f = m_mapModuleClient.find(strModuleName);
	if(f != m_mapModuleClient.end())
	{
		_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
		if(NULL != pModuleClient)
		{
			pModuleClient->m_u1ModuleState = u1State;
			for(int i = 0; i < (int)pModuleClient->m_vecClientCommandInfo.size(); i++)
			{
				_ClientCommandInfo* pClientCommandInfo = (_ClientCommandInfo* )pModuleClient->m_vecClientCommandInfo[i];
				if(NULL != pClientCommandInfo)
				{
					pClientCommandInfo->m_u1State = 1;
				}
			}

			//�����Ѿ�����0�ģ�ɾ��֮
			vector<_ClientCommandInfo*>::iterator fd = std::remove_if(pModuleClient->m_vecClientCommandInfo.begin(), pModuleClient->m_vecClientCommandInfo.end(), Delete_CommandInfo);
			pModuleClient->m_vecClientCommandInfo.erase (fd, pModuleClient->m_vecClientCommandInfo.end());

			if((int)pModuleClient->m_vecClientCommandInfo.size() == 0)
			{
				//���û�����ģ��Ҫ�������Ϣ�ˣ���ر�֮��
				if(pModuleClient->m_u1ModuleState == (uint8)1)
				{
					App_ModuleLoader::instance()->UnLoadModule(pModuleName);
					m_mapModuleClient.erase(f);
					SAFE_DELETE(pModuleClient);
				}

				//�����2�������¼������ģ��
				if(pModuleClient->m_u1ModuleState == (uint8)2)
				{
					_ModuleInfo* pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);
					if(NULL != pModuleInfo)
					{
						//��ȡ������Ϣ
						string strModuleName = pModuleInfo->strModuleName;
						string strModulePath = pModuleInfo->strModulePath;

						//�ر�ģ�����
						App_ModuleLoader::instance()->UnLoadModule(pModuleName);
						m_mapModuleClient.erase(f);
						SAFE_DELETE(pModuleClient);

						//���¼���
						App_ModuleLoader::instance()->LoadModule(strModulePath.c_str(), strModuleName.c_str());
					}
				}
			}

		}
		return true;
	}
	else
	{
		return false;
	}
}

int CMessageManager::GetCommandCount()
{
	return (int)m_mapClientCommand.size();
}

void CMessageManager::Close()
{
	//��رյ�������
	for(mapClientCommand::iterator b = m_mapClientCommand.begin(); b != m_mapClientCommand.end(); b++)
	{
		 CClientCommandList* pClientCommandList = (CClientCommandList* )b->second;
		 SAFE_DELETE(pClientCommandList);
	}

	for(mapModuleClient::iterator Mb = m_mapModuleClient.begin(); Mb != m_mapModuleClient.end(); Mb++)
	{
		_ModuleClient* pModuleClient = (_ModuleClient* )Mb->second;
		SAFE_DELETE(pModuleClient);
	}

	m_mapClientCommand.clear();
}

bool CMessageManager::CloseCommandInfo(const char* pModuleName)
{
	string strModuleName = pModuleName;
	mapModuleClient::iterator f = m_mapModuleClient.find(strModuleName);
	if(f != m_mapModuleClient.end())
	{
		_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
		if(NULL != pModuleClient)
		{
			for(int i = 0; i < (int)pModuleClient->m_vecClientCommandInfo.size(); i++)
			{
				_ClientCommandInfo* pClientCommandInfo = (_ClientCommandInfo* )pModuleClient->m_vecClientCommandInfo[i];
				if(NULL != pClientCommandInfo)
				{
					if(pClientCommandInfo->m_u4CurrUsedCount == 0)
					{
						pModuleClient->m_vecClientCommandInfo.erase((pModuleClient->m_vecClientCommandInfo.begin() + i));
						break;
					}
				}
			}

			if((int)pModuleClient->m_vecClientCommandInfo.size() == 0)
			{
				//���û�����ģ��Ҫ�������Ϣ�ˣ���ر�֮��
				if(pModuleClient->m_u1ModuleState == (uint8)1)
				{
					App_ModuleLoader::instance()->UnLoadModule(pModuleName);
					m_mapModuleClient.erase(f);
					SAFE_DELETE(pModuleClient);
				}

				//�����2�������¼������ģ��
				if(pModuleClient->m_u1ModuleState == (uint8)2)
				{
					_ModuleInfo* pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);
					if(NULL != pModuleInfo)
					{
						//��ȡ������Ϣ
						string strModuleName = pModuleInfo->strModuleName;
						string strModulePath = pModuleInfo->strModulePath;

						//�ر�ģ�����
						App_ModuleLoader::instance()->UnLoadModule(pModuleName);
						m_mapModuleClient.erase(f);
						SAFE_DELETE(pModuleClient);

						//���¼���
						App_ModuleLoader::instance()->LoadModule(strModulePath.c_str(), strModuleName.c_str());
					}
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

mapModuleClient* CMessageManager::GetModuleClient()
{
	return &m_mapModuleClient;
}
