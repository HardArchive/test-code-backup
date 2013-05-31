#include "ServerManager.h"

CServerManager::CServerManager(void)
{
	m_pFileLogger = NULL;
}

CServerManager::~CServerManager(void)
{
	OUR_DEBUG((LM_INFO, "[CServerManager::~CServerManager].\n"));
	SAFE_DELETE(m_pFileLogger);
}

bool CServerManager::Init()
{
	int nServerPortCount    = App_MainConfig::instance()->GetServerPortCount();
	int nReactorCount       = App_MainConfig::instance()->GetReactorCount();

	bool blState = false;

	//��ʼ����ֹIP�б�
	App_ForbiddenIP::instance()->Init(FORBIDDENIP_FILE);

	//��ʼ��������
	if(!m_ConnectAcceptorManager.InitConnectAcceptor(nServerPortCount))
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init]%s.\n", m_ConnectAcceptorManager.GetError()));
		return false;
	}

	//��ʼ����Ӧ��
	for(int i = 0 ; i < nReactorCount; i++)
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init()]... i=[%d].\n",i));
		if(i == 0)
		{
			//�������ֲ���ϵͳ�汾��ʹ�ò�ͬ�ķ�Ӧ��
#ifdef WIN32
			blState = App_ReactorManager::instance()->AddNewReactor(REACTOR_CLIENTDEFINE, Reactor_Select, 0);
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_Select.\n"));
#else
			blState = App_ReactorManager::instance()->AddNewReactor(REACTOR_CLIENTDEFINE, Reactor_DEV_POLL, 0);
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL.\n"));	
#endif		
			if(!blState)
			{
				OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE Error.\n"));
				return false;
			}
		}
		else
		{
#ifdef WIN32
			//blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_WFMO, 1);
			blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_Select, 1);
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_Select.\n"));
#else
			blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_DEV_POLL, 1);
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL.\n"));
#endif
			if(!blState)
			{
				OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor [%d] Error.\n", i));
				return false;
			}
		}
	}

	//��ʼ����־ϵͳ�߳�
	m_pFileLogger = new CFileLogger();
	if(NULL == m_pFileLogger)
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init]pFileLogger new is NULL.\n"));
		return false;
	}

	m_pFileLogger->Init();
	AppLogManager::instance()->Init();
	if(0 != AppLogManager::instance()->RegisterLog(m_pFileLogger))
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init]AppLogManager::instance()->RegisterLog error.\n"));
		return false;
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init]AppLogManager is OK.\n"));
	}

	//��ʼ��������ϵͳ
	App_IPAccount::instance()->Init(App_MainConfig::instance()->GetValid(), App_MainConfig::instance()->GetValidConnectCount());

	//��ʼ��BuffPacket�����
	App_BuffPacketManager::instance()->Init(BUFFPACKET_MAX_COUNT);

	//��ʼ��PacketParse�����
	App_PacketParsePool::instance()->Init(MAX_PACKET_PARSE);

	//��ʼ��MakePacket�����
	App_MakePacket::instance()->Init();

	//��ʼ��Message�����
	App_MessagePool::instance()->Init(MAX_MESSAGE_POOL);

	//��ʼ��ConnectHandler�����
	if(App_MainConfig::instance()->GetHandleCount() <= 0)
	{
		App_ConnectHandlerPool::instance()->Init(MAX_HANDLE_POOL);
	}
	else
	{
		App_ConnectHandlerPool::instance()->Init(App_MainConfig::instance()->GetHandleCount());
	}
	
	//��ʼ��ͳ��ģ�鹦��
	App_CommandAccount::instance()->Init(App_MainConfig::instance()->GetCommandAccount(), App_MainConfig::instance()->GetPacketTimeOut());

	//��ʼ�����ӹ�����
	App_ConnectManager::instance()->Init(App_MainConfig::instance()->GetSendQueueCount());

	//��ʼ����Ϣ�����߳�
	App_MessageService::instance()->Init(App_MainConfig::instance()->GetThreadCount(), App_MainConfig::instance()->GetMsgMaxQueue(), App_MainConfig::instance()->GetMgsHighMark(), App_MainConfig::instance()->GetMsgLowMark());

	//��ʼ����DLL�Ķ���ӿ�
	App_ServerObject::instance()->SetMessageManager((IMessageManager* )App_MessageManager::instance());
	App_ServerObject::instance()->SetLogManager((ILogManager*  )AppLogManager::instance());
	App_ServerObject::instance()->SetConnectManager((IConnectManager* )App_ConnectManager::instance());
	App_ServerObject::instance()->SetPacketManager((IPacketManager* )App_BuffPacketManager::instance());
	App_ServerObject::instance()->SetClientManager((IClientManager* )App_ClientReConnectManager::instance());
	App_ServerObject::instance()->SetUDPConnectManager((IUDPConnectManager* )App_ReUDPManager::instance());
	App_ServerObject::instance()->SetTimerManager((ActiveTimer* )App_TimerManager::instance());
	App_ServerObject::instance()->SetModuleMessageManager((IModuleMessageManager* )App_ModuleMessageManager::instance());

	return true;
}

