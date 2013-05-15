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

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <malloc.h>

#include "../GeneralHelper.h"
#include "../CriticalSection.h"


/*****************************************************************************************************/
/******************************************** 公共类、接口 ********************************************/
/*****************************************************************************************************/


/************************************************************************
名称：Windows Socket 组件初始化类
描述：自动加载和卸载 Windows Socket 组件
************************************************************************/
class CInitSocket
{
public:
	CInitSocket(LPWSADATA lpWSAData = nullptr, BYTE minorVersion = 2, BYTE majorVersion = 2)
	{
		LPWSADATA lpTemp = lpWSAData;
		if(!lpTemp)
		{
			//_alloca在栈(stack)上申请空间,用完马上就释放.
			lpTemp	= (LPWSADATA)_alloca(sizeof(WSADATA));
		}

		m_iResult	= ::WSAStartup(MAKEWORD(minorVersion, majorVersion), lpTemp);
	}

	~CInitSocket()
	{
		if(IsValid())
			::WSACleanup();
	}

	int		GetResult()	{return m_iResult;}
	BOOL	IsValid()	{return m_iResult == 0;}

private:
	int		m_iResult;
};

/************************************************************************
名称：通信组件服务状态
描述：应用程序可以通过通信组件的 GetState() 方法获取组件当前服务状态
************************************************************************/
enum EnServiceState
{
	SS_STARTING	= 0,	// 正在启动
	SS_STARTED	= 1,	// 已经启动
	SS_STOPING	= 2,	// 正在停止
	SS_STOPED	= 3,	// 已经启动
};

/************************************************************************
名称：Socket 操作类型
描述：应用程序的 OnErrror() 事件中通过该参数标识是哪种操作导致的错误
************************************************************************/
enum EnSocketOperation
{
	SO_UNKNOWN	= 0,	// Unknown
	SO_ACCEPT	= 1,	// Acccept
	SO_CONNECT	= 2,	// Connnect
	SO_SEND		= 3,	// Send
	SO_RECEIVE	= 4,	// Receive
};

/************************************************************************
名称：Socket 监听器基接口
描述：定义服务端和客户端 Socket 监听器的公共信息
************************************************************************/
class ISocketListener
{
public:
	/************************************************************************
	名称：事件通知处理结果
	描述：事件通知的返回值，不同的返回值会影响通信组件的后续行为
	************************************************************************/
	enum EnHandleResult
	{
		HR_OK		= 0,	// 成功
		HR_IGNORE	= 1,	// 忽略
		HR_ERROR	= 2,	// 错误
	};

public:

	/*
	* 名称：已发送数据通知
	* 描述：成功发送数据后，Socket 监听器将收到该通知
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			pData		-- 已发送数据缓冲区
	*			iLength		-- 已发送数据长度
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 该通知不允许返回 HR_ERROR（调试模式下引发断言错误）
	*/
	virtual EnHandleResult OnSend(DWORD dwConnID, const BYTE* pData, int iLength)					= 0;

	/*
	* 名称：已接收数据通知
	* 描述：成功接收数据后，Socket 监听器将收到该通知
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			pData		-- 已接收数据缓冲区
	*			iLength		-- 已接收数据长度
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 引发 OnError() 事件时间并关闭连接
	*/
	virtual EnHandleResult OnReceive(DWORD dwConnID, const BYTE* pData, int iLength)				= 0;

	/*
	* 名称：已关闭连接通知
	* 描述：正常关闭连接后，Socket 监听器将收到该通知
	*		
	* 参数：		dwConnID -- 连接 ID
	* 返回值：	忽略返回值
	*/
	virtual EnHandleResult OnClose(DWORD dwConnID)													= 0;

	/*
	* 名称：通信错误通知
	* 描述：通信发生错误后，Socket 监听器将收到该通知，并关闭连接
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			enOperation	-- Socket 操作类型
	*			iErrorCode	-- 错误代码
	* 返回值：	忽略返回值
	*/
	virtual EnHandleResult OnError(DWORD dwConnID, EnSocketOperation enOperation, int iErrorCode)	= 0;

public:
	virtual ~ISocketListener() {}
};

/************************************************************************
名称：服务端 Socket 监听器接口
描述：定义服务端 Socket 监听器的所有事件通知
************************************************************************/
class IServerSocketListener : public ISocketListener
{
public:

	/*
	* 名称：准备监听通知
	* 描述：通信服务端组件启动时，在监听 Socket 创建完成并开始执行监听前，Socket 监听
	*		器将收到该通知，监听器可以在通知处理方法中执行 Socket 选项设置等额外工作
	*		
	* 参数：		soListen	-- 监听 Socket
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 终止启动通信服务组件
	*/
	virtual EnHandleResult OnPrepareListen(SOCKET soListen)				= 0;

