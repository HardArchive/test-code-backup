#include "StdAfx.h"
#include "SockeHelperIOCP.h"
#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib
#include <TraceUtil.h>


bool g_bExit = false;   //ȫ���˳�����

//PER_IO_OPERATION_DATA �ṹ����-----------------------
#define MAX_MEM_ITEM_COUNT 10000
static CRITICAL_SECTION  g_csMem;    //�����ڴ��ٽ�������
//-----------------------------------------------------



CSockeHelperIOCP::CSockeHelperIOCP(void)
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if(::WSAStartup(sockVersion, &wsaData) != 0)
	{
		QNA::TRACE(_T("Socket2.0��ʼ��ʧ�ܣ�Exit!"));
		exit(0);
	}
	m_hIoPort = NULL;
	m_dwDataCount = 0;
	m_pAllMemory = NULL;
	m_pstuIoDataUnUsed = NULL;	
	g_bExit = true;
}

CSockeHelperIOCP::~CSockeHelperIOCP(void)
{
	UnInit();

	g_bExit = false;
	m_hIoPort = NULL;
	m_dwDataCount = 0;
	m_pAllMemory = NULL;
	m_pstuIoDataUnUsed = NULL;	
}

//��ʼ��������������ɶ˿�
bool CSockeHelperIOCP::Init(int nPort)
{
	::InitializeCriticalSection(&g_csMem);
	if (!MemoryApply(MAX_MEM_ITEM_COUNT))
	{
		QNA::TRACE(_T("CSockeHelperIOCP::Init: MemoryApply failure!\n"));
		return false;
	}

	if (!InitIocp(nPort))
	{
		QNA::TRACE(_T("CSockeHelperIOCP::Init: InitIocp failure!\n"));
		return false;
	}
	

	//�������������߳�
	m_hListenThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_ListenThreadProc, (LPVOID)m_sockListen, 0, NULL);
	if(!m_hListenThreadHandle)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::Init: Create listen thread failure!\n"));
		return false;
	}

	if (!CreateWorkThread())
	{
		QNA::TRACE(_T("CSockeHelperIOCP::Init: CreateWorkThread failure!\n"));
		return false;
	}
	
	return true;
}

void CSockeHelperIOCP::UnInit()
{
	//�ر���ɶ˿�
	if(m_hIoPort)
	{
		CloseHandle(m_hIoPort);
	}

	//�رռ����߳�
	if(m_hListenThreadHandle)
	{
		::TerminateThread(m_hListenThreadHandle, 1);
		CloseHandle(m_hListenThreadHandle);
	}

	//�رռ���socket
	if(m_sockListen!=INVALID_SOCKET)
	{
		closesocket(m_sockListen);
	}
	//�رս����߳������߳� �ͷſռ�
	if(m_pszThreadHandle)
	{
		for(int i=0; i<m_iThreadCount; i++)
		{
			if(m_pszThreadHandle[i])
			{
				::TerminateThread(m_pszThreadHandle[i], 1);
				CloseHandle(m_pszThreadHandle[i]);
			}
		}
		free(m_pszThreadHandle);
	}

	::DeleteCriticalSection(&g_csMem);

	if(m_pAllMemory !=NULL)
	{
		free(m_pAllMemory);
		m_pAllMemory = NULL;
	}
}

