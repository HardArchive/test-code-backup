/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.1.1
 * Author	: Bruce Liang
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: http://qun.qq.com/#jointhegroup/gid/75375912
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
#include "IocpServer.h"
#include "../WaitFor.h"
#include "../FuncHelper.h"

#include <malloc.h>
#include <process.h>


#define FirePrepareListen(soListen)			(m_psoListener->OnPrepareListen(soListen))
#define FireAccept(id, soClient)			(m_psoListener->OnAccept(id, soClient))
#define FireSend(id, buff, len)				(m_psoListener->OnSend(id, buff, len))
#define FireReceive(id, buff, len)			(m_psoListener->OnReceive(id, buff, len))
#define FireClose(id)						(m_psoListener->OnClose(id))
#define FireError(id, op, code)				(m_psoListener->OnError(id, op, code))
#define FireServerShutdown()				(m_psoListener->OnServerShutdown())


const DWORD	CIocpServer::DEFAULT_WORKER_THREAD_COUNT	= ::GetCpuCount() * 2 + 2;
const DWORD	CIocpServer::DEFAULT_ACCEPT_SOCKET_COUNT	= 2 * DEFAULT_WORKER_THREAD_COUNT;
const DWORD	CIocpServer::DEFAULT_SOCKET_BUFFER_SIZE		= 4 * 1024 - sizeof(TBufferObj);
const DWORD	CIocpServer::DEFAULT_SOCKET_LISTEN_QUEUE	= 30;
const DWORD	CIocpServer::DEFAULT_FREE_SOCKETOBJ_POOL	= 100;
const DWORD	CIocpServer::DEFAULT_FREE_BUFFEROBJ_POOL	= 200;
const DWORD	CIocpServer::DEFAULT_FREE_SOCKETOBJ_HOLD	= 300;
const DWORD	CIocpServer::DEFAULT_FREE_BUFFEROBJ_HOLD	= 600;
const DWORD	CIocpServer::DEFALUT_KEEPALIVE_TIMES		= 3;
const DWORD	CIocpServer::DEFALUT_KEEPALIVE_INTERVAL		= 10 * 1000;
const DWORD	CIocpServer::DEFAULT_MAX_SHUTDOWN_WAIT_TIME	= 5 * 1000;


void CIocpServer::SetLastError(En_ISS_Error code, LPCTSTR func, int ec)
{
	m_enLastError = code;

	TRACE3("%s --> Error: %d, EC: %d\n", func, code, ec);
}

BOOL CIocpServer::Start(LPCTSTR pszBindAddress, USHORT usPort)
{
	if(!CheckParams() || !CheckStarting())
		return FALSE;

	if(CreateListenSocket(pszBindAddress, usPort))
		if(CreateCompletePort())
			if(CreateWorkerThreads())
				if(StartAcceptThread())
				{
					m_enState = SS_STARTED;
					return TRUE;
				}

	Stop();

	return FALSE;
}

BOOL CIocpServer::CheckParams()
{
	BOOL isOK = TRUE;
	if((int)m_dwWorkerThreadCount > 0 && m_dwWorkerThreadCount <= MAXIMUM_WAIT_OBJECTS)
		if((int)m_dwAcceptSocketCount > 0)
			if((int)m_dwSocketBufferSize > 0)
				if((int)m_dwSocketListenQueue > 0)
					if((int)m_dwFreeSocketObjPool >= 0)
						if((int)m_dwFreeBufferObjPool >= 0)
							if((int)m_dwFreeSocketObjHold >= m_dwFreeSocketObjPool)
								if((int)m_dwFreeBufferObjHold >= m_dwFreeBufferObjPool)
									if((int)m_dwKeepAliveTimes >= 0)
										if((int)m_dwKeepAliveInterval >= 0)
											if((int)m_dwMaxShutdownWaitTime >= 0)
												return TRUE;

	SetLastError(ISS_INVALID_PARAM, _T(__FUNCTION__), ERROR_INVALID_PARAMETER);
	return FALSE;
}