	/*
	* 名称：接收连接通知
	* 描述：接收到客户端连接请求时，Socket 监听器将收到该通知，监听器可以在通知处理方
	*		法中执行 Socket 选项设置或拒绝客户端连接等额外工作
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			soClient	-- 客户端 Socket
	* 返回值：	HR_OK / HR_IGNORE	-- 接受连接
	*			HR_ERROR			-- 拒绝连接
	*/
	virtual EnHandleResult OnAccept(DWORD dwConnID, SOCKET soClient)	= 0;

	/*
	* 名称：关闭通信组件通知
	* 描述：通信组件关闭时，Socket 监听器将收到该通知
	*		
	* 参数：	
	* 返回值：忽略返回值
	*/
	virtual EnHandleResult OnServerShutdown()							= 0;
};

/************************************************************************
名称：服务端 Socket 监听器抽象基类
描述：定义某些事件通知的默认处理方法（忽略事件）
************************************************************************/
class CServerSocketListener : public IServerSocketListener
{
public:
	virtual EnHandleResult OnSend(DWORD dwConnID, const BYTE* pData, int iLength)	{return HR_IGNORE;}

	virtual EnHandleResult OnPrepareListen(SOCKET soListen)							{return HR_IGNORE;}
	virtual EnHandleResult OnAccept(DWORD dwConnID, SOCKET soClient)				{return HR_IGNORE;}
	virtual EnHandleResult OnServerShutdown()										{return HR_IGNORE;}
};

/************************************************************************
名称：客户端 Socket 监听器接口
描述：定义客户端 Socket 监听器的所有事件通知
************************************************************************/
class IClientSocketListener : public ISocketListener
{
public:

	/*
	* 名称：准备连接通知
	* 描述：通信客户端组件启动时，在客户端 Socket 创建完成并开始执行连接前，Socket 监听
	*		器将收到该通知，监听器可以在通知处理方法中执行 Socket 选项设置等额外工作
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			socket		-- 客户端 Socket
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 终止启动通信客户端组件
	*/
	virtual EnHandleResult OnPrepareConnect(DWORD dwConnID, SOCKET socket)		= 0;

	/*
	* 名称：连接完成通知
	* 描述：与服务端成功建立连接时，Socket 监听器将收到该通知
	*		
	* 参数：		dwConnID	-- 连接 ID
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 同步连接：终止启动通信客户端组件
	*								   异步连接：关闭连接
	*/
	virtual EnHandleResult OnConnect(DWORD dwConnID)							= 0;
};

/************************************************************************
名称：客户端 Socket 监听器抽象基类
描述：定义某些事件通知的默认处理方法（忽略事件）
************************************************************************/
class CClientSocketListener : public IClientSocketListener
{
public:
	virtual EnHandleResult OnSend(DWORD dwConnID, const BYTE* pData, int iLength)	{return HR_IGNORE;}

	virtual EnHandleResult OnPrepareConnect(DWORD dwConnID, SOCKET socket)			{return HR_IGNORE;}
	virtual EnHandleResult OnConnect(DWORD dwConnID)								{return HR_IGNORE;}
};

/************************************************************************
名称：通信服务端组件接口
描述：定义通信服务端组件的所有操作方法和属性访问方法
************************************************************************/
class ISocketServer
{
public:

	/************************************************************************
	名称：操作结果代码
	描述：Start() / Stop() 方法执行失败时，可通过 GetLastError() 获取错误代码
	************************************************************************/
	enum En_ISS_Error
	{
		ISS_OK						= 0,	// 成功
		ISS_ILLEGAL_STATE			= 1,	// 当前状态不允许操作
		ISS_INVALID_PARAM			= 2,	// 非法参数
		ISS_SOCKET_CREATE			= 3,	// 创建监听 SOCKET 失败
		ISS_SOCKET_BIND				= 4,	// 绑定监听地址失败
		ISS_SOCKET_PREPARE			= 5,	// 设置监听 SOCKET 失败
		ISS_SOCKET_LISTEN			= 6,	// 启动监听失败
		ISS_CP_CREATE				= 7,	// 创建完成端口失败
		ISS_WORKER_THREAD_CREATE	= 8,	// 创建工作线程失败
		ISS_SOCKE_ATTACH_TO_CP		= 9,	// 监听 SOCKET 绑定到完成端口失败
		ISS_ACCEPT_THREAD_CREATE	= 10,	// 创建监听线程失败
	};

public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：启动通信组件
	* 描述：启动服务端通信组件，启动完成后可开始接收客户端连接并收发数据
	*		
	* 参数：		pszBindAddress	-- 监听地址
	*			usPort			-- 监听端口
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Start	(LPCTSTR pszBindAddress, USHORT usPort)								= 0;

