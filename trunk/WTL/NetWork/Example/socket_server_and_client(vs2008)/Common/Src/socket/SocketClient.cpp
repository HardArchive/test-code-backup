#include "stdafx.h"
#include "SocketClient.h"
#include "../WaitFor.h"

#ifndef _WIN32_WCE
	#include <process.h>
#else
	#define _beginthreadex	::CreateThread
#endif

#define FireConnect(id)					m_pListener->OnConnect(id)
#define FireSend(id, data, len)			(HasStarted() ? m_pListener->OnSend(id, data, len)		: ISocketListener::HR_IGNORE)
#define FireReceive(id, data, len)		(HasStarted() ? m_pListener->OnReceive(id, data, len)	: ISocketListener::HR_IGNORE)
#define FireClose(id)					(HasStarted() ? m_pListener->OnClose(id)				: ISocketListener::HR_IGNORE)
#define FireError(id, op, code)			(HasStarted() ? m_pListener->OnError(id, op, code)		: ISocketListener::HR_IGNORE)

BOOL CSocketClient::Start(LPCTSTR pszRemoteAddress, USHORT usPort)
{
	if(!CheckParams() || !CheckStarting())
		return FALSE;

	BOOL isOK = FALSE;

	if(CreateClientSocket())
	{
		if(ConnectToServer(pszRemoteAddress, usPort))
		{
			if(CreateWorkerThread())
				isOK = TRUE;
			else
				SetLastError(ISC_WORKER_CREATE_FAIL, _T(__FUNCTION__), ERROR_CREATE_FAILED);
		}
		else
			SetLastError(ISC_CONNECT_SERVER_FAIL, _T(__FUNCTION__), ::WSAGetLastError());
	}
	else
		SetLastError(ISC_SOCKET_CREATE_FAIL, _T(__FUNCTION__), ::WSAGetLastError());

	isOK ? m_enState = SS_STARTED : Stop();

	return isOK;
}

BOOL CSocketClient::CheckParams()
{

	if(m_dwSocketBufferSize > 0 && m_dwSocketBufferSize < SO_MAX_MSG_SIZE)
		if(m_dwKeepAliveTimes <= 0X7FFFFFFF)
			if(m_dwKeepAliveInterval >= 1000 && m_dwKeepAliveInterval <= 0X7FFFFFFF)
				return TRUE;

	SetLastError(ISC_INVALID_PARAM, _T(__FUNCTION__), ERROR_INVALID_PARAMETER);
	return FALSE;
}

BOOL CSocketClient::CheckStarting()
{
	if(m_enState == SS_STOPED)
		m_enState = SS_STARTING;
	else
	{
		SetLastError(ISC_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_OPERATION);
		return FALSE;
	}

	return TRUE;
}

BOOL CSocketClient::CheckStoping()
{
	if(m_enState == SS_STARTED || m_enState == SS_STARTING)
		m_enState = SS_STOPING;
	else
	{
		SetLastError(ISC_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_OPERATION);
		return FALSE;
	}

	return TRUE;
}

BOOL CSocketClient::CreateClientSocket()
{
	BOOL isOK = FALSE;

	m_soClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_soClient != INVALID_SOCKET)
	{
#ifndef _WIN32_WCE

		if(m_dwKeepAliveTimes > 0 && m_dwKeepAliveInterval > 0)
			VERIFY(::SSO_KeepAliveVals(m_soClient, TRUE, m_dwKeepAliveTimes, m_dwKeepAliveInterval) == 0);

		// VERIFY(::SSO_NoDelay(m_soClient, TRUE) == 0);
#endif

		m_evSocket	= ::WSACreateEvent();
		ASSERT(m_evSocket != WSA_INVALID_EVENT);

		m_dwConnID	= (DWORD)m_evSocket;
		isOK		= TRUE;
	}

	return isOK;
}

BOOL CSocketClient::ConnectToServer(LPCTSTR pszRemoteAddress, USHORT usPort)
{
	CStringA strAddress;
	if(!::GetIPAddress(pszRemoteAddress, strAddress))
		return FALSE;

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr(strAddress);
	addr.sin_port			= htons(usPort);

	BOOL isOK = FALSE;

	if(connect(m_soClient, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) != SOCKET_ERROR)
	{
		if(::WSAEventSelect(m_soClient, m_evSocket, FD_READ | FD_WRITE | FD_CLOSE) != SOCKET_ERROR)
		{
			FireConnect(m_dwConnID);
			isOK = TRUE;
		}
	}

	return isOK;
}

