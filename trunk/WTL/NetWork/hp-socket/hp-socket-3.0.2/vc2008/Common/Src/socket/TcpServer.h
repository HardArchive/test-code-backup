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
 
#pragma once

#include "SocketHelper.h"
#include "../PrivateHeap.h"
#include "../Event.h"
#include "../RWLock.h"
#include "../CriticalSection.h"
#include "../STLHelper.h"

class CTcpServer : public ITcpServer
{
public:
	static const DWORD MAX_WORKER_THREAD_COUNT;
	static const DWORD MIN_SOCKET_BUFFER_SIZE;

	static const DWORD DEFAULT_WORKER_THREAD_COUNT;
	static const DWORD DEFAULT_ACCEPT_SOCKET_COUNT;
	static const DWORD DEFAULT_SOCKET_BUFFER_SIZE;
	static const DWORD DEFAULT_SOCKET_LISTEN_QUEUE;
	static const DWORD DEFAULT_FREE_SOCKETOBJ_LOCK_TIME;
	static const DWORD DEFAULT_FREE_SOCKETOBJ_POOL;
	static const DWORD DEFAULT_FREE_BUFFEROBJ_POOL;
	static const DWORD DEFAULT_FREE_SOCKETOBJ_HOLD;
	static const DWORD DEFAULT_FREE_BUFFEROBJ_HOLD;
	static const DWORD DEFALUT_KEEPALIVE_TIME;
	static const DWORD DEFALUT_KEEPALIVE_INTERVAL;
	static const DWORD DEFAULT_MAX_SHUTDOWN_WAIT_TIME;

public:
	CTcpServer(ITcpServerListener* psoListener)
	: m_psoListener				(psoListener)
	, m_hCompletePort			(nullptr)
	, m_soListen				(INVALID_SOCKET)
	, m_pfnAcceptEx				(nullptr)
	, m_pfnGetAcceptExSockaddrs	(nullptr)
	, m_pfnDisconnectEx			(nullptr)
	, m_enLastError				(SE_OK)
	, m_enState					(SS_STOPED)
	, m_dwWorkerThreadCount		(DEFAULT_WORKER_THREAD_COUNT)
	, m_dwAcceptSocketCount		(DEFAULT_ACCEPT_SOCKET_COUNT)
	, m_dwSocketBufferSize		(DEFAULT_SOCKET_BUFFER_SIZE)
	, m_dwSocketListenQueue		(DEFAULT_SOCKET_LISTEN_QUEUE)
	, m_dwFreeSocketObjLockTime	(DEFAULT_FREE_SOCKETOBJ_LOCK_TIME)
	, m_dwFreeSocketObjPool		(DEFAULT_FREE_SOCKETOBJ_POOL)
	, m_dwFreeBufferObjPool		(DEFAULT_FREE_BUFFEROBJ_POOL)
	, m_dwFreeSocketObjHold		(DEFAULT_FREE_SOCKETOBJ_HOLD)
	, m_dwFreeBufferObjHold		(DEFAULT_FREE_BUFFEROBJ_HOLD)
	, m_dwKeepAliveTime			(DEFALUT_KEEPALIVE_TIME)
	, m_dwKeepAliveInterval		(DEFALUT_KEEPALIVE_INTERVAL)
	, m_dwMaxShutdownWaitTime	(DEFAULT_MAX_SHUTDOWN_WAIT_TIME)
	{
		ASSERT(m_wsSocket.IsValid());
		ASSERT(m_psoListener);

		Reset();
	}

	virtual ~CTcpServer()
	{
		if(HasStarted())
			Stop();
	}

public:
	virtual BOOL Start	(LPCTSTR pszBindAddress, USHORT usPort);
	virtual BOOL Stop	();
	virtual BOOL Send	(CONNID dwConnID, const BYTE* pBuffer, int iLength);
	virtual BOOL			HasStarted		()	{return m_enState == SS_STARTED || m_enState == SS_STARTING;}
	virtual EnServiceState	GetState		()	{return m_enState;}
	virtual EnServerError	GetLastError	()	{return m_enLastError;}
	virtual BOOL			Disconnect		(CONNID dwConnID, BOOL bForce = TRUE);
	virtual BOOL			GetListenAddress(CString& strAddress, USHORT& usPort);
	virtual BOOL			GetClientAddress(CONNID dwConnID, CString& strAddress, USHORT& usPort);
	virtual LPCTSTR			GetLastErrorDesc();

public:
	virtual BOOL SetConnectionExtra(CONNID dwConnID, PVOID pExtra);
	virtual BOOL GetConnectionExtra(CONNID dwConnID, PVOID* ppExtra);
	virtual BOOL GetConnectionCriSec(CONNID dwConnID, CCriSec2** ppCriSec);