	/*
	* 名称：关闭通信组件
	* 描述：关闭服务端通信组件，关闭完成后断开所有客户端连接并释放所有资源
	*		
	* 参数：	
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Stop	()																	= 0;

	/*
	* 名称：发送数据
	* 描述：用户通过该方法向指定客户端发送数据
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			pBuffer		-- 发送数据缓冲区
	*			iLen		-- 发送数据长度
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen)						= 0;

	/*
	* 名称：断开连接
	* 描述：断开与某个客户端的连接
	*		
	* 参数：		dwConnID	-- 连接 ID
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL Disconnect(DWORD dwConnID)													= 0;

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/


	/* 检查通信组件是否已启动 */
	virtual BOOL HasStarted				()													= 0;
	/* 查看通信组件当前状态 */
	virtual EnServiceState GetState		()													= 0;
	/* 获取最近一次失败操作的错误代码 */
	virtual En_ISS_Error GetLastError	()													= 0;
	/* 获取最近一次失败操作的错误描述 */
	virtual LPCTSTR		GetLastErrorDesc()													= 0;
	/* 获取监听 Socket 的地址信息 */
	virtual BOOL GetListenAddress(CString& strAddress, USHORT& usPort)						= 0;
	/* 获取某个客户端连接的地址信息 */
	virtual BOOL GetClientAddress(DWORD dwConnID, CString& strAddress, USHORT& usPort)		= 0;

	/* 设置工作线程数量（通常设置为 2 * CPU + 2） */
	virtual void SetWorkerThreadCount	(DWORD dwWorkerThreadCount)		= 0;
	/* 设置 Accept 预投递 Socket 数量（通常设置为工作线程数的 1 - 2 倍） */
	virtual void SetAcceptSocketCount	(DWORD dwAcceptSocketCount)		= 0;
	/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为 1024 的倍数） */
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)		= 0;
	/* 设置监听 Socket 的等候队列大小（根据并发连接数量调整设置） */
	virtual void SetSocketListenQueue	(DWORD dwSocketListenQueue)		= 0;
	/* 设置 Socket 缓存池大小（通常设置为平均并发连接数量的 1/3 - 1/2） */
	virtual void SetFreeSocketObjPool	(DWORD dwFreeSocketObjPool)		= 0;
	/* 设置内存块缓存池大小（通常设置为 Socket 缓存池大小的 2 - 3 倍） */
	virtual void SetFreeBufferObjPool	(DWORD dwFreeBufferObjPool)		= 0;
	/* 设置 Socket 缓存池回收阀值（通常设置为 Socket 缓存池大小的 3 倍） */
	virtual void SetFreeSocketObjHold	(DWORD dwFreeSocketObjHold)		= 0;
	/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
	virtual void SetFreeBufferObjHold	(DWORD dwFreeBufferObjHold)		= 0;
	/* 设置心跳检查次数（心跳检查失败超过该次数则认为已断开连接，0 则不检查） */
	virtual void SetKeepAliveTimes		(DWORD dwKeepAliveTimes)		= 0;
	/* 设置心跳检查间隔（毫秒，0 则不检查） */
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)		= 0;
	/* 设置关闭服务前等待连接关闭的最长时限（毫秒，0 则不等待） */
	virtual void SetMaxShutdownWaitTime	(DWORD dwMaxShutdownWaitTime)	= 0;

	/* 获取工作线程数量 */
	virtual DWORD GetWorkerThreadCount	()	= 0;
	/* 获取 Accept 预投递 Socket 数量 */
	virtual DWORD GetAcceptSocketCount	()	= 0;
	/* 获取通信数据缓冲区大小 */
	virtual DWORD GetSocketBufferSize	()	= 0;
	/* 获取监听 Socket 的等候队列大小 */
	virtual DWORD GetSocketListenQueue	()	= 0;
	/* 获取 Socket 缓存池大小 */
	virtual DWORD GetFreeSocketObjPool	()	= 0;
	/* 获取内存块缓存池大小 */
	virtual DWORD GetFreeBufferObjPool	()	= 0;
	/* 获取 Socket 缓存池回收阀值 */
	virtual DWORD GetFreeSocketObjHold	()	= 0;
	/* 获取内存块缓存池回收阀值 */
	virtual DWORD GetFreeBufferObjHold	()	= 0;
	/* 获取心跳检查次数 */
	virtual DWORD GetKeepAliveTimes		()	= 0;
	/* 获取心跳检查间隔 */
	virtual DWORD GetKeepAliveInterval	()	= 0;
	/* 获取关闭服务前等待连接关闭的最长时限 */
	virtual DWORD GetMaxShutdownWaitTime()	= 0;

