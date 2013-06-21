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
		{
			//_alloca��ջ(stack)������ռ�,�������Ͼ��ͷ�.
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


enum EnSocketState
{
	SS_STARTING	= 0,  //������ʼ
	SS_STARTED	= 1,  //�Ѿ���ʼ
	SS_STOPING	= 2,  //����ֹͣ
	SS_STOPED	= 3,  //�Ѿ�ֹͣ
};
//��������
enum EnSocketOperation
{
	SO_UNKNOWN	= 0,   //δ֪
	SO_ACCEPT	= 1,   //���յ���������
	SO_CONNECT	= 2,   //����
	SO_SEND		= 3,   //��������
	SO_RECEIVE	= 4,   //���յ�����
};

//���������ӿ�
class ISocketListener
{
public:
	//����ֵ����
	enum EnHandleResult
	{
		HR_OK		= 0,
		HR_IGNORE	= 1,
		HR_ERROR	= 2,
	};

public:
	//�ѷ�������֪ͨ
	virtual EnHandleResult OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength)					= 0;
	//�ѽ�������֪ͨ
	virtual EnHandleResult OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength)				= 0;
	//�ر�����֪ͨ
	virtual EnHandleResult OnClose(DWORD dwConnectionID)												= 0;
	//ͨ�Ŵ���֪ͨ
	virtual EnHandleResult OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode)	= 0;

public:
	virtual ~ISocketListener() {}
};

//�������ӿ�
//IServerSocketListener �ӿ��ṩ���֪ͨ���������ϲ�Ӧ��ʵ��
class IServerSocketListener : public ISocketListener
{
public:
	//��������֪ͨ
	virtual EnHandleResult OnAccept(DWORD dwConnectionID)	= 0;
	//����ر�֪ͨ
	virtual EnHandleResult OnServerShutdown()				= 0;
};

class IClientSocketListener : public ISocketListener
{
public:
	virtual EnHandleResult OnConnect(DWORD dwConnectionID)	= 0;
};

//�����ӿ�
//ISocketServer �ӿ��ṩ����������������ϲ�Ӧ��ֱ�ӵ���
class ISocketServer
{
public:
	//������
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
	//����ͨ��
	virtual BOOL Start	(LPCTSTR pszBindAddress, USHORT usPort)								= 0;
	//�ر�ͨ��
	virtual BOOL Stop	()																	= 0;
	//��������
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen)						= 0;
	//�������Ƿ�������
	virtual BOOL HasStarted				()													= 0;
	
	virtual EnSocketState GetSocketState()													= 0;
	//��ȡ������
	virtual En_ISS_Error GetLastError	()													= 0;
	//��ȡ��������
	virtual LPCTSTR		GetLastErrorDesc()													= 0;
	//��ȡ�ͻ��˵ĵ�ַ��Ϣ
	virtual BOOL GetConnectionAddress(DWORD dwConnID, CString& strAddress, USHORT& usPort)	= 0;

public:
	virtual ~ISocketServer() {}
};
//ISocketServer �ӿ�����ָ��
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
	OVERLAPPED			ov;           //WSAOVERLAPPED �첽 Overlapped
	WSABUF				buff;         //���ݻ��� buffer
	EnSocketOperation	operation;    //��������

	TBufferObjBase()
	{
		memset(this, 0, sizeof(TBufferObjBase));
	}

	~TBufferObjBase()
	{
		ResetBuf();
		memset(this, 0, sizeof(TBufferObjBase));
	}

	inline void ResetBuf()
	{
		if (!buff.buf) return;
		memset(buff.buf, 0, buff.len);
		buff.len = 0;
	}
};

//�ڴ滺����
struct TBufferObj : public TBufferObjBase
{
	SOCKET	client;    //Client Socket
};
//�ڴ滺������
typedef list<TBufferObj*>	TBufferObjPtrList;   

struct TSocketObjBase
{
	SOCKET	socket;     //Client Socket
};
//Socket ��صĽṹ��
struct TSocketObj : public TSocketObjBase
{
	SOCKADDR_IN		clientAddr;   //Socket ��ַ
	DWORD			connID;       //Connection ID
	CCriSec2		crisec;       //Critical Session
	DWORD           dwTime;       //����ͨ��ʱ��

	inline void SetCurrentTime()
	{
		dwTime = ::GetTickCount();
	}

	// �Ƿ�ʱ 30�볬ʱ
	inline bool IsTimeout()
	{
		return (GetTickCount() - dwTime)>60*1000;
	}
	//����
	inline void Reset()
	{
		memset(this, 0, sizeof(TSocketObj));
	}
};
//�׽�����ؽṹ��
typedef list<TSocketObj*>					TSocketObjPtrList;
typedef hash_map<DWORD, TSocketObj*>		TSocketObjPtrMap;   //Connection ID ��TSocketObj��
typedef TSocketObjPtrMap::iterator			TSocketObjPtrMapI;
typedef TSocketObjPtrMap::const_iterator	TSocketObjPtrMapCI;

BOOL IsIPAddress(LPCTSTR lpszAddress);
BOOL GetIPAddress(LPCTSTR lpszHost, CStringA& strIP);

PVOID GetExtensionFuncPtr					(SOCKET sock, GUID guid); //��ȡ��չ����ָ��
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
int PostAccept				(LPFN_ACCEPTEX pfnAcceptEx, SOCKET soListen, SOCKET soClient, TBufferObj* pBufferObj); //��������
int PostSend				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);   //Ͷ�� ��������
int PostReceive				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);   //��������