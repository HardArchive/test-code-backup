#include "ClientProConnectManager.h"

CProactorClientInfo::CProactorClientInfo()
{
	m_pProConnectClient = NULL;
	m_pProAsynchConnect = NULL;
	m_pClientMessage    = NULL;
	m_szServerIP[0]     = '\0';
	m_nPort             = 0;
}

CProactorClientInfo::~CProactorClientInfo()
{
}

bool CProactorClientInfo::Init(const char* pIP, int nPort, CProAsynchConnect* pProAsynchConnect, IClientMessage* pClientMessage)
{
	ACE_DEBUG((LM_ERROR, "[CProactorClientInfo::Init]SetAddrServer(%s:%d) Begin.\n", pIP, nPort));
	int nRet = m_AddrServer.set(nPort, pIP);
	if(-1 == nRet)
	{
		ACE_DEBUG((LM_ERROR, "[CProactorClientInfo::Init]adrClient(%s:%d) error.\n", pIP, nPort));
		return false;
	}

	m_pProAsynchConnect = pProAsynchConnect;
	m_pClientMessage    = pClientMessage;

	sprintf_safe(m_szServerIP, MAX_BUFF_20, "%s", pIP);
	m_nPort = nPort;

	return true;
}

bool CProactorClientInfo::Run(bool blIsReadly)
{
	if(NULL != m_pProConnectClient)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]Connect is exist.\n"));
		return false;
	}

	if(NULL == m_pProAsynchConnect)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pAsynchConnect is NULL.\n"));
		return false;
	}

	/*
	//����Զ�˷�����
	if(m_pProAsynchConnect->GetConnectState() == true)
	{
	OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pProAsynchConnect is run.\n"));
	return false;
	}
	*/

	if(true == blIsReadly)
	{
		m_pProAsynchConnect->SetConnectState(true);
		OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]Connect IP=%s,Port=%d.\n", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number()));
		if(m_pProAsynchConnect->connect(m_AddrServer) == -1)
		{
			OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pAsynchConnect open error(%d).\n", ACE_OS::last_error()));
			return false;
		}
	}

	return true;
}

bool CProactorClientInfo::SendData(ACE_Message_Block* pmblk)
{
	if(NULL == m_pProConnectClient)
	{
		//������Ӳ����ڣ��������ӡ�
		Run(true);
		if(NULL != pmblk)
		{
			pmblk->release();
		}

		//�����Ϣ�д���ӿڣ��򷵻�ʧ�ܽӿ�
		if(NULL != m_pClientMessage)
		{
			//�������Ѿ��Ͽ�����Ҫ�ȴ��������ӵĽ��
			m_pClientMessage->ConnectError(101);
		}

		return false;
	}
	else
	{
		//��������
		return m_pProConnectClient->SendData(pmblk);
	}
}

bool CProactorClientInfo::SetServerID(int nServerID)
{
	if(NULL == m_pProAsynchConnect)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::SetServerID]m_pProAsynchConnect is NULL.\n"));
		return false;
	}
	else
	{
		m_pProAsynchConnect->SetServerID(nServerID);
		return true;
	}
}

int CProactorClientInfo::GetServerID()
{
	if(NULL == m_pProConnectClient)
	{
		return 0;
	}
	else
	{
		return m_pProConnectClient->GetServerID();
	}
}

bool CProactorClientInfo::Close()
{
	if(NULL == m_pProConnectClient)
	{
		return false;
	}
	else
	{
		m_pProConnectClient->ClientClose();
		return true;
	}

	SAFE_DELETE(m_pClientMessage);
}

void CProactorClientInfo::SetProConnectClient(CProConnectClient* pProConnectClient)
{
	m_pProConnectClient = pProConnectClient;
}

CProConnectClient* CProactorClientInfo::GetProConnectClient()
{
	return m_pProConnectClient;
}

IClientMessage* CProactorClientInfo::GetClientMessage()
{
	return m_pClientMessage;
}

ACE_INET_Addr CProactorClientInfo::GetServerAddr()
{
	return m_AddrServer;
}


CClientProConnectManager::CClientProConnectManager(void)
{
	m_nTaskID          = -1;
	m_blProactorFinish = false;
}