bool CServerManager::Start()
{
	//ע���ź���
	if(0 != App_SigHandler::instance()->RegisterSignal(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE)))
	{
		return false;
	}

	//����TCP����
	int nServerPortCount = App_MainConfig::instance()->GetServerPortCount();

	for(int i = 0 ; i < nServerPortCount; i++)
	{
		ACE_INET_Addr listenAddr;

		_ServerInfo* pServerInfo = App_MainConfig::instance()->GetServerPort(i);
		if(NULL == pServerInfo)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]pServerInfo [%d] is NULL.\n", i));
			return false;
		}

		int nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP);
		if(nErr != 0)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start](%d)set_address error[%d].\n", i, errno));
			return false;
		}

		//�õ�������
		ConnectAcceptor* pConnectAcceptor = m_ConnectAcceptorManager.GetConnectAcceptor(i);

		if(NULL == pConnectAcceptor)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]pConnectAcceptor[%d] is NULL.\n", i));
			return false;
		}

		int nRet = pConnectAcceptor->open(listenAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE), ACE_NONBLOCK);
		if(-1 == nRet)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] pConnectAcceptor->open[%d] is error.\n", i));
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] error(%d).\n",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
			return false;
		}
		OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] OK.\n",listenAddr.get_host_addr(), listenAddr.get_port_number()));
	}

	//����UDP����
	int nUDPServerPortCount = App_MainConfig::instance()->GetUDPServerPortCount();

	for(int i = 0 ; i < nUDPServerPortCount; i++)
	{
		ACE_INET_Addr listenAddr;

		_ServerInfo* pServerInfo = App_MainConfig::instance()->GetUDPServerPort(i);
		if(NULL == pServerInfo)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]UDP pServerInfo [%d] is NULL.\n", i));
			return false;
		}

		int nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP);
		if(nErr != 0)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]UDP (%d)set_address error[%d].\n", i, errno));
			return false;
		}

		//�õ�������
		CReactorUDPHander* pReactorUDPHandler = App_ReUDPManager::instance()->Create();

		if(NULL == pReactorUDPHandler)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]UDP pReactorUDPHandler[%d] is NULL.\n", i));
			return false;
		}

		int nRet = pReactorUDPHandler->OpenAddress(listenAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));
		if(-1 == nRet)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] UDP Listen from [%s:%d] error(%d).\n",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
			return false;
		}
		OUR_DEBUG((LM_INFO, "[CServerManager::Start] UDP Listen from [%s:%d] OK.\n",listenAddr.get_host_addr(), listenAddr.get_port_number()));
	}

	//������̨����˿ڼ���
	if(App_MainConfig::instance()->GetConsoleSupport() == CONSOLE_ENABLE)
	{
		ACE_INET_Addr listenConsoleAddr;

		int nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), App_MainConfig::instance()->GetConsoleIP());
		if(nErr != 0)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]listenConsoleAddr set_address error[%d].\n", errno));
			return false;
		}

		int nRet = m_ConnectConsoleAcceptor.open(listenConsoleAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE), ACE_NONBLOCK);
		if(-1 == nRet)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] pConnectAcceptor->open is error.\n"));
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] error(%d).\n",listenConsoleAddr.get_host_addr(), listenConsoleAddr.get_port_number(), errno));
			return false;
		}
	}

	//������־�����߳�
	if(0 != AppLogManager::instance()->Start())
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM, "[CServerManager::Init]AppLogManager is ERROR.");
	}
	else
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM, "[CServerManager::Init]AppLogManager is OK.");
	}

	/*
	//���ö�ʱ������(ʹ�ø߾��ȶ�ʱ��)
	(void) ACE_High_Res_Timer::global_scale_factor ();
	auto_ptr<ACE_Timer_Heap_Variable_Time_Source> tq(new ACE_Timer_Heap_Variable_Time_Source);
	tq->set_time_policy(&ACE_High_Res_Timer::gettimeofday_hr);
	App_TimerManager::instance()->timer_queue(tq);
	tq.release();
	*/

	//������ʱ��
	if(0 != App_TimerManager::instance()->activate())
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_TimerManager::instance()->Start() is error.\n"));
		return false;
	}

	//������������Reactor���������ԭʼ��Reactor����Ϊԭʼ�Ļ�����̣߳������������һ�¡�
	if(!App_ReactorManager::instance()->StartReactor())
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Start]App_ReactorManager::instance()->StartReactor is error.\n"));
		return false;
	}

	//�����м���������ӹ�����
	App_ClientReConnectManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE));
	App_ClientReConnectManager::instance()->StartConnectTask(App_MainConfig::instance()->GetConnectServerCheck());

	//��ʼ��ģ����أ���Ϊ������ܰ������м���������Ӽ���
	bool blState = App_ModuleLoader::instance()->LoadModule(App_MainConfig::instance()->GetModulePath(), App_MainConfig::instance()->GetModuleString());
	if(false == blState)
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Start]LoadModule is error.\n"));
		return false;
	}

	//��ʼ��Ϣ�����߳�
	App_MessageService::instance()->Start();

	//��ʼ�������ӷ��Ͷ�ʱ��
	App_ConnectManager::instance()->StartTimer();


	//���������Ӧ��
	OUR_DEBUG((LM_INFO, "[CServerManager::Start]App_ReactorManager::instance()->StartReactorDefault begin....\n"));
	if(!App_ReactorManager::instance()->StartReactorDefault())
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Start]App_ReactorManager::instance()->StartReactorDefault is error.\n"));
		return false;
	}

	return true;
}

bool CServerManager::Close()
{
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close begin....\n"));
	m_ConnectAcceptorManager.Close();
	m_ConnectConsoleAcceptor.close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_TimerManager OK.\n"));
	App_TimerManager::instance()->deactivate();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ReUDPManager OK.\n"));
	App_ReUDPManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ModuleLoader OK.\n"));
	App_ModuleLoader::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_MessageManager OK.\n"));
	App_MessageManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ConnectManager OK.\n"));
	App_MessageService::instance()->Close();
	OUR_DEBUG((LM_INFO, "[App_MessageService::Close]Close App_MessageService OK.\n"));
	App_ConnectManager::instance()->CloseAll();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ConnectManager OK.\n"));
	App_ClientReConnectManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ClientReConnectManager OK.\n"));
	App_CommandAccount::instance()->SaveCommandDataLog();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Save App_CommandAccount OK.\n"));
	AppLogManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]AppLogManager OK\n"));
	App_BuffPacketManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]BuffPacketManager OK\n"));
	App_ReactorManager::instance()->StopReactor();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ReactorManager OK.\n"));	
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close end....\n"));
	return true;
}
