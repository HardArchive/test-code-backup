#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <malloc.h>

#include "../GeneralHelper.h"
#include "../CriticalSection.h"

class CInitSocket
{
public:
	CInitSocket(LPWSADATA lpWSAData = NULL, BYTE minorVersion = 2, BYTE majorVersion = 2)
	{
		LPWSADATA lpTemp = lpWSAData;
		if(!lpTemp)
			lpTemp	= (LPWSADATA)_alloca(sizeof(WSADATA));

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


enum EnSocketState
{
	SS_STARTING	= 0,  //即将开始
	SS_STARTED	= 1,  //已经开始
	SS_STOPING	= 2,  //即将停止
	SS_STOPED	= 3,  //已经停止
};
//操作类型
enum EnSocketOperation
{
	SO_UNKNOWN	= 0,   //未知
	SO_ACCEPT	= 1,   //接收到连接请求
	SO_CONNECT	= 2,   //连接
	SO_SEND		= 3,   //发送数据
	SO_RECEIVE	= 4,   //接收到数据
};

//监听器基接口
class ISocketListener
{
public:
	//返回值类型
	enum EnHandleResult
	{
		HR_OK		= 0,
		HR_IGNORE	= 1,
		HR_ERROR	= 2,
	};

public:
	//已发出数据通知
	virtual EnHandleResult OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength)					= 0;
	//已接收数据通知
	virtual EnHandleResult OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength)				= 0;
	//关闭连接通知
	virtual EnHandleResult OnClose(DWORD dwConnectionID)												= 0;
	//通信错误通知
	virtual EnHandleResult OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode)	= 0;

public:
	virtual ~ISocketListener() {}
};

//监听器接口
//IServerSocketListener 接口提供组件通知方法，由上层应用实现
class IServerSocketListener : public ISocketListener
{
public:
	//接收连接通知
	virtual EnHandleResult OnAccept(DWORD dwConnectionID)	= 0;
	//服务关闭通知
	virtual EnHandleResult OnServerShutdown()				= 0;
};

class IClientSocketListener : public ISocketListener
{
public:
	virtual EnHandleResult OnConnect(DWORD dwConnectionID)	= 0;
};

//操作接口
//ISocketServer 接口提供组件操作方法，由上层应用直接调用
class ISocketServer
{
public:
	//错误码
	enum En_ISS_Error
	{
		ISS_OK						= 0,
		ISS_ILLEGAL_STATE			= 1,
		ISS_INVALID_PARAM			= 2,
		ISS_SOCKET_CREATE			= 3,
		ISS_SOCKET_BIND				= 4,
		ISS_SOCKET_LISTEN			= 5,
		ISS_CP_CREATE				= 6,
		ISS_WORKER_THREAD_CREATE	= 7,
		ISS_SOCKE_ATTACH_TO_CP		= 8,
		ISS_ACCEPT_THREAD_CREATE	= 9,
	};

public:
	//启动通信
	virtual BOOL Start	(LPCTSTR pszBindAddress, USHORT usPort)								= 0;
	//关闭通信
	virtual BOOL Stop	()																	= 0;
	//发送数据
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen)						= 0;
	//是否已启动
	virtual BOOL HasStarted				()													= 0;
	
	virtual EnSocketState GetSocketState()													= 0;
	//获取错误码
	virtual En_ISS_Error GetLastError	()													= 0;
	//获取错误描述
	virtual LPCTSTR		GetLastErrorDesc()													= 0;
	//获取客户端的地址信息
	virtual BOOL GetConnectionAddress(DWORD dwConnID, CString& strAddress, USHORT& usPort)	= 0;

public:
	virtual ~ISocketServer() {}
};
//ISocketServer 接口智能指针
typedef auto_ptr<ISocketServer>	ISocketServerPtr;

