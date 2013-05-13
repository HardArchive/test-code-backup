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
#include "../Event.h"
#include "../bufferptr.h"
#include "../CriticalSection.h"

class CSocketClient : public ISocketClient
{
public:
	virtual BOOL Start	(LPCTSTR pszRemoteAddress, USHORT usPortt, BOOL bAsyncConnect = FALSE);
	virtual BOOL Stop	();
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen);
	virtual BOOL			HasStarted		()	{return m_enState == SS_STARTED || m_enState == SS_STARTING;}
	virtual EnServiceState	GetState		()	{return m_enState;}
	virtual DWORD			GetConnectionID	()	{return m_dwConnID;};
	virtual En_ISC_Error	GetLastError	()	{return m_enLastError;}
	virtual BOOL			GetLocalAddress	(CString& strAddress, USHORT& usPort);
	virtual LPCTSTR			GetLastErrorDesc();

public:
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)	{m_dwSocketBufferSize	= dwSocketBufferSize;}
	virtual void SetKeepAliveTimes		(DWORD dwKeepAliveTimes)	{m_dwKeepAliveTimes		= dwKeepAliveTimes;}
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)	{m_dwKeepAliveInterval	= dwKeepAliveInterval;}

	virtual DWORD GetSocketBufferSize	()	{return m_dwSocketBufferSize;}
	virtual DWORD GetKeepAliveTimes		()	{return m_dwKeepAliveTimes;}
	virtual DWORD GetKeepAliveInterval	()	{return m_dwKeepAliveInterval;}

private:
	BOOL CheckParams();
	BOOL CheckStarting();
	BOOL CheckStoping();
	BOOL CreateClientSocket();
	BOOL ConnectToServer(LPCTSTR pszRemoteAddress, USHORT usPort);
	BOOL CreateWorkerThread();
	BOOL ProcessNetworkEvent();
	void WaitForWorkerThreadEnd();
	BOOL ReadData();
	BOOL SendData();
	CBufferPtr* GetSendBuffer();
	BOOL DoSendData(CBufferPtr& sndData);

	void SetLastError(En_ISC_Error code, LPCTSTR func, int ec);

static 
#ifndef _WIN32_WCE
	UINT
#else
	DWORD
#endif
	 WINAPI WorkerThreadProc(LPVOID pv);

private:
	static const DWORD DEFAULT_SOCKET_BUFFER_SIZE	= 4 * 1024;
	static const DWORD DEFALUT_KEEPALIVE_TIMES		= 3;
	static const DWORD DEFALUT_KEEPALIVE_INTERVAL	= 10 * 1000;

	static volatile DWORD	sm_dwConnID;

public:
	CSocketClient(IClientSocketListener* pListener)
	: m_pListener(pListener)
	, m_soClient			(INVALID_SOCKET)
	, m_evSocket			(nullptr)
	, m_dwConnID			(0)
	, m_hWorker				(nullptr)
	, m_dwWorkerID			(0)
	, m_bAsyncConnect		(FALSE)
	, m_enState				(SS_STOPED)
	, m_enLastError			(ISC_OK)
	, m_dwSocketBufferSize	(DEFAULT_SOCKET_BUFFER_SIZE)
	, m_dwKeepAliveTimes	(DEFALUT_KEEPALIVE_TIMES)
	, m_dwKeepAliveInterval	(DEFALUT_KEEPALIVE_INTERVAL)
	{
		ASSERT(m_pListener);
	}

	virtual ~CSocketClient()	{if(HasStarted()) Stop();}

private:
	CInitSocket		m_wsSocket;
	SOCKET			m_soClient;
	HANDLE			m_evSocket;
	DWORD			m_dwConnID;

	CEvt			m_evStop;
	HANDLE			m_hWorker;

#ifndef _WIN32_WCE
	UINT
#else
	DWORD
#endif
					m_dwWorkerID;

	BOOL			m_bAsyncConnect;

	VectorWrapper<list<CBufferPtr*>, PtrSet_Cleaner<list<CBufferPtr*>>> m_sndBuffers;
	CBufferPtr		m_sndData;
	CBufferPtr		m_rcBuffer;
	CCriSec			m_scBuffer;
	CEvt			m_evBuffer;

	volatile EnServiceState	m_enState;
	volatile En_ISC_Error	m_enLastError;

private:
	IClientSocketListener*	m_pListener;

private:
	DWORD m_dwSocketBufferSize;
	DWORD m_dwKeepAliveTimes;
	DWORD m_dwKeepAliveInterval;
};