CClientProConnectManager::~CClientProConnectManager(void)
{
	Close();
}

bool CClientProConnectManager::Init(ACE_Proactor* pProactor)
{
	if(pProactor == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Init]pProactor is NULL.\n"));
		return false;
	}

	m_u4ConnectServerTimeout = App_MainConfig::instance()->GetConnectServerTimeout() * 1000; //ת��Ϊ΢��
	if(m_u4ConnectServerTimeout == 0)
	{
		m_u4ConnectServerTimeout = PRO_CONNECT_SERVER_TIMEOUT;
	}

	if(-1 == m_ProAsynchConnect.open(false, pProactor, true))
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Init]m_ProAsynchConnect open error(%d).\n", ACE_OS::last_error()));
		return false;
	}
	else
	{
		//���Proactor�Ѿ����ӳɹ�
		m_blProactorFinish = true;

		//�����������ɹ�����ʱ��������ʱ��
		m_ActiveTimer.activate();

		return true;
	}
}

bool CClientProConnectManager::Connect(int nServerID, const char* pIP, int nPort, IClientMessage* pClientMessage)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f != m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	//��ʼ��������Ϣ
	CProactorClientInfo* pClientInfo = new CProactorClientInfo();
	if(false == pClientInfo->Init(pIP, nPort, &m_ProAsynchConnect, pClientMessage))
	{
		delete pClientInfo;
		pClientInfo = NULL;
		return false;
	}

	//��ʼ����
	if(false == pClientInfo->Run(m_blProactorFinish))
	{
		delete pClientInfo;
		pClientInfo = NULL;
		return false;
	}

	pClientInfo->SetServerID(nServerID);

	//�����Ѿ���������ӽ�map
	m_mapClientInfo[nServerID] = pClientInfo;
	OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));

	//�Զ�����0.1��
	ACE_Time_Value tvSleep(0, m_u4ConnectServerTimeout);
	ACE_OS::sleep(tvSleep);

	return true;
}

bool CClientProConnectManager::ConnectUDP(int nServerID, const char* pIP, int nPort, IClientUDPMessage* pClientUDPMessage)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

	mapProactorUDPClientInfo::iterator f = m_mapProactorUDPClientInfo.find(nServerID);
	if(f != m_mapProactorUDPClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUDP]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorUDPClient* pProactorUDPClient = new CProactorUDPClient();
	if(NULL == pProactorUDPClient)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUDP]nServerID =(%d) pProactorUDPClient is NULL.\n", nServerID));
		return false;
	}

	ACE_INET_Addr AddrLocal;
	int nErr = AddrLocal.set(nPort, pIP);
	if(nErr != 0)
	{
		OUR_DEBUG((LM_INFO, "[CClientProConnectManager::ConnectUDP](%d)UDP set_address error[%d].\n", nServerID, errno));
		SAFE_DELETE(pProactorUDPClient);
		return false;
	}

	if(0 != pProactorUDPClient->OpenAddress(AddrLocal, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_UDPDEFINE), pClientUDPMessage))
	{
		OUR_DEBUG((LM_INFO, "[CClientProConnectManager::ConnectUDP](%d)UDP OpenAddress error.\n", nServerID));
		SAFE_DELETE(pProactorUDPClient);
		return false;
	}

	m_mapProactorUDPClientInfo[nServerID] = pProactorUDPClient;
	return true;
}

bool CClientProConnectManager::SetHandler(int nServerID, CProConnectClient* pProConnectClient)
{
	if(NULL == pProConnectClient)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SetHandler]pProConnectClient is NULL.\n"));
		return false;
	}

	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ�������ӵ��Ѿ����ڵĿͻ���map������
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SetHandler]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	pClientInfo->SetProConnectClient(pProConnectClient);
	return true;
}

bool CClientProConnectManager::Close(int nServerID)
{
	//�������Ϊ�������Ͽ�����ֻɾ��ProConnectClient��ָ��
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	//�ر����Ӷ���
	if(NULL != pClientInfo->GetProConnectClient())
	{
		pClientInfo->GetProConnectClient()->ClientClose();
	}

	pClientInfo->SetProConnectClient(NULL);
	SAFE_DELETE(pClientInfo);
	//��map����ɾ����ǰ���ڵĶ���
	m_mapClientInfo.erase(f);
	return true;
}

