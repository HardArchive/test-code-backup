#include "ProConnectAccept.h"

CProConsoleHandle* CProConsoleConnectAcceptor::make_handler (void)
{
	validate_new_connection(true);
	CProConsoleHandle* pProConsoleHandle = new CProConsoleHandle();
	return pProConsoleHandle;
}

int CProConsoleConnectAcceptor::validate_connection (const ACE_Asynch_Accept::Result& result,
											 const ACE_INET_Addr &remote,
											 const ACE_INET_Addr& local)
{
	//������ڷ����������IP��Χ�У���������
	if(App_MainConfig::instance()->CompareConsoleClinetIP(remote.get_host_addr()) == false)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

//===============================================================================================

CProConnectHandle* ProConnectAcceptor::make_handler (void)
{
	return App_ProConnectHandlerPool::instance()->Create();
}

int ProConnectAcceptor::validate_connection (const ACE_Asynch_Accept::Result& result,
						   		             const ACE_INET_Addr &remote,
								             const ACE_INET_Addr& local)
{
	if(App_ForbiddenIP::instance()->CheckIP(remote.get_host_addr()) == false)
	{
		//�ڽ�ֹ�б��У����������
		return -1;
	}

	//������ڴ�������ӳ����˷������趨����ֵ�����������Ӽ������ӷ�����
	if(App_ProConnectHandlerPool::instance()->GetUsedCount() > App_MainConfig::instance()->GetMaxHandlerCount())
	{
		OUR_DEBUG((LM_ERROR, "[ProConnectAcceptor::validate_connection]Connect is more MaxHandlerCount(%d > %d).\n", App_ProConnectHandlerPool::instance()->GetUsedCount(), App_MainConfig::instance()->GetMaxHandlerCount()));
		//����������
		return -1;
	}
	else
	{
		//��������
		return 0;
	}
}

CProConnectAcceptManager::CProConnectAcceptManager(void)
{
	m_nAcceptorCount = 0;
	m_szError[0]     = '\0';
}

CProConnectAcceptManager::~CProConnectAcceptManager(void)
{
	Close();
}

bool CProConnectAcceptManager::InitConnectAcceptor(int nCount)
{
	try
	{
		Close();

		for(int i = 0; i < nCount; i++)
		{
			ProConnectAcceptor* pConnectAcceptor = new ProConnectAcceptor();
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

void CProConnectAcceptManager::Close()
{
	for(int i = 0; i < (int)m_vecConnectAcceptor.size(); i++)
	{
		ProConnectAcceptor* pConnectAcceptor = (ProConnectAcceptor* )m_vecConnectAcceptor[i];
		if(NULL != pConnectAcceptor)
		{
			delete pConnectAcceptor;
			pConnectAcceptor = NULL;
		}
	}

	m_vecConnectAcceptor.clear();
	m_nAcceptorCount = 0;
}

int CProConnectAcceptManager::GetCount()
{
	return (int)m_vecConnectAcceptor.size();
}

ProConnectAcceptor* CProConnectAcceptManager::GetConnectAcceptor(int nIndex)
{
	if(nIndex < 0 || nIndex >= (int)m_vecConnectAcceptor.size())
	{
		return NULL;
	}

	return (ProConnectAcceptor* )m_vecConnectAcceptor[nIndex];
}

const char* CProConnectAcceptManager::GetError()
{
	return m_szError;
}