public:
	virtual ~ISocketServer() {}
};

/* 通信服务端组件接口智能指针 */
typedef auto_ptr<ISocketServer>	ISocketServerPtr;

/************************************************************************
名称：通信服务端组件接口
描述：定义通信客户端组件的所有操作方法和属性访问方法
************************************************************************/
class ISocketClient
{
public:
	/************************************************************************
	名称：操作结果代码
	描述：Start() / Stop() 方法执行失败时，可通过 GetLastError() 获取错误代码
	************************************************************************/
	enum En_ISC_Error
	{
		ISC_OK						= 0,	// 成功
		ISC_ILLEGAL_STATE			= 1,	// 当前状态不允许操作
		ISC_INVALID_PARAM			= 2,	// 非法参数
		ISC_SOCKET_CREATE_FAIL		= 3,	// 创建 Client Socket 失败
		ISC_SOCKET_PREPARE_FAIL		= 4,	// 设置 Client Socket 失败
		ISC_CONNECT_SERVER_FAIL		= 5,	// 连接服务器失败
		ISC_WORKER_CREATE_FAIL		= 6,	// 创建工作线程失败
		ISC_NETWORK_ERROR			= 7,	// 网络错误
		ISC_DATA_PROC_ERROR			= 8,	// 数据处理错误
	};

public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：启动通信组件
	* 描述：启动客户端通信组件并连接服务端，启动完成后可开始收发数据
	*		
	* 参数：		pszRemoteAddress	-- 服务端地址
	*			usPort				-- 服务端端口
	*			bAsyncConnect		-- 是否采用异步 Connnect
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Start	(LPCTSTR pszRemoteAddress, USHORT usPort, BOOL bAsyncConnect = FALSE)	= 0;

	/*
	* 名称：关闭通信组件
	* 描述：关闭客户端通信组件，关闭完成后断开与服务端的连接并释放所有资源
	*		
	* 参数：	
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Stop	()																		= 0;

	/*
	* 名称：发送数据
	* 描述：用户通过该方法向服务端发送数据
	*		
	* 参数：		dwConnID	-- 连接 ID（保留参数，目前该参数并未使用）
	*			pBuffer		-- 发送数据缓冲区
	*			iLen		-- 发送数据长度
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen)							= 0;

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/


	/* 检查通信组件是否已启动 */
	virtual BOOL HasStarted					()													= 0;
	/* 查看通信组件当前状态 */
	virtual EnServiceState	GetState		()													= 0;
	/* 获取最近一次失败操作的错误代码 */
	virtual En_ISC_Error	GetLastError	()													= 0;
	/* 获取最近一次失败操作的错误描述 */
	virtual LPCTSTR			GetLastErrorDesc()													= 0;
	/* 获取该组件对象的连接 ID */
	virtual DWORD			GetConnectionID	()													= 0;
	/* 获取 Client Socket 的地址信息 */
	virtual BOOL			GetLocalAddress	(CString& strAddress, USHORT& usPort)				= 0;

	/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为：(N * 1024) - sizeof(TBufferObj)） */
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)	= 0;
	/* 设置心跳检查次数（心跳检查失败超过该次数则认为已断开连接，0 则不检查） */
	virtual void SetKeepAliveTimes		(DWORD dwKeepAliveTimes)	= 0;
	/* 设置心跳检查间隔（毫秒，0 则不检查） */
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)	= 0;

	/* 获取通信数据缓冲区大小 */
	virtual DWORD GetSocketBufferSize	()	= 0;
	/* 获取心跳检查次数 */
	virtual DWORD GetKeepAliveTimes		()	= 0;
	/* 获取心跳检查间隔 */
	virtual DWORD GetKeepAliveInterval	()	= 0;

public:
	virtual ~ISocketClient() {}
};

/* 通信客户端组件接口智能指针 */
typedef auto_ptr<ISocketClient>	ISocketClientPtr;

/* 数据缓冲区基础结构 */
struct TBufferObjBase
{
	OVERLAPPED			ov;
	WSABUF				buff;
	EnSocketOperation	operation;
};

/* 数据缓冲区结构 */
struct TBufferObj : public TBufferObjBase
{
	SOCKET	client;
};

/* 数据缓冲区结构链表 */
typedef list<TBufferObj*>	TBufferObjPtrList;

/* Socket 缓冲区基础结构 */
struct TSocketObjBase
{
	SOCKET	socket;
};