BOOL CIocpServer::CheckStarting()
{
	if(m_enState == SS_STOPED)
		m_enState = SS_STARTING;
	else
	{
		SetLastError(ISS_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_OPERATION);
		return FALSE;
	}

	return TRUE;
}

BOOL CIocpServer::CheckStoping()
{
	if(m_enState == SS_STARTED || m_enState == SS_STARTING)
		m_enState = SS_STOPING;
	else
	{
		SetLastError(ISS_ILLEGAL_STATE, _T(__FUNCTION__), ERROR_INVALID_OPERATION);
		return FALSE;
	}

	return TRUE;
}

BOOL CIocpServer::CreateListenSocket(LPCTSTR pszBindAddress, USHORT usPort)
{
	BOOL isOK = FALSE;

	m_soListen	= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_soListen != INVALID_SOCKET)
	{
		SOCKADDR_IN addr;
		::sockaddr_A_2_IN(AF_INET, pszBindAddress, usPort, addr);

		if(::bind(m_soListen, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) != SOCKET_ERROR)
		{
			if(FirePrepareListen(m_soListen) != ISocketListener::HR_ERROR)
			{
				if(::listen(m_soListen, m_dwSocketListenQueue) != SOCKET_ERROR)
				{
					m_pfnAcceptEx				= ::Get_AcceptEx_FuncPtr(m_soListen);
					m_pfnGetAcceptExSockaddrs	= ::Get_GetAcceptExSockaddrs_FuncPtr(m_soListen);

					ASSERT(m_pfnAcceptEx);
					ASSERT(m_pfnGetAcceptExSockaddrs);

					isOK = TRUE;
				}
				else
					SetLastError(ISS_SOCKET_LISTEN, _T(__FUNCTION__), ::WSAGetLastError());
			}
			else
				SetLastError(ISS_SOCKET_PREPARE, _T(__FUNCTION__), ERROR_FUNCTION_FAILED);
		}
		else
			SetLastError(ISS_SOCKET_BIND, _T(__FUNCTION__), ::WSAGetLastError());
	}
	else
		SetLastError(ISS_SOCKET_CREATE, _T(__FUNCTION__), ::WSAGetLastError());

	return isOK;
}

BOOL CIocpServer::CreateCompletePort()
{
	m_hCompletePort	= ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if(m_hCompletePort == nullptr)
		SetLastError(ISS_CP_CREATE, _T(__FUNCTION__), ::GetLastError());

	return (m_hCompletePort != nullptr);
}

BOOL CIocpServer::CreateWorkerThreads()
{
	BOOL isOK = TRUE;

	for(DWORD i = 0; i < m_dwWorkerThreadCount; i++)
	{
		HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThreadProc, (LPVOID)this, 0, nullptr);
		if(hThread)
			m_vtWorkerThreads.push_back(hThread);
		else
		{
			SetLastError(ISS_WORKER_THREAD_CREATE, _T(__FUNCTION__), ::GetLastError());
			isOK = FALSE;
			break;
		}
	}

	return isOK;
}

BOOL CIocpServer::StartAcceptThread()
{
	BOOL isOK = FALSE;

	if(::CreateIoCompletionPort((HANDLE)m_soListen, m_hCompletePort, m_soListen, 0))
	{
		m_hAcceptThread = (HANDLE)_beginthreadex(nullptr, 0, AcceptThreadProc, (LPVOID)this, 0, nullptr);
		if(m_hAcceptThread)
			isOK = TRUE;
		else
			SetLastError(ISS_ACCEPT_THREAD_CREATE, _T(__FUNCTION__), ::GetLastError());
	}
	else
		SetLastError(ISS_SOCKE_ATTACH_TO_CP, _T(__FUNCTION__), ::GetLastError());

	return isOK;
}