	virtual void SetWorkerThreadCount		(DWORD dwWorkerThreadCount)		{m_dwWorkerThreadCount		= dwWorkerThreadCount;}
	virtual void SetAcceptSocketCount		(DWORD dwAcceptSocketCount)		{m_dwAcceptSocketCount		= dwAcceptSocketCount;}
	virtual void SetSocketBufferSize		(DWORD dwSocketBufferSize)		{m_dwSocketBufferSize		= dwSocketBufferSize;}
	virtual void SetSocketListenQueue		(DWORD dwSocketListenQueue)		{m_dwSocketListenQueue		= dwSocketListenQueue;}
	virtual void SetFreeSocketObjLockTime	(DWORD dwFreeSocketObjLockTime)	{m_dwFreeSocketObjLockTime	= dwFreeSocketObjLockTime;}
	virtual void SetFreeSocketObjPool		(DWORD dwFreeSocketObjPool)		{m_dwFreeSocketObjPool		= dwFreeSocketObjPool;}
	virtual void SetFreeBufferObjPool		(DWORD dwFreeBufferObjPool)		{m_dwFreeBufferObjPool		= dwFreeBufferObjPool;}
	virtual void SetFreeSocketObjHold		(DWORD dwFreeSocketObjHold)		{m_dwFreeSocketObjHold		= dwFreeSocketObjHold;}
	virtual void SetFreeBufferObjHold		(DWORD dwFreeBufferObjHold)		{m_dwFreeBufferObjHold		= dwFreeBufferObjHold;}
	virtual void SetKeepAliveTime			(DWORD dwKeepAliveTime)			{m_dwKeepAliveTime			= dwKeepAliveTime;}
	virtual void SetKeepAliveInterval		(DWORD dwKeepAliveInterval)		{m_dwKeepAliveInterval		= dwKeepAliveInterval;}
	virtual void SetMaxShutdownWaitTime		(DWORD dwMaxShutdownWaitTime)	{m_dwMaxShutdownWaitTime	= dwMaxShutdownWaitTime;}

	virtual DWORD GetWorkerThreadCount		()	{return m_dwWorkerThreadCount;}
	virtual DWORD GetAcceptSocketCount		()	{return m_dwAcceptSocketCount;}
	virtual DWORD GetSocketBufferSize		()	{return m_dwSocketBufferSize;}
	virtual DWORD GetSocketListenQueue		()	{return m_dwSocketListenQueue;}
	virtual DWORD GetFreeSocketObjLockTime	()	{return m_dwFreeSocketObjLockTime;}
	virtual DWORD GetFreeSocketObjPool		()	{return m_dwFreeSocketObjPool;}
	virtual DWORD GetFreeBufferObjPool		()	{return m_dwFreeBufferObjPool;}
	virtual DWORD GetFreeSocketObjHold		()	{return m_dwFreeSocketObjHold;}
	virtual DWORD GetFreeBufferObjHold		()	{return m_dwFreeBufferObjHold;}
	virtual DWORD GetKeepAliveTime			()	{return m_dwKeepAliveTime;}
	virtual DWORD GetKeepAliveInterval		()	{return m_dwKeepAliveInterval;}
	virtual DWORD GetMaxShutdownWaitTime	()	{return m_dwMaxShutdownWaitTime;}

protected:
	virtual ISocketListener::EnHandleResult FirePrepareListen(SOCKET soListen)
		{return m_psoListener->OnPrepareListen(soListen);}
	virtual ISocketListener::EnHandleResult FireAccept(CONNID dwConnID, SOCKET soClient)
		{return m_psoListener->OnAccept(dwConnID, soClient);}
	virtual ISocketListener::EnHandleResult FireSend(CONNID dwConnID, const BYTE* pData, int iLength)
		{return m_psoListener->OnSend(dwConnID, pData, iLength);}
	virtual ISocketListener::EnHandleResult FireReceive(CONNID dwConnID, const BYTE* pData, int iLength)
		{return m_psoListener->OnReceive(dwConnID, pData, iLength);}
	virtual ISocketListener::EnHandleResult FireReceive(CONNID dwConnID, int iLength)
		{return m_psoListener->OnReceive(dwConnID, iLength);}
	virtual ISocketListener::EnHandleResult FireClose(CONNID dwConnID)
		{return m_psoListener->OnClose(dwConnID);}
	virtual ISocketListener::EnHandleResult FireError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
		{return m_psoListener->OnError(dwConnID, enOperation, iErrorCode);}
	virtual ISocketListener::EnHandleResult FireServerShutdown()
		{return m_psoListener->OnServerShutdown();}

