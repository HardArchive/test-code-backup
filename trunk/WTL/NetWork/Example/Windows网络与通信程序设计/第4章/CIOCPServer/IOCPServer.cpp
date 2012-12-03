#include "StdAfx.h"
#include ".\iocpserver.h"
#define OP_ACCEPT 1
#define OP_WRITE  2
#define OP_READ   3

CIOCPServer::CIOCPServer(void)
{//�б�
    m_pFreeBufferList=NULL;
	m_pFreeContextList=NULL;
	m_pPendingAccepts=NULL;
	m_pConnectionList=NULL;
	m_nFreeBufferCount=0;
    m_nFreeContextCount=0;
	m_nPendingAcceptCount=0;
	m_nCurrentConnection=0;
	::InitializeCriticalSection(&m_FreeBufferListLock);
	::InitializeCriticalSection(&m_FreeContextListLock);
	::InitializeCriticalSection(&m_PendingAcceptsLock);
	::InitializeCriticalSection(&m_ConnectionListLock);
	//Accept����
	m_hAcceptEvent=::CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hRepostEvent=::CreateEvent(NULL,FALSE,FALSE,NULL);
	m_nRepostCount=0;
	m_nPort=4567;
	m_nInitialAccepts=10;
	m_nInitialReads=4;
	m_nMaxAccepts=100;
    m_nMaxSends=20;
    m_nMaxFreeBuffers=200;
	m_nMaxFreeContexts=100;
	m_nMaxConnections=2000;
	m_hListenThread=NULL;
	m_hCompletion=NULL;
	m_sListen=INVALID_SOCKET;
	m_lpfnAcceptEx=NULL;
	m_lpfnGetAcceptExSockaddrs=NULL;
	m_bShutDown=FALSE;
	m_bServerStarted=FALSE;
	//��ʼ��WSA_32.DLL
	WSADATA wsaData;
	WORD sockVersion=MAKEWORD(2,2);
	::WSAStartup(sockVersion,&wsaData);
}

CIOCPServer::~CIOCPServer(void)
{
     Shutdown();
	 if(m_sListen!=INVALID_SOCKET)
		 ::closesocket(m_sListen);
	 if(m_hListenThread!=NULL)
		 ::CloseHandle(m_hListenThread);
	 ::CloseHandle(m_hRepostEvent);
	 ::CloseHandle(m_hAcceptEvent);
	 ::DeleteCriticalSection(&m_FreeBufferListLock);
	 ::DeleteCriticalSection(&m_FreeContextListLock);
	 ::DeleteCriticalSection(&m_PendingAcceptsLock);
	 ::DeleteCriticalSection(&m_ConnectionListLock);
	 ::WSACleanup();
}
/*
 *���ܣ������ڴ�ռ�
 *������I/O������ʹ�õĻ���������
 *����ֵ:I/O������ָ��
 */
CIOCPBuffer *CIOCPServer::AllocateBuffer(int nLen)
{
     CIOCPBuffer *pBuffer=NULL;
	 if(nLen>BUFFER_SIZE)
		 return NULL;
	 //Ϊ���������������ڴ�
	 ::EnterCriticalSection(&m_FreeBufferListLock);
	 if(m_pFreeBufferList==NULL)//�ڴ��Ϊ�գ������µ��ڴ�
	 {
		 pBuffer=(CIOCPBuffer *)::HeapAlloc(GetProcessHeap(),
			 HEAP_ZERO_MEMORY,sizeof(CIOCPBuffer)+BUFFER_SIZE);
	 }
	 else
	 {
		 pBuffer=m_pFreeBufferList;
		 m_pFreeBufferList=m_pFreeBufferList->pNext;
		 pBuffer->pNext=NULL;
		 m_nFreeBufferCount--;
	 }
	 ::LeaveCriticalSection(&m_FreeBufferListLock);
	 //��ʼ���µĻ���������
	 if(pBuffer!=NULL)
	 {
		 pBuffer->buff=(char *)(pBuffer+1);
		 pBuffer->nLen=nLen;
	 }
	 return pBuffer;
}
void CIOCPServer::ReleaseBuffer(CIOCPBuffer *pBuffer)
{
	::EnterCriticalSection(&m_FreeBufferListLock);
	if(m_nFreeBufferCount<=m_nMaxFreeBuffers)//��Ҫ�ͷŵ��ڴ���ӵ������б���
	{
        memset(pBuffer,0,sizeof(CIOCPBuffer)+BUFFER_SIZE);
		pBuffer->pNext=m_pFreeBufferList;
		m_pFreeBufferList=pBuffer;
		m_nFreeBufferCount++;
	}
	else//�Ѿ��ﵽ���ֵ�������ͷ��ڴ�
	{
		::HeapFree(::GetProcessHeap(),0,pBuffer);
	}
	::LeaveCriticalSection(&m_FreeBufferListLock);
}

