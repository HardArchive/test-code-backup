#include "StdAfx.h"
#include "TestIOCPServer.h"

CTestIOCPServer::CTestIOCPServer(void):m_Server(this)
{
}

CTestIOCPServer::~CTestIOCPServer(void)
{
}

void CTestIOCPServer::Start()
{
	//m_Server.Start(_T("0.0.0.0"), 5555);
	m_Server->Start(_T("0.0.0.0"), 5555);
}

void CTestIOCPServer::Stop()
{
	m_Server->Stop();

}


ISocketListener::EnHandleResult CTestIOCPServer::OnPrepareListen(SOCKET soListen)
{
	USHORT usPort = 0;
	TCHAR tszAddress[40] = {0};
	TCHAR  tszContent[256] = {0};	
	int iAddressLen = sizeof(tszAddress) / sizeof(TCHAR);
	

	m_Server->GetListenAddress(tszAddress, iAddressLen, usPort);
	_stprintf(tszContent, _T("bind address: %s:%d"), tszAddress, usPort);
	_tprintf(_T("*****%s\r\n"), tszContent);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnAccept(CONNID dwConnID, SOCKET soClient)
{
	BOOL bPass = TRUE;
	USHORT usPort = 0;	
	TCHAR tszAddress[40] = {0};
	int iAddressLen = sizeof(tszAddress) / sizeof(TCHAR);
	

	m_Server->GetClientAddress(dwConnID, tszAddress, iAddressLen, usPort);

	TCHAR  tszContent[100] = {0};

	_stprintf(tszContent, _T("%s (%s:%d)"), bPass ? _T("PASS") : _T("REJECT"), tszAddress, usPort);

	_tprintf(_T("%s\r\n"), tszContent);


	return bPass ? ISocketListener::HR_OK : ISocketListener::HR_ERROR;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnSend(CONNID dwConnID, const BYTE* pData, int iLength)
{
	TCHAR  tszContent[256] = {0};
	_stprintf(tszContent, _T("OnSend dwConnID:%d; iLength:%d; pData:%s\r\n"), dwConnID, iLength, pData);
	_tprintf(_T("%s\r\n"), tszContent);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnReceive(CONNID dwConnID, int iLength)
{
	TCHAR  tszContent[256] = {0};
	_stprintf(tszContent, _T("OnReceive dwConnID:%d; iLength:%d\r\n"), dwConnID, iLength);
	_tprintf(_T("%s\r\n\r\n"), tszContent);


	CBufferPtr buffer(iLength);

	IPullSocket::EnFetchResult result = m_Server->Fetch(dwConnID, buffer, (int)buffer.Size());
	if(result == IPullSocket::FR_OK)
	{
		_stprintf(tszContent, _T("OnReceive dwConnID:%d; iLength:%d; buffer:%s\r\n"), dwConnID, iLength, buffer);
		_tprintf(_T("%s\r\n\r\n"), tszContent);

		if(!m_Server->Send(dwConnID, buffer, (int)buffer.Size()))
			return ISocketListener::HR_ERROR;
	}
	
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnClose(CONNID dwConnID)
{
	USHORT usPort = 0;
	TCHAR tszAddress[40] = {0};
	TCHAR  tszContent[100] = {0};	
	int iAddressLen = sizeof(tszAddress) / sizeof(TCHAR);


	m_Server->GetListenAddress(tszAddress, iAddressLen, usPort);
	_stprintf(tszContent, _T("OnClose dwConnID:%d;address: %s:%d"), dwConnID, tszAddress, usPort);
	_tprintf(_T("%s\r\n"), tszContent);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	TCHAR  tszContent[256] = {0};
	_stprintf(tszContent, _T("OnError dwConnID:%d; enOperation:%d; iErrorCode:%d\r\n"), dwConnID, enOperation, iErrorCode);
	_tprintf(_T("%s\r\n"), tszContent);

	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnServerShutdown()
{
	USHORT usPort = 0;
	TCHAR tszAddress[40] = {0};
	TCHAR  tszContent[100] = {0};	
	int iAddressLen = sizeof(tszAddress) / sizeof(TCHAR);


	m_Server->GetListenAddress(tszAddress, iAddressLen, usPort);
	_stprintf(tszContent, _T("OnServerShutdown address: %s:%d"), tszAddress, usPort);
	_tprintf(_T("*****%s\r\n"), tszContent);

	return ISocketListener::HR_OK;
}

//TPkgInfo* CTestIOCPServer::FindPkgInfo(CONNID dwConnID)
//{
//	PVOID pInfo = nullptr;
//
//	m_Server->GetConnectionExtra(dwConnID, &pInfo);
//
//	/*
//	auto it = m_mpPkgInfo.find(dwConnID);
//
//	if(it != m_mpPkgInfo.end())
//		pInfo = it->second;
//	*/
//
//	return (TPkgInfo*)pInfo;
//}
//
//void CTestIOCPServer::RemovePkgInfo(CONNID dwConnID)
//{
//	//CCriSecLock locallock(m_csPkgInfo);
//
//	CCriSec2* pcs = nullptr;
//
//	if(m_Server->GetConnectionCriSec(dwConnID, &pcs) && pcs != nullptr)
//	{
//		PVOID pInfo = nullptr;
//
//		CCriSecLock2 locallock(*pcs);
//
//		if(m_Server->GetConnectionExtra(dwConnID, &pInfo) && pInfo != nullptr)
//		{
//			m_Server->SetConnectionExtra(dwConnID, nullptr);
//			delete (TPkgInfo*)pInfo;
//		}
//	}
//
//	/*
//	TPkgInfo* pInfo = FindPkgInfo(dwConnID);
//
//	if(pInfo != nullptr)
//	{
//		m_mpPkgInfo.erase(dwConnID);
//		delete pInfo;
//	}
//	*/
//}