BOOL CIocpServer::Stop()
{
	if(!CheckStoping())
		return FALSE;

	WaitForAcceptThreadEnd();

	DisconnectClientSocket();
	WaitForClientSocketClose();
	WaitForWorkerThreadEnd();

	ReleaseClientSocket();
	CloseListenSocket();

	FireServerShutdown();

	ReleaseFreeSocket();
	ReleaseFreeBuffer();

	CloseCompletePort();

	Reset();

	return TRUE;
}

void CIocpServer::Reset()
{
	m_phSocket.Reset();
	m_phBuffer.Reset();

	m_dwConnID					= 0;
	m_pfnAcceptEx				= nullptr;
	m_pfnGetAcceptExSockaddrs	= nullptr;
	m_psemAccept				= nullptr;
	m_enState					= SS_STOPED;
}

void CIocpServer::CloseListenSocket()
{
	if(m_soListen != INVALID_SOCKET)
	{
		::ManualCloseSocket(m_soListen);
		m_soListen = INVALID_SOCKET;
	}
}

void CIocpServer::WaitForAcceptThreadEnd()
{
	if(m_hAcceptThread != nullptr)
	{
		m_evtAccept.Set();
		VERIFY(::WaitForSingleObject(m_hAcceptThread, INFINITE) == WAIT_OBJECT_0);

		::CloseHandle(m_hAcceptThread);
		m_hAcceptThread = nullptr;
	}
}

void CIocpServer::DisconnectClientSocket()
{
	CCriSecLock locallock(m_csClientSocket);

	for(TSocketObjPtrMapI it = m_mpClientSocket.begin(); it != m_mpClientSocket.end(); ++it)
	{
		TSocketObj* pSocketObj = it->second;

		shutdown(pSocketObj->socket, SD_SEND);
	}
}

void CIocpServer::ReleaseClientSocket()
{
	CCriSecLock locallock(m_csClientSocket);

	for(TSocketObjPtrMapI it = m_mpClientSocket.begin(); it != m_mpClientSocket.end(); ++it)
	{
		TSocketObj* pSocketObj = it->second;

		CloseSocketObj(pSocketObj);
		DeleteSocketObj(pSocketObj);
	}

	 m_mpClientSocket.clear();
}

TSocketObj*	CIocpServer::GetFreeSocketObj()
{
	TSocketObj* pSocketObj = nullptr;

	if(m_lsFreeSocket.size() > 0)
	{
		CCriSecLock locallock(m_csFreeSocket);

		if(m_lsFreeSocket.size() > 0)
		{
			pSocketObj = m_lsFreeSocket.front();
			m_lsFreeSocket.pop_front();
		}
	}

	if(!pSocketObj) pSocketObj = CreateSocketObj();

	return pSocketObj;
}

void CIocpServer::AddFreeSocketObj(DWORD dwConnID)
{
	BOOL bDone	= FALSE;

	{
		CCriSecLock locallock1(m_csClientSocket);
		TSocketObj* pSocketObj = FindSocketObj(dwConnID);

		if(pSocketObj != nullptr)
		{
			CloseSocketObj(pSocketObj);
			m_mpClientSocket.erase(dwConnID);

			CCriSecLock locallock2(m_csFreeSocket);
			m_lsFreeSocket.push_back(pSocketObj);

			bDone = TRUE;
		}
	}

	if(bDone && m_lsFreeSocket.size() > m_dwFreeSocketObjHold)
		CompressFreeSocket(m_dwFreeSocketObjPool);
}

void CIocpServer::AddClientSocketObj(DWORD dwConnID, TSocketObj* pSocketObj)
{
	CCriSecLock locallock(m_csClientSocket);

	ASSERT(FindSocketObj(dwConnID) == nullptr);

	m_mpClientSocket[dwConnID] = pSocketObj;
}

void CIocpServer::ReleaseFreeSocket()
{
	CompressFreeSocket(0);
}

