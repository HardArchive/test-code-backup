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
	static const DWORD DEFAULT_WORKER_THREAD_COUNT;                        //工作线程数量::GetCpuCount() * 2 + 2;
	static const DWORD DEFAULT_ACCEPT_SOCKET_COUNT;                        // 2 * DEFAULT_WORKER_THREAD_COUNT
	static const DWORD DEFAULT_SOCKET_BUFFER_SIZE;                         // 4 * 1024 - sizeof(TBufferObj)
	static const DWORD DEFAULT_SOCKET_LISTEN_QUEUE;                        // 30
	static const DWORD DEFAULT_FREE_SOCKETOBJ_POOL;                        // 100
	static const DWORD DEFAULT_FREE_BUFFEROBJ_POOL;                        // 200
	static const DWORD DEFAULT_FREE_SOCKETOBJ_HOLD;                        // 300
	static const DWORD DEFAULT_FREE_BUFFEROBJ_HOLD;                        // 600
	static const DWORD DEFALUT_KEEPALIVE_TIMES;                            // 3
	static const DWORD DEFALUT_KEEPALIVE_INTERVAL;                         // 10 * 1000
	static const DWORD DEFAULT_MAX_SHUTDOWN_WAIT_TIME;                     // 5 * 1000

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
	BOOL CheckParams();                                                    //参数检查
	BOOL CheckStarting();
	BOOL CheckStoping();
	BOOL CreateListenSocket(LPCTSTR pszBindAddress, USHORT usPort);        //创建监听套接字开始监听端口
	BOOL CreateCompletePort();                                             //创建完成端口
	BOOL CreateWorkerThreads();                                            //创建工作线程
	BOOL StartAcceptThread();                                              //创建接收连接线程

	void CloseListenSocket();
	void WaitForAcceptThreadEnd();                                         //设置m_evtAccept事件 ，退出接收线程
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

	/* TBufferObj 和 TSocketObj 缓冲池系列方法 */
	TBufferObj*	GetFreeBufferObj(int iLen = 0);                             //取到空闲buffer 用于存储收发数据TBufferObj
	TSocketObj*	GetFreeSocketObj();                                         //取到空闲buffer 用于存储连接套接字TSocketObj
	void		AddFreeBufferObj(TBufferObj* pBufferObj);
	void		AddFreeSocketObj(DWORD dwConnID);
	TBufferObj*	CreateBufferObj();                                          
	TSocketObj*	CreateSocketObj();                                          
	void		DeleteBufferObj(TBufferObj* pBufferObj);
	void		DeleteSocketObj(TSocketObj* pSocketObj);

	void		AddClientSocketObj(DWORD dwConnID, TSocketObj* pSocketObj);//将Connection ID 和TSocketObj添加到m_mpClientSocket表中
	TSocketObj* FindSocketObj(DWORD dwConnID);
	void		CloseSocketObj(TSocketObj* pSocketObj, int iShutdownFlag = SD_SEND);

private:
	//使用 AcceptEx() 接收客户端连接请求并创建 Client Socket 的线程，
	//将其独立出来，实现为单独的线程将使组件的模块划分更清晰，更重要的是避免与业务逻辑和通信处理的相互影响
	static UINT WINAPI AcceptThreadProc(LPVOID pv);                        //接收连接线程
	//使用 GetQueuedCompletionStatus() 监听网络事件并处理网络交互的多个线程，
	//工作线程处理完网络事件后会向上层应用发送 OnAccept/OnSend/OnReceive 等组件通知。
	//工作线程的数量可以根据实际情况之行设置（通常建议为：CPU Core Number * 2 + 2）
	static UINT WINAPI WorkerThreadProc(LPVOID pv);                        //IOCP工作线程

	void HandleIo		(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwBytes, DWORD dwErrorCode); //IO处理
	void HandleError	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwErrorCode);                //错误连接处理
	void HandleAccept	(SOCKET soListen, TBufferObj* pBufferObj);                                                          //接受连接处理
	void HandleSend		(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);                                   //发送数据处理
	void HandleReceive	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);                                   //接收数据处理
 
	int DoSend		(DWORD dwConnID, TSocketObj* pSocketObj, const BYTE* pBuffer, int iLen);                                //投递一个发送
	int DoReceive	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);                                       //投递一个接收

