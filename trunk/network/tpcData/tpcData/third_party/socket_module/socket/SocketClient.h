#pragma once

#include "SocketHelper.h"
#include "../Event.h"
#include "../bufferptr.h"
#include "../CriticalSection.h"

class CSocketClient : public ISocketClient
{
public:
	virtual BOOL Start	(LPCTSTR pszRemoteAddress, USHORT usPortt);
	virtual BOOL Stop	();
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen);
	virtual BOOL			HasStarted		()	{return m_enState == SS_STARTED;}
	virtual EnSocketState	GetSocketState	()	{return m_enState;}
	virtual DWORD		GetConnectionID		()	{return m_dwConnID;};
	virtual En_ISC_Error GetLastError		()	{return m_enLastError;}
	virtual LPCTSTR		GetLastErrorDesc	();


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

	void SetLastError(En_ISC_Error code, LPCTSTR func, int ec);

static 
#ifndef _WIN32_WCE
	UINT
#else
	DWORD
#endif
	 WINAPI WorkerThreadProc(LPVOID pv);

private:
	static const int	DEFAULT_SOCKET_BUFFER_SIZE	= 4 * 1024;
	//static const int	WORKER_THREAD_END_TIME		= 5 * 1000;

	static const long	DEFALUT_KEEPALIVE_TIMES		= 3;
	static const long	DEFALUT_KEEPALIVE_INTERVAL	= 10 * 1000;


public:
	CSocketClient(IClientSocketListener* pListener)
	: m_pListener(pListener)
	, m_soClient			(INVALID_SOCKET)
	, m_evSocket			(NULL)
	, m_dwConnID			(0)
	, m_hWorker				(NULL)
	, m_dwWorkerID			(0)
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

	CBufferPtr		m_sndData;
	CBufferPtr		m_sndBuffer;
	CBufferPtr		m_rcBuffer;
	CCriSec			m_scBuffer;
	CEvt			m_evBuffer;

	volatile EnSocketState	m_enState;
	volatile En_ISC_Error	m_enLastError;

private:
	IClientSocketListener*	m_pListener;

public:
	void SetSocketBufferSize	(DWORD dwSocketBufferSize)	{m_dwSocketBufferSize	= dwSocketBufferSize;}
	void SetKeepAliveTimes		(DWORD dwKeepAliveTimes)	{m_dwKeepAliveTimes		= dwKeepAliveTimes;}
	void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)	{m_dwKeepAliveInterval	= dwKeepAliveInterval;}

	DWORD GetSocketBufferSize	()	{return m_dwSocketBufferSize;}
	DWORD GetKeepAliveTimes		()	{return m_dwKeepAliveTimes;}
	DWORD GetKeepAliveInterval	()	{return m_dwKeepAliveInterval;}

private:
	DWORD m_dwSocketBufferSize;
	DWORD m_dwKeepAliveTimes;
	DWORD m_dwKeepAliveInterval;
};