BOOL CSocketClient::CreateWorkerThread()
{
	m_hWorker = (HANDLE)_beginthreadex(NULL, 0, WorkerThreadProc, (LPVOID)this, 0, &m_dwWorkerID);

	return m_hWorker != NULL;
}

#ifndef _WIN32_WCE
	UINT
#else
	DWORD
#endif
	WINAPI CSocketClient::WorkerThreadProc(LPVOID pv)
{
	TRACE1("---------------> Worker Thread 0x%08X startped <---------------\n", ::GetCurrentThreadId());

	CSocketClient* pClient = (CSocketClient*)pv;

	pClient->m_sndBuffer.Malloc(pClient->m_dwSocketBufferSize);
	pClient->m_rcBuffer.Malloc(pClient->m_dwSocketBufferSize);

	HANDLE hEvents[] = {pClient->m_evSocket, pClient->m_evBuffer, pClient->m_evStop};

	while(pClient->HasStarted())
	{
		DWORD retval = ::WSAWaitForMultipleEvents(3, hEvents, FALSE, WSA_INFINITE, FALSE);

		if(retval == WSA_WAIT_EVENT_0)
		{
			if(!pClient->ProcessNetworkEvent())
			{
				if(pClient->HasStarted())
					pClient->Stop();

				break;
			}
		}
		else if(retval == WSA_WAIT_EVENT_0 + 1)
		{
			if(!pClient->SendData())
			{
				if(pClient->HasStarted())
					pClient->Stop();

				break;
			}
		}
		else if(retval == WSA_WAIT_EVENT_0 + 2)
			break;
		else
			ASSERT(FALSE);
	}

	TRACE1("---------------> Worker Thread 0x%08X stoped <---------------\n", ::GetCurrentThreadId());

	return 0;
}

BOOL CSocketClient::ProcessNetworkEvent()
{
	WSANETWORKEVENTS events;
	
	int rc = ::WSAEnumNetworkEvents(m_soClient, m_evSocket, &events);
	VERIFY(::WSAResetEvent(m_evSocket));
	
	if(rc == SOCKET_ERROR)
	{
		int code = ::WSAGetLastError();
		SetLastError(ISC_NETWORK_ERROR, _T(__FUNCTION__), code);
		FireError(m_dwConnID, SO_UNKNOWN, code);

		return FALSE;
	}

	if(events.lNetworkEvents & FD_READ)
	{
		int iCode = events.iErrorCode[FD_READ_BIT];

		if(iCode == 0)
			return ReadData();
		else
		{
			SetLastError(ISC_NETWORK_ERROR, _T(__FUNCTION__), iCode);
			FireError(m_dwConnID, SO_RECEIVE, iCode);
			return FALSE;
		}
	}

	if(events.lNetworkEvents & FD_WRITE)
	{
		int iCode = events.iErrorCode[FD_WRITE_BIT];

		if(iCode == 0)
			return SendData();
		else
		{
			SetLastError(ISC_NETWORK_ERROR, _T(__FUNCTION__), iCode);
			FireError(m_dwConnID, SO_SEND, iCode);
			return FALSE;
		}
	}

	if(events.lNetworkEvents & FD_CLOSE)
	{
		int iCode = events.iErrorCode[FD_CLOSE_BIT];

		if(iCode == 0)
			FireClose(m_dwConnID);
		else
		{
			SetLastError(ISC_NETWORK_ERROR, _T(__FUNCTION__), iCode);
			FireError(m_dwConnID, SO_UNKNOWN, iCode);
		}

		return FALSE;
	}

	return TRUE;
}

BOOL CSocketClient::ReadData()
{
	BOOL isOK = FALSE;

	while(TRUE)
	{
		int rc = recv(m_soClient, (char*)(BYTE*)m_rcBuffer, m_dwSocketBufferSize, 0);

		if(rc > 0)
		{
			if(FireReceive(m_dwConnID, m_rcBuffer, rc) == ISocketListener::HR_ERROR)
			{
				TRACE1("<CNNID: %d> FireReceive() return HR_ERROR, connection will be closed", m_dwConnID);

				SetLastError(ISC_PROTOCOL_ERROR, _T(__FUNCTION__), ERROR_INVALID_DATA);
				FireError(m_dwConnID, SO_RECEIVE, ERROR_INVALID_DATA);

				break;
			}
			else
				isOK = TRUE;
		}
		else if(rc == SOCKET_ERROR)
		{
			int code = ::WSAGetLastError();

			if(code == WSAEWOULDBLOCK)
				isOK = TRUE;
			else
			{
				SetLastError(ISC_NETWORK_ERROR, _T(__FUNCTION__), code);
				FireError(m_dwConnID, SO_RECEIVE, code);
			}

			break;
		}
		else if(rc == 0)
		{
			FireClose(m_dwConnID);
			break;
		}
		else
			ASSERT(FALSE);
	}

	return isOK;
}