	virtual BOOL CheckParams(BOOL bPreconditions = TRUE);

private:
	BOOL CheckStarting();
	BOOL CheckStoping();
	BOOL CreateListenSocket(LPCTSTR pszBindAddress, USHORT usPort);
	BOOL CreateCompletePort();
	BOOL CreateWorkerThreads();
	BOOL StartAccept();

	void CloseListenSocket();
	void DisconnectClientSocket();
	void WaitForClientSocketClose();
	void ReleaseClientSocket();
	void ReleaseFreeSocket();
	void CompressFreeSocket(size_t size, BOOL bForce = FALSE);
	void ReleaseFreeBuffer();
	void CompressFreeBuffer(size_t size);
	void WaitForWorkerThreadEnd();
	void TerminateWorkerThread();
	void CloseCompletePort();

	void Reset();

	TBufferObj*	GetFreeBufferObj(int iLen = 0);
	TSocketObj*	GetFreeSocketObj();
	void		AddFreeBufferObj(TBufferObj* pBufferObj);
	void		AddFreeSocketObj(CONNID dwConnID);
	TBufferObj*	CreateBufferObj();
	TSocketObj*	CreateSocketObj();
	void		DeleteBufferObj(TBufferObj* pBufferObj);
	void		DeleteSocketObj(TSocketObj* pSocketObj);

	void		AddClientSocketObj(CONNID dwConnID, TSocketObj* pSocketObj);
	TSocketObj* FindSocketObj(CONNID dwConnID);
	void		CloseSocketObj(TSocketObj* pSocketObj, int iShutdownFlag = SD_SEND);

private:
	void SetLastError(EnServerError code, LPCTSTR func, int ec);

private:
	static UINT WINAPI WorkerThreadProc(LPVOID pv);

	int CheckSpecialIndative(OVERLAPPED* pOverlapped, DWORD dwBytes, TSocketObj* pSocketObj);
	void ForceDisconnect	(CONNID dwConnID);
	void HandleIo			(CONNID dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwBytes, DWORD dwErrorCode);
	void HandleError		(CONNID dwConnID, TBufferObj* pBufferObj, DWORD dwErrorCode);
	void HandleAccept		(SOCKET soListen, TBufferObj* pBufferObj);
	void HandleSend			(CONNID dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);
	void HandleReceive		(CONNID dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);

	BOOL DoAccept	();
	int DoSend		(CONNID dwConnID, TSocketObj* pSocketObj, const BYTE* pBuffer, int iLen);
	int DoReceive	(CONNID dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);

	void CheckError	(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);

private:
	CInitSocket					m_wsSocket;
	LPFN_ACCEPTEX				m_pfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;
	LPFN_DISCONNECTEX			m_pfnDisconnectEx;

private:
	CPrivateHeap		m_phSocket;
	CPrivateHeap		m_phBuffer;

	CCriSec				m_csFreeBuffer;
	CCriSec				m_csFreeSocket;
	CRWLock				m_csClientSocket;

	TBufferObjPtrList	m_lsFreeBuffer;
	TSocketObjPtrList	m_lsFreeSocket;
	TSocketObjPtrMap	m_mpClientSocket;

	vector<HANDLE>		m_vtWorkerThreads;

	volatile EnServiceState	m_enState;
	volatile CONNID			m_dwConnID;

	EnServerError	m_enLastError;

	SOCKET			m_soListen;
	HANDLE			m_hCompletePort;

	ITcpServerListener*	m_psoListener;

private:
	DWORD m_dwWorkerThreadCount;
	DWORD m_dwAcceptSocketCount;
	DWORD m_dwSocketBufferSize;
	DWORD m_dwSocketListenQueue;
	DWORD m_dwFreeSocketObjLockTime;
	DWORD m_dwFreeSocketObjPool;
	DWORD m_dwFreeBufferObjPool;
	DWORD m_dwFreeSocketObjHold;
	DWORD m_dwFreeBufferObjHold;
	DWORD m_dwKeepAliveTime;
	DWORD m_dwKeepAliveInterval;
	DWORD m_dwMaxShutdownWaitTime;
};