class ISocketClient
{
public:
	enum En_ISC_Error
	{
		ISC_OK						= 0,
		ISC_ILLEGAL_STATE			= 1,
		ISC_INVALID_PARAM			= 2,
		ISC_SOCKET_CREATE_FAIL		= 3,
		ISC_CONNECT_SERVER_FAIL		= 4,
		ISC_WORKER_CREATE_FAIL		= 5,
		ISC_NETWORK_ERROR			= 6,
		ISC_PROTOCOL_ERROR			= 7,
	};

public:
	virtual BOOL Start	(LPCTSTR pszRemoteAddress, USHORT usPort)		= 0;
	virtual BOOL Stop	()												= 0;
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen)	= 0;
	virtual BOOL HasStarted				()								= 0;
	virtual EnSocketState GetSocketState()								= 0;
	virtual En_ISC_Error GetLastError	()								= 0;
	virtual LPCTSTR		GetLastErrorDesc()								= 0;
	virtual DWORD		GetConnectionID	()								= 0;

public:
	virtual ~ISocketClient() {}
};

typedef auto_ptr<ISocketClient>	ISocketClientPtr;

struct TBufferObjBase
{
	OVERLAPPED			ov;           //WSAOVERLAPPED 异步 Overlapped
	WSABUF				buff;         //数据缓冲 buffer
	EnSocketOperation	operation;    //操作类型
};

//内存缓冲区
struct TBufferObj : public TBufferObjBase
{
	SOCKET	client;    //Client Socket
};
//内存缓冲区表
typedef list<TBufferObj*>	TBufferObjPtrList;   

struct TSocketObjBase
{
	SOCKET	socket;     //Client Socket
};
//Socket 相关的结构体
struct TSocketObj : public TSocketObjBase
{
	SOCKADDR_IN		clientAddr;   //Socket 地址
	DWORD			connID;       //Connection ID
	CCriSec2		crisec;       //Critical Session
};
//套接字相关结构表
typedef list<TSocketObj*>					TSocketObjPtrList;
typedef hash_map<DWORD, TSocketObj*>		TSocketObjPtrMap;   //Connection ID 到TSocketObj的
typedef TSocketObjPtrMap::iterator			TSocketObjPtrMapI;
typedef TSocketObjPtrMap::const_iterator	TSocketObjPtrMapCI;

BOOL IsIPAddress(LPCTSTR lpszAddress);
BOOL GetIPAddress(LPCTSTR lpszHost, CStringA& strIP);

PVOID GetExtensionFuncPtr					(SOCKET sock, GUID guid); //获取扩展函数指针
LPFN_ACCEPTEX Get_AcceptEx_FuncPtr			(SOCKET sock);  
LPFN_GETACCEPTEXSOCKADDRS Get_GetAcceptExSockaddrs_FuncPtr(SOCKET sock);
LPFN_CONNECTEX Get_ConnectEx_FuncPtr		(SOCKET sock);
LPFN_TRANSMITFILE Get_TransmitFile_FuncPtr	(SOCKET sock);

int SSO_UpdateAcceptContext	(SOCKET soClient, SOCKET soBind);
int SSO_UpdateConnectContext(SOCKET soClient, int iOption);
int SSO_NoDelay				(SOCKET sock, BOOL bNoDelay = TRUE);
int SSO_DontLinger			(SOCKET sock, BOOL bDont = TRUE);
int SSO_Linger				(SOCKET sock, USHORT l_onoff, USHORT l_linger);
int SSO_KeepAlive			(SOCKET sock, BOOL bKeepAlive = TRUE);
int SSO_KeepAliveVals		(SOCKET sock, u_long onoff, u_long time, u_long interval);
int SSO_RecvBuffSize		(SOCKET sock, int size);
int SSO_SendBuffSize		(SOCKET sock, int size);
int SSO_ReuseAddress		(SOCKET sock, BOOL bReuse = TRUE);

int ManualCloseSocket		(SOCKET sock, BOOL bGraceful = TRUE, BOOL bReuseAddress = FALSE);
int PostAccept				(LPFN_ACCEPTEX pfnAcceptEx, SOCKET soListen, SOCKET soClient, TBufferObj* pBufferObj); //接受连接
int PostSend				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);   //投递 发送数据
int PostReceive				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);   //接收数据