void CIocpServer::CompressFreeSocket(size_t size)
{
	CCriSecLock locallock(m_csFreeSocket);

	//if((m_lsFreeSocket.size() > size))
	//{
		while(m_lsFreeSocket.size() > size)
		{
			TSocketObj* pSocketObj	= m_lsFreeSocket.front();
			m_lsFreeSocket.pop_front();
			DeleteSocketObj(pSocketObj);
		}

	//	m_phSocket.Compact();
	//}
}

TSocketObj*	CIocpServer::CreateSocketObj()
{
	TSocketObj*	pSocketObj		= (TSocketObj*)m_phSocket.Alloc(sizeof(TSocketObj) + sizeof(CRITICAL_SECTION), HEAP_ZERO_MEMORY);
	CRITICAL_SECTION* pcrisec	= (CRITICAL_SECTION*)(((char*)pSocketObj) + sizeof(TSocketObj));

	ASSERT(pSocketObj);

	::InitializeCriticalSection(pcrisec);
	pSocketObj->crisec.Attach(pcrisec);
	
	return pSocketObj;
}

void CIocpServer::DeleteSocketObj(TSocketObj* pSocketObj)
{
	CRITICAL_SECTION* pcrisec = pSocketObj->crisec.Detach();

	::DeleteCriticalSection(pcrisec);
	m_phSocket.Free(pSocketObj);
}

TBufferObj*	CIocpServer::GetFreeBufferObj(int iLen)
{
	ASSERT(iLen >= 0 && iLen <= (int)m_dwSocketBufferSize);

	TBufferObj* pBufferObj = nullptr;

	if(m_lsFreeBuffer.size() > 0)
	{
		CCriSecLock locallock(m_csFreeBuffer);

		if(m_lsFreeBuffer.size() > 0)
		{
			pBufferObj = m_lsFreeBuffer.front();
			m_lsFreeBuffer.pop_front();
		}
	}

	if(!pBufferObj) pBufferObj = CreateBufferObj();

	if(iLen <= 0) iLen	 = m_dwSocketBufferSize;
	pBufferObj->buff.len = iLen;

	return pBufferObj;
}

void CIocpServer::AddFreeBufferObj(TBufferObj* pBufferObj)
{
	{
		CCriSecLock locallock(m_csFreeBuffer);
		m_lsFreeBuffer.push_back(pBufferObj);
	}

	if(m_lsFreeBuffer.size() > m_dwFreeBufferObjHold)
		CompressFreeBuffer(m_dwFreeBufferObjPool);
}

void CIocpServer::ReleaseFreeBuffer()
{
	CompressFreeBuffer(0);
}

void CIocpServer::CompressFreeBuffer(size_t size)
{
	CCriSecLock locallock(m_csFreeBuffer);

	//if(m_lsFreeBuffer.size() > size)
	//{
		while(m_lsFreeBuffer.size() > size)
		{
			TBufferObj* pBufferObj	= m_lsFreeBuffer.front();
			m_lsFreeBuffer.pop_front();
			DeleteBufferObj(pBufferObj);
		}

	//	m_phBuffer.Compact();
	//}
}

TBufferObj*	CIocpServer::CreateBufferObj()
{
	TBufferObj*	pBufferObj	= (TBufferObj*)m_phBuffer.Alloc(sizeof(TBufferObj) + m_dwSocketBufferSize, HEAP_ZERO_MEMORY);
	pBufferObj->buff.buf	= ((char*)pBufferObj) + sizeof(TBufferObj);

	ASSERT(pBufferObj);

	return pBufferObj;
}

void CIocpServer::DeleteBufferObj(TBufferObj* pBufferObj)
{
	m_phBuffer.Free(pBufferObj);
}

TSocketObj* CIocpServer::FindSocketObj(DWORD dwConnID)
{
	TSocketObj* pSocketObj	= nullptr;
	TSocketObjPtrMapCI it	= m_mpClientSocket.find(dwConnID);

	if(it != m_mpClientSocket.end())
		pSocketObj = it->second;

	return pSocketObj;
}