bool CClientProConnectManager::CloseUDP(int nServerID)
{
	//�������Ϊ�������Ͽ�����ֻɾ��ProConnectClient��ָ��
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorUDPClientInfo::iterator f = m_mapProactorUDPClientInfo.find(nServerID);
	if(f == m_mapProactorUDPClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorUDPClient* pClientInfo = (CProactorUDPClient* )f->second;
	if(NULL == pClientInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	pClientInfo->Close();
	SAFE_DELETE(pClientInfo);
	//��map����ɾ����ǰ���ڵĶ���
	m_mapProactorUDPClientInfo.erase(f);
	return true;
}

bool CClientProConnectManager::ConnectErrorClose(int nServerID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	//��map����ɾ����ǰ���ڵĶ���
	m_mapClientInfo.erase(f);
	SAFE_DELETE(pClientInfo);

	return true;
}

IClientMessage* CClientProConnectManager::GetClientMessage(int nServerID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		return NULL;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL != pClientInfo)
	{
		return pClientInfo->GetClientMessage();
	}

	return NULL;
}

bool CClientProConnectManager::SendData(int nServerID, const char* pData, int nSize, bool blIsDelete)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));
		if(true == blIsDelete)
		{
			SAFE_DELETE_ARRAY(pData);
		}
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;

	ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(nSize);
	if(NULL == pmblk)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pmblk is NULL.\n", nServerID));
		if(true == blIsDelete)
		{
			SAFE_DELETE_ARRAY(pData);
		}
		return false;
	}

	ACE_OS::memcpy(pmblk->wr_ptr(), pData, nSize);
	pmblk->wr_ptr(nSize);

	if(true == blIsDelete)
	{
		SAFE_DELETE_ARRAY(pData);
	}

	//��������
	return pClientInfo->SendData(pmblk);
}

bool CClientProConnectManager::SendDataUDP(int nServerID,const char* pIP, int nPort, const char* pMessage, uint32 u4Len)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorUDPClientInfo::iterator f = m_mapProactorUDPClientInfo.find(nServerID);
	if(f == m_mapProactorUDPClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SendDataUDP]nServerID =(%d) is not exist.\n", nServerID));
		SAFE_DELETE_ARRAY(pMessage);
		return false;
	}

	CProactorUDPClient* pClientInfo = (CProactorUDPClient* )f->second;

	//��������
	return pClientInfo->SendMessage(pMessage, u4Len, pIP, nPort);
}

bool CClientProConnectManager::StartConnectTask(int nIntervalTime)
{
	CancelConnectTask();
	m_nTaskID = m_ActiveTimer.schedule(this, (void* )NULL, ACE_OS::gettimeofday() + ACE_Time_Value(nIntervalTime), ACE_Time_Value(nIntervalTime));
	if(m_nTaskID == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::StartConnectTask].StartConnectTask is fail, time is (%d).\n", nIntervalTime));
		return false;
	}

	m_ActiveTimer.activate();
	return true;
}

void CClientProConnectManager::CancelConnectTask()
{
	if(m_nTaskID != -1)
	{
		//ɱ��֮ǰ�Ķ�ʱ�������¿����µĶ�ʱ��
		m_ActiveTimer.cancel(m_nTaskID);
		m_nTaskID = -1;
	}
}

void CClientProConnectManager::Close()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

	//����ж�ʱ�����رն�ʱ��
	CancelConnectTask();

	//�ر������Ѵ��ڵ�����
	mapProactorClientInfo::iterator b = m_mapClientInfo.begin();
	mapProactorClientInfo::iterator e = m_mapClientInfo.end();
	for(b; b!= e; b++)
	{
		CProactorClientInfo* pClientInfo = (CProactorClientInfo* )b->second;
		pClientInfo->Close();
		SAFE_DELETE(pClientInfo);
	}

	mapProactorUDPClientInfo::iterator ub = m_mapProactorUDPClientInfo.begin();
	mapProactorUDPClientInfo::iterator ue = m_mapProactorUDPClientInfo.end();
	for(ub; ub!= ue; ub++)
	{
		CProactorUDPClient* pClientInfo = (CProactorUDPClient* )ub->second;
		pClientInfo->Close();
		SAFE_DELETE(pClientInfo);
	}

	m_mapClientInfo.clear();
	m_mapProactorUDPClientInfo.clear();
}

