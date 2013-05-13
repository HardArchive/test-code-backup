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
 
#pragma once

#include "SocketHelper.h"
#include "../PrivateHeap.h"
#include "../Event.h"
#include "../Semaphore.h"
#include "../CriticalSection.h"
#include "../STLHelper.h"


class CIocpServer : public ISocketServer
{
public:
	static const DWORD DEFAULT_WORKER_THREAD_COUNT;
	static const DWORD DEFAULT_ACCEPT_SOCKET_COUNT;
	static const DWORD DEFAULT_SOCKET_BUFFER_SIZE;
	static const DWORD DEFAULT_SOCKET_LISTEN_QUEUE;
	static const DWORD DEFAULT_FREE_SOCKETOBJ_POOL;
	static const DWORD DEFAULT_FREE_BUFFEROBJ_POOL;
	static const DWORD DEFAULT_FREE_SOCKETOBJ_HOLD;
	static const DWORD DEFAULT_FREE_BUFFEROBJ_HOLD;
	static const DWORD DEFALUT_KEEPALIVE_TIMES;
	static const DWORD DEFALUT_KEEPALIVE_INTERVAL;
	static const DWORD DEFAULT_MAX_SHUTDOWN_WAIT_TIME;

public:
	CIocpServer(IServerSocketListener* pListener)
		: m_psoListener				(pListener)
		, m_hAcceptThread			(nullptr)
		, m_hCompletePort			(nullptr)
		, m_soListen				(INVALID_SOCKET)
		, m_pfnAcceptEx				(nullptr)
		, m_pfnGetAcceptExSockaddrs	(nullptr)
		, m_enLastError				(ISS_OK)
		, m_enState					(SS_STOPED)
		, m_dwWorkerThreadCount		(DEFAULT_WORKER_THREAD_COUNT)
		, m_dwAcceptSocketCount		(DEFAULT_ACCEPT_SOCKET_COUNT)
		, m_dwSocketBufferSize		(DEFAULT_SOCKET_BUFFER_SIZE)
		, m_dwSocketListenQueue		(DEFAULT_SOCKET_LISTEN_QUEUE)
		, m_dwFreeSocketObjPool		(DEFAULT_FREE_SOCKETOBJ_POOL)
		, m_dwFreeBufferObjPool		(DEFAULT_FREE_BUFFEROBJ_POOL)
		, m_dwFreeSocketObjHold		(DEFAULT_FREE_SOCKETOBJ_HOLD)
		, m_dwFreeBufferObjHold		(DEFAULT_FREE_BUFFEROBJ_HOLD)
		, m_dwKeepAliveTimes		(DEFALUT_KEEPALIVE_TIMES)
		, m_dwKeepAliveInterval		(DEFALUT_KEEPALIVE_INTERVAL)
		, m_dwMaxShutdownWaitTime	(DEFAULT_MAX_SHUTDOWN_WAIT_TIME)
	{
		ASSERT(m_wsSocket.IsValid());
		ASSERT(m_psoListener);

		Reset();
	}

	virtual ~CIocpServer()
	{
		if(HasStarted())
			Stop();
	}

public:
	virtual BOOL Start	(LPCTSTR pszBindAddress, USHORT usPort);
	virtual BOOL Stop	();
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen);
	virtual BOOL			HasStarted		()	{return m_enState == SS_STARTED || m_enState == SS_STARTING;}
	virtual EnServiceState	GetState		()	{return m_enState;}
	virtual En_ISS_Error	GetLastError	()	{return m_enLastError;}
	virtual BOOL			Disconnect		(DWORD dwConnID);
	virtual BOOL			GetListenAddress(CString& strAddress, USHORT& usPort);
	virtual BOOL			GetClientAddress(DWORD dwConnID, CString& strAddress, USHORT& usPort);
	virtual LPCTSTR			GetLastErrorDesc();

public:
	virtual void SetWorkerThreadCount	(DWORD dwWorkerThreadCount)		{m_dwWorkerThreadCount	= dwWorkerThreadCount;}
	virtual void SetAcceptSocketCount	(DWORD dwAcceptSocketCount)		{m_dwAcceptSocketCount	= dwAcceptSocketCount;}
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)		{m_dwSocketBufferSize	= dwSocketBufferSize;}
	virtual void SetSocketListenQueue	(DWORD dwSocketListenQueue)		{m_dwSocketListenQueue	= dwSocketListenQueue;}
	virtual void SetFreeSocketObjPool	(DWORD dwFreeSocketObjPool)		{m_dwFreeSocketObjPool	= dwFreeSocketObjPool;}
	virtual void SetFreeBufferObjPool	(DWORD dwFreeBufferObjPool)		{m_dwFreeBufferObjPool	= dwFreeBufferObjPool;}
	virtual void SetFreeSocketObjHold	(DWORD dwFreeSocketObjHold)		{m_dwFreeSocketObjHold	= dwFreeSocketObjHold;}
	virtual void SetFreeBufferObjHold	(DWORD dwFreeBufferObjHold)		{m_dwFreeBufferObjHold	= dwFreeBufferObjHold;}
	virtual void SetKeepAliveTimes		(DWORD dwKeepAliveTimes)		{m_dwKeepAliveTimes		= dwKeepAliveTimes;}
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)		{m_dwKeepAliveInterval	= dwKeepAliveInterval;}
	virtual void SetMaxShutdownWaitTime	(DWORD dwMaxShutdownWaitTime)	{m_dwKeepAliveInterval	= dwMaxShutdownWaitTime;}

	virtual DWORD GetWorkerThreadCount	()	{return m_dwWorkerThreadCount;}
	virtual DWORD GetAcceptSocketCount	()	{return m_dwAcceptSocketCount;}
	virtual DWORD GetSocketBufferSize	()	{return m_dwSocketBufferSize;}
	virtual DWORD GetSocketListenQueue	()	{return m_dwSocketListenQueue;}
	virtual DWORD GetFreeSocketObjPool	()	{return m_dwFreeSocketObjPool;}
	virtual DWORD GetFreeBufferObjPool	()	{return m_dwFreeBufferObjPool;}
	virtual DWORD GetFreeSocketObjHold	()	{return m_dwFreeSocketObjHold;}
	virtual DWORD GetFreeBufferObjHold	()	{return m_dwFreeBufferObjHold;}
	virtual DWORD GetKeepAliveTimes		()	{return m_dwKeepAliveTimes;}
	virtual DWORD GetKeepAliveInterval	()	{return m_dwKeepAliveInterval;}
	virtual DWORD GetMaxShutdownWaitTime()	{return m_dwMaxShutdownWaitTime;}