void CIocpServer::CloseSocketObj(TSocketObj* pSocketObj, int iShutdownFlag)
{
	if(pSocketObj != nullptr && pSocketObj->socket != INVALID_SOCKET)
	{
		CCriSecLock2 locallock(pSocketObj->crisec);

		if(pSocketObj->socket != INVALID_SOCKET)
		{
			::ManualCloseSocket(pSocketObj->socket, iShutdownFlag);
			pSocketObj->socket = INVALID_SOCKET;
		}
	}
}

BOOL CIocpServer::GetListenAddress(CString& strAddress, USHORT& usPort)
{
	return ::GetSocketLocalAddress(m_soListen, strAddress, usPort);
}

BOOL CIocpServer::GetClientAddress(DWORD dwConnID, CString& strAddress, USHORT& usPort)
{
	TSocketObj* pSocketObj = FindSocketObj(dwConnID);

	if(pSocketObj != nullptr)
	{
		ADDRESS_FAMILY usFamily;
		return ::sockaddr_IN_2_A(pSocketObj->clientAddr, usFamily, strAddress, usPort);
	}

	return FALSE;
}

BOOL CIocpServer::Disconnect(DWORD dwConnID)
{
	TSocketObj* pSocketObj = FindSocketObj(dwConnID);

	if(pSocketObj != nullptr && pSocketObj->socket != INVALID_SOCKET)
		return shutdown(pSocketObj->socket, SD_SEND) != SOCKET_ERROR;

	return FALSE;
}

void CIocpServer::WaitForClientSocketClose()
{
	DWORD dwWait = 0;
	DWORD dwOrig = ::TimeGetTime();

	while(m_mpClientSocket.size() > 0 && dwWait < m_dwMaxShutdownWaitTime)
	{
		::WaitWithMessageLoop(100);
		dwWait = ::GetTimeGap32(dwOrig);
	}
}

void CIocpServer::WaitForWorkerThreadEnd()
{
	size_t count = m_vtWorkerThreads.size();

	if(count > 0)
	{
		HANDLE* pHandles = (HANDLE*)_alloca(sizeof(HANDLE) * count);

		for(size_t i = 0; i < count; i++)
		{
			::PostQueuedCompletionStatus(m_hCompletePort, 0, 0, nullptr);
			pHandles[i]	= m_vtWorkerThreads[i];
		}

		VERIFY(::WaitForMultipleObjects((DWORD)count, pHandles, TRUE, INFINITE) == WAIT_OBJECT_0);

		for(size_t i = 0; i < count; i++)
			::CloseHandle(pHandles[i]);

		m_vtWorkerThreads.clear();
	}
}

void CIocpServer::TerminateWorkerThread()
{
	size_t count = m_vtWorkerThreads.size();

	for(size_t i = 0; i < count; i++)
	{
		HANDLE hThread = m_vtWorkerThreads[i];
		::TerminateThread(hThread, 1);
		::CloseHandle(hThread);
	}

	m_vtWorkerThreads.clear();
}

void CIocpServer::CloseCompletePort()
{
	if(m_hCompletePort != nullptr)
	{
		::CloseHandle(m_hCompletePort);
		m_hCompletePort = nullptr;
	}
}

