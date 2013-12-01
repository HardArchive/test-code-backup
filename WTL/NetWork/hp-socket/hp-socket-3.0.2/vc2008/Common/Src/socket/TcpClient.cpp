/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 3.0.1
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "stdafx.h"
#include "TcpClient.h"
#include "../WaitFor.h"

#ifndef _WIN32_WCE
	#include <process.h>
#else
	#define _beginthreadex	::CreateThread
#endif

volatile CONNID CTcpClient::sm_dwConnID = 0;

BOOL CTcpClient::Start(LPCTSTR pszRemoteAddress, USHORT usPort, BOOL bAsyncConnect)
{
	if(!CheckParams() || !CheckStarting())
		return FALSE;

	BOOL isOK		= FALSE;
	m_bAsyncConnect	= bAsyncConnect;

	if(CreateClientSocket())
	{
		if(FirePrepareConnect(m_dwConnID, m_soClient) != ISocketListener::HR_ERROR)
		{
			if(ConnectToServer(pszRemoteAddress, usPort))
			{
				if(CreateWorkerThread())
					isOK = TRUE;
				else
					SetLastError(CE_WORKER_CREATE_FAIL, _T(__FUNCTION__), ERROR_CREATE_FAILED);
			}
			else
				SetLastError(CE_CONNECT_SERVER_FAIL, _T(__FUNCTION__), ::WSAGetLastError());
		}
		else
			SetLastError(CE_SOCKET_PREPARE_FAIL, _T(__FUNCTION__), ERROR_FUNCTION_FAILED);
	}
	else
		SetLastError(CE_SOCKET_CREATE_FAIL, _T(__FUNCTION__), ::WSAGetLastError());

	if(!isOK) Stop();

	return isOK;
}

BOOL CTcpClient::CheckParams()
{

	if((int)m_dwSocketBufferSize > 0)
		if((int)m_dwKeepAliveTime >= 0)
			if((int)m_dwKeepAliveInterval >= 0)
				return TRUE;

	SetLastError(CE_INVALID_PARAM, _T(__FUNCTION__), ERROR_INVALID_PARAMETER);
	return FALSE;
}

BOOL CTcpClient::CheckStarting()
{
	if(m_enState == SS_STOPED)
		m_enState = SS_STARTING;
	else
	{
		SetLastError(CE_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_OPERATION);
		return FALSE;
	}

	return TRUE;
}

BOOL CTcpClient::CheckStoping()
{
	if(m_enState == SS_STARTED || m_enState == SS_STARTING)
		m_enState = SS_STOPING;
	else
	{
		SetLastError(CE_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_OPERATION);
		return FALSE;
	}

	return TRUE;
}

BOOL CTcpClient::CreateClientSocket()
{
	m_soClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_soClient != INVALID_SOCKET)
	{
#ifndef _WIN32_WCE
		BOOL bOnOff	= (m_dwKeepAliveTime > 0 && m_dwKeepAliveInterval > 0);
		VERIFY(::SSO_KeepAliveVals(m_soClient, bOnOff, m_dwKeepAliveTime, m_dwKeepAliveInterval) == NO_ERROR);
#endif

		m_evSocket	= ::WSACreateEvent();
		ASSERT(m_evSocket != WSA_INVALID_EVENT);

		m_dwConnID	= ::GenerateConnectionID(sm_dwConnID);

		return TRUE;
	}

	return FALSE;
}

