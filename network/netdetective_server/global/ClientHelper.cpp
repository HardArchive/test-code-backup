#include "stdafx.h"

#include "ClientHelper.h"


CClientHelper::CClientHelper(void):m_Client(this)
{
	m_bRevcFlag = false;
}

CClientHelper::~CClientHelper(void)
{
	//Reset();
	m_bRevcFlag = false;
}


//打开连接IP和端口  
bool CClientHelper::Start(LPCTSTR pszBindAddress, USHORT usPort)
{
	return (bool)m_Client.Start(pszBindAddress, usPort);
}

bool CClientHelper::Stop()
{
	return m_Client.Stop();
}

bool CClientHelper::Send(const BYTE* pBuffer, int iLen)
{		
	return m_Client.Send(m_Client.GetConnectionID(), pBuffer, iLen);
}


ISocketListener::EnHandleResult CClientHelper::OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CClientHelper::OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	PDATAHEAD pstuDataHead = (PDATAHEAD)pData;

	if (sizeof(PDATAHEAD)>iLength)
	{
		TRACE("Client Recive Len Is Error  Len:%d\r\n", iLength);
		printf("Client Recive Len Is Error  Len:%d\r\n", iLength);
		return ISocketListener::HR_OK;
	}

	if (TYPE_OK == pstuDataHead->dwReturn)
	{
		TRACE("Client Recive dwReturn Is OK!\r\n");
		printf("Client Recive dwReturn Is OK!\r\n");
	}
	
	if (TYPE_ERROR == pstuDataHead->dwReturn)
	{
		TRACE("Client Recive dwReturn Is Error\r\n");
		printf("Client Recive dwReturn Is OK!\r\n");
	}

	m_bRevcFlag = true;
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CClientHelper::OnClose(DWORD dwConnectionID)
{
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CClientHelper::OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode)
{
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CClientHelper::OnConnect(DWORD dwConnectionID)
{
	return ISocketListener::HR_OK;
}