UINT WINAPI CIocpServer::AcceptThreadProc(LPVOID pv)
{
	CIocpServer* pServer	= (CIocpServer*)pv;
	pServer->m_psemAccept	= new CSEM(pServer->m_dwAcceptSocketCount, pServer->m_dwAcceptSocketCount);

	ASSERT(pServer->m_soListen		!= INVALID_SOCKET);
	ASSERT(*(pServer->m_psemAccept)	!= nullptr);

	TRACE1("---------------> Accept Thread 0x%08X started <---------------\n", ::GetCurrentThreadId());

	while(TRUE)
	{
		HANDLE handles[]	= {*(pServer->m_psemAccept), pServer->m_evtAccept};
		DWORD dwResult		= ::WaitForMultipleObjectsEx(2, handles, FALSE, INFINITE, FALSE);

		if(dwResult == WAIT_OBJECT_0)
		{
			TBufferObj* pBufferObj	= pServer->GetFreeBufferObj();
			SOCKET soClient			= pServer->GetAcceptSocket();

			VERIFY(::PostAccept(pServer->m_pfnAcceptEx, pServer->m_soListen, soClient, pBufferObj) == NO_ERROR);
		}
		else if(dwResult == WAIT_OBJECT_0 + 1)
		{
			pServer->ReleaseAcceptSockets();

			TRACE1("---------------> Accept Thread 0x%08X stoped <---------------\n", ::GetCurrentThreadId());
			break;
		}
		else
			VERIFY(FALSE);
	}

	return 0;
}

SOCKET CIocpServer::GetAcceptSocket()
{
	SOCKET soClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ASSERT(soClient != INVALID_SOCKET);

	CCriSecLock locallock(m_csAccept);
	m_setAccept.insert(soClient);

	return soClient;
}

BOOL CIocpServer::DeleteAcceptSocket(SOCKET socket, BOOL bCloseSocket)
{
	BOOL isOK		= FALSE;
	BOOL isExist	= FALSE;

	{
		CCriSecLock locallock(m_csAccept);

		ulong_ptr_set::iterator it = m_setAccept.find(socket);
		if(it != m_setAccept.end())
		{
			isExist = TRUE;
			m_setAccept.erase(it);
		}
	}

	if(isExist)
	{
		isOK = m_psemAccept->Release();

		if(bCloseSocket)
			::ManualCloseSocket(socket);
	}

	return isOK;
}

void CIocpServer::ReleaseAcceptSockets()
{
	{
		CCriSecLock locallock(m_csAccept);

		for(ulong_ptr_set::iterator it = m_setAccept.begin(); it != m_setAccept.end(); ++it)
			::ManualCloseSocket(*it);

		m_setAccept.clear();
	}

	while(m_psemAccept->Release()) {}
}


UINT WINAPI CIocpServer::WorkerThreadProc(LPVOID pv)
{
	CIocpServer* pServer	= (CIocpServer*)pv;

	while (TRUE)
	{
		DWORD dwErrorCode = NO_ERROR;

		DWORD dwBytes;
		OVERLAPPED* pOverlapped;
		TSocketObj* pSocketObj;
		
		BOOL result = ::GetQueuedCompletionStatus
												(
													pServer->m_hCompletePort,
													&dwBytes,
													(PULONG_PTR)&pSocketObj,
													&pOverlapped,
													INFINITE
												);

		if(dwBytes == 0 && pSocketObj == nullptr && pOverlapped == nullptr)
			return 0;

		TBufferObj* pBufferObj	= CONTAINING_RECORD(pOverlapped, TBufferObj, ov);
		DWORD dwConnID			= pBufferObj->operation != SO_ACCEPT ? pSocketObj->connID : 0;

		if (!result)
		{
			DWORD dwFlag	= 0;
			DWORD dwSysCode = ::GetLastError();

			if(pServer->HasStarted())
			{
				SOCKET sock	= pBufferObj->operation != SO_ACCEPT ? pSocketObj->socket : (SOCKET)pSocketObj;
				result		= ::WSAGetOverlappedResult(sock, &pBufferObj->ov, &dwBytes, FALSE, &dwFlag);

				if (!result)
				{
					dwErrorCode = ::WSAGetLastError();
					TRACE("GetQueuedCompletionStatus error (<CNNID: %d> SYS: %d, SOCK: %d, FLAG: %d)\n", dwConnID, dwSysCode, dwErrorCode, dwFlag);
				}
			}
			else
				dwErrorCode		= dwSysCode;
		}

		pServer->HandleIo(dwConnID, pSocketObj, pBufferObj, dwBytes, dwErrorCode);
	}

	return 0;
}

