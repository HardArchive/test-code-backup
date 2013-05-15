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
/******************************************** �����ࡢ�ӿ� ********************************************/
/*****************************************************************************************************/


/************************************************************************
���ƣ�Windows Socket �����ʼ����
�������Զ����غ�ж�� Windows Socket ���
************************************************************************/
class CInitSocket
{
public:
	CInitSocket(LPWSADATA lpWSAData = nullptr, BYTE minorVersion = 2, BYTE majorVersion = 2)
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

/************************************************************************
���ƣ�ͨ���������״̬
������Ӧ�ó������ͨ��ͨ������� GetState() ������ȡ�����ǰ����״̬
************************************************************************/
enum EnServiceState
{
	SS_STARTING	= 0,	// ��������
	SS_STARTED	= 1,	// �Ѿ�����
	SS_STOPING	= 2,	// ����ֹͣ
	SS_STOPED	= 3,	// �Ѿ�����
};

/************************************************************************
���ƣ�Socket ��������
������Ӧ�ó���� OnErrror() �¼���ͨ���ò�����ʶ�����ֲ������µĴ���
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
���ƣ�Socket ���������ӿ�
�������������˺Ϳͻ��� Socket �������Ĺ�����Ϣ
************************************************************************/
class ISocketListener
{
public:
	/************************************************************************
	���ƣ��¼�֪ͨ������
	�������¼�֪ͨ�ķ���ֵ����ͬ�ķ���ֵ��Ӱ��ͨ������ĺ�����Ϊ
	************************************************************************/
	enum EnHandleResult
	{
		HR_OK		= 0,	// �ɹ�
		HR_IGNORE	= 1,	// ����
		HR_ERROR	= 2,	// ����
	};

public:

	/*
	* ���ƣ��ѷ�������֪ͨ
	* �������ɹ��������ݺ�Socket ���������յ���֪ͨ
	*		
	* ������		dwConnID	-- ���� ID
	*			pData		-- �ѷ������ݻ�����
	*			iLength		-- �ѷ������ݳ���
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ��֪ͨ�������� HR_ERROR������ģʽ���������Դ���
	*/
	virtual EnHandleResult OnSend(DWORD dwConnID, const BYTE* pData, int iLength)					= 0;

	/*
	* ���ƣ��ѽ�������֪ͨ
	* �������ɹ��������ݺ�Socket ���������յ���֪ͨ
	*		
	* ������		dwConnID	-- ���� ID
	*			pData		-- �ѽ������ݻ�����
	*			iLength		-- �ѽ������ݳ���
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ���� OnError() �¼�ʱ�䲢�ر�����
	*/
	virtual EnHandleResult OnReceive(DWORD dwConnID, const BYTE* pData, int iLength)				= 0;

	/*
	* ���ƣ��ѹر�����֪ͨ
	* �����������ر����Ӻ�Socket ���������յ���֪ͨ
	*		
	* ������		dwConnID -- ���� ID
	* ����ֵ��	���Է���ֵ
	*/
	virtual EnHandleResult OnClose(DWORD dwConnID)													= 0;

	/*
	* ���ƣ�ͨ�Ŵ���֪ͨ
	* ������ͨ�ŷ��������Socket ���������յ���֪ͨ�����ر�����
	*		
	* ������		dwConnID	-- ���� ID
	*			enOperation	-- Socket ��������
	*			iErrorCode	-- �������
	* ����ֵ��	���Է���ֵ
	*/
	virtual EnHandleResult OnError(DWORD dwConnID, EnSocketOperation enOperation, int iErrorCode)	= 0;

public:
	virtual ~ISocketListener() {}
};

/************************************************************************
���ƣ������ Socket �������ӿ�
�������������� Socket �������������¼�֪ͨ
************************************************************************/
class IServerSocketListener : public ISocketListener
{
public:

