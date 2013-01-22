
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
	/* 如果需要，可以提供 getter & setter 方法设置下列工作参数 */
	static const DWORD DEFAULT_IOCP_THREAD_COUNT;      //默认工作线程数
	static const DWORD DEFAULT_ACCEPT_SOCKET_COUNT;    //默认并发 AcceptEx 调用次数
	static const DWORD DEFAULT_IOCP_BUFFER_SIZE;       //默认 TBufferObj 数据缓冲区大小
	static const DWORD DEFAULT_SOCKET_LISTEN_QUEUE;    //默认 Socket 等候队列数目
	static const DWORD DEFAULT_FREE_SOCKETOBJ_POOL;    //TSocketObj 缓冲池大小
	static const DWORD DEFAULT_FREE_BUFFEROBJ_POOL;    //TBufferObj 缓冲池大小
	static const DWORD DEFAULT_FREE_SOCKETOBJ_HOLD;    //TSocketObj 缓冲池压缩阀值
	static const DWORD DEFAULT_FREE_BUFFEROBJ_HOLD;    //TBufferObj 缓冲池压缩阀值
	static const DWORD DEFALUT_KEEPALIVE_TIMES;        //心跳检测次数
	static const DWORD DEFALUT_KEEPALIVE_INTERVAL;     //心跳检测间隔

public:
	//IServerSocketListener 监听器
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

public:/* ISocketServer 接口方法实现 */
	virtual BOOL Start	(LPCTSTR pszBindAddress, USHORT usPort);
	virtual BOOL Stop	();
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen);  //发送数据 连接ID 内容 长度
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

	 /* TBufferObj 和 TSocketObj 缓冲池系列方法 */
	TBufferObj*	GetFreeBufferObj(int iLen = 0);  //取到空闲buffer
	TSocketObj*	GetFreeSocketObj();
	void		AddFreeBufferObj(TBufferObj* pBufferObj);
	//将dwConnID对应的套接字加入空闲缓冲池队列
	void		AddFreeSocketObj(DWORD dwConnID, BOOL bClose = TRUE, BOOL bGraceful = TRUE, BOOL bReuseAddress = FALSE);
	TBufferObj*	CreateBufferObj();              //创建空闲buffer
	TSocketObj*	CreateSocketObj();
	void		DeleteBufferObj(TBufferObj* pBufferObj);
	void		DeleteSocketObj(TSocketObj* pSocketObj);

	void		AddClientSocketObj(DWORD dwConnID, TSocketObj* pSocketObj); //将Connection ID 和TSocketObj添加到m_mpClientSocket表中
	TSocketObj* FindSocketObj(DWORD dwConnID);

private:
	//使用 AcceptEx() 接收客户端连接请求并创建 Client Socket 的线程，
	//将其独立出来，实现为单独的线程将使组件的模块划分更清晰，更重要的是避免与业务逻辑和通信处理的相互影响
	static UINT WINAPI AcceptThreadProc(LPVOID pv);   //接收连接线程
	//使用 GetQueuedCompletionStatus() 监听网络事件并处理网络交互的多个线程，
	//工作线程处理完网络事件后会向上层应用发送 OnAccept/OnSend/OnReceive 等组件通知。
	//工作线程的数量可以根据实际情况之行设置（通常建议为：CPU Core Number * 2 + 2）
	static UINT WINAPI WorkerThreadProc(LPVOID pv);   //IOCP工作线程

	void HandleIo		(TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwBytes, DWORD dwErrorCode); //IO处理
	void HandleAccept	(SOCKET soListen, TBufferObj* pBufferObj);        //接受连接处理
	void HandleSend		(TSocketObj* pSocketObj, TBufferObj* pBufferObj); //发送数据处理
	void HandleReceive	(TSocketObj* pSocketObj, TBufferObj* pBufferObj); //接收数据处理

	int DoSend		(TSocketObj* pSocketObj, TBufferObj* pBufferObj);     //投递一个发送
	int DoReceive	(TSocketObj* pSocketObj, TBufferObj* pBufferObj);     //投递一个接收

private:
	SOCKET	GetAcceptSocket(); //创建并取到客户端接受连接套接字
	 //将接受连接的socket从等待接受连接的socket集合m_setAccept中删掉
	BOOL	DeleteAcceptSocket(SOCKET socket, BOOL bCloseSocket = FALSE); 
	void	ReleaseAcceptSockets();

private:
	CInitSocket					m_wsSocket;
	//接受连接函数指针
	LPFN_ACCEPTEX				m_pfnAcceptEx;          //AcceptEx函数指针  
	//GetAcceptExSockaddrs是专门为AcceptEx函数准备的，它将AcceptEx接受的第一块数据中的本地和远程机器的地址返回给用户。
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;    //GetAcceptExSockaddrs函数指针用来取得接收socket的IP端口
	
private:
	IServerSocketListener*	m_psoListener;  //监听器指针

	volatile EnSocketState	m_enState;      //启动标识
	volatile DWORD			m_dwConnID;     //Connection ID 当前值

	En_ISS_Error	m_enLastError;

	SOCKET			m_soListen;         //监听套接字
	HANDLE			m_hCompletePort;    //完成端口
	HANDLE			m_hAcceptThread;    //Accept 线程句柄
	vector<HANDLE>	m_vtWorkerThreads;  //工作线程句柄集合

	TBufferObjPtrList	m_lsFreeBuffer;    //TBufferObj 缓冲池空闲队列
	TSocketObjPtrList	m_lsFreeSocket;    //TSocketObj 缓冲池空闲队列
	TSocketObjPtrMap	m_mpClientSocket;  //Connection ID 到TSocketObj的 映射

	CCriSec				m_csFreeBuffer;
	CCriSec				m_csFreeSocket;
	CCriSec				m_csClientSocket;  //临界区处理类

	CEvt				m_evtAccept;       //事件内核对象昝类-接收连接事件

	smart_simple_ptr<CSEM>	m_psemAccept;  //单实体智能指针-接收信号量
	CCriSec					m_csAccept;
	ulong_set				m_setAccept;   //客户端套接字集合 等待连接套接字集合

	CPrivateHeap			m_hpPrivate;   // 缓冲池私有堆

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
	DWORD m_dwAcceptSocketCount;   //默认并发 AcceptEx 调用次数
	DWORD m_dwIocpBufferSize;      //TBufferObj 数据缓冲区大小
	DWORD m_dwSocketListenQueue;  //指定内核为此套接口排队的最大连接个数 此处为30
	DWORD m_dwFreeSocketObjPool;
	DWORD m_dwFreeBufferObjPool;
	DWORD m_dwFreeSocketObjHold;
	DWORD m_dwFreeBufferObjHold;
	DWORD m_dwKeepAliveTimes;
	DWORD m_dwKeepAliveInterval;
};