//�����²���
//����������
CIOCPContext *CIOCPServer::AllocateContext(SOCKET s)
{
   CIOCPContext *pContext=NULL;
   ::EnterCriticalSection(&m_FreeContextListLock);
   if(m_pFreeContextList==NULL)//�������������Ϊ��,�����µľ��������
   {
	   pContext=(CIOCPContext *)::HeapAlloc(GetProcessHeap(),
		   HEAP_ZERO_MEMORY,sizeof(CIOCPContext)+sizeof(CIOCPBuffer));
   }
   else
   {
	   pContext=m_pFreeContextList;
	   m_pFreeContextList=m_pFreeContextList->pNext;
	   pContext->pNext=NULL;
	   m_nFreeContextCount--;
   }
   ::LeaveCriticalSection(&m_FreeContextListLock);
   if(pContext!=NULL)
   {
	   pContext->pOutOfOrderReads=(CIOCPBuffer *)(pContext+1);
	   pContext->pOutOfOrderReads->buff=NULL;
	   pContext->s=s;
   }
	return pContext;
}
void CIOCPServer::ReleaseContext(CIOCPContext *pContext)
{
	if(pContext->s!=INVALID_SOCKET)
		::closesocket(pContext->s);
	//�����ͷ�(����еĻ�)���׽����ϵ�û�а�˳����ɵĶ�I/0�Ļ�����
	CIOCPBuffer *pNext;
	while(pContext->pOutOfOrderReads!=NULL)
	{
		pNext=pContext->pOutOfOrderReads->pNext;
		ReleaseBuffer(pContext->pOutOfOrderReads);
		pContext->pOutOfOrderReads=pNext;
	}
	::EnterCriticalSection(&m_FreeContextListLock);
	if(m_nFreeContextCount<=m_nMaxFreeContexts)//��ӵ������б�
	{
		CRITICAL_SECTION cstmp=pContext->Lock;//�Ƚ��ؼ�����α���������һ����ʱ������
		memset(pContext,0,sizeof(CIOCPContext));//��Ҫ�ͷŵ������ʶ����ʼ��0
		//��Ҫ�ͷŵ������Ķ�����ӵ������б��ͷ
		pContext->Lock=cstmp;
		pContext->pNext=m_pFreeContextList;
		m_pFreeContextList=pContext;
		m_nFreeContextCount++;//���¼���
	}
	else
	{
		::DeleteCriticalSection(&pContext->Lock);
		::HeapFree(::GetProcessHeap(),0,pContext);
	}
}
//�������ر�ʱ�ͷ��ڴ��ռ�õĵط�ʵ�ַ�������m_pFreeBufferList
//�����²���
void CIOCPServer::FreeBuffers()
{
	if(m_pFreeBufferList!=NULL)
	{
		CIOCPBuffer *pBufferList=m_pFreeBufferList;
		CIOCPBuffer *pPreBufferList=m_pFreeBufferList;
		while(pBufferList->pNext!=NULL)
		{
			pBufferList=pBufferList->pNext;
			::HeapFree(::GetProcessHeap(),0,pPreBufferList);
			pPreBufferList=pBufferList;
		}
	}
}
//�������ر�ʱ�ͷ��ڴ��ռ��ʵ�ַ�������m_pFreeContextList
//�����²���
void CIOCPServer::FreeContexts()
{
	if(m_pConnectionList!=NULL)
	{
		CIOCPContext *pContextList=m_pConnectionList;
		CIOCPContext *pPreContextList=m_pConnectionList;
		while(pContextList->pNext!=NULL)
		{
			pContextList=pContextList->pNext;
			::HeapFree(::GetProcessHeap(),0,pPreContextList);
			pPreContextList=pContextList;
		}
	}
}
//��Ա����m_pConnectionListָ��λ������б������������
//�ӵ�CIOCPContext
//������ɵı�
//��������һ��CIOCPContext��������Ѿ��ﵽ������
//������������������FALSE
//�����²���
BOOL CIOCPServer::AddAConnection(CIOCPContext *pContext)
{
   if(m_nCurrentConnection<=m_nMaxConnections)
   {
        pContext->pNext=m_pConnectionList;
		m_pConnectionList=pContext;
		return TRUE;
   }
   return FALSE;
}
//�ر�ָ���Ŀͻ����ӣ����ȴ����ӣ��б����Ƴ�Ҫ�رյ�����
//Ȼ���ٹرտͻ��׽���
//�����²���
void CIOCPServer::CloseAConnection(CIOCPContext *pContext)
{
	CIOCPContext *pZanShiContext;
	CIOCPContext *pPreContext;
	pZanShiContext=m_pConnectionList;
	pPreContext=m_pConnectionList;
	::WaitForSingleObject(m_hCompletion,INFINITE);
	if(pZanShiContext==pContext)//�ж��ǲ��Ǳ�ͷ
		m_pConnectionList=pContext->pNext;
	while(pZanShiContext->pNext!=pContext&&pZanShiContext->pNext!=NULL)//������ǵĻ���ָ������������
	{
		pZanShiContext=pZanShiContext->pNext;
	}
	pPreContext=pZanShiContext->pNext;
	pZanShiContext->pNext=pZanShiContext->pNext->pNext;
	ReleaseContext(pPreContext);
}
//�������������б�ر����еĿͻ��׽���
//�����²���
void CIOCPServer::CloseAllConnection()
{
	if(m_pConnectionList!=NULL)
	{
		CIOCPContext *pZanShiContext=m_pConnectionList;
		CIOCPContext *pPreContext=m_pConnectionList;
		::WaitForSingleObject(m_hCompletion,INFINITE);
		while(pZanShiContext->pNext!=NULL)
		{
			pZanShiContext=pZanShiContext->pNext;
			::closesocket(pPreContext->s);
			::HeapFree(GetProcessHeap(),0,pZanShiContext);
		}
	}
}
//��һ��I/0������������뵽m_pPendingAccepts���н��������������
//�����²���
BOOL CIOCPServer::InsertPendingAccept(CIOCPBuffer *pBuffer)
{
	::EnterCriticalSection(&m_PendingAcceptsLock);
	if(m_pPendingAccepts==NULL)
	{
		m_pPendingAccepts=pBuffer;
		m_pPendingAccepts->pNext=NULL;
	}
	else
	{
        pBuffer->pNext=m_pPendingAccepts;
		m_pPendingAccepts=pBuffer;
	}
	::LeaveCriticalSection(&m_PendingAcceptsLock);
	return TRUE;
}
//�������������Ƴ�ָ���Ļ���������
BOOL CIOCPServer::RemovePendingAccept(CIOCPBuffer *pBuffer)
{
	CIOCPBuffer *pZanShiBuffer;
	CIOCPBuffer *pPreBuffer;
	pZanShiBuffer=m_pPendingAccepts;
	pPreBuffer=m_pPendingAccepts;
	::EnterCriticalSection(&m_PendingAcceptsLock);
	if(m_pPendingAccepts!=NULL&&m_pPendingAccepts==pBuffer)
	{
		m_pPendingAccepts=m_pPendingAccepts->pNext;
	}
	else
	{
		while(pZanShiBuffer->pNext!=NULL&&pZanShiBuffer->pNext!=pBuffer)
		{
			pZanShiBuffer=pZanShiBuffer->pNext;
		}
		pPreBuffer=pZanShiBuffer->pNext;
        pZanShiBuffer=pZanShiBuffer->pNext->pNext;
	}
	ReleaseBuffer(pPreBuffer);
	return TRUE;
}
//pOutOfOrderReads�б��е�Ԫ���ǰ��������кŴ�С�����˳�����У�������˳������
//���������뵽�б�֮��CIOCPServer��Ҫ����ͷԪ�أ����������к��Ƿ����һ��Ҫ����
//���к�һ�£�һ�µĻ������ӱ����Ƴ������ظ��û�
CIOCPBuffer *CIOCPServer::GetNextReadBuffer(CIOCPContext *pContext,CIOCPBuffer *pBuffer)
{
	if(pBuffer!=NULL)
	{//�����Ҫ������һ�����к���ȣ������黺����
		if(pBuffer->nSequenceNumber==pContext->nCurrentReadSequence)
		{
			return pBuffer;
		}
	  //�������ȣ���˵��û�а���˳��������ݣ�
	  //����黺�������浽���ӵ�pOutOfOrderReads�б���
	  //�б��еĻ������ǰ��������кŴ�С�����˳������
		pBuffer->pNext=NULL;
		CIOCPBuffer *ptr=pContext->pOutOfOrderReads;
		CIOCPBuffer *pPre=NULL;
		while(ptr!=NULL)
		{
			if(pBuffer->nSequenceNumber<ptr->nSequenceNumber)
				break;
			pPre=ptr;
			ptr=ptr->pNext;
		}
		if(pPre==NULL)//Ӧ�ò��뵽��ͷ
		{
			pBuffer->pNext=pContext->pOutOfOrderReads;
			pContext->pOutOfOrderReads=pBuffer;
		}
		else//Ӧ�ò��뵽����м�
		{
			pBuffer->pNext=pPre->pNext;
			pPre->pNext=pBuffer->pNext;
		}
	}
	//����ͷԪ�ص����кţ������Ҫ�������к�һ�£��ͽ����ӱ����Ƴ������ظ��û�
	CIOCPBuffer *ptr=pContext->pOutOfOrderReads;
	if(ptr!=NULL&&(ptr->nSequenceNumber==pContext->nCurrentReadSequence))
	{
		pContext->pOutOfOrderReads=ptr->pNext;
		return ptr;
	}
	return NULL;
}
//PostAccept�����������׽�����Ͷ��AcceptI/0
//����ʵ�ַ�ʽ������ͬ�����������û�����I/0���ͣ�
//�ٵ��ö�Ӧ���ص�API����Ͷ���ص�I/O���
//��������׽����ϵ��ص�I/O����
BOOL CIOCPServer::PostRecv(CIOCPContext *pContext,CIOCPBuffer *pBuffer)
{//����I/0����
	pBuffer->nOperation=OP_READ;
	::EnterCriticalSection(&pContext->Lock);
	pBuffer->nSequenceNumber=pContext->nReadSequence;//�������к�
    //Ͷ�ݴ��ص�I/O
	DWORD dwBytes;
	DWORD dwFlags=0;
	WSABUF buf;
	buf.buf=pBuffer->buff;
	buf.len=pBuffer->nLen;
	if(::WSARecv(pContext->s,&buf,1,&dwBytes,&dwFlags,&pBuffer->ol,NULL)!=NO_ERROR)
	{
		if(::WSAGetLastError()!=WSA_IO_PENDING)
		{
			::LeaveCriticalSection(&pContext->Lock);
			return FALSE;
		}
	}
	//�����׽����ϵ��ص�I/O�����Ͷ����кż���
	pContext->nOutStandingRecv++;
	pContext->nReadSequence++;
	::LeaveCriticalSection(&pContext->Lock);
	return TRUE;
}
////PostSend�����������׽�����Ͷ��SendI/0
BOOL CIOCPServer::PostSend(CIOCPContext *pContext,CIOCPBuffer *pBuffer)
{//����I/O����
	pBuffer->nOperation=OP_WRITE;
	::EnterCriticalSection(&pContext->Lock);
	//pBuffer->nSequenceNumber=pContext->nReadSequence;//�������к�
    //Ͷ�ݴ��ص�I/O
    DWORD dwBytes;
	DWORD dwFlags=0;
	WSABUF buf;
	buf.buf=pBuffer->buff;
	buf.len=pBuffer->nLen;
	if(WSASend(pContext->s,&buf,sizeof(BUFFER_SIZE),&dwBytes,dwFlags,&pBuffer->ol,NULL)!=NO_ERROR)
	{
		if(::WSAGetLastError()!=WSA_IO_PENDING)
		{
			::LeaveCriticalSection(&pContext->Lock);
			return FALSE;
		}
	}
	pContext->nOutStandingSend++;
	pContext->pOutOfOrderReads++;
	::LeaveCriticalSection(&pContext->Lock);
	return TRUE;
}
//PostSend�����������׽�����Ͷ��RecvI/0
BOOL CIOCPServer::PostAccept(CIOCPBuffer *pBuffer)
{ //����I/O����
	pBuffer->nOperation=OP_ACCEPT;
	pBuffer->nSequenceNumber;
    
	return FALSE;
}
//�������񣬳�ʼ��״̬���������������׽��֣����س���ʹ�õ���չAPI����
//������ɶ˿ڶ��󣬽��������׽��ֺ���ɶ˿ڶ����Ĺ�����Ȼ��Ϊ�����׽���ע��FD_ACCEPT
//�¼����Ա㵱Ͷ�ݵ�AcceptExI/oʱ���Եõ�֪ͨ����󴴽������߳�
BOOL CIOCPServer::Start(int nPort,int nMaxConnections,int nMaxFreeBuffers,
						int nMaxFreeContexts,int nInitialReads)
{//�������Ƿ��Ѿ�����
	if(m_bServerStarted)
		return FALSE;
	//�����û�����
	m_nPort=nPort;
	m_nMaxConnections=nMaxConnections;
	m_nMaxFreeBuffers=nMaxFreeBuffers;
    m_nMaxFreeContexts=nMaxFreeContexts;
	m_nInitialReads=nInitialReads;
	//��ʼ��״̬����
	m_bShutDown=FALSE;
	m_bServerStarted=TRUE;
	//���������׽��֣��󶨵����ض˿ڣ��������ģʽ
	m_sListen=::WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
    SOCKADDR_IN si;
    si.sin_family=AF_INET;
	si.sin_port=::ntohs(m_nPort);
	si.sin_addr.S_un.S_addr=INADDR_ANY;
	if(::bind(m_sListen,(sockaddr *)&si,sizeof(si))==SOCKET_ERROR)
	{
		m_bServerStarted=FALSE;
		return FALSE;
	}
	::listen(m_sListen,200);
	//������ɶ˿ڶ���
	m_hCompletion=::CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
	//������չ����AcceptEx
	GUID GuidAcceptEx=WSAID_ACCEPTEX;
	DWORD dwBytes;
	::WSAIoctl(m_sListen,SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx,
		sizeof(GuidAcceptEx),
		&m_lpfnAcceptEx,
		sizeof(m_lpfnAcceptEx),
		&dwBytes,
		NULL,
		NULL);
	//������չ����GetAcceptExSockaddrs
	GUID GuidGetAcceptExSockaddrs=WSAID_GETACCEPTEXSOCKADDRS;
	::WSAIoctl(m_sListen,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidGetAcceptExSockaddrs,
        sizeof(GuidGetAcceptExSockaddrs),
		&m_lpfnGetAcceptExSockaddrs,
		sizeof(m_lpfnGetAcceptExSockaddrs),
		&dwBytes,
		NULL,
		NULL);
	//�������׽��ֹ�������ɶ˿ڣ�ע�⣬����Ϊ�����ݵ�CompletionKeyΪ0
	::CreateIoCompletionPort((HANDLE)m_sListen,m_hCompletion,(DWORD)0,0);
    //ע��FD_ACCEPT�¼�
	//������ݵ�AcceptExI/O�������̻߳���ܵ�FD_accept�����¼�˵����Ͷ�ݸ����
	//AcceptExI/O
	WSAEventSelect(m_sListen,m_hAcceptEvent,FD_ACCEPT);
	//�����������߳�
	m_hListenThread=::CreateThread(NULL,0,_ListenThreadProc,this,0,NULL);
	return TRUE;
}
DWORD WINAPI CIOCPServer::_ListenThreadProc(LPVOID lpParam)
{
	CIOCPServer*pThis=(CIOCPServer *)lpParam;
	//���ڼ����׽�����Ͷ�ݼ���Accept I/O
	CIOCPBuffer *pBuffer;
	int i=0;
	for(i=0;i<pThis->m_nInitialAccepts;i++)
	{
		pBuffer=pThis->AllocateBuffer(BUFFER_SIZE);
		if(pBuffer==NULL)
			return -1;
        pThis->InsertPendingAccept(pBuffer);
		pThis->PostAccept(pBuffer);
	}
	//�����¼��������飬�Ա����������WSAWaitForMultipleevents����
	HANDLE hWaitEvents[2+MAX_THREAD];
	int nEventCount=0;
	hWaitEvents[nEventCount++]=pThis->m_hAcceptEvent;
	hWaitEvents[nEventCount++]=pThis->m_hRepostEvent;
	//����ָ�������Ĺ����߳�����ɶ˿��ϴ���I/O
	for(i=0;i<MAX_THREAD;i++)
	{
		hWaitEvents[nEventCount++]=::CreateThread(NULL,0,_WorkerThreadProc,pThis,0,NULL);
	}
	//�����������ѭ���������¼����������е��¼�
	while(true)
	{
		int nIndex=::WSAWaitForMultipleEvents(nEventCount,hWaitEvents,FALSE,60*1000,FALSE);
		//���ȼ���Ƿ�Ҫֹͣ����
		if(pThis->m_bShutDown||nIndex==WSA_WAIT_FAILED)
		{//�ر���������
			pThis->CloseAllConnection();
			::Sleep(0);//��I/O�����߳�һ��ִ�еĻ���
			//�رռ����׽���
			::closesocket(pThis->m_sListen);
			pThis->m_sListen=INVALID_SOCKET;
			::Sleep(0);//��I/O�����߳�һ��ִ�еĻ���
			//֪ͨ����I/O�����߳��˳�
			int i=0;
			for(i=2;i<MAX_THREAD+2;i++)
			{
				::PostQueuedCompletionStatus(pThis->m_hCompletion,-1,0,NULL);
			}
			//�ȴ�I/O�����߳��˳�
			::WaitForMultipleObjects(MAX_THREAD,&hWaitEvents[2],TRUE,5*1000);
			for(i=2;i<MAX_THREAD+2;i++)
			{
				::CloseHandle(hWaitEvents[i]);
			}
			::CloseHandle(pThis->m_hCompletion);
			pThis->FreeBuffers();
			pThis->FreeContexts();
			::ExitThread(0);
		}
		//��ʱ�������δ���ص�AcceptExI/O�����ӽ���ʱ��
		if(nIndex==WSA_WAIT_TIMEOUT)
		{
			pBuffer=pThis->m_pPendingAccepts;
			while(pBuffer!=NULL)
			{
				int nSeconds;
				int nLen=sizeof(nSeconds);
				//ȡ�����ӽ�����ʱ��
				::getsockopt(pBuffer->sClient,SOL_SOCKET,SO_CONNECT_TIME,(char *)&nSeconds,&nLen);
				//������������ӿͻ��������ͳ�ʼ���ݣ���������ͻ��뿪
				if(nSeconds!=-1&&nSeconds>2*60)
				{
					closesocket(pBuffer->sClient);
					pBuffer->sClient=INVALID_SOCKET;
				}
				pBuffer=pBuffer->pNext;
			}
		}
		else
		{
			nIndex=nIndex-WAIT_OBJECT_0;
            WSANETWORKEVENTS ne;
			int nLimit=0;
			if(nIndex==0)//2)m_hAcceptEvent�¼��������ţ�˵��Ͷ�ݵ�Accept���󲻹�����Ҫ����
			{
				::WSAEnumNetworkEvents(pThis->m_sListen,hWaitEvents[nIndex],&ne);
				if(ne.lNetworkEvents&FD_ACCEPT)
				{
					nLimit=50;//���ӵĸ�����������Ϊ50��
				}
			}
			else if(nIndex==1)//3)m_hRepostEvent�¼��������ţ�˵������I/O���߳̽��յ��µĿͻ�
			{
				nLimit=InterlockedExchange(&pThis->m_nRepostCount,0);
			}
			else if(nIndex>1)//I/O�����߳��˳���˵�����������رշ�����
			{
				pThis->m_bShutDown=TRUE;
				continue;
			}
			//Ͷ��nLimit��AcceptExI/O����
			int i=0;
			while(i++<nLimit&&pThis->m_nPendingAcceptCount<pThis->m_nMaxAccepts)
			{
				pBuffer=pThis->AllocateBuffer(BUFFER_SIZE);
				if(pBuffer!=NULL)
				{
                    pThis->InsertPendingAccept(pBuffer);
					pThis->PostAccept(pBuffer);
				}
			}
		}
	}
	return 0;
}
//ֹͣ����������Shutdown�ܼ򵥣�����Ҫ������֪ͨ�����̣߳��ü����߳�����ֹͣ
void CIOCPServer::Shutdown()
{
     if(!m_bServerStarted)
		 return ;
	 //֪ͨ�����̣߳�����ֹͣ����
	 m_bShutDown=TRUE;
	 ::SetEvent(m_hAcceptEvent);
	 //�ȴ������߳��˳�
	 ::WaitForSingleObject(m_hListenThread,INFINITE);
	 ::CloseHandle(m_hListenThread);
	 m_hListenThread=NULL;
	 m_bServerStarted=FALSE;
}
//�ͻ�I/O�������ڹ����߳�_WorkerThreadProc�����_WorkerThread��Ҫ������ɶ˿���
//����GetQueuedCompletionStatus�����ȴ�I/0����ɲ��ҵ���HandleIO������I/O
DWORD WINAPI CIOCPServer::_WorkerThreadProc(LPVOID lpParam)
{
	
#ifdef _DEBUG
	::OutputDebugString("WorkerThread����...\n");
#endif _DEBUG
	CIOCPServer *pThis=(CIOCPServer *)lpParam;
	CIOCPBuffer *pBuffer;
	DWORD dwKey;
	DWORD dwTrans;
	LPOVERLAPPED lpol;
	while(true)
	{//�ڹ���������ɶ˿ڵ������׽����ϵȴ�I/O���
		BOOL bOK=::GetQueuedCompletionStatus(pThis->m_hCompletion,&dwTrans,(LPDWORD)&dwKey,
			(LPOVERLAPPED *)&lpol,WSA_INFINITE);
        if(dwTrans==-1)//�û�֪ͨ�˳�
		{
#ifdef _DEBUG
			::OutputDebugString("WorkThread�˳�\n");
#endif _DEBUG
			::ExitThread(0);
		}
		pBuffer=CONTAINING_RECORD(lpol,CIOCPBuffer,ol);
        int nError=NO_ERROR;
		if(!bOK)//�ٴ��׽������д�����
		{
			SOCKET s;
			if(pBuffer->nOperation==OP_ACCEPT)
			{
				s=pThis->m_sListen;
			}
			else
			{
				if(dwKey==0)
					break;
				s=((CIOCPContext*)dwKey)->s;
			}
			DWORD dwFlags=0;
			if(!::WSAGetOverlappedResult(s,&pBuffer->ol,&dwTrans,FALSE,&dwFlags))
			{
				nError=::WSAGetLastError();
			}
		}
		pThis->HandleIO(dwKey,pBuffer,dwTrans,nError);
	}
#ifdef _DEBUG
	::OutputDebugString("WorkerThread�˳�\n");
#endif _DEBUG
	return 0;
}
//�����HandleIO()��������ؼ��ģ���������ɵ�I/O����Ͷ���µ�I/O����,�ͷ�
//��ɵĻ���������͹رյĿͻ������ʶ���
void CIOCPServer::HandleIO(DWORD dwKey,CIOCPBuffer*pBuffer,DWORD dwtrans,int nError)
{
	CIOCPContext *pContext=(CIOCPContext *)dwKey;
#ifdef _DEBUG
	::OutputDebugString("HandleIO...\n");
#endif _DEBUG
	//1)���ȼ����׽����ϵ�δ��I/O����
	if(pContext!=NULL)
	{
		::EnterCriticalSection(&pContext->Lock);
		if(pBuffer->nOperation==OP_READ)
		{
			pContext->nOutStandingRecv--;
		}
		else if(pBuffer->nOperation==OP_WRITE)
			pContext->nOutStandingSend--;
		::LeaveCriticalSection(&pContext->Lock);
        //2)����׽����Ƿ��Ѿ������ǹر�
		if(pContext->bClosing)
		{
#ifdef _DEBUG
			::OutputDebugString("��鵽�׽����Ѿ������ǹر�\n");
#endif _DEBUG
			if(pContext->nOutStandingRecv==0&&pContext->nOutStandingSend==0)
			{
				ReleaseContext(pContext);
			}
			//�ͷ��ѹر��׽��ֵ�I/O
			ReleaseBuffer(pBuffer);
			return;
		}
	}
	else
	{
		RemovePendingAccept(pBuffer);
	}
	//3)����׽����Ϸ����Ĵ�������еĻ�֪ͨ�û���Ȼ��ر��׽���
    if(nError!=NO_ERROR)
	{
         if(pBuffer->nOperation!=OP_ACCEPT)
		 {
			 OnConnectionError(pContext,pBuffer,nError);
			 CloseAConnection(pContext);
			 if(pContext->nOutStandingRecv&&pContext->nOutStandingSend==0)
			 {
				 ReleaseContext(pContext);
			 }
#ifdef _DEBUF
			 ::OutputDebugString("��鵽�ͻ��׽����Ϸ����Ĵ���\n");
#endif _DEBUG
		 }
		 else    //�ڼ����׽����Ϸ�������Ҳ���Ǽ����׽��ִ���Ŀͻ�������
		 {
			 //�ͻ��˳����ͷ�I/O������
			 if(pBuffer->sClient!=INVALID_SOCKET)
			 {
				 ::closesocket(pBuffer->sClient);
				 pBuffer->sClient=INVALID_SOCKET;
			 }
#ifdef _DEBUG
			 ::OutputDebugString("��鵽�����׽����Ϸ�������\n");
#endif _DEBUG
		 }
		 ReleaseBuffer(pBuffer);
		 return;
	}
	//��ʼ����
	if(pBuffer->nOperation==OP_ACCEPT)
	{
		if(dwtrans==0)
		{
#ifdef _DEBUG
			::OutputDebugString("�����׽����ϵĿͻ��˹ر�\n");
#endif _DEBUG
            if(pBuffer->sClient!=INVALID_SOCKET)
			{
				::closesocket(pBuffer->sClient);
				pBuffer->sClient=INVALID_SOCKET;
			}
		}
		else
		{//Ϊ�½��ܵ���������ͻ������Ķ���
			CIOCPContext *pClient=AllocateContext(pBuffer->sClient);
			if(pClient!=NULL)
			{
				if(AddAConnection(pClient))
				{//ȡ�ÿͻ���ַ
                   int nLocalLen,nRemoteLen;
				   LPSOCKADDR pLocalAddr,pRemoteAddr;
                   m_lpfnGetAcceptExSockaddrs(
					   pBuffer->buff,
					   pBuffer->nLen-((sizeof(sockaddr_in)+16)*2),
					   sizeof(sockaddr_in)+16,
					   sizeof(sockaddr_in)+16,
					   (SOCKADDR **)&pLocalAddr,
					   &nLocalLen,
					   (SOCKADDR **)&pRemoteAddr,
					   &nRemoteLen);
				   memcpy(&pClient->addrLocal,pLocalAddr,nLocalLen);
				   memcpy(&pClient->addrRemote,pRemoteAddr,nRemoteLen);
				   //���������ӵ���ɶ˿ڶ���
				   ::CreateIoCompletionPort((HANDLE)pClient->s,m_hCompletion,(DWORD)pClient,0);
				   //֪ͨ�û�
				   pBuffer->nLen=dwtrans;
				   //��ͻ�
				   OnConnectionEstablished(pClient,pBuffer);
				   //��������Ͷ�ݼ���Read������Щ�ռ����׽��ֹرջ��߳���ʱ�ͷ�
				   for(int i=0;i<5;i++)
				   {
					   CIOCPBuffer *p=AllocateBuffer(BUFFER_SIZE);
					   if(p!=NULL)
					   {
						   if(!PostRecv(pClient,p))
						   {
							   CloseAConnection(pClient);
							   break;
						   }
					   }
				   }
				}
				else        //���������������ر�����
				{
					CloseAConnection(pClient);
					ReleaseContext(pClient);
				}
			}
			else
			{//��Դ���㣬�ر���ͻ������Ӽ���
				::closesocket(pBuffer->sClient);
				pBuffer->sClient=INVALID_SOCKET;
			}
		}
		//Accept������ɣ��ͷ�I/O������
		ReleaseBuffer(pBuffer);
		//֪ͨ�����̼߳�����Ͷ��һ��Accept����
		::InterlockedIncrement(&m_nRepostCount);
		::SetEvent(m_hRepostEvent);
	}
	else if(pBuffer->nOperation==OP_READ)
	{
		if(dwtrans==0)//�Է��ر��׽���
		{//��֪ͨ�û�
			pBuffer->nLen=0;
			OnConnectionClosing(pContext,pBuffer);
			//�ڹر�����
            CloseAConnection(pContext);
			//�ͷſͻ������ĺͻ���������
		    if(pContext->nOutStandingRecv==0&&pContext->nOutStandingSend==0)
			{
				ReleaseContext(pContext);
			}
            ReleaseBuffer(pBuffer);
			
		}
		else
		{//д������ɣ�֪ͨ�û�
			pBuffer->nLen=dwtrans;
			//����I/OͶ�ݵ�˳���ȡ���ܵ�������
			CIOCPBuffer *p=GetNextReadBuffer(pContext,pBuffer);
			while(p!=NULL)
			{
				OnReadCompleted(pContext,p);
				//����Ҫ��ȡ�����кŵ�ֵ
				::InterlockedIncrement((LONG*)&pContext->nCurrentReadSequence);
				//�ͷ��������ɵ�I/O
				ReleaseBuffer(p);
				p=GetNextReadBuffer(pContext,NULL);
			}
            //����Ͷ��һ���µĽ�������
			pBuffer=AllocateBuffer(BUFFER_SIZE);
			if(pBuffer==NULL||!PostRecv(pContext,pBuffer))
			{
				CloseAConnection(pContext);
			}
		}
	}
	else if(pBuffer->nOperation==OP_WRITE)
	{
		if(dwtrans==0)//�Է��ر��׽���
		{
			//��֪ͨ�û�
			pBuffer->nLen;
			OnConnectionClosing(pContext,pBuffer);
            //�ٹر�����
			CloseAConnection(pContext);
			//�ͷſͻ������ʺͻ���������
			if(pContext->nOutStandingRecv==0&&pContext->nOutStandingSend==0)
			{
                ReleaseContext(pContext);
			}
			ReleaseBuffer(pBuffer);
		}
		else
		{
			//д������ɣ�֪ͨ�û�
            pBuffer->nLen=dwtrans;
			OnWriteCompleted(pContext,pBuffer);
			//�ͷ�SendText��������Ļ�����
			ReleaseBuffer(pBuffer);
		}
	}
}
BOOL CIOCPServer::SendText(CIOCPContext *pContext,char *pszText,int nLen)
{
	CIOCPBuffer *pBuffer=AllocateBuffer(nLen);
	if(pBuffer!=NULL)
	{
		memcpy(pBuffer->buff,pszText,nLen);
		return PostSend(pContext,pBuffer);
	}
	return FALSE;
}
//����һ���µ�����
 void  CIOCPServer::OnConnectionEstablished(CIOCPContext *pContext,CIOCPBuffer*)
{

}

//һ�����ӹر�
void CIOCPServer::OnConnectionClosing(CIOCPContext *pContext,CIOCPBuffer*)
{
}

//��һ�������Ϸ�������
void CIOCPServer::OnConnectionError(CIOCPContext *pContext,CIOCPBuffer*,int nError)
{

}
//��һ�������ϵĶ��������
void CIOCPServer::OnReadCompleted(CIOCPContext *pContext,CIOCPBuffer*)
{

}
//��һ��������д�������
void CIOCPServer::OnWriteCompleted(CIOCPContext *pContext,CIOCPBuffer*)
{

}