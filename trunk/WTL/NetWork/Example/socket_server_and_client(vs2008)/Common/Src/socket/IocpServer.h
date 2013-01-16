
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
	static const DWORD	DEFAULT_IOCP_THREAD_COUNT;
	static const DWORD	DEFAULT_ACCEPT_SOCKET_COUNT;
	static const DWORD	DEFAULT_IOCP_BUFFER_SIZE;
	static const DWORD	DEFAULT_SOCKET_LISTEN_QUEUE;
	static const DWORD	DEFAULT_FREE_SOCKETOBJ_POOL;
	static const DWORD	DEFAULT_FREE_BUFFEROBJ_POOL;
	static const DWORD	DEFAULT_FREE_SOCKETOBJ_HOLD;
	static const DWORD	DEFAULT_FREE_BUFFEROBJ_HOLD;
	static const DWORD	DEFALUT_KEEPALIVE_TIMES;
	static const DWORD	DEFALUT_KEEPALIVE_INTERVAL;

public:
	CIocpServer(IServerSocketListener* pListener)
		: m_psoListener				(pListener)
		, m_hAcceptThread			(NULL)
		, m_hCompletePort			(NULL)
		, m_soListen				(INVALID_SOCKET)
		, m_pfnAcceptEx				(NULL)
		, m_pfnGetAcceptExSockaddrs	(NULL)
		, m_enLastError				(ISS_OK)
		, m_enState					(SS_STOPED)
		, m_dwIocpThreadCount		(DEFAULT_IOCP_THREAD_COUNT)
		, m_dwAcceptSocketCount		(DEFAULT_ACCEPT_SOCKET_COUNT)
		, m_dwIocpBufferSize		(DEFAULT_IOCP_BUFFER_SIZE)
		, m_dwSocketListenQueue		(DEFAULT_SOCKET_LISTEN_QUEUE)
		, m_dwFreeSocketObjPool		(DEFAULT_FREE_SOCKETOBJ_POOL)
		, m_dwFreeBufferObjPool		(DEFAULT_FREE_BUFFEROBJ_POOL)
		, m_dwFreeSocketObjHold		(DEFAULT_FREE_SOCKETOBJ_HOLD)
		, m_dwFreeBufferObjHold		(DEFAULT_FREE_BUFFEROBJ_HOLD)
		, m_dwKeepAliveTimes		(DEFALUT_KEEPALIVE_TIMES)
		, m_dwKeepAliveInterval		(DEFALUT_KEEPALIVE_INTERVAL)
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
	virtual BOOL			HasStarted		()	{return m_enState == SS_STARTED;}
	virtual EnSocketState	GetSocketState	()	{return m_enState;}
	virtual En_ISS_Error	GetLastError	()	{return m_enLastError;}
	virtual LPCTSTR			GetLastErrorDesc();
	virtual BOOL			GetConnectionAddress(DWORD dwConnID, CString& strAddress, USHORT& usPort);

private:
	void SetLastError(En_ISS_Error code, LPCTSTR func, int ec);

private:
	BOOL CheckParams();
	BOOL CheckStarting();
	BOOL CheckStoping();
	BOOL CreateListenSocket(LPCTSTR pszBindAddress, USHORT usPort);  //创建监听套接字开始监听端口
	BOOL CreateCompletePort();                                       //创建完成端口
	BOOL CreateWorkerThreads();          //创建工作线程
	BOOL StartAcceptThread();            //创建接收连接线程

	void CloseListenSocket();
	void WaitForAcceptThreadEnd();      //设置m_evtAccept事件 ，退出接收线程
	void CloseClientSocket();
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
	void		AddFreeSocketObj(DWORD dwConnID, BOOL bClose = TRUE, BOOL bGraceful = TRUE, BOOL bReuseAddress = FALSE);
	TBufferObj*	CreateBufferObj();
	TSocketObj*	CreateSocketObj();
	void		DeleteBufferObj(TBufferObj* pBufferObj);
	void		DeleteSocketObj(TSocketObj* pSocketObj);

	void		AddClientSocketObj(DWORD dwConnID, TSocketObj* pSocketObj);
	TSocketObj* FindSocketObj(DWORD dwConnID);