/* 数据缓冲区结构 */
struct TSocketObj : public TSocketObjBase
{
	SOCKADDR_IN		clientAddr;
	DWORD			connID;
	CCriSec2		crisec;
};

/* 数据缓冲区结构链表 */
typedef list<TSocketObj*>					TSocketObjPtrList;
/* 数据缓冲区结构哈希表 */
typedef hash_map<DWORD, TSocketObj*>		TSocketObjPtrMap;
/* 数据缓冲区结构哈希表迭代器 */
typedef TSocketObjPtrMap::iterator			TSocketObjPtrMapI;
/* 数据缓冲区结构哈希表 const 迭代器 */
typedef TSocketObjPtrMap::const_iterator	TSocketObjPtrMapCI;


/*****************************************************************************************************/
/******************************************** 公共帮助方法 ********************************************/
/*****************************************************************************************************/

/* 检查字符串是否符合 IP 地址格式 */
BOOL IsIPAddress(LPCTSTR lpszAddress);
/* 通过主机名获取 IP 地址 */
BOOL GetIPAddress(LPCTSTR lpszHost, CString& strIP);
/* 把 SOCKADDR_IN 结构转换为地址数据 */
BOOL sockaddr_IN_2_A(const SOCKADDR_IN& addr, ADDRESS_FAMILY& usFamily, CString& strAddress, USHORT& usPort);
/* 把地址数据转换为 SOCKADDR_IN 结构 */
BOOL sockaddr_A_2_IN(ADDRESS_FAMILY usFamily, LPCTSTR pszAddress, USHORT usPort, SOCKADDR_IN& addr);
/* 获取 Socket 的本地或远程地址信息 */
BOOL GetSocketAddress(SOCKET socket, CString& strAddress, USHORT& usPort, BOOL bLocal = TRUE);
/* 获取 Socket 的本地地址信息 */
BOOL GetSocketLocalAddress(SOCKET socket, CString& strAddress, USHORT& usPort);
/* 获取 Socket 的远程地址信息 */
BOOL GetSocketRemoteAddress(SOCKET socket, CString& strAddress, USHORT& usPort);

/* 获取 Socket 的某个扩展函数的指针 */
PVOID GetExtensionFuncPtr					(SOCKET sock, GUID guid);
/* 获取 AcceptEx 扩展函数指针 */
LPFN_ACCEPTEX Get_AcceptEx_FuncPtr			(SOCKET sock);
/* 获取 GetAcceptExSockaddrs 扩展函数指针 */
LPFN_GETACCEPTEXSOCKADDRS Get_GetAcceptExSockaddrs_FuncPtr(SOCKET sock);
/* 获取 ConnectEx 扩展函数指针 */
LPFN_CONNECTEX Get_ConnectEx_FuncPtr		(SOCKET sock);
/* 获取 TransmitFile 扩展函数指针 */
LPFN_TRANSMITFILE Get_TransmitFile_FuncPtr	(SOCKET sock);

/************************************************************************
名称：setsockopt() 帮助方法
描述：简化常用的 setsockopt() 调用
************************************************************************/

int SSO_UpdateAcceptContext	(SOCKET soClient, SOCKET soBind);
int SSO_UpdateConnectContext(SOCKET soClient, int iOption);
int SSO_NoDelay				(SOCKET sock, BOOL bNoDelay = TRUE);
int SSO_DontLinger			(SOCKET sock, BOOL bDont = TRUE);
int SSO_Linger				(SOCKET sock, USHORT l_onoff, USHORT l_linger);
int SSO_KeepAlive			(SOCKET sock, BOOL bKeepAlive = TRUE);
int SSO_KeepAliveVals		(SOCKET sock, u_long onoff, u_long time, u_long interval);  //保活机制 值设置
int SSO_RecvBuffSize		(SOCKET sock, int size);
int SSO_SendBuffSize		(SOCKET sock, int size);
int SSO_ReuseAddress		(SOCKET sock, BOOL bReuse = TRUE);

/************************************************************************
名称：Socket 操作方法
描述：Socket 操作包装方法
************************************************************************/

/* 关闭 Socket */
int ManualCloseSocket		(SOCKET sock, int iShutdownFlag = 0xFF, BOOL bGraceful = TRUE, BOOL bReuseAddress = FALSE);
/* 投递 AccceptEx() */
int PostAccept				(LPFN_ACCEPTEX pfnAcceptEx, SOCKET soListen, SOCKET soClient, TBufferObj* pBufferObj);
/* 投递 WSASend() */
int PostSend				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
/* 投递 WSARecv() */
int PostReceive				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);