int CClientProConnectManager::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

	//OUR_DEBUG((LM_DEBUG, "[CClientProConnectManager::handle_timeout]Begin.\n"));
	if(m_ProAsynchConnect.GetConnectState() == true)
	{
		return 0;
	}

	mapProactorClientInfo::iterator b = m_mapClientInfo.begin();
	mapProactorClientInfo::iterator e = m_mapClientInfo.end();

	for(b; b!= e; b++)
	{
		int nServerID = (int)b->first;

		/*
		//���Դ���
		if(GetConnectState(nServerID) == false)
		{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::handle_timeout]nServerID == false (%d).\n", nServerID));
		}
		else
		{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::handle_timeout]nServerID == true (%d).\n", nServerID));
		}
		*/

		CProactorClientInfo* pClientInfo = (CProactorClientInfo* )b->second;
		if(NULL == pClientInfo->GetProConnectClient())
		{
			//���õ�ǰnServerID
			pClientInfo->SetServerID(nServerID);

			//������Ӳ����ڣ������½�������
			pClientInfo->Run(m_blProactorFinish);

			//�Զ�����0.1��
			ACE_Time_Value tvSleep(0, m_u4ConnectServerTimeout);
			ACE_OS::sleep(tvSleep);
		}
	}
	return 0;
}

void CClientProConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

	VecClientConnectInfo.clear();

	mapProactorClientInfo::iterator b = m_mapClientInfo.begin();
	mapProactorClientInfo::iterator e = m_mapClientInfo.end();
	for(b; b!= e; b++)
	{
		CProactorClientInfo* pClientInfo = (CProactorClientInfo* )b->second;
		if(NULL != pClientInfo)
		{
			if(NULL != pClientInfo->GetProConnectClient())
			{
				//�����Ѿ�����
				_ClientConnectInfo ClientConnectInfo = pClientInfo->GetProConnectClient()->GetClientConnectInfo();
				ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
				VecClientConnectInfo.push_back(ClientConnectInfo);
			}
			else
			{
				//����δ����
				_ClientConnectInfo ClientConnectInfo;
				ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
				ClientConnectInfo.m_blValid    = false;
				VecClientConnectInfo.push_back(ClientConnectInfo);
			}
		}
	}
}

void CClientProConnectManager::GetUDPConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

	mapProactorUDPClientInfo::iterator b = m_mapProactorUDPClientInfo.begin();
	mapProactorUDPClientInfo::iterator e = m_mapProactorUDPClientInfo.end();

	for(b; b!= e; b++)
	{
		CProactorUDPClient* pClientInfo = (CProactorUDPClient* )b->second;
		if(NULL != pClientInfo)
		{
			_ClientConnectInfo ClientConnectInfo = pClientInfo->GetClientConnectInfo();
			VecClientConnectInfo.push_back(ClientConnectInfo);
		}
	}

}

bool CClientProConnectManager::CloseByClient(int nServerID)
{
	//�������ΪԶ�̿ͻ��˶Ͽ�����ֻɾ��ProConnectClient��ָ��
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	pClientInfo->SetProConnectClient(NULL);

	return true;
}


bool CClientProConnectManager::GetConnectState(int nServerID)
{
	//��鵱ǰ�����Ƿ��ǻ�Ծ��
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::GetConnectState]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::GetConnectState]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	if(NULL == pClientInfo->GetProConnectClient())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CClientProConnectManager::ReConnect( int nServerID )
{
	//��鵱ǰ�����Ƿ��ǻ�Ծ��
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//�����������Ѿ����ڣ��򲻴����µ�����
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::Close]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	if(NULL == pClientInfo->GetProConnectClient())
	{
		//���õ�ǰnServerID
		pClientInfo->SetServerID(nServerID);

		//������Ӳ����ڣ������½�������
		pClientInfo->Run(m_blProactorFinish);

		//�Զ�����0.1��
		ACE_Time_Value tvSleep(0, m_u4ConnectServerTimeout);
		ACE_OS::sleep(tvSleep);

		return true;
	}
	else
	{
		return true;
	}
}