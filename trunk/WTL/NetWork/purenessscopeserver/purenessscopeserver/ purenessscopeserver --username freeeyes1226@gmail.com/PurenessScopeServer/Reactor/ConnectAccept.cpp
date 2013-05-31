#include "ConnectAccept.h"

int ConnectConsoleAcceptor::make_svc_handler (CConsoleHandler *&sh)
{
	//������ڴ�������ӳ����˷������趨����ֵ�����������Ӽ������ӷ�����
	CConsoleHandler* pConsoleHandler = new CConsoleHandler();
	if(NULL != pConsoleHandler)
	{
		pConsoleHandler->reactor(this->reactor());
		sh = pConsoleHandler;
		return 0;
	}
	else
	{
		return -1;
	}
}


//==============================================================================

int ConnectAcceptor::make_svc_handler (CConnectHandler *&sh)
{
	//������ڴ�������ӳ����˷������趨����ֵ�����������Ӽ������ӷ�����
	if(App_ConnectHandlerPool::instance()->GetUsedCount() > App_MainConfig::instance()->GetMaxHandlerCount())
	{
		OUR_DEBUG((LM_ERROR, "[ConnectAcceptor::make_svc_handler]Connect is more MaxHandlerCount(%d > %d).\n", App_ConnectHandlerPool::instance()->GetUsedCount(), App_MainConfig::instance()->GetMaxHandlerCount()));
		//����������
		return -1;
	}
	else
	{
		//��������
		CConnectHandler* pConnectHandler = App_ConnectHandlerPool::instance()->Create();
		if(NULL != pConnectHandler)
		{
			pConnectHandler->reactor(this->reactor());
			sh = pConnectHandler;
			return 0;
		}
		else
		{
			return -1;
		}
	}
}

CConnectAcceptorManager::CConnectAcceptorManager(void)
{
	m_nAcceptorCount = 0;
	m_szError[0]     = '\0';
}

CConnectAcceptorManager::~CConnectAcceptorManager(void)
{
	OUR_DEBUG((LM_INFO, "[CConnectAcceptorManager::~CConnectAcceptorManager].\n"));
	Close();
	OUR_DEBUG((LM_INFO, "[CConnectAcceptorManager::~CConnectAcceptorManager]End.\n"));
}

bool CConnectAcceptorManager::InitConnectAcceptor(int nCount)
{
	try
	{
		Close();

		for(int i = 0; i < nCount; i++)
		{
			ConnectAcceptor* pConnectAcceptor = new ConnectAcceptor();
			if(NULL == pConnectAcceptor)
			{
				throw "[CConnectAcceptorManager::InitConnectAcceptor]pConnectAcceptor new is fail.";
			}

			m_vecConnectAcceptor.push_back(pConnectAcceptor);
		}

		return true;
	}
	catch (const char* szError)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
		return false;
	}
}

void CConnectAcceptorManager::Close()
{
	for(int i = 0; i < (int)m_vecConnectAcceptor.size(); i++)
	{
		ConnectAcceptor* pConnectAcceptor = (ConnectAcceptor* )m_vecConnectAcceptor[i];
		if(NULL != pConnectAcceptor)
		{
			pConnectAcceptor->close();
			delete pConnectAcceptor;
			pConnectAcceptor = NULL;
		}
	}

	m_vecConnectAcceptor.clear();
	m_nAcceptorCount = 0;
}

int CConnectAcceptorManager::GetCount()
{
	return (int)m_vecConnectAcceptor.size();
}

ConnectAcceptor* CConnectAcceptorManager::GetConnectAcceptor(int nIndex)
{
	if(nIndex < 0 || nIndex >= (int)m_vecConnectAcceptor.size())
	{
		return NULL;
	}

	return (ConnectAcceptor* )m_vecConnectAcceptor[nIndex];
}

const char* CConnectAcceptorManager::GetError()
{
	return m_szError;
}
