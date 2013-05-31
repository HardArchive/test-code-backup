#ifndef _PROUDPMANAGER_H
#define _PROUDPMANAGER_H

#include "ProactorUDPHandler.h"
#include "IUDPConnectManager.h"

#include <vector>

using namespace std;

class CProUDPManager : public IUDPConnectManager
{
public:
	CProUDPManager() {};
	~CProUDPManager() {};

	CProactorUDPHandler* Create()
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
		CProactorUDPHandler* pProactorUDPHandler = new CProactorUDPHandler();
		if(NULL != pProactorUDPHandler)
		{
			m_vecProactorUDPHandler.push_back(pProactorUDPHandler);
			return pProactorUDPHandler;
		}

		return NULL;
	};

	void Close()
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
		for(int i = 0; i < (int)m_vecProactorUDPHandler.size(); i++)
		{
			CProactorUDPHandler* pProactorUDPHandler = m_vecProactorUDPHandler[i];
			pProactorUDPHandler->Close();
			SAFE_DELETE(pProactorUDPHandler);
		}
	};

	void GetClientConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
		VecClientConnectInfo.clear();
		for(int i = 0; i < (int)m_vecProactorUDPHandler.size(); i++)
		{
			CProactorUDPHandler* pProactotUDPHandler = m_vecProactorUDPHandler[i];
			if(NULL != pProactotUDPHandler)
			{
				VecClientConnectInfo.push_back(pProactotUDPHandler->GetClientConnectInfo());
			}
		}
	}

	 bool SendMessage(int nConnectID, const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead = true, uint16 u2CommandID = 0)
	 {
		 ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
		 if(nConnectID >= (int)m_vecProactorUDPHandler.size())
		 {
			 OUR_DEBUG((LM_ERROR, "[CProUDPManager::SendMessage]Send (%d) UDP ConnectID is out of range.\n", nConnectID));
			 return false;
		 }

		 CProactorUDPHandler* pProactorUDPHandler = (CProactorUDPHandler* )m_vecProactorUDPHandler[nConnectID];
		 if(NULL != pProactorUDPHandler)
		 {
			 return pProactorUDPHandler->SendMessage(pMessage, u4Len, szIP, nPort, blHead, u2CommandID);
		 }
		 else
		 {
			 return false;
		 }
	 };

private:
	typedef vector<CProactorUDPHandler*> vecProactorUDPHandler;
	vecProactorUDPHandler       m_vecProactorUDPHandler;
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;       //�߳�������֤���̵߳��õ�һ����
};

typedef ACE_Singleton<CProUDPManager, ACE_Null_Mutex> App_ProUDPManager;

#endif