	/*
	* ���ƣ�׼������֪ͨ
	* ������ͨ�ŷ�����������ʱ���ڼ��� Socket ������ɲ���ʼִ�м���ǰ��Socket ����
	*		�����յ���֪ͨ��������������֪ͨ��������ִ�� Socket ѡ�����õȶ��⹤��
	*		
	* ������		soListen	-- ���� Socket
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ��ֹ����ͨ�ŷ������
	*/
	virtual EnHandleResult OnPrepareListen(SOCKET soListen)				= 0;

	/*
	* ���ƣ���������֪ͨ
	* ���������յ��ͻ�����������ʱ��Socket ���������յ���֪ͨ��������������֪ͨ����
	*		����ִ�� Socket ѡ�����û�ܾ��ͻ������ӵȶ��⹤��
	*		
	* ������		dwConnID	-- ���� ID
	*			soClient	-- �ͻ��� Socket
	* ����ֵ��	HR_OK / HR_IGNORE	-- ��������
	*			HR_ERROR			-- �ܾ�����
	*/
	virtual EnHandleResult OnAccept(DWORD dwConnID, SOCKET soClient)	= 0;

	/*
	* ���ƣ��ر�ͨ�����֪ͨ
	* ������ͨ������ر�ʱ��Socket ���������յ���֪ͨ
	*		
	* ������	
	* ����ֵ�����Է���ֵ
	*/
	virtual EnHandleResult OnServerShutdown()							= 0;
};

/************************************************************************
���ƣ������ Socket �������������
����������ĳЩ�¼�֪ͨ��Ĭ�ϴ������������¼���
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
���ƣ��ͻ��� Socket �������ӿ�
����������ͻ��� Socket �������������¼�֪ͨ
************************************************************************/
class IClientSocketListener : public ISocketListener
{
public:

	/*
	* ���ƣ�׼������֪ͨ
	* ������ͨ�ſͻ����������ʱ���ڿͻ��� Socket ������ɲ���ʼִ������ǰ��Socket ����
	*		�����յ���֪ͨ��������������֪ͨ��������ִ�� Socket ѡ�����õȶ��⹤��
	*		
	* ������		dwConnID	-- ���� ID
	*			socket		-- �ͻ��� Socket
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ��ֹ����ͨ�ſͻ������
	*/
	virtual EnHandleResult OnPrepareConnect(DWORD dwConnID, SOCKET socket)		= 0;

	/*
	* ���ƣ��������֪ͨ
	* �����������˳ɹ���������ʱ��Socket ���������յ���֪ͨ
	*		
	* ������		dwConnID	-- ���� ID
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ͬ�����ӣ���ֹ����ͨ�ſͻ������
	*								   �첽���ӣ��ر�����
	*/
	virtual EnHandleResult OnConnect(DWORD dwConnID)							= 0;
};

/************************************************************************
���ƣ��ͻ��� Socket �������������
����������ĳЩ�¼�֪ͨ��Ĭ�ϴ������������¼���
************************************************************************/
class CClientSocketListener : public IClientSocketListener
{
public:
	virtual EnHandleResult OnSend(DWORD dwConnID, const BYTE* pData, int iLength)	{return HR_IGNORE;}

	virtual EnHandleResult OnPrepareConnect(DWORD dwConnID, SOCKET socket)			{return HR_IGNORE;}
	virtual EnHandleResult OnConnect(DWORD dwConnID)								{return HR_IGNORE;}
};

/************************************************************************
���ƣ�ͨ�ŷ��������ӿ�
����������ͨ�ŷ������������в������������Է��ʷ���
************************************************************************/
class ISocketServer
{
public:

	/************************************************************************
	���ƣ������������
	������Start() / Stop() ����ִ��ʧ��ʱ����ͨ�� GetLastError() ��ȡ�������
	************************************************************************/
	enum En_ISS_Error
	{
		ISS_OK						= 0,	// �ɹ�
		ISS_ILLEGAL_STATE			= 1,	// ��ǰ״̬���������
		ISS_INVALID_PARAM			= 2,	// �Ƿ�����
		ISS_SOCKET_CREATE			= 3,	// �������� SOCKET ʧ��
		ISS_SOCKET_BIND				= 4,	// �󶨼�����ַʧ��
		ISS_SOCKET_PREPARE			= 5,	// ���ü��� SOCKET ʧ��
		ISS_SOCKET_LISTEN			= 6,	// ��������ʧ��
		ISS_CP_CREATE				= 7,	// ������ɶ˿�ʧ��
		ISS_WORKER_THREAD_CREATE	= 8,	// ���������߳�ʧ��
		ISS_SOCKE_ATTACH_TO_CP		= 9,	// ���� SOCKET �󶨵���ɶ˿�ʧ��
		ISS_ACCEPT_THREAD_CREATE	= 10,	// ���������߳�ʧ��
	};

public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ�����ͨ�����
	* ���������������ͨ�������������ɺ�ɿ�ʼ���տͻ������Ӳ��շ�����
	*		
	* ������		pszBindAddress	-- ������ַ
	*			usPort			-- �����˿�
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Start	(LPCTSTR pszBindAddress, USHORT usPort)								= 0;

	/*
	* ���ƣ��ر�ͨ�����
	* �������رշ����ͨ��������ر���ɺ�Ͽ����пͻ������Ӳ��ͷ�������Դ
	*		
	* ������	
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Stop	()																	= 0;

	/*
	* ���ƣ���������
	* �������û�ͨ���÷�����ָ���ͻ��˷�������
	*		
	* ������		dwConnID	-- ���� ID
	*			pBuffer		-- �������ݻ�����
	*			iLen		-- �������ݳ���
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ��
	*/
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen)						= 0;

	/*
	* ���ƣ��Ͽ�����
	* �������Ͽ���ĳ���ͻ��˵�����
	*		
	* ������		dwConnID	-- ���� ID
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ��
	*/
	virtual BOOL Disconnect(DWORD dwConnID)													= 0;

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/


	/* ���ͨ������Ƿ������� */
	virtual BOOL HasStarted				()													= 0;
	/* �鿴ͨ�������ǰ״̬ */
	virtual EnServiceState GetState		()													= 0;
	/* ��ȡ���һ��ʧ�ܲ����Ĵ������ */
	virtual En_ISS_Error GetLastError	()													= 0;
	/* ��ȡ���һ��ʧ�ܲ����Ĵ������� */
	virtual LPCTSTR		GetLastErrorDesc()													= 0;
	/* ��ȡ���� Socket �ĵ�ַ��Ϣ */
	virtual BOOL GetListenAddress(CString& strAddress, USHORT& usPort)						= 0;
	/* ��ȡĳ���ͻ������ӵĵ�ַ��Ϣ */
	virtual BOOL GetClientAddress(DWORD dwConnID, CString& strAddress, USHORT& usPort)		= 0;