BOOL CSocketClient::SendData()
{
	CCriSecLock locallock(m_scBuffer);

	BOOL isOK = TRUE;

	while(m_sndData.Size() > 0)
	{
		int rc = send(m_soClient, (char*)(BYTE*)m_sndData, min(m_sndData.Size(), (int)m_dwSocketBufferSize), 0);
		
		if(rc > 0)
		{
			memcpy((BYTE*)m_sndBuffer, (BYTE*)m_sndData, rc);

			int left = m_sndData.Size() - rc;
			memcpy((BYTE*)m_sndData, (BYTE*)m_sndData + rc, left);
			m_sndData.Realloc(left);

			FireSend(m_dwConnID, m_sndBuffer, rc);

		}
		else if(rc == SOCKET_ERROR)
		{
			int code = ::WSAGetLastError();

			if(code != WSAEWOULDBLOCK)
			{
				SetLastError(ISC_NETWORK_ERROR, _T(__FUNCTION__), code);
				FireError(m_dwConnID, SO_SEND, code);
				
				isOK = FALSE;
			}

			break;
		}
		else
			ASSERT(FALSE);
	}
	
	return isOK;
}

BOOL CSocketClient::Stop()
{
	if(!CheckStoping())
		return FALSE;

	if(m_hWorker != NULL)
	{
		if(::GetCurrentThreadId() != m_dwWorkerID)
			WaitForWorkerThreadEnd();

		::CloseHandle(m_hWorker);
		m_hWorker		= NULL;
		m_dwWorkerID	= 0;
	}

	if(m_evSocket != NULL)
	{
		::WSACloseEvent(m_evSocket);
		m_evSocket	= NULL;
	}

	if(m_soClient != INVALID_SOCKET)
	{
		shutdown(m_soClient, SD_SEND);
		closesocket(m_soClient);
		m_soClient	= INVALID_SOCKET;
	}

	m_sndData.Free();
	m_sndBuffer.Free();
	m_rcBuffer.Free();
	m_evBuffer.Reset();
	m_evStop.Reset();

	m_dwConnID	= 0;
	m_enState	= SS_STOPED;

	return TRUE;
}

void CSocketClient::WaitForWorkerThreadEnd()
{
	m_evStop.Set();

	VERIFY(::WaitForSingleObject(m_hWorker, INFINITE) == WAIT_OBJECT_0);

	//if(!::MsgWaitForSingleObject(m_hWorker, /*INFINITE*/ WORKER_THREAD_END_TIME))

	/*
	DWORD dwResult = ::WaitForSingleObject(m_hWorker, WORKER_THREAD_END_TIME);
	ASSERT(dwResult != WAIT_FAILED);

	if(dwResult == WAIT_TIMEOUT)
		::TerminateThread(m_hWorker, -1);
	*/
}

BOOL CSocketClient::Send(DWORD dwConnID, const BYTE* pBuffer, int iLen)
{
	ASSERT(pBuffer && iLen > 0);

	if(!HasStarted())
	{
		SetLastError(ISC_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_STATE);
		return FALSE;
	}

	if(!pBuffer || iLen <= 0)
	{
		SetLastError(ISC_INVALID_PARAM, _T(__FUNCTION__), ERROR_NO_DATA);
		return FALSE;
	}

	CCriSecLock locallock(m_scBuffer);

	m_sndData.Cat(pBuffer, iLen);
	m_evBuffer.Set();

	return TRUE;
}

void CSocketClient::SetLastError(En_ISC_Error code, LPCTSTR func, int ec)
{
	m_enLastError = code;

	TRACE3("%s --> Error: %d, EC: %d\n", func, code, ec);
}

LPCTSTR CSocketClient::GetLastErrorDesc()
{
	switch(m_enLastError)
	{
	case ISC_OK:					return _T("Client Socket 启动成功");
	case ISC_ILLEGAL_STATE:			return _T("当前状态不允许操作");
	case ISC_INVALID_PARAM:			return _T("非法参数");
	case ISC_SOCKET_CREATE_FAIL:	return _T("创建 Client Socket 失败");
	case ISC_CONNECT_SERVER_FAIL:	return _T("连接服务器失败");
	case ISC_WORKER_CREATE_FAIL:	return _T("创建工作线程失败");
	case ISC_NETWORK_ERROR:			return _T("网络错误");
	case ISC_PROTOCOL_ERROR:		return _T("通信协议错误");
	default: ASSERT(FALSE);			return _T("");
	}
}
