
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
	BOOL CreateListenSocket(LPCTSTR pszBindAddress, USHORT usPort);  //���������׽��ֿ�ʼ�����˿�
	BOOL CreateCompletePort();                                       //������ɶ˿�
	BOOL CreateWorkerThreads();          //���������߳�
	BOOL StartAcceptThread();            //�������������߳�

	void CloseListenSocket();
	void WaitForAcceptThreadEnd();      //����m_evtAccept�¼� ���˳������߳�
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
	//ʹ�� AcceptEx() ���տͻ����������󲢴��� Client Socket ���̣߳�
	//�������������ʵ��Ϊ�������߳̽�ʹ�����ģ�黮�ָ�����������Ҫ���Ǳ�����ҵ���߼���ͨ�Ŵ�����໥Ӱ��
	static UINT WINAPI AcceptThreadProc(LPVOID pv);   //���������߳�
	//ʹ�� GetQueuedCompletionStatus() ���������¼����������罻���Ķ���̣߳�
	//�����̴߳����������¼�������ϲ�Ӧ�÷��� OnAccept/OnSend/OnReceive �����֪ͨ��
	//�����̵߳��������Ը���ʵ�����֮�����ã�ͨ������Ϊ��CPU Core Number * 2 + 2��
	static UINT WINAPI WorkerThreadProc(LPVOID pv);   //IOCP�����߳�

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
	//�������Ӻ���ָ��
	LPFN_ACCEPTEX				m_pfnAcceptEx;          //AcceptEx����ָ��       
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;    //����ָ������ȡ�ý���socket��IP�˿�
	//GetAcceptExSockaddrs��ר��ΪAcceptEx����׼���ģ�����AcceptEx���ܵĵ�һ�������еı��غ�Զ�̻����ĵ�ַ���ظ��û���
private:
	IServerSocketListener*	m_psoListener;

	volatile EnSocketState	m_enState;
	volatile DWORD			m_dwConnID;

	En_ISS_Error	m_enLastError;

	SOCKET			m_soListen;       //�����׽���
	HANDLE			m_hCompletePort;
	HANDLE			m_hAcceptThread;
	vector<HANDLE>	m_vtWorkerThreads;

	TBufferObjPtrList	m_lsFreeBuffer;
	TSocketObjPtrList	m_lsFreeSocket;
	TSocketObjPtrMap	m_mpClientSocket;

	CCriSec				m_csFreeBuffer;
	CCriSec				m_csFreeSocket;
	CCriSec				m_csClientSocket;  //�ٽ���������

	CEvt				m_evtAccept;       //�¼��ں˶�������-�����¼�

	smart_simple_ptr<CSEM>	m_psemAccept;  //��ʵ������ָ��-�����ź���
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
	DWORD m_dwIocpThreadCount;     //�����߳�����  CPU Core Number * 2 + 2
	DWORD m_dwAcceptSocketCount;   //������������
	DWORD m_dwIocpBufferSize;
	DWORD m_dwSocketListenQueue;  //ָ���ں�Ϊ���׽ӿ��Ŷӵ�������Ӹ��� �˴�Ϊ30
	DWORD m_dwFreeSocketObjPool;
	DWORD m_dwFreeBufferObjPool;
	DWORD m_dwFreeSocketObjHold;
	DWORD m_dwFreeBufferObjHold;
	DWORD m_dwKeepAliveTimes;
	DWORD m_dwKeepAliveInterval;
};