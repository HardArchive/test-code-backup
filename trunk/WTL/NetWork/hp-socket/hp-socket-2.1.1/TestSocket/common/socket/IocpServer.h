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
	static const DWORD DEFAULT_WORKER_THREAD_COUNT;                        //�����߳�����::GetCpuCount() * 2 + 2;
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
	BOOL CheckParams();                                                    //�������
	BOOL CheckStarting();
	BOOL CheckStoping();
	BOOL CreateListenSocket(LPCTSTR pszBindAddress, USHORT usPort);        //���������׽��ֿ�ʼ�����˿�
	BOOL CreateCompletePort();                                             //������ɶ˿�
	BOOL CreateWorkerThreads();                                            //���������߳�
	BOOL StartAcceptThread();                                              //�������������߳�

	void CloseListenSocket();
	void WaitForAcceptThreadEnd();                                         //����m_evtAccept�¼� ���˳������߳�
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

	/* TBufferObj �� TSocketObj �����ϵ�з��� */
	TBufferObj*	GetFreeBufferObj(int iLen = 0);                             //ȡ������buffer ���ڴ洢�շ�����TBufferObj
	TSocketObj*	GetFreeSocketObj();                                         //ȡ������buffer ���ڴ洢�����׽���TSocketObj
	void		AddFreeBufferObj(TBufferObj* pBufferObj);
	void		AddFreeSocketObj(DWORD dwConnID);
	TBufferObj*	CreateBufferObj();                                          
	TSocketObj*	CreateSocketObj();                                          
	void		DeleteBufferObj(TBufferObj* pBufferObj);
	void		DeleteSocketObj(TSocketObj* pSocketObj);

	void		AddClientSocketObj(DWORD dwConnID, TSocketObj* pSocketObj);//��Connection ID ��TSocketObj��ӵ�m_mpClientSocket����
	TSocketObj* FindSocketObj(DWORD dwConnID);
	void		CloseSocketObj(TSocketObj* pSocketObj, int iShutdownFlag = SD_SEND);

private:
	//ʹ�� AcceptEx() ���տͻ����������󲢴��� Client Socket ���̣߳�
	//�������������ʵ��Ϊ�������߳̽�ʹ�����ģ�黮�ָ�����������Ҫ���Ǳ�����ҵ���߼���ͨ�Ŵ�����໥Ӱ��
	static UINT WINAPI AcceptThreadProc(LPVOID pv);                        //���������߳�
	//ʹ�� GetQueuedCompletionStatus() ���������¼����������罻���Ķ���̣߳�
	//�����̴߳����������¼�������ϲ�Ӧ�÷��� OnAccept/OnSend/OnReceive �����֪ͨ��
	//�����̵߳��������Ը���ʵ�����֮�����ã�ͨ������Ϊ��CPU Core Number * 2 + 2��
	static UINT WINAPI WorkerThreadProc(LPVOID pv);                        //IOCP�����߳�

	void HandleIo		(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwBytes, DWORD dwErrorCode); //IO����
	void HandleError	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj, DWORD dwErrorCode);                //�������Ӵ���
	void HandleAccept	(SOCKET soListen, TBufferObj* pBufferObj);                                                          //�������Ӵ���
	void HandleSend		(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);                                   //�������ݴ���
	void HandleReceive	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);                                   //�������ݴ���
 
	int DoSend		(DWORD dwConnID, TSocketObj* pSocketObj, const BYTE* pBuffer, int iLen);                                //Ͷ��һ������
	int DoReceive	(DWORD dwConnID, TSocketObj* pSocketObj, TBufferObj* pBufferObj);                                       //Ͷ��һ������

private:
	SOCKET	GetAcceptSocket();                                             //������ȡ���ͻ��˽��������׽���
	BOOL	DeleteAcceptSocket(SOCKET socket, BOOL bCloseSocket = FALSE);  //���������ӵ�socket�ӵȴ��������ӵ�socket����m_setAccept��ɾ��
	void	ReleaseAcceptSockets();

private:
	CInitSocket					m_wsSocket;
	//�������Ӻ���ָ��
	LPFN_ACCEPTEX				m_pfnAcceptEx;                             //AcceptEx����ָ��  
	//GetAcceptExSockaddrs��ר��ΪAcceptEx����׼���ģ�����AcceptEx���ܵĵ�һ�������еı��غ�Զ�̻����ĵ�ַ���ظ��û�
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;                 //GetAcceptExSockaddrs����ָ������ȡ�ý���socket��IP�˿�

private:
	IServerSocketListener*	m_psoListener;                                 //������ָ��

	volatile EnServiceState	m_enState;                                     //ͨ�����״̬��ʶ
	volatile DWORD			m_dwConnID;                                    //Connection ID ��ǰֵ

	En_ISS_Error	m_enLastError;

	SOCKET			m_soListen;                                            //�����׽���
	HANDLE			m_hCompletePort;                                       //��ɶ˿ھ��
	HANDLE			m_hAcceptThread;                                       //Accept �߳̾��
	vector<HANDLE>	m_vtWorkerThreads;                                     //�����߳̾������

	TBufferObjPtrList	m_lsFreeBuffer;                                    //TBufferObj ����ؿ��ж���
	TSocketObjPtrList	m_lsFreeSocket;                                    //TSocketObj ����ؿ��ж���
	TSocketObjPtrMap	m_mpClientSocket;                                  //Connection ID ��TSocketObj�� ӳ��

	//�ٽ���������
	CCriSec				m_csFreeBuffer;
	CCriSec				m_csFreeSocket;
	CCriSec				m_csClientSocket;

	CEvt				m_evtAccept;                                       //�¼��ں˶�������-���������¼�

	smart_simple_ptr<CSEM>	m_psemAccept;                                  //��ʵ������ָ��-�����ź���
	CCriSec					m_csAccept;
	ulong_ptr_set			m_setAccept;                                   //�ͻ����׽��ּ��� �ȴ������׽��ּ���

	CPrivateHeap			m_phSocket;                                    //
	CPrivateHeap			m_phBuffer;                                    //�����˽�ж�

private:
	DWORD m_dwWorkerThreadCount;    //�����߳�����  CPU Core Number * 2 + 2
	DWORD m_dwAcceptSocketCount;    //Ĭ�ϲ��� AcceptEx ���ô���
	DWORD m_dwSocketBufferSize;     //TBufferObj ���ݻ�������С
	DWORD m_dwSocketListenQueue;    //ָ���ں�Ϊ���׽ӿ��Ŷӵ�������Ӹ��� �˴�Ϊ30
	DWORD m_dwFreeSocketObjPool;    //����TSocketObj ����� �Ķ�����������
	DWORD m_dwFreeBufferObjPool;    //����TBufferObj �������������
	DWORD m_dwFreeSocketObjHold;    //
	DWORD m_dwFreeBufferObjHold;    //
	DWORD m_dwKeepAliveTimes;       //��һ�ο�ʼ���͵�ʱ�䣨��λ���룩��ֵΪ3
	DWORD m_dwKeepAliveInterval;    //TCP���Ӵ��ڳ�ͨʱ���̽��Ƶ��  ÿ�μ��ļ�� ����λ���룩 ֵ 10 * 1000
	DWORD m_dwMaxShutdownWaitTime;  //���ֹͣ�ȴ�ʱ�� 5 * 1000
};