void CIocpServer::HandleIo(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwBytes, DWORD dwErrorCode)
{
	ASSERT(pBufferObj != nullptr);
	ASSERT(pSocketObj != nullptr);

	if(dwErrorCode != NO_ERROR)
	{
		HandleError(dwConnID, pSocketObj, pBufferObj, dwErrorCode);
		return;
	}

	if(dwBytes == 0 && pBufferObj->operation != SO_ACCEPT)
	{
		FireClose(dwConnID);
		AddFreeSocketObj(dwConnID);
		AddFreeBufferObj(pBufferObj);
		return;
	}

	pBufferObj->buff.len = dwBytes;

	switch(pBufferObj->operation)
	{
	case SO_ACCEPT:
		HandleAccept((SOCKET)pSocketObj, pBufferObj);
		break;
	case SO_SEND:
		HandleSend(dwConnID, pSocketObj, pBufferObj);
		break;
	case SO_RECEIVE:
		HandleReceive(dwConnID, pSocketObj, pBufferObj);
		break;
	default:
		ASSERT(FALSE);
	}
}

void CIocpServer::HandleError(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwErrorCode)
{
	if(pBufferObj->operation != SO_ACCEPT)
	{
		if(dwErrorCode != WSAENOTSOCK)
		{
			FireError(dwConnID, pBufferObj->operation, dwErrorCode);
			AddFreeSocketObj(dwConnID);
		}
	}
	else
	{
		DeleteAcceptSocket(pBufferObj->client, TRUE);
	}

	AddFreeBufferObj(pBufferObj);
}

void CIocpServer::HandleAccept(SOCKET soListen, TBufferObj* pBufferObj)
{
	DeleteAcceptSocket(pBufferObj->client);

	int iLocalSockaddrLen;
	int iRemoteSockaddrLen;
	SOCKADDR* pLocalSockAddr;
	SOCKADDR* pRemoteSockAddr;

	m_pfnGetAcceptExSockaddrs
							(
								pBufferObj->buff.buf,
								0,
								sizeof(SOCKADDR_IN) + 16,
								sizeof(SOCKADDR_IN) + 16,
								(SOCKADDR **)&pLocalSockAddr,
								&iLocalSockaddrLen,
								(SOCKADDR **)&pRemoteSockAddr,
								&iRemoteSockaddrLen
							);

	DWORD dwConnID			= ::InterlockedIncrement((LONG*)&m_dwConnID);
	SOCKET socket			= pBufferObj->client;
	TSocketObj* pSocketObj	= GetFreeSocketObj();
	pSocketObj->connID		= dwConnID;
	pSocketObj->socket		= socket;

	memcpy(&pSocketObj->clientAddr, pRemoteSockAddr, sizeof(SOCKADDR_IN));
	AddClientSocketObj(dwConnID, pSocketObj);

	int result = ::SSO_UpdateAcceptContext(socket, soListen);
	ASSERT(result == 0);

	if(m_dwKeepAliveTimes > 0 && m_dwKeepAliveInterval > 0)
	{
		result = ::SSO_KeepAliveVals(socket, TRUE, m_dwKeepAliveTimes, m_dwKeepAliveInterval);
		ASSERT(result == 0);
	}

	if(FireAccept(dwConnID, socket) != ISocketListener::HR_ERROR)
	{
		VERIFY(::CreateIoCompletionPort((HANDLE)socket, m_hCompletePort, (ULONG_PTR)pSocketObj, 0));
		DoReceive(dwConnID, pSocketObj, pBufferObj);
	}
	else
	{
		AddFreeSocketObj(dwConnID);
		AddFreeBufferObj(pBufferObj);
	}
}

