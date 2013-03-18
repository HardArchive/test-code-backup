#include "StdAfx.h"
#include "IOCPServerHelper.h"

CIOCPServerHelper::CIOCPServerHelper(void):m_Server(this)
{
}

CIOCPServerHelper::~CIOCPServerHelper(void)
{
}

bool CIOCPServerHelper::Start(LPCTSTR pszBindAddress, USHORT usPort)
{
	return (bool)m_Server.Start(pszBindAddress, usPort);
}

bool CIOCPServerHelper::Send(DWORD dwConnID, const BYTE* pBuffer, int iLen)
{
	return m_Server.Send(dwConnID, pBuffer, iLen);
}

bool CIOCPServerHelper::Stop()
{
	return m_Server.Stop();
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	::PostOnSend(dwConnectionID, pData, iLength);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	::PostOnReceive(dwConnectionID, pData, iLength);

	if(m_Server.Send(dwConnectionID, pData, iLength))
		return ISocketListener::HR_OK;
	else
		return ISocketListener::HR_ERROR;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnClose(DWORD dwConnectionID)
{
	::PostOnClose(dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode)
{
	::PostOnError(dwConnectionID, enOperation, iErrorCode);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnAccept(DWORD dwConnectionID)
{
	::PostOnAccept(dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CIOCPServerHelper::OnServerShutdown()
{
	::PostOnShutdown();
	return ISocketListener::HR_OK;
}