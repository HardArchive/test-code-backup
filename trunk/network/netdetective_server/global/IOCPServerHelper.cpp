#include "StdAfx.h"
#include "IOCPServerHelper.h"

DWORD g_dwConnectTimeoutTime = 0;
CIOCPServerHelper::CIOCPServerHelper(void):m_clsIOCPServer(this)
{
}

CIOCPServerHelper::~CIOCPServerHelper(void)
{
	Reset();
}

void CIOCPServerHelper::Reset()
{
	TRACE(_T("EEEEEEEE�˳��������������������!!!\r\n"));

	m_mapConnIDClient.Lock();
	if (m_mapConnIDClient.size())
	{
		for (map<DWORD, PCLIENTINFO>::iterator it=m_mapConnIDClient.begin(); it!=m_mapConnIDClient.end(); ++it)
		{
			it->second->bIsDelete = true;
			m_queueDeleteClient.Lock();
			m_queueDeleteClient.push(it->second);
			m_queueDeleteClient.Unlock();
			m_mapConnIDClient.erase(it);
		}
	}

	m_mapConnIDClient.clear();
	m_mapConnIDClient.Unlock();

	m_objRecvPacketPool.Free();
}

void CIOCPServerHelper::Reset(DWORD dwConnID)
{
	m_mapConnIDClient.Lock();
	if (!m_mapConnIDClient.size()) return;
	for (map<DWORD, PCLIENTINFO>::iterator it=m_mapConnIDClient.begin(); it!=m_mapConnIDClient.end(); it++)
	{
		if (dwConnID == it->first)
		{	
			it->second->bIsDelete = true;
			m_queueDeleteClient.Lock();
			m_queueDeleteClient.push(it->second);
			m_queueDeleteClient.Unlock();
			m_mapConnIDClient.erase(it);
			break;
		}
	}
	m_mapConnIDClient.Unlock();

}

//������IP�Ͷ˿�  
bool CIOCPServerHelper::Start(LPCTSTR pszBindAddress, USHORT usPort)
{
	return (bool)m_clsIOCPServer.Start(pszBindAddress, usPort);
}

bool CIOCPServerHelper::Send(DWORD dwConnID, const BYTE* pBuffer, int iLen)
{
	return m_clsIOCPServer.Send(dwConnID, pBuffer, iLen);
}

bool CIOCPServerHelper::CloseLink(DWORD dwConnID)
{
	//������Ӧ���������
	bool bRet = m_clsIOCPServer.Disconnect(dwConnID);
	return bRet;
}
//
//int CIOCPServerHelper::CheckAllTimeout()
//{	
//	m_mapConnIDClient.Lock();
//	if (!m_mapConnIDClient.size()) return 0;
//	for (map<DWORD, PCLIENTINFO>::iterator it=m_mapConnIDClient.begin(); it!=m_mapConnIDClient.end(); it++)
//	{
//		if (!m_bIsRunning) break;
//		if (!it->second->CheckTimeout(g_dwConnectTimeoutTime))
//		{
//			TRACE("connID:%d�ر�����!!!\r\n", it->first);
//			CloseLink(it->first);
//			Reset(it->first);
//			if (m_mapConnIDClient.empty()) break;
//		}	
//	}
//	m_mapConnIDClient.Unlock();
//	
//	return 1;
//}
//
//bool CIOCPServerHelper::Send2(string strBarcode, const BYTE* pBuffer, int iLen)
//{
//	DWORD dwConnID = -1;
//	m_mapBarcodeID.Lock();
//	if (0 == m_mapBarcodeID.count(strBarcode))
//	{
//		TRACE("Send2 Faile Barcode:%s;Len; Reason:δ�ҵ�����������;m_mapBarcodeID.count():%d\r\n", strBarcode.c_str(), strlen(strBarcode.c_str()), m_mapBarcodeID.size());
//		m_mapBarcodeID.Unlock();
//		return false;
//	}
//	dwConnID = m_mapBarcodeID[strBarcode];
//	m_mapBarcodeID.Unlock();
//	return Send(dwConnID, pBuffer, iLen);
//}

bool CIOCPServerHelper::Stop()
{	
	bool bRet = m_clsIOCPServer.Stop();
	return bRet;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	//::PostOnSend(dwConnectionID, pData, iLength);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	m_mapConnIDClient.Lock();
	//��������ǰ������
	if (m_mapConnIDClient.count(dwConnectionID) == 0)
	{
		TRACE(_T("CIOCPServerHelper::OnReceive dwConnectionID:0x%X������\r\n"), dwConnectionID);
		m_mapConnIDClient.Unlock();
		return ISocketListener::HR_ERROR;
	}
	PCLIENTINFO pstuClientInfo = m_mapConnIDClient[dwConnectionID];
	m_mapConnIDClient.Unlock();

	pstuClientInfo->Lock();
	if (pstuClientInfo->bIsDelete)
	{
		TRACE(_T("CIOCPServerHelper::OnReceive dwConnectionID:0x%X��ǰ������ɾ��\r\n"), dwConnectionID);
		pstuClientInfo->Unlock();
		return ISocketListener::HR_ERROR;
	}
	if (!pstuClientInfo->pclsClientBusinessHelper)
	{
		TRACE(_T("CIOCPServerHelper::OnReceive m_mapConnIDlientHelper: Error \r\n"));		
		pstuClientInfo->Unlock();
		return ISocketListener::HR_ERROR;
	}
	int iRet = pstuClientInfo->pclsClientBusinessHelper->Decode((char*)pData, iLength);
	pstuClientInfo->Unlock();
	if (iRet < 0)
	{
		TRACE(_T("������������ʧ�ܴ�������:%d(-1��ͷ���� -2���������������ʧ�� -3������ʧ�� >0�ɹ�);connID:%d;IP:%s\r\n"), 
			iRet, dwConnectionID, pstuClientInfo->stuClientBaseInfo.szIP);	
		CloseLink(dwConnectionID);
		Reset(dwConnectionID);
		return ISocketListener::HR_ERROR;
	}
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnClose(DWORD dwConnectionID)
{
	TRACE(_T("IOCPServer Client Close ID:%d!!!\r\n"), dwConnectionID);	
	Reset(dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode)
{
	TRACE(_T("IOCPServer Client Error WSAGetlastError:%d!!!\r\n"), dwConnectionID);
	Reset(dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnAccept(DWORD dwConnectionID, DWORD dwIPValue)
{
	PCLIENTINFO pstuClientInfo = NULL;
	m_mapConnIDClient.Lock();
	//�����ͻ��˶���
	if (0 == m_mapConnIDClient.count(dwConnectionID) )
	{
		//�������򴴽�
		pstuClientInfo = new CLIENTINFO;
		pstuClientInfo->Reset();
		pstuClientInfo->stuClientBaseInfo.dwConnID = dwConnectionID;
		strcpy_s(pstuClientInfo->stuClientBaseInfo.szIP, 16, ValueToIPZero(dwIPValue).c_str());
		pstuClientInfo->UpdateActiveTime();
		pstuClientInfo->pclsClientBusinessHelper = new CClientBusinessHelper(&m_objRecvPacketPool, &m_clsIOCPServer, pstuClientInfo->stuClientBaseInfo);
		m_mapConnIDClient[dwConnectionID] = pstuClientInfo;
	}
	m_mapConnIDClient.Unlock();
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnServerShutdown()
{
	return ISocketListener::HR_OK;
}