	/* ���ù����߳�������ͨ������Ϊ 2 * CPU + 2�� */
	virtual void SetWorkerThreadCount	(DWORD dwWorkerThreadCount)		= 0;
	/* ���� Accept ԤͶ�� Socket ������ͨ������Ϊ�����߳����� 1 - 2 ���� */
	virtual void SetAcceptSocketCount	(DWORD dwAcceptSocketCount)		= 0;
	/* ����ͨ�����ݻ�������С������ƽ��ͨ�����ݰ���С�������ã�ͨ������Ϊ 1024 �ı����� */
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)		= 0;
	/* ���ü��� Socket �ĵȺ���д�С�����ݲ������������������ã� */
	virtual void SetSocketListenQueue	(DWORD dwSocketListenQueue)		= 0;
	/* ���� Socket ����ش�С��ͨ������Ϊƽ���������������� 1/3 - 1/2�� */
	virtual void SetFreeSocketObjPool	(DWORD dwFreeSocketObjPool)		= 0;
	/* �����ڴ�黺��ش�С��ͨ������Ϊ Socket ����ش�С�� 2 - 3 ���� */
	virtual void SetFreeBufferObjPool	(DWORD dwFreeBufferObjPool)		= 0;
	/* ���� Socket ����ػ��շ�ֵ��ͨ������Ϊ Socket ����ش�С�� 3 ���� */
	virtual void SetFreeSocketObjHold	(DWORD dwFreeSocketObjHold)		= 0;
	/* �����ڴ�黺��ػ��շ�ֵ��ͨ������Ϊ�ڴ�黺��ش�С�� 3 ���� */
	virtual void SetFreeBufferObjHold	(DWORD dwFreeBufferObjHold)		= 0;
	/* �����������������������ʧ�ܳ����ô�������Ϊ�ѶϿ����ӣ�0 �򲻼�飩 */
	virtual void SetKeepAliveTimes		(DWORD dwKeepAliveTimes)		= 0;
	/* ������������������룬0 �򲻼�飩 */
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)		= 0;
	/* ���ùرշ���ǰ�ȴ����ӹرյ��ʱ�ޣ����룬0 �򲻵ȴ��� */
	virtual void SetMaxShutdownWaitTime	(DWORD dwMaxShutdownWaitTime)	= 0;

	/* ��ȡ�����߳����� */
	virtual DWORD GetWorkerThreadCount	()	= 0;
	/* ��ȡ Accept ԤͶ�� Socket ���� */
	virtual DWORD GetAcceptSocketCount	()	= 0;
	/* ��ȡͨ�����ݻ�������С */
	virtual DWORD GetSocketBufferSize	()	= 0;
	/* ��ȡ���� Socket �ĵȺ���д�С */
	virtual DWORD GetSocketListenQueue	()	= 0;
	/* ��ȡ Socket ����ش�С */
	virtual DWORD GetFreeSocketObjPool	()	= 0;
	/* ��ȡ�ڴ�黺��ش�С */
	virtual DWORD GetFreeBufferObjPool	()	= 0;
	/* ��ȡ Socket ����ػ��շ�ֵ */
	virtual DWORD GetFreeSocketObjHold	()	= 0;
	/* ��ȡ�ڴ�黺��ػ��շ�ֵ */
	virtual DWORD GetFreeBufferObjHold	()	= 0;
	/* ��ȡ���������� */
	virtual DWORD GetKeepAliveTimes		()	= 0;
	/* ��ȡ��������� */
	virtual DWORD GetKeepAliveInterval	()	= 0;
	/* ��ȡ�رշ���ǰ�ȴ����ӹرյ��ʱ�� */
	virtual DWORD GetMaxShutdownWaitTime()	= 0;

public:
	virtual ~ISocketServer() {}
};

/* ͨ�ŷ��������ӿ�����ָ�� */
typedef auto_ptr<ISocketServer>	ISocketServerPtr;

/************************************************************************
���ƣ�ͨ�ŷ��������ӿ�
����������ͨ�ſͻ�����������в������������Է��ʷ���
************************************************************************/
class ISocketClient
{
public:
	/************************************************************************
	���ƣ������������
	������Start() / Stop() ����ִ��ʧ��ʱ����ͨ�� GetLastError() ��ȡ�������
	************************************************************************/
	enum En_ISC_Error
	{
		ISC_OK						= 0,	// �ɹ�
		ISC_ILLEGAL_STATE			= 1,	// ��ǰ״̬���������
		ISC_INVALID_PARAM			= 2,	// �Ƿ�����
		ISC_SOCKET_CREATE_FAIL		= 3,	// ���� Client Socket ʧ��
		ISC_SOCKET_PREPARE_FAIL		= 4,	// ���� Client Socket ʧ��
		ISC_CONNECT_SERVER_FAIL		= 5,	// ���ӷ�����ʧ��
		ISC_WORKER_CREATE_FAIL		= 6,	// ���������߳�ʧ��
		ISC_NETWORK_ERROR			= 7,	// �������
		ISC_DATA_PROC_ERROR			= 8,	// ���ݴ������
	};

public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ�����ͨ�����
	* �����������ͻ���ͨ����������ӷ���ˣ�������ɺ�ɿ�ʼ�շ�����
	*		
	* ������		pszRemoteAddress	-- ����˵�ַ
	*			usPort				-- ����˶˿�
	*			bAsyncConnect		-- �Ƿ�����첽 Connnect
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Start	(LPCTSTR pszRemoteAddress, USHORT usPort, BOOL bAsyncConnect = FALSE)	= 0;