BOOL CTcpClient::ConnectToServer(LPCTSTR pszRemoteAddress, USHORT usPort)
{
	CString strAddress;
	if(!::GetIPAddress(pszRemoteAddress, strAddress))
		return FALSE;

	SOCKADDR_IN addr;
	if(!::sockaddr_A_2_IN(AF_INET, strAddress, usPort, addr))
		return FALSE;

	BOOL isOK = FALSE;

	if(m_bAsyncConnect)
	{
		if(::WSAEventSelect(m_soClient, m_evSocket, FD_CONNECT | FD_CLOSE) != SOCKET_ERROR)
		{
			int rc = connect(m_soClient, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
			isOK = (rc == NO_ERROR || (rc == SOCKET_ERROR && ::WSAGetLastError() == WSAEWOULDBLOCK));
		}
	}
	else
	{
		if(connect(m_soClient, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) != SOCKET_ERROR)
		{
			if(::WSAEventSelect(m_soClient, m_evSocket, FD_READ | FD_WRITE | FD_CLOSE) != SOCKET_ERROR)
			{
				if(FireConnect(m_dwConnID) != ISocketListener::HR_ERROR)
				{
					m_enState	= SS_STARTED;
					isOK		= TRUE;
				}
			}
		}
	}

	return isOK;
}

BOOL CTcpClient::CreateWorkerThread()
{
	m_hWorker = (HANDLE)_beginthreadex(nullptr, 0, WorkerThreadProc, (LPVOID)this, 0, &m_dwWorkerID);

	return m_hWorker != nullptr;
}

#ifndef _WIN32_WCE
	UINT
#else
	DWORD
#endif
	WINAPI CTcpClient::WorkerThreadProc(LPVOID pv)
{
	TRACE1("---------------> Client Worker Thread 0x%08X started <---------------\n", ::GetCurrentThreadId());

	CTcpClient* pClient	= (CTcpClient*)pv;
	HANDLE hEvents[]	= {pClient->m_evSocket, pClient->m_evBuffer, pClient->m_evWorker};

	pClient->m_rcBuffer.Malloc(pClient->m_dwSocketBufferSize);

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

	TRACE1("---------------> Client Worker Thread 0x%08X stoped <---------------\n", ::GetCurrentThreadId());

	return 0;
}

BOOL CTcpClient::ProcessNetworkEvent()
{
	BOOL bContinue = TRUE;
	WSANETWORKEVENTS events;
	
	int rc = ::WSAEnumNetworkEvents(m_soClient, m_evSocket, &events);

	if(rc == SOCKET_ERROR)
	{
		int code = ::WSAGetLastError();
		SetLastError(CE_NETWORK_ERROR, _T(__FUNCTION__), code);

		VERIFY(::WSAResetEvent(m_evSocket));
		FireError(m_dwConnID, SO_UNKNOWN, code);

		bContinue = FALSE;
	}

	if(bContinue && events.lNetworkEvents & FD_READ)
	{
		int iCode = events.iErrorCode[FD_READ_BIT];

		if(iCode == 0)
			bContinue = ReadData();
		else
		{
			SetLastError(CE_NETWORK_ERROR, _T(__FUNCTION__), iCode);
			FireError(m_dwConnID, SO_RECEIVE, iCode);
			bContinue = FALSE;
		}
	}

	if(bContinue && events.lNetworkEvents & FD_WRITE)
	{
		int iCode = events.iErrorCode[FD_WRITE_BIT];

		if(iCode == 0)
			bContinue = SendData();
		else
		{
			SetLastError(CE_NETWORK_ERROR, _T(__FUNCTION__), iCode);
			FireError(m_dwConnID, SO_SEND, iCode);
			bContinue = FALSE;
		}
	}

	if(m_bAsyncConnect && bContinue && events.lNetworkEvents & FD_CONNECT)
	{
		int iCode = events.iErrorCode[FD_CONNECT_BIT];

		if(iCode == 0)
		{
			if(::WSAEventSelect(m_soClient, m_evSocket, FD_READ | FD_WRITE | FD_CLOSE) != SOCKET_ERROR)
			{
				if(FireConnect(m_dwConnID) != ISocketListener::HR_ERROR)
					m_enState = SS_STARTED;
				else
					iCode = ERROR_FUNCTION_FAILED;
			}
			else
				iCode = ::WSAGetLastError();
		}

		if(iCode != 0)
		{
			SetLastError(CE_NETWORK_ERROR, _T(__FUNCTION__), iCode);
			FireError(m_dwConnID, SO_CONNECT, iCode);
			bContinue = FALSE;
		}
	}

	if(bContinue && events.lNetworkEvents & FD_CLOSE)
	{
		int iCode = events.iErrorCode[FD_CLOSE_BIT];

		if(iCode == 0)
			FireClose(m_dwConnID);
		else
		{
			SetLastError(CE_NETWORK_ERROR, _T(__FUNCTION__), iCode);
			FireError(m_dwConnID, SO_UNKNOWN, iCode);
		}

		bContinue = FALSE;
	}

	return bContinue;
}

BOOL CTcpClient::ReadData()
{
	while(TRUE)
	{
		int rc = recv(m_soClient, (char*)(BYTE*)m_rcBuffer, m_dwSocketBufferSize, 0);

		if(rc > 0)
		{
			if(FireReceive(m_dwConnID, m_rcBuffer, rc) == ISocketListener::HR_ERROR)
			{
				TRACE1("<C-CNNID: %Iu> OnReceive() event return 'HR_ERROR', connection will be closed !\n", m_dwConnID);

				SetLastError(CE_DATA_PROC_ERROR, _T(__FUNCTION__), ERROR_FUNCTION_FAILED);
				FireError(m_dwConnID, SO_RECEIVE, ERROR_FUNCTION_FAILED);

				return FALSE;
			}
		}
		else if(rc == SOCKET_ERROR)
		{
			int code = ::WSAGetLastError();

			if(code == WSAEWOULDBLOCK)
				break;
			else
			{
				SetLastError(CE_NETWORK_ERROR, _T(__FUNCTION__), code);
				FireError(m_dwConnID, SO_RECEIVE, code);

				return FALSE;
			}
		}
		else if(rc == 0)
		{
			FireClose(m_dwConnID);
			return FALSE;
		}
		else
			ASSERT(FALSE);
	}

	return TRUE;
}

BOOL CTcpClient::SendData()
{
	BOOL isOK = TRUE;

	while(TRUE)
	{
		smart_simple_ptr<CBufferPtr> bufferPtr = GetSendBuffer();

		if(bufferPtr.is_valid())
		{
			CBufferPtr& sndData	= *bufferPtr;
			isOK				= DoSendData(sndData);

			if(isOK)
			{
				if(sndData.Size() > 0)
				{
					CCriSecLock locallock(m_scBuffer);
					m_sndBuffers->push_front(bufferPtr.release());
					break;
				}
			}
			else
				break;
		}
		else
			break;
	}

	return isOK;
}

CBufferPtr* CTcpClient::GetSendBuffer()
{
	CBufferPtr* pBuffer = nullptr;

	if(m_sndBuffers.Size() > 0)
	{
		CCriSecLock locallock(m_scBuffer);

		if(m_sndBuffers.Size() > 0)
		{
			pBuffer = m_sndBuffers->front();
			m_sndBuffers->pop_front();
		}
	}

	return pBuffer;
}

BOOL CTcpClient::DoSendData(CBufferPtr& sndData)
{
	while(sndData.Size() > 0)
	{
		int rc = send(m_soClient, (char*)(BYTE*)sndData, min((int)sndData.Size(), (int)m_dwSocketBufferSize), 0);

		if(rc > 0)
		{
			if(FireSend(m_dwConnID, sndData, rc) == ISocketListener::HR_ERROR)
			{
				TRACE1("<C-CNNID: %Iu> OnSend() event should not return 'HR_ERROR' !!\n", m_dwConnID);
				ASSERT(FALSE);
			}

			int left = (int)sndData.Size() - rc;
			memcpy((BYTE*)sndData, (BYTE*)sndData + rc, left);
			sndData.Realloc(left);
		}
		else if(rc == SOCKET_ERROR)
		{
			int code = ::WSAGetLastError();

			if(code == WSAEWOULDBLOCK)
				break;
			else
			{
				SetLastError(CE_NETWORK_ERROR, _T(__FUNCTION__), code);
				FireError(m_dwConnID, SO_SEND, code);

				return FALSE;
			}
		}
		else
			ASSERT(FALSE);
	}

	return TRUE;
}

BOOL CTcpClient::Stop()
{
	BOOL bNeedFireClose			= FALSE;
	EnServiceState enCurState	= m_enState;
	DWORD dwCurrentThreadID		= ::GetCurrentThreadId();

	if(!CheckStoping())
		return FALSE;

	if(enCurState == SS_STARTED && dwCurrentThreadID != m_dwWorkerID)
		bNeedFireClose = TRUE;

	WaitForWorkerThreadEnd(dwCurrentThreadID);

	if(bNeedFireClose)
		FireClose(m_dwConnID);

	if(m_evSocket != nullptr)
	{
		::WSACloseEvent(m_evSocket);
		m_evSocket	= nullptr;
	}

	if(m_soClient != INVALID_SOCKET)
	{
		shutdown(m_soClient, SD_SEND);
		closesocket(m_soClient);
		m_soClient	= INVALID_SOCKET;
	}

	m_sndBuffers.Clear();
	m_rcBuffer.Free();
	m_evBuffer.Reset();
	m_evWorker.Reset();

	m_enState = SS_STOPED;

	return TRUE;
}

void CTcpClient::WaitForWorkerThreadEnd(DWORD dwCurrentThreadID)
{
	if(m_hWorker != nullptr)
	{
		if(dwCurrentThreadID != m_dwWorkerID)
		{
			m_evWorker.Set();
			VERIFY(::WaitForSingleObject(m_hWorker, INFINITE) == WAIT_OBJECT_0);
		}

		::CloseHandle(m_hWorker);

		m_hWorker		= nullptr;
		m_dwWorkerID	= 0;
	}
}

BOOL CTcpClient::Send(CONNID dwConnID, const BYTE* pBuffer, int iLength)
{
	ASSERT(pBuffer && iLength > 0);

	if(!HasStarted())
	{
		SetLastError(CE_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_STATE);
		return FALSE;
	}

	if(!pBuffer || iLength <= 0)
	{
		SetLastError(CE_INVALID_PARAM, _T(__FUNCTION__), ERROR_NO_DATA);
		return FALSE;
	}

	{
		CCriSecLock locallock(m_scBuffer);

		if(!HasStarted())
		{
			SetLastError(CE_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_STATE);
			return FALSE;
		}

		m_sndBuffers->push_back(new CBufferPtr(pBuffer, iLength));
		m_evBuffer.Set();
	}

	return TRUE;
}

void CTcpClient::SetLastError(EnClientError code, LPCTSTR func, int ec)
{
	m_enLastError = code;

	TRACE3("%s --> Error: %d, EC: %d\n", func, code, ec);
}

BOOL CTcpClient::GetLocalAddress(CString& strAddress, USHORT& usPort)
{
	return ::GetSocketLocalAddress(m_soClient, strAddress, usPort);
}

LPCTSTR CTcpClient::GetLastErrorDesc()
{
	switch(m_enLastError)
	{
	case CE_OK:					return _T("成功");
	case CE_ILLEGAL_STATE:			return _T("当前状态不允许操作");
	case CE_INVALID_PARAM:			return _T("非法参数");
	case CE_SOCKET_CREATE_FAIL:	return _T("创建 Client Socket 失败");
	case CE_SOCKET_PREPARE_FAIL:	return _T("设置 Client Socket 失败");
	case CE_CONNECT_SERVER_FAIL:	return _T("连接服务器失败");
	case CE_WORKER_CREATE_FAIL:	return _T("创建工作线程失败");
	case CE_NETWORK_ERROR:			return _T("网络错误");
	case CE_DATA_PROC_ERROR:		return _T("数据处理错误");
	default: ASSERT(FALSE);			return _T("");
	}
}