private:
	//使用 AcceptEx() 接收客户端连接请求并创建 Client Socket 的线程，
	//将其独立出来，实现为单独的线程将使组件的模块划分更清晰，更重要的是避免与业务逻辑和通信处理的相互影响
	static UINT WINAPI AcceptThreadProc(LPVOID pv);   //接收连接线程
	//使用 GetQueuedCompletionStatus() 监听网络事件并处理网络交互的多个线程，
	//工作线程处理完网络事件后会向上层应用发送 OnAccept/OnSend/OnReceive 等组件通知。
	//工作线程的数量可以根据实际情况之行设置（通常建议为：CPU Core Number * 2 + 2）
	static UINT WINAPI WorkerThreadProc(LPVOID pv);   //IOCP工作线程

	void HandleIo		(TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwBytes, DWORD dwErrorCode);
	void HandleAccept	(SOCKET soListen, TBufferObj* pBufferObj);
	void HandleSend		(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
	void HandleReceive	(TSocketObj* pSocketObj, TBufferObj* pBufferObj);

	int DoSend		(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
	int DoReceive	(TSocketObj* pSocketObj, TBufferObj* pBufferObj);

private:
	SOCKET	GetAcceptSocket();
	BOOL	DeleteAcceptSocket(SOCKET socket, BOOL bCloseSocket = FALSE);
	void	ReleaseAcceptSockets();

private:
	CInitSocket					m_wsSocket;
	//接受连接函数指针
	LPFN_ACCEPTEX				m_pfnAcceptEx;          //AcceptEx函数指针       
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;    //函数指针用来取得接收socket的IP端口
	//GetAcceptExSockaddrs是专门为AcceptEx函数准备的，它将AcceptEx接受的第一块数据中的本地和远程机器的地址返回给用户。
private:
	IServerSocketListener*	m_psoListener;

	volatile EnSocketState	m_enState;
	volatile DWORD			m_dwConnID;

	En_ISS_Error	m_enLastError;

	SOCKET			m_soListen;       //监听套接字
	HANDLE			m_hCompletePort;
	HANDLE			m_hAcceptThread;
	vector<HANDLE>	m_vtWorkerThreads;

	TBufferObjPtrList	m_lsFreeBuffer;
	TSocketObjPtrList	m_lsFreeSocket;
	TSocketObjPtrMap	m_mpClientSocket;

	CCriSec				m_csFreeBuffer;
	CCriSec				m_csFreeSocket;
	CCriSec				m_csClientSocket;  //临界区处理类

	CEvt				m_evtAccept;       //事件内核对象昝类-接收事件

	smart_simple_ptr<CSEM>	m_psemAccept;  //单实体智能指针-接收信号量
	CCriSec					m_csAccept;
	ulong_set				m_setAccept;

	CPrivateHeap			m_hpPrivate;

public:
	void SetIocpThreadCount		(DWORD dwIocpThreadCount)	{m_dwIocpThreadCount	= dwIocpThreadCount;}
	void SetAcceptSocketCount	(DWORD dwAcceptSocketCount)	{m_dwAcceptSocketCount	= dwAcceptSocketCount;}
	void SetIocpBufferSize		(DWORD dwIocpBufferSize)	{m_dwIocpBufferSize		= dwIocpBufferSize;}
	void SetSocketListenQueue	(DWORD dwSocketListenQueue)	{m_dwSocketListenQueue	= dwSocketListenQueue;}
	void SetFreeSocketObjPool	(DWORD dwFreeSocketObjPool)	{m_dwFreeSocketObjPool	= dwFreeSocketObjPool;}
	void SetFreeBufferObjPool	(DWORD dwFreeBufferObjPool)	{m_dwFreeBufferObjPool	= dwFreeBufferObjPool;}
	void SetFreeSocketObjHold	(DWORD dwFreeSocketObjHold)	{m_dwFreeSocketObjHold	= dwFreeSocketObjHold;}
	void SetFreeBufferObjHold	(DWORD dwFreeBufferObjHold)	{m_dwFreeBufferObjHold	= dwFreeBufferObjHold;}
	void SetKeepAliveTimes		(DWORD dwKeepAliveTimes)	{m_dwKeepAliveTimes		= dwKeepAliveTimes;}
	void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)	{m_dwKeepAliveInterval	= dwKeepAliveInterval;}

	DWORD GetIocpThreadCount	()	{return m_dwIocpThreadCount;}
	DWORD GetAcceptSocketCount	()	{return m_dwAcceptSocketCount;}
	DWORD GetIocpBufferSize		()	{return m_dwIocpBufferSize;}
	DWORD GetSocketListenQueue	()	{return m_dwSocketListenQueue;}
	DWORD GetFreeSocketObjPool	()	{return m_dwFreeSocketObjPool;}
	DWORD GetFreeBufferObjPool	()	{return m_dwFreeBufferObjPool;}
	DWORD GetFreeSocketObjHold	()	{return m_dwFreeSocketObjHold;}
	DWORD GetFreeBufferObjHold	()	{return m_dwFreeBufferObjHold;}
	DWORD GetKeepAliveTimes		()	{return m_dwKeepAliveTimes;}
	DWORD GetKeepAliveInterval	()	{return m_dwKeepAliveInterval;}

private:
	DWORD m_dwIocpThreadCount;     //工作线程数量  CPU Core Number * 2 + 2
	DWORD m_dwAcceptSocketCount;   //接受连接数量
	DWORD m_dwIocpBufferSize;
	DWORD m_dwSocketListenQueue;  //指定内核为此套接口排队的最大连接个数 此处为30
	DWORD m_dwFreeSocketObjPool;
	DWORD m_dwFreeBufferObjPool;
	DWORD m_dwFreeSocketObjHold;
	DWORD m_dwFreeBufferObjHold;
	DWORD m_dwKeepAliveTimes;
	DWORD m_dwKeepAliveInterval;
};