private:
	SOCKET	GetAcceptSocket();                                             //创建并取到客户端接受连接套接字
	BOOL	DeleteAcceptSocket(SOCKET socket, BOOL bCloseSocket = FALSE);  //将接受连接的socket从等待接受连接的socket集合m_setAccept中删掉
	void	ReleaseAcceptSockets();

private:
	CInitSocket					m_wsSocket;
	//接受连接函数指针
	LPFN_ACCEPTEX				m_pfnAcceptEx;                             //AcceptEx函数指针  
	//GetAcceptExSockaddrs是专门为AcceptEx函数准备的，它将AcceptEx接受的第一块数据中的本地和远程机器的地址返回给用户
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;                 //GetAcceptExSockaddrs函数指针用来取得接收socket的IP端口

private:
	IServerSocketListener*	m_psoListener;                                 //监听器指针

	volatile EnServiceState	m_enState;                                     //通信组件状态标识
	volatile DWORD			m_dwConnID;                                    //Connection ID 当前值

	En_ISS_Error	m_enLastError;

	SOCKET			m_soListen;                                            //监听套接字
	HANDLE			m_hCompletePort;                                       //完成端口句柄
	HANDLE			m_hAcceptThread;                                       //Accept 线程句柄
	vector<HANDLE>	m_vtWorkerThreads;                                     //工作线程句柄集合

	TBufferObjPtrList	m_lsFreeBuffer;                                    //TBufferObj 缓冲池空闲队列
	TSocketObjPtrList	m_lsFreeSocket;                                    //TSocketObj 缓冲池空闲队列
	TSocketObjPtrMap	m_mpClientSocket;                                  //Connection ID 到TSocketObj的 映射

	//临界区处理类
	CCriSec				m_csFreeBuffer;
	CCriSec				m_csFreeSocket;
	CCriSec				m_csClientSocket;

	CEvt				m_evtAccept;                                       //事件内核对象昝类-接收连接事件

	smart_simple_ptr<CSEM>	m_psemAccept;                                  //单实体智能指针-接收信号量
	CCriSec					m_csAccept;
	ulong_ptr_set			m_setAccept;                                   //客户端套接字集合 等待连接套接字集合

	CPrivateHeap			m_phSocket;                                    //
	CPrivateHeap			m_phBuffer;                                    //缓冲池私有堆

private:
	DWORD m_dwWorkerThreadCount;    //工作线程数量  CPU Core Number * 2 + 2
	DWORD m_dwAcceptSocketCount;    //默认并发 AcceptEx 调用次数
	DWORD m_dwSocketBufferSize;     //TBufferObj 数据缓冲区大小
	DWORD m_dwSocketListenQueue;    //指定内核为此套接口排队的最大连接个数 此处为30
	DWORD m_dwFreeSocketObjPool;    //空闲TSocketObj 对象池 的对象数量？？
	DWORD m_dwFreeBufferObjPool;    //空闲TBufferObj 缓冲池数量？？
	DWORD m_dwFreeSocketObjHold;    //
	DWORD m_dwFreeBufferObjHold;    //
	DWORD m_dwKeepAliveTimes;       //第一次开始发送的时间（单位毫秒）此值为3
	DWORD m_dwKeepAliveInterval;    //TCP连接处于畅通时候的探测频率  每次检测的间隔 （单位毫秒） 值 10 * 1000
	DWORD m_dwMaxShutdownWaitTime;  //最大停止等待时间 5 * 1000
};