private:
	void SetLastError(En_ISS_Error code, LPCTSTR func, int ec);

private:
	BOOL CheckParams();
	BOOL CheckStarting();
	BOOL CheckStoping();
	BOOL CreateListenSocket(LPCTSTR pszBindAddress, USHORT usPort);
	BOOL CreateCompletePort();
	BOOL CreateWorkerThreads();
	BOOL StartAcceptThread();

	void CloseListenSocket();
	void WaitForAcceptThreadEnd();
	void DisconnectClientSocket();
	void WaitForClientSocketClose();
	void ReleaseClientSocket();
	void ReleaseFreeSocket();
	void CompressFreeSocket(size_t size);
	void ReleaseFreeBuffer();
	void CompressFreeBuffer(size_t size);
	void WaitForWorkerThreadEnd();
	void TerminateWorkerThread();
	void CloseCompletePort();

	void Reset();

	TBufferObj*	GetFreeBufferObj(int iLen = 0);
	TSocketObj*	GetFreeSocketObj();
	void		AddFreeBufferObj(TBufferObj* pBufferObj);
	void		AddFreeSocketObj(DWORD dwConnID);
	TBufferObj*	CreateBufferObj();
	TSocketObj*	CreateSocketObj();
	void		DeleteBufferObj(TBufferObj* pBufferObj);
	void		DeleteSocketObj(TSocketObj* pSocketObj);

	void		AddClientSocketObj(DWORD dwConnID, TSocketObj* pSocketObj);
	TSocketObj* FindSocketObj(DWORD dwConnID);
	void		CloseSocketObj(TSocketObj* pSocketObj, int iShutdownFlag = SD_SEND);

private:
	static UINT WINAPI AcceptThreadProc(LPVOID pv);
	static UINT WINAPI WorkerThreadProc(LPVOID pv);

	void HandleIo		(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwBytes, DWORD dwErrorCode);
	void HandleError	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwErrorCode);
	void HandleAccept	(SOCKET soListen, TBufferObj* pBufferObj);
	void HandleSend		(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);
	void HandleReceive	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);

	int DoSend		(DWORD dwConnID, TSocketObj* pSocketObj, const BYTE* pBuffer, int iLen);
	int DoReceive	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);

private:
	SOCKET	GetAcceptSocket();
	BOOL	DeleteAcceptSocket(SOCKET socket, BOOL bCloseSocket = FALSE);
	void	ReleaseAcceptSockets();

private:
	CInitSocket					m_wsSocket;
	LPFN_ACCEPTEX				m_pfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;

private:
	IServerSocketListener*	m_psoListener;

	volatile EnServiceState	m_enState;
	volatile DWORD			m_dwConnID;

	En_ISS_Error	m_enLastError;

	SOCKET			m_soListen;
	HANDLE			m_hCompletePort;
	HANDLE			m_hAcceptThread;
	vector<HANDLE>	m_vtWorkerThreads;

	TBufferObjPtrList	m_lsFreeBuffer;
	TSocketObjPtrList	m_lsFreeSocket;
	TSocketObjPtrMap	m_mpClientSocket;

	CCriSec				m_csFreeBuffer;
	CCriSec				m_csFreeSocket;
	CCriSec				m_csClientSocket;

	CEvt				m_evtAccept;

	smart_simple_ptr<CSEM>	m_psemAccept;
	CCriSec					m_csAccept;
	ulong_ptr_set			m_setAccept;

	CPrivateHeap			m_phSocket;
	CPrivateHeap			m_phBuffer;

private:
	DWORD m_dwWorkerThreadCount;
	DWORD m_dwAcceptSocketCount;
	DWORD m_dwSocketBufferSize;
	DWORD m_dwSocketListenQueue;
	DWORD m_dwFreeSocketObjPool;
	DWORD m_dwFreeBufferObjPool;
	DWORD m_dwFreeSocketObjHold;
	DWORD m_dwFreeBufferObjHold;
	DWORD m_dwKeepAliveTimes;
	DWORD m_dwKeepAliveInterval;
	DWORD m_dwMaxShutdownWaitTime;
};