void CIocpServer::HandleSend(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	if(FireSend(dwConnID, (BYTE*)pBufferObj->buff.buf, pBufferObj->buff.len) == ISocketListener::HR_ERROR)
	{
		TRACE1("<CNNID: %d> OnSend() event should not return 'HR_ERROR' !!\n", dwConnID);
		ASSERT(FALSE);
	}

	AddFreeBufferObj(pBufferObj);
}

void CIocpServer::HandleReceive(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	if(FireReceive(dwConnID, (BYTE*)pBufferObj->buff.buf, pBufferObj->buff.len) != ISocketListener::HR_ERROR)
		DoReceive(dwConnID, pSocketObj, pBufferObj);
	else
	{
		TRACE1("<CNNID: %d> OnReceive() event return 'HR_ERROR', connection will be closed !\n", dwConnID);
		FireError(dwConnID, SO_RECEIVE, ERROR_FUNCTION_FAILED);
		AddFreeSocketObj(dwConnID);
		AddFreeBufferObj(pBufferObj);
	}
}

int CIocpServer::DoReceive(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	pBufferObj->buff.len = m_dwSocketBufferSize;
	int result			 = ::PostReceive(pSocketObj, pBufferObj);

	if(result != NO_ERROR)
	{
		if(result != WSAENOTSOCK)
		{
			FireError(dwConnID, SO_RECEIVE, result);
			AddFreeSocketObj(dwConnID);
		}

		AddFreeBufferObj(pBufferObj);
	}

	return result;
}

BOOL CIocpServer::Send(DWORD dwConnID, const BYTE* pBuffer, int iLen)
{
	ASSERT(iLen > 0);

	TSocketObj* pSocketObj = FindSocketObj(dwConnID);

	if(pSocketObj == nullptr)
		return FALSE;

	int result = DoSend(dwConnID, pSocketObj, pBuffer, iLen);

	if(result != NO_ERROR && result != WSAENOTSOCK)
	{
		FireError(dwConnID, SO_SEND, result);
		AddFreeSocketObj(dwConnID);
	}

	return (result == NO_ERROR);
}

int CIocpServer::DoSend(DWORD dwConnID, TSocketObj* pSocketObj, const BYTE* pBuffer, int iLen)
{
	int result	= NO_ERROR;
	int iRemain	= iLen;

	CCriSecLock2 locallock(pSocketObj->crisec);

	while(iRemain > 0)
	{
		int iBufferSize = min(iRemain, (int)m_dwSocketBufferSize);
		TBufferObj* pBufferObj = GetFreeBufferObj(iBufferSize);
		memcpy(pBufferObj->buff.buf, pBuffer, iBufferSize);

		result = ::PostSend(pSocketObj, pBufferObj);

		if(result != NO_ERROR)
		{
			AddFreeBufferObj(pBufferObj);
			break;
		}

		iRemain -= iBufferSize;
		pBuffer += iBufferSize;
	}

	return result;
}

LPCTSTR CIocpServer::GetLastErrorDesc()
{
	switch(m_enLastError)
	{
	case ISS_OK:					return _T("�ɹ�");
	case ISS_ILLEGAL_STATE:			return _T("��ǰ״̬���������");
	case ISS_INVALID_PARAM:			return _T("�Ƿ�����");
	case ISS_SOCKET_CREATE:			return _T("�������� SOCKET ʧ��");
	case ISS_SOCKET_BIND:			return _T("�󶨼�����ַʧ��");
	case ISS_SOCKET_PREPARE:		return _T("���ü��� SOCKET ʧ��");
	case ISS_SOCKET_LISTEN:			return _T("��������ʧ��");
	case ISS_CP_CREATE:				return _T("������ɶ˿�ʧ��");
	case ISS_WORKER_THREAD_CREATE:	return _T("���������߳�ʧ��");
	case ISS_SOCKE_ATTACH_TO_CP:	return _T("���� SOCKET �󶨵���ɶ˿�ʧ��");
	case ISS_ACCEPT_THREAD_CREATE:	return _T("���������߳�ʧ��");
	default: ASSERT(FALSE);			return _T("");
	}
}