//�����˿��߳�
DWORD WINAPI CSockeHelperIOCP::_ListenThreadProc(LPVOID lpParam)
{
	//�ͻ���Socket��Ϣ
	SOCKADDR_IN clietnAddr;
	int len = sizeof(SOCKADDR_IN);

	::Sleep(500);
	QNA::TRACE(_T("CSockeHelperIOCP::_ListenThreadProc:�����߳� - ���������߳�����!!!\r\n"));
	//����socket
	SOCKET sockListen = (SOCKET)lpParam;
	//�����������ѭ���������¼����������е��¼�
	while(g_bExit)
	{
		try
		{
			//���տͻ��˵�����
			SOCKET clientSocket = accept(sockListen, (sockaddr *)&clietnAddr, &len);
			if(SOCKET_ERROR !=clientSocket)
			{
				char *pClientIp = inet_ntoa(clietnAddr.sin_addr);

				//���ͻ���Socket������ɶ˿�
				//////////if(NULL==CreateIoCompletionPort((HANDLE)clientSocket, m_hIoPort, (ULONG_PTR)clientSocket, 0))
				//////////{//����һ���Ѵ򿪵��ļ�ʵ�����½��Ļ��Ѵ��ڵ�I/0��ɶ˿ڣ����ߴ���һ��δ�����κ��ļ���I/O��ɶ˿ڡ�
				//////////	QNA::TRACE(_T("CSockeHelperIOCP::_ListenThreadProc:�ؼ��׽��ֵ���ɶ˿���ʧ�ܣ�\r\n"));
				//////////	continue;
				//////////}
				//����PPER_IO_OPERATION_DATA �ṹ�ڴ��
				LPPERIODATA lpOperationData = NULL;
				////////::EnterCriticalSection(&csMem);
				////////if(pDataUnUsed !=NULL)
				////////{
				////////	lpOperationData=pDataUnUsed;
				////////	pDataUnUsed=pDataUnUsed->pNext;
				////////	dwDataCount--;
				////////	TracePrint(LOG_INFO, "AuthenCenterService ����:�ɹ����յ�%s������,���ɹ������ڴ�[%d/%d]��\n\r",pClientIp,dwDataCount,MEM_ITEM_COUNT);

				////////}
				////////else
				////////{
				////////	lpOperationData=(LPPER_IO_OPERATION_DATA)malloc(sizeof(PER_IO_OPERATION_DATA));
				////////	if(NULL==lpOperationData)
				////////	{
				////////		TracePrint(LOG_INFO, "AuthenCenterService ����:�ɹ����յ�%s������,�������ڴ�ʧ�ܣ�\n\r",pClientIp);
				////////		closesocket(clientSocket);
				////////		::Sleep(500);
				////////		continue;
				////////	}
				////////	else
				////////	{
				////////		TracePrint(LOG_INFO, "AuthenCenterService ����:�ɹ����յ�%s������,���ɹ������ڴ棡\n\r",pClientIp);
				////////	}

				////////}
				////////::LeaveCriticalSection(&csMem);

				////////::memset(lpOperationData,0,sizeof(PER_IO_OPERATION_DATA));
				////////lpOperationData->dataBuf.len =sizeof(MsgHead);
				////////lpOperationData->dataBuf.buf =lpOperationData->buffer;
				////////lpOperationData->dwNeed=sizeof(MsgHead);
				////////lpOperationData->dwRecvBytes=0;
				////////lpOperationData->dwCommand=0;//����ֻ���հ�ͷ
				////////strcpy_s(lpOperationData->szClientIP,32,pClientIp);

				//�첽����---------------------------------------------------------------------------------------------------------------------------------------------
				if(SOCKET_ERROR==WSARecv(clientSocket,&lpOperationData->dataBuf,1,&lpOperationData->BytesRecv,&lpOperationData->dwFlags,(LPWSAOVERLAPPED)lpOperationData,NULL))
				{
					if(WSAGetLastError()!=ERROR_IO_PENDING)
					{
						QNA::TRACE(_T("CSockeHelperIOCP::_ListenThreadProc:WSARecv���ճ���\r\n"));
						//CloseClient(clientSocket,lpOperationData);
					}
				}
				//------------------------------------------------------------------------------------------------------------------------------------------------------
			}
			else
			{
				::Sleep(500);
			}
		}
		catch(...)
		{
			//��������ʧ��
			QNA::TRACE(_T("CSockeHelperIOCP::_ListenThreadProc:���տͻ������ӳ���!!!\r\n"));
			::Sleep(500);
		}
	}
	return 0;
}


