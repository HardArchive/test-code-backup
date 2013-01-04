
#include "stdafx.h"
#include "IocpServer.h"
#include "../WaitFor.h"
#include "../FuncHelper.h"

#include <malloc.h>
#include <process.h>


#define FireAccept(id)					(HasStarted() ? m_psoListener->OnAccept(id)				: ISocketListener::HR_IGNORE)
#define FireSend(id, buff, len)			(HasStarted() ? m_psoListener->OnSend(id, buff, len)	: ISocketListener::HR_IGNORE)
#define FireReceive(id, buff, len)		(HasStarted() ? m_psoListener->OnReceive(id, buff, len)	: ISocketListener::HR_IGNORE)
#define FireClose(id)					(HasStarted() ? m_psoListener->OnClose(id)				: ISocketListener::HR_IGNORE)
#define FireError(id, op, code)			(HasStarted() ? m_psoListener->OnError(id, op, code)	: ISocketListener::HR_IGNORE)
#define FireServerShutdown()			m_psoListener->OnServerShutdown()


const DWORD	CIocpServer::DEFAULT_IOCP_THREAD_COUNT		= ::GetCpuCount() * 2 + 2;
const DWORD	CIocpServer::DEFAULT_ACCEPT_SOCKET_COUNT	= DEFAULT_IOCP_THREAD_COUNT;
const DWORD	CIocpServer::DEFAULT_IOCP_BUFFER_SIZE		= 4 * 1024;
const DWORD	CIocpServer::DEFAULT_SOCKET_LISTEN_QUEUE	= 30;
const DWORD	CIocpServer::DEFAULT_FREE_SOCKETOBJ_POOL	= 50;
const DWORD	CIocpServer::DEFAULT_FREE_BUFFEROBJ_POOL	= 150;
const DWORD	CIocpServer::DEFAULT_FREE_SOCKETOBJ_HOLD	= 150;
const DWORD	CIocpServer::DEFAULT_FREE_BUFFEROBJ_HOLD	= 450;
const DWORD	CIocpServer::DEFALUT_KEEPALIVE_TIMES		= 3;
const DWORD	CIocpServer::DEFALUT_KEEPALIVE_INTERVAL		= 10 * 1000;


void CIocpServer::SetLastError(En_ISS_Error code, LPCTSTR func, int ec)
{
	m_enLastError = code;

	TRACE3("%s --> Error: %d, EC: %d\n", func, code, ec);
}

BOOL CIocpServer::Start(LPCTSTR pszBindAddress, USHORT usPort)
{
	if(!CheckParams() || !CheckStarting())
		return FALSE;

	//创建监听套接字开始监听端口
	if(CreateListenSocket(pszBindAddress, usPort))
	{
		if(CreateCompletePort())
		{
			if(CreateWorkerThreads())
			{
				if(StartAcceptThread())
				{
					m_enState = SS_STARTED;
					return TRUE;
				}
			}
		}
	}


	Stop();

	return FALSE;
}

