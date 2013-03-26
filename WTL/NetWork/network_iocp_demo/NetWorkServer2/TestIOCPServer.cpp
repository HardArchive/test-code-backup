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
	m_Server.Start(_T("0.0.0.0"), 5555);
}

void CTestIOCPServer::Stop()
{
	m_Server.Stop();

}



ISocketListener::EnHandleResult CTestIOCPServer::OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	::PostOnSend(dwConnectionID, pData, iLength);
	printf("ID:%d Send Len:%d Data:%s\r\n", dwConnectionID, iLength, pData);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	::PostOnReceive(dwConnectionID, pData, iLength);
	printf("ID:%d Recv Len:%d Data:%s\r\n", dwConnectionID, iLength, pData);


	if(m_Server.Send(dwConnectionID, pData, iLength))
		return ISocketListener::HR_OK;
	else
		return ISocketListener::HR_ERROR;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnClose(DWORD dwConnectionID)
{
	::PostOnClose(dwConnectionID);
	printf("ID:%d Close\r\n", dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode)
{
	::PostOnError(dwConnectionID, enOperation, iErrorCode);
	printf("ID:%d Error\r\n", dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnAccept(DWORD dwConnectionID)
{
	::PostOnAccept(dwConnectionID);
	printf("ID:%d Accept\r\n", dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CTestIOCPServer::OnServerShutdown()
{
	::PostOnShutdown();
	return ISocketListener::HR_OK;
}