//�ͻ�I/O�������ڹ����߳�_WorkerThreadProc�����_WorkerThread��Ҫ������ɶ˿���
//����GetQueuedCompletionStatus�����ȴ�I/0����ɲ��ҵ���HandleIO������I/O
DWORD WINAPI CSockeHelperIOCP::_WorkerThreadProc(LPVOID lpParam)
{

#ifdef _DEBUG
	QNA::TRACE("WorkerThread����...\n");
#endif _DEBUG
	//CIOCPServer *pThis=(CIOCPServer *)lpParam;
	//CIOCPBuffer *pBuffer;
	//DWORD dwKey;
	//DWORD dwTrans;
	//LPOVERLAPPED lpol;
	while(true)
	{//�ڹ���������ɶ˿ڵ������׽����ϵȴ�I/O���
		//BOOL bOK=::GetQueuedCompletionStatus(pThis->m_hCompletion,&dwTrans,(LPDWORD)&dwKey,
		//	(LPOVERLAPPED *)&lpol,WSA_INFINITE);
		if(!g_bExit)//�û�֪ͨ�˳�
		{
#ifdef _DEBUG
			QNA::TRACE("WorkThread�˳�\n");
#endif _DEBUG
			::ExitThread(0);
		}
		//pBuffer=CONTAINING_RECORD(lpol,CIOCPBuffer,ol);
		//int nError=NO_ERROR;
		//if(!bOK)//�ٴ��׽������д�����
		//{
		//	SOCKET s;
		//	if(pBuffer->nOperation==OP_ACCEPT)
		//	{
		//		s=pThis->m_sListen;
		//	}
		//	else
		//	{
		//		if(dwKey==0)
		//			break;
		//		s=((CIOCPContext*)dwKey)->s;
		//	}
		//	DWORD dwFlags=0;
		//	if(!::WSAGetOverlappedResult(s,&pBuffer->ol,&dwTrans,false,&dwFlags))
		//	{
		//		nError=::WSAGetLastError();
		//	}
		//}
		//pThis->HandleIO(dwKey,pBuffer,dwTrans,nError);
	}
#ifdef _DEBUG
	QNA::TRACE("WorkerThread�˳�\n");
#endif _DEBUG
	return 0;
}





/*******************************************************************************************************************
�ڲ����ú��� �ǹ��� 
*********************************************************************************************************************/
//����������ڴ��
bool CSockeHelperIOCP::MemoryApply(int iCount)
{
	LPPERIODATA pFirstData = NULL;
	LPPERIODATA pCurrent = NULL;
	::EnterCriticalSection(&g_csMem);
	m_pAllMemory = malloc(iCount * sizeof(PER_IO_OPERATION_DATA));
	if(NULL == m_pAllMemory)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::MemoryApply ������ڴ��ʧ��!\n"));
		return false;
	}

	ZeroMemory(m_pAllMemory, iCount * sizeof(PER_IO_OPERATION_DATA));
	pFirstData = (LPPERIODATA)m_pAllMemory;
	for(int i=0; i<iCount; i++)
	{
		pCurrent = NULL;
		pCurrent = pFirstData + i;
		pCurrent->pNext = m_pstuIoDataUnUsed;
		m_pstuIoDataUnUsed = pCurrent;		
		m_dwDataCount++;
	}
	::LeaveCriticalSection(&g_csMem);
	return true;
}


bool CSockeHelperIOCP::InitIocp(int nPort)
{
	//������ɶ˿�
	m_hIoPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(NULL == m_hIoPort)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::InitIocp:CreateIoCompletionPort failure!\n"));
		return false;
	}

	//��������Socket //�󶨵��˿� //�ڶ˿ڼ���
	m_sockListen =WSASocket(AF_INET,SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_sockListen)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::InitIocp: create listen socket failure!\n"));
		return false;
	}

	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(nPort);
	localAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if(SOCKET_ERROR == bind(m_sockListen, (sockaddr *)&localAddr, sizeof(localAddr)))
	{
		QNA::TRACE(_T("CSockeHelperIOCP::InitIocp:  bind socket failure!\n"));
		return false;
	}
	if(SOCKET_ERROR == listen(m_sockListen, 300))
	{
		QNA::TRACE(_T("CSockeHelperIOCP::InitIocp:  listen socket failure!\n"));
		return false;
	}
	return true;
}


//�������������߳�
bool CSockeHelperIOCP::CreateWorkThread()
{
	//�������������߳�(��cpu*2����)
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	m_iThreadCount = si.dwNumberOfProcessors * 2;   //cpu����*2

	m_pszThreadHandle = (HANDLE*)malloc(sizeof(HANDLE) * m_iThreadCount);
	::ZeroMemory(m_pszThreadHandle, sizeof(HANDLE) * m_iThreadCount);
	if(NULL == m_pszThreadHandle)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::CreateWorkThread malloc thread array failure!\n"));
		return false;
	}
	//�����������ݴ����߳�
	for(int i=0; i<m_iThreadCount; i++)
	{
		m_pszThreadHandle[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_WorkerThreadProc, NULL, 0, NULL);
		if(!m_pszThreadHandle[i])
		{
			QNA::TRACE(_T("CSockeHelperIOCP::CreateWorkThread create receive thread failure!\n"));
			return false;
		}
	}
	return true;
}