	/*
	* ���ƣ��ر�ͨ�����
	* �������رտͻ���ͨ��������ر���ɺ�Ͽ������˵����Ӳ��ͷ�������Դ
	*		
	* ������	
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Stop	()																		= 0;

	/*
	* ���ƣ���������
	* �������û�ͨ���÷��������˷�������
	*		
	* ������		dwConnID	-- ���� ID������������Ŀǰ�ò�����δʹ�ã�
	*			pBuffer		-- �������ݻ�����
	*			iLen		-- �������ݳ���
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Send	(DWORD dwConnID, const BYTE* pBuffer, int iLen)							= 0;

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/


	/* ���ͨ������Ƿ������� */
	virtual BOOL HasStarted					()													= 0;
	/* �鿴ͨ�������ǰ״̬ */
	virtual EnServiceState	GetState		()													= 0;
	/* ��ȡ���һ��ʧ�ܲ����Ĵ������ */
	virtual En_ISC_Error	GetLastError	()													= 0;
	/* ��ȡ���һ��ʧ�ܲ����Ĵ������� */
	virtual LPCTSTR			GetLastErrorDesc()													= 0;
	/* ��ȡ�������������� ID */
	virtual DWORD			GetConnectionID	()													= 0;
	/* ��ȡ Client Socket �ĵ�ַ��Ϣ */
	virtual BOOL			GetLocalAddress	(CString& strAddress, USHORT& usPort)				= 0;

	/* ����ͨ�����ݻ�������С������ƽ��ͨ�����ݰ���С�������ã�ͨ������Ϊ��(N * 1024) - sizeof(TBufferObj)�� */
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)	= 0;
	/* �����������������������ʧ�ܳ����ô�������Ϊ�ѶϿ����ӣ�0 �򲻼�飩 */
	virtual void SetKeepAliveTimes		(DWORD dwKeepAliveTimes)	= 0;
	/* ������������������룬0 �򲻼�飩 */
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)	= 0;

	/* ��ȡͨ�����ݻ�������С */
	virtual DWORD GetSocketBufferSize	()	= 0;
	/* ��ȡ���������� */
	virtual DWORD GetKeepAliveTimes		()	= 0;
	/* ��ȡ��������� */
	virtual DWORD GetKeepAliveInterval	()	= 0;

public:
	virtual ~ISocketClient() {}
};

/* ͨ�ſͻ�������ӿ�����ָ�� */
typedef auto_ptr<ISocketClient>	ISocketClientPtr;

/* ���ݻ����������ṹ */
struct TBufferObjBase
{
	OVERLAPPED			ov;
	WSABUF				buff;
	EnSocketOperation	operation;
};

/* ���ݻ������ṹ */
struct TBufferObj : public TBufferObjBase
{
	SOCKET	client;
};

/* ���ݻ������ṹ���� */
typedef list<TBufferObj*>	TBufferObjPtrList;

/* Socket �����������ṹ */
struct TSocketObjBase
{
	SOCKET	socket;
};

/* ���ݻ������ṹ */
struct TSocketObj : public TSocketObjBase
{
	SOCKADDR_IN		clientAddr;
	DWORD			connID;
	CCriSec2		crisec;
};

/* ���ݻ������ṹ���� */
typedef list<TSocketObj*>					TSocketObjPtrList;
/* ���ݻ������ṹ��ϣ�� */
typedef hash_map<DWORD, TSocketObj*>		TSocketObjPtrMap;
/* ���ݻ������ṹ��ϣ������� */
typedef TSocketObjPtrMap::iterator			TSocketObjPtrMapI;
/* ���ݻ������ṹ��ϣ�� const ������ */
typedef TSocketObjPtrMap::const_iterator	TSocketObjPtrMapCI;