BOOL CIocpServer::CheckParams()
{
	BOOL isOK = TRUE;
	if(m_dwIocpThreadCount > 0 && m_dwIocpThreadCount <= MAXIMUM_WAIT_OBJECTS)
		if(m_dwAcceptSocketCount > 0)
			if(m_dwIocpBufferSize > 0 && m_dwIocpBufferSize < SO_MAX_MSG_SIZE)
				if(m_dwSocketListenQueue > 0)
					if(m_dwFreeSocketObjPool >= 0)
						if(m_dwFreeBufferObjPool >= 0)
							if(m_dwFreeSocketObjHold >= m_dwFreeSocketObjPool)
								if(m_dwFreeBufferObjHold >= m_dwFreeBufferObjPool)
									if(m_dwKeepAliveTimes <= 0X7FFFFFFF)
										if(m_dwKeepAliveInterval >= 1000 && m_dwKeepAliveInterval <= 0X7FFFFFFF)
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
		addr.sin_family			= AF_INET;
		addr.sin_addr.s_addr	= inet_addr(CT2A(pszBindAddress));
		addr.sin_port			= ::htons(usPort);

		if(::bind(m_soListen, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) != SOCKET_ERROR)
		{
			if(::listen(m_soListen, m_dwSocketListenQueue) != SOCKET_ERROR)
			{
				m_pfnAcceptEx				= ::Get_AcceptEx_FuncPtr(m_soListen);  //取得接收扩展函数指针
				m_pfnGetAcceptExSockaddrs	= ::Get_GetAcceptExSockaddrs_FuncPtr(m_soListen);

				ASSERT(m_pfnAcceptEx);
				ASSERT(m_pfnGetAcceptExSockaddrs);

				isOK = TRUE;
			}
			else
				SetLastError(ISS_SOCKET_LISTEN, _T(__FUNCTION__), ::WSAGetLastError());
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
	m_hCompletePort	= ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(m_hCompletePort == NULL)
		SetLastError(ISS_CP_CREATE, _T(__FUNCTION__), ::GetLastError());

	return (m_hCompletePort != NULL);
}

BOOL CIocpServer::CreateWorkerThreads()
{
	BOOL isOK = TRUE;

	for(DWORD i = 0; i < m_dwIocpThreadCount; i++)
	{
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, WorkerThreadProc, (LPVOID)this, 0, NULL);
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
		m_hAcceptThread = (HANDLE)_beginthreadex(NULL, 0, AcceptThreadProc, (LPVOID)this, 0, NULL);
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
	WaitForWorkerThreadEnd();

	CloseClientSocket();
	CloseListenSocket();

	FireServerShutdown();

	ReleaseFreeSocket();
	ReleaseFreeBuffer();

	CloseCompletePort();

	Reset();

	m_enState = SS_STOPED;

	return TRUE;
}

void CIocpServer::CloseListenSocket()
{
	if(m_soListen != INVALID_SOCKET)
	{
		::ManualCloseSocket(m_soListen);
		m_soListen = NULL;
	}
}

void CIocpServer::WaitForAcceptThreadEnd()
{
	if(m_hAcceptThread != NULL)
	{
		m_evtAccept.Set();
		VERIFY(::WaitForSingleObject(m_hAcceptThread, INFINITE) == WAIT_OBJECT_0);

		::CloseHandle(m_hAcceptThread);
		m_hAcceptThread = NULL;
	}
}

void CIocpServer::CloseClientSocket()
{
	CCriSecLock locallock1(m_csClientSocket);

	for(TSocketObjPtrMapI it = m_mpClientSocket.begin(); it != m_mpClientSocket.end(); ++it)
	{
		TSocketObj* pSocketObj	= it->second;

		{
			CCriSecLock2 locallock2(pSocketObj->crisec);

			SOCKET& sock = pSocketObj->socket;
			if(sock != INVALID_SOCKET)
			{
				shutdown(sock, SD_RECEIVE);
				::ManualCloseSocket(sock, FALSE, TRUE);
				sock = INVALID_SOCKET;
			}
		}

		DeleteSocketObj(pSocketObj);
	}

	m_mpClientSocket.clear();
}

TSocketObj*	CIocpServer::GetFreeSocketObj()
{
	CCriSecLock locallock(m_csFreeSocket);

	TSocketObj* pSocketObj = NULL;
	if(m_lsFreeSocket.size() > 0)
	{
		pSocketObj = m_lsFreeSocket.front();
		m_lsFreeSocket.pop_front();
	}
	else
		pSocketObj = CreateSocketObj();

	return pSocketObj;
}

void CIocpServer::AddFreeSocketObj(DWORD dwConnID, BOOL bClose, BOOL bGraceful, BOOL bReuseAddress)
{
	BOOL bDone	= FALSE;

	{
		CCriSecLock locallock1(m_csClientSocket);
		CCriSecLock locallock2(m_csFreeSocket);

		TSocketObjPtrMapI it = m_mpClientSocket.find(dwConnID);

		if(it != m_mpClientSocket.end())
		{
			TSocketObj* pSocketObj = it->second;
			if(bClose && pSocketObj->socket != INVALID_SOCKET)
			{
				::ManualCloseSocket(pSocketObj->socket, bGraceful, bReuseAddress);
				pSocketObj->socket = INVALID_SOCKET;
			}

			m_mpClientSocket.erase(it);
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

	ASSERT(m_mpClientSocket.find(dwConnID) == m_mpClientSocket.end());

	m_mpClientSocket[dwConnID] = pSocketObj;
}

void CIocpServer::ReleaseFreeSocket()
{
	CompressFreeSocket(0);
}

void CIocpServer::CompressFreeSocket(size_t size)
{
	CCriSecLock locallock(m_csFreeSocket);

	while(m_lsFreeSocket.size() > size)
	{
		TSocketObj* pSocketObj	= m_lsFreeSocket.front();
		m_lsFreeSocket.pop_front();
		DeleteSocketObj(pSocketObj);
	}
}

TSocketObj*	CIocpServer::CreateSocketObj()
{
	TSocketObj*	pSocketObj = (TSocketObj*)m_hpPrivate.Alloc(sizeof(TSocketObj), CPrivateHeap::AO_ZERO_MEMORY);
	ASSERT(pSocketObj);
	CRITICAL_SECTION* pcrisec = (CRITICAL_SECTION*)m_hpPrivate.Alloc(sizeof(CRITICAL_SECTION), CPrivateHeap::AO_ZERO_MEMORY);
	ASSERT(pcrisec);
	::InitializeCriticalSection(pcrisec);
	pSocketObj->crisec.Attach(pcrisec);

	return pSocketObj;
}

void CIocpServer::DeleteSocketObj(TSocketObj* pSocketObj)
{
	CRITICAL_SECTION* pcrisec = pSocketObj->crisec.Detach();
	::DeleteCriticalSection(pcrisec);
	m_hpPrivate.Free(pcrisec);
	m_hpPrivate.Free(pSocketObj);
}

TBufferObj*	CIocpServer::GetFreeBufferObj(int iLen)
{
	if(iLen <= 0) iLen = m_dwIocpBufferSize;

	CCriSecLock locallock(m_csFreeBuffer);

	TBufferObj* pBufferObj = NULL;
	if(m_lsFreeBuffer.size() > 0)
	{
		pBufferObj = m_lsFreeBuffer.front();
		m_lsFreeBuffer.pop_front();
	}
	else
		pBufferObj = CreateBufferObj();

	pBufferObj->buff.len = iLen;
	return pBufferObj;
}

void CIocpServer::AddFreeBufferObj(TBufferObj* pBufferObj)
{
	CCriSecLock locallock(m_csFreeBuffer);

	m_lsFreeBuffer.push_back(pBufferObj);

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

	while(m_lsFreeBuffer.size() > size)
	{
		TBufferObj* pBufferObj	= m_lsFreeBuffer.front();
		m_lsFreeBuffer.pop_front();
		DeleteBufferObj(pBufferObj);
	}
}

TBufferObj*	CIocpServer::CreateBufferObj()
{
	TBufferObj*	pBufferObj	= (TBufferObj*)m_hpPrivate.Alloc(sizeof(TBufferObj), CPrivateHeap::AO_ZERO_MEMORY);
	ASSERT(pBufferObj);
	pBufferObj->buff.buf	= (char*)m_hpPrivate.Alloc(m_dwIocpBufferSize, CPrivateHeap::AO_ZERO_MEMORY);
	ASSERT(pBufferObj->buff.buf);

	return pBufferObj;
}

void CIocpServer::DeleteBufferObj(TBufferObj* pBufferObj)
{
	m_hpPrivate.Free(pBufferObj->buff.buf);
	m_hpPrivate.Free(pBufferObj);
}

TSocketObj* CIocpServer::FindSocketObj(DWORD dwConnID)
{
	TSocketObj* pSocketObj = NULL;

	CCriSecLock locallock(m_csClientSocket);

	TSocketObjPtrMapCI it = m_mpClientSocket.find(dwConnID);
	if(it != m_mpClientSocket.end())
		pSocketObj = it->second;

	return pSocketObj;
}

BOOL CIocpServer::GetConnectionAddress(DWORD dwConnID, CString& strAddress, USHORT& usPort)
{
	TSocketObj* pSocketObj = FindSocketObj(dwConnID);
	if(pSocketObj)
	{
		strAddress	= CA2T(inet_ntoa(pSocketObj->clientAddr.sin_addr));
		usPort		= ntohs(pSocketObj->clientAddr.sin_port);
	}

	return pSocketObj != NULL;
}

void CIocpServer::WaitForWorkerThreadEnd()
{
	size_t count = m_vtWorkerThreads.size();

	if(count > 0)
	{
		HANDLE* pHandles = (HANDLE*)_alloca(sizeof(HANDLE) * count);

		for(size_t i = 0; i < count; i++)
		{
			::PostQueuedCompletionStatus(m_hCompletePort, 0, 0, NULL);
			pHandles[i]	= m_vtWorkerThreads[i];
		}

		VERIFY(::WaitForMultipleObjects(count, pHandles, TRUE, INFINITE) == WAIT_OBJECT_0);

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
	if(m_hCompletePort != NULL)
	{
		::CloseHandle(m_hCompletePort);
		m_hCompletePort = NULL;
	}
}

void CIocpServer::Reset()
{
	m_dwConnID					= 0;
	m_pfnAcceptEx				= NULL;
	m_pfnGetAcceptExSockaddrs	= NULL;
	m_psemAccept				= NULL;
}

UINT WINAPI CIocpServer::AcceptThreadProc(LPVOID pv)
{
	CIocpServer* pServer	= (CIocpServer*)pv;
	pServer->m_psemAccept	= new CSEM(pServer->m_dwAcceptSocketCount, pServer->m_dwAcceptSocketCount);

	ASSERT(pServer->m_soListen		!= INVALID_SOCKET);
	ASSERT(*(pServer->m_psemAccept)	!= NULL);

	TRACE1("---------------> Accept Thread 0x%08X started <---------------\n", ::GetCurrentThreadId());

	while(TRUE)
	{
		HANDLE handles[]	= {*(pServer->m_psemAccept), pServer->m_evtAccept};
		DWORD dwResult		= ::WaitForMultipleObjectsEx(2, handles, FALSE, INFINITE, FALSE);

		/*
		if(dwResult == WAIT_OBJECT_0)
		{
		DWORD dwResult2 = ::WaitForSingleObjectEx(pServer->m_evtAccept, 0L, FALSE);

		if(dwResult2 == WAIT_OBJECT_0)
		dwResult = WAIT_OBJECT_0 + 1;
		else if(dwResult2 != WAIT_TIMEOUT)
		VERIFY(FALSE);
		}
		*/

		if(dwResult == WAIT_OBJECT_0)
		{
			TBufferObj* pBufferObj	= pServer->GetFreeBufferObj();
			SOCKET soClient			= pServer->GetAcceptSocket();

			VERIFY(::PostAccept(pServer->m_pfnAcceptEx, pServer->m_soListen, soClient, pBufferObj) == NO_ERROR);

			/*
			if(::PostAccept(pServer->m_pfnAcceptEx, pServer->m_soListen, soClient, pBufferObj) != NO_ERROR)
			{
			pServer->DeleteBufferObj(pBufferObj);
			pServer->ReleaseAcceptSockets();
			pServer->Stop();

			TRACE1("-----> Accept Thread terminated (EC: %d) <-----\n", ::WSAGetLastError());
			break;
			}
			*/
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

		ulong_set::iterator it = m_setAccept.find(socket);
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

		for(ulong_set::iterator it = m_setAccept.begin(); it != m_setAccept.end(); ++it)
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
		TBufferObj* pBufferObj;

		BOOL result = ::GetQueuedCompletionStatus
			(
			pServer->m_hCompletePort,
			&dwBytes,                 //一次完成后的I/O操作所传送数据的字节数
			(PULONG_PTR)&pSocketObj,  //当文件I/O操作完成后，用于存放与之关联的CK socket
			&pOverlapped,             //为调用IOCP机制所引用的OVERLAPPED结构
			INFINITE
			);

		//只有当 接收长度 接收socket 接收缓冲区都为空时才退出
		if((0 == dwBytes) && (NULL == pSocketObj) && (NULL == pOverlapped))
			return 0;

		//用于取得内存中任何结构体的首地址，
		//要提供的参数是：结构体中某个成员（field）的地址address、结构体的类型type、提供地址那个成员的名字field。 
		pBufferObj = CONTAINING_RECORD(pOverlapped, TBufferObj, ov);

		//GetQueuedCompletionStatus失败则返回零值
		if (!result)
		{
			DWORD dwFlag	= 0;
			DWORD dwSysCode = ::GetLastError();

			//判断socket状态  如果还处于启用则继续投递一个接收
			if(pServer->HasStarted())
			{
				SOCKET sock	= pBufferObj->operation != SO_ACCEPT ? pSocketObj->socket : (SOCKET)pSocketObj;
				result		= ::WSAGetOverlappedResult(sock, &pBufferObj->ov, &dwBytes, FALSE, &dwFlag);

				//投递失败
				if (!result)
				{
					dwErrorCode = ::WSAGetLastError();
					TRACE3("GetQueuedCompletionStatus failed (SYS: %d, SOCK: %d, FLAG: %d)\n", dwSysCode, dwErrorCode, dwFlag);
				}
			}
			else   
				dwErrorCode		= dwSysCode;   //否则标记失败
		}

		pServer->HandleIo(pSocketObj, pBufferObj, dwBytes, dwErrorCode);
	}

	return 0;
}

void CIocpServer::HandleIo(TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwBytes, DWORD dwErrorCode)
{
	ASSERT(pBufferObj != NULL);
	ASSERT(pSocketObj != NULL);

	//判断是否已经标记失败
	if(dwErrorCode != NO_ERROR)
	{
		if(pBufferObj->operation != SO_ACCEPT)
		{
			FireError(pSocketObj->connID, pBufferObj->operation, dwErrorCode);
			AddFreeSocketObj(pSocketObj->connID);
		}
		else
		{
			DeleteAcceptSocket(pBufferObj->client, TRUE);
		}

		AddFreeBufferObj(pBufferObj);
		return;
	}

	if(dwBytes == 0 && pBufferObj->operation != SO_ACCEPT)
	{
		FireClose(pSocketObj->connID);
		AddFreeSocketObj(pSocketObj->connID);
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
		HandleSend(pSocketObj, pBufferObj);
		break;
	case SO_RECEIVE:
		HandleReceive(pSocketObj, pBufferObj);
		break;
	default:
		ASSERT(FALSE);
	}
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

	TSocketObj* pSocketObj	= GetFreeSocketObj();
	pSocketObj->socket		= pBufferObj->client;
	pSocketObj->connID		= ::InterlockedIncrement((LONG*)&m_dwConnID);
	memcpy(&pSocketObj->clientAddr, pRemoteSockAddr, sizeof(SOCKADDR_IN));
	AddClientSocketObj(pSocketObj->connID, pSocketObj);

	int result = 0;
	result = ::SSO_UpdateAcceptContext(pSocketObj->socket, soListen);
	ASSERT(result == 0);

	if(m_dwKeepAliveTimes > 0 && m_dwKeepAliveInterval > 0)
	{
		result = ::SSO_KeepAliveVals(pSocketObj->socket, TRUE, m_dwKeepAliveTimes, m_dwKeepAliveInterval);
		ASSERT(result == 0);
	}

	// result = ::SSO_NoDelay(pSocketObj->socket, TRUE);
	// ASSERT(result == 0);

	VERIFY(::CreateIoCompletionPort((HANDLE)pSocketObj->socket, m_hCompletePort, (ULONG_PTR)pSocketObj, 0));

	FireAccept(pSocketObj->connID);
	DoReceive(pSocketObj, pBufferObj);
}

void CIocpServer::HandleSend(TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	FireSend(pSocketObj->connID, (BYTE*)pBufferObj->buff.buf, pBufferObj->buff.len);
	AddFreeBufferObj(pBufferObj);
}

void CIocpServer::HandleReceive(TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	if(FireReceive(pSocketObj->connID, (BYTE*)pBufferObj->buff.buf, pBufferObj->buff.len) != ISocketListener::HR_ERROR)
		DoReceive(pSocketObj, pBufferObj);
	else
	{
		TRACE1("<CNNID: %d> FireReceive() return HR_ERROR, connection will be closed", pSocketObj->connID);
		FireError(pSocketObj->connID, SO_RECEIVE, WSAEPROTOTYPE);
		AddFreeSocketObj(pSocketObj->connID);
		AddFreeBufferObj(pBufferObj);
	}
}

int CIocpServer::DoReceive(TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	pBufferObj->buff.len = m_dwIocpBufferSize;
	int result = ::PostReceive(pSocketObj, pBufferObj);
	if(result != NO_ERROR)
	{
		FireError(pSocketObj->connID, SO_RECEIVE, result);
		AddFreeSocketObj(pSocketObj->connID);
		AddFreeBufferObj(pBufferObj);
	}

	return result;
}

BOOL CIocpServer::Send(DWORD dwConnID, const BYTE* pBuffer, int iLen)
{
	ASSERT(iLen > 0);

	TSocketObj* pSocketObj	= NULL;

	{
		CCriSecLock locallock1(m_csClientSocket);

		TSocketObjPtrMapCI it = m_mpClientSocket.find(dwConnID);
		if(it != m_mpClientSocket.end())
			pSocketObj = it->second;
	}

	if(pSocketObj == NULL)
		return FALSE;

	CCriSecLock2 locallock2(pSocketObj->crisec);

	int iRemain	= iLen;

	while(iRemain > 0)
	{
		int iBufferSize = min(iRemain, (int)m_dwIocpBufferSize);
		TBufferObj* pBufferObj = GetFreeBufferObj(iBufferSize);
		memcpy(pBufferObj->buff.buf, pBuffer, iBufferSize);

		if(DoSend(pSocketObj, pBufferObj) != NO_ERROR)
			return FALSE;

		iRemain -= iBufferSize;
		pBuffer += iBufferSize;
	}

	return TRUE;
}

int CIocpServer::DoSend(TSocketObj* pSocketObj, TBufferObj* pBufferObj)
{
	int result = ::PostSend(pSocketObj, pBufferObj);
	if(result != NO_ERROR)
	{
		FireError(pSocketObj->connID, SO_SEND, result);
		AddFreeSocketObj(pSocketObj->connID);
		AddFreeBufferObj(pBufferObj);
	}

	return result;
}

LPCTSTR CIocpServer::GetLastErrorDesc()
{
	switch(m_enLastError)
	{
	case ISS_OK:					return _T("IOCP Server 启动成功");
	case ISS_ILLEGAL_STATE:			return _T("当前状态不允许操作");
	case ISS_INVALID_PARAM:			return _T("非法参数");
	case ISS_SOCKET_CREATE:			return _T("创建监听 SOCKET 失败");
	case ISS_SOCKET_BIND:			return _T("绑定监听地址失败");
	case ISS_SOCKET_LISTEN:			return _T("启动监听失败");
	case ISS_CP_CREATE:				return _T("创建完成端口失败");
	case ISS_WORKER_THREAD_CREATE:	return _T("创建工作线程失败");
	case ISS_SOCKE_ATTACH_TO_CP:	return _T("监听 SOCKET 绑定到完成端口失败");
	case ISS_ACCEPT_THREAD_CREATE:	return _T("创建监听线程失败");
	default: ASSERT(FALSE);			return _T("");
	}
}