/*****************************************************************************************************/
/******************************************** ������������ ********************************************/
/*****************************************************************************************************/

/* ����ַ����Ƿ���� IP ��ַ��ʽ */
BOOL IsIPAddress(LPCTSTR lpszAddress);
/* ͨ����������ȡ IP ��ַ */
BOOL GetIPAddress(LPCTSTR lpszHost, CString& strIP);
/* �� SOCKADDR_IN �ṹת��Ϊ��ַ���� */
BOOL sockaddr_IN_2_A(const SOCKADDR_IN& addr, ADDRESS_FAMILY& usFamily, CString& strAddress, USHORT& usPort);
/* �ѵ�ַ����ת��Ϊ SOCKADDR_IN �ṹ */
BOOL sockaddr_A_2_IN(ADDRESS_FAMILY usFamily, LPCTSTR pszAddress, USHORT usPort, SOCKADDR_IN& addr);
/* ��ȡ Socket �ı��ػ�Զ�̵�ַ��Ϣ */
BOOL GetSocketAddress(SOCKET socket, CString& strAddress, USHORT& usPort, BOOL bLocal = TRUE);
/* ��ȡ Socket �ı��ص�ַ��Ϣ */
BOOL GetSocketLocalAddress(SOCKET socket, CString& strAddress, USHORT& usPort);
/* ��ȡ Socket ��Զ�̵�ַ��Ϣ */
BOOL GetSocketRemoteAddress(SOCKET socket, CString& strAddress, USHORT& usPort);

/* ��ȡ Socket ��ĳ����չ������ָ�� */
PVOID GetExtensionFuncPtr					(SOCKET sock, GUID guid);
/* ��ȡ AcceptEx ��չ����ָ�� */
LPFN_ACCEPTEX Get_AcceptEx_FuncPtr			(SOCKET sock);
/* ��ȡ GetAcceptExSockaddrs ��չ����ָ�� */
LPFN_GETACCEPTEXSOCKADDRS Get_GetAcceptExSockaddrs_FuncPtr(SOCKET sock);
/* ��ȡ ConnectEx ��չ����ָ�� */
LPFN_CONNECTEX Get_ConnectEx_FuncPtr		(SOCKET sock);
/* ��ȡ TransmitFile ��չ����ָ�� */
LPFN_TRANSMITFILE Get_TransmitFile_FuncPtr	(SOCKET sock);

/************************************************************************
���ƣ�setsockopt() ��������
�������򻯳��õ� setsockopt() ����
************************************************************************/

int SSO_UpdateAcceptContext	(SOCKET soClient, SOCKET soBind);
int SSO_UpdateConnectContext(SOCKET soClient, int iOption);
int SSO_NoDelay				(SOCKET sock, BOOL bNoDelay = TRUE);
int SSO_DontLinger			(SOCKET sock, BOOL bDont = TRUE);
int SSO_Linger				(SOCKET sock, USHORT l_onoff, USHORT l_linger);
int SSO_KeepAlive			(SOCKET sock, BOOL bKeepAlive = TRUE);
int SSO_KeepAliveVals		(SOCKET sock, u_long onoff, u_long time, u_long interval);  //������� ֵ����
int SSO_RecvBuffSize		(SOCKET sock, int size);
int SSO_SendBuffSize		(SOCKET sock, int size);
int SSO_ReuseAddress		(SOCKET sock, BOOL bReuse = TRUE);

/************************************************************************
���ƣ�Socket ��������
������Socket ������װ����
************************************************************************/

/* �ر� Socket */
int ManualCloseSocket		(SOCKET sock, int iShutdownFlag = 0xFF, BOOL bGraceful = TRUE, BOOL bReuseAddress = FALSE);
/* Ͷ�� AccceptEx() */
int PostAccept				(LPFN_ACCEPTEX pfnAcceptEx, SOCKET soListen, SOCKET soClient, TBufferObj* pBufferObj);
/* Ͷ�� WSASend() */
int PostSend				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
/* Ͷ�� WSARecv() */
int PostReceive				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);