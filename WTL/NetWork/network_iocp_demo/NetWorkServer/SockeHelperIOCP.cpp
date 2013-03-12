#include "StdAfx.h"
#include "SockeHelperIOCP.h"
#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib
#include <TraceUtil.h>


bool g_bExit = false;   //全局退出函数

//PER_IO_OPERATION_DATA 结构链表-----------------------
#define MAX_MEM_ITEM_COUNT 10000
//static CRITICAL_SECTION  g_csMem;    //申请内存临界区变量
//PVOID g_pAllMemory;                 //指向申请的大块内存首地址
//DWORD g_dwDataCount;                //总的已使用的数据的数量
//LPPERIODATA g_pstuIoDataUnUsed;     //已标记未使用的数据

//-----------------------------------------------------



CSockeHelperIOCP::CSockeHelperIOCP(void)
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if(::WSAStartup(sockVersion, &wsaData) != 0)
	{
		QNA::TRACE(_T("Socket2.0初始化失败，Exit!"));
		exit(0);
	}
	ZeroMemory(&m_stuThreadAfferent, sizeof(THREADAFFERENT));
	g_bExit = true;
}

CSockeHelperIOCP::~CSockeHelperIOCP(void)
{
	UnInit();

	g_bExit = false;
	ZeroMemory(&m_stuThreadAfferent, sizeof(THREADAFFERENT));
}

//初始化函数，开启完成端口
bool CSockeHelperIOCP::Init(int nPort)
{
	::InitializeCriticalSection(&m_stuThreadAfferent.csMem);
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
	

	//创建接受连接线程
	m_hListenThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_ListenThreadProc, (LPVOID)&m_stuThreadAfferent, 0, NULL);
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
	//关闭完成端口
	if(m_stuThreadAfferent.hIoPort)
	{
		CloseHandle(m_stuThreadAfferent.hIoPort);
	}

	//关闭监听线程
	if(m_hListenThreadHandle)
	{
		::TerminateThread(m_hListenThreadHandle, 1);
		CloseHandle(m_hListenThreadHandle);
	}

	//关闭监听socket
	if(m_stuThreadAfferent.sockListen!=INVALID_SOCKET)
	{
		closesocket(m_stuThreadAfferent.sockListen);
	}
	//关闭接收线程数组线程 释放空间
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

	::DeleteCriticalSection(&m_stuThreadAfferent.csMem);

	if(m_stuThreadAfferent.pAllMemory !=NULL)
	{
		free(m_stuThreadAfferent.pAllMemory);
		m_stuThreadAfferent.pAllMemory = NULL;
	}
}

//监听端口线程
DWORD WINAPI CSockeHelperIOCP::_ListenThreadProc(LPVOID lpParam)
{
	//客户端Socket信息
	SOCKADDR_IN clietnAddr;
	int len = sizeof(SOCKADDR_IN);

	::Sleep(500);
	QNA::TRACE(_T("CSockeHelperIOCP::_ListenThreadProc:监听线程 - 接受连接线程启动!!!\r\n"));
	//监听socket
	SOCKET sockListen = (SOCKET)lpParam;
	//下面进入无限循环，处理事件对象数组中的事件
	while(g_bExit)
	{
		try
		{
			//接收客户端的连接
			SOCKET clientSocket = accept(sockListen, (sockaddr *)&clietnAddr, &len);
			if(SOCKET_ERROR !=clientSocket)
			{
				char *pClientIp = inet_ntoa(clietnAddr.sin_addr);

				//将客户端Socket加上完成端口
				//if(NULL==CreateIoCompletionPort((HANDLE)clientSocket, m_hIoPort, (ULONG_PTR)clientSocket, 0))
				{//关联一个已打开的文件实例和新建的或已存在的I/0完成端口，或者创建一个未关联任何文件的I/O完成端口。
					QNA::TRACE(_T("CSockeHelperIOCP::_ListenThreadProc:关键套接字到完成端口上失败！\r\n"));
					continue;
				}
				//申请PPER_IO_OPERATION_DATA 结构内存块
				LPPERIODATA lpOperationData = NULL;
				////////::EnterCriticalSection(&csMem);
				////////if(pDataUnUsed !=NULL)
				////////{
				////////	lpOperationData=pDataUnUsed;
				////////	pDataUnUsed=pDataUnUsed->pNext;
				////////	dwDataCount--;
				////////	TracePrint(LOG_INFO, "AuthenCenterService 服务:成功接收到%s的连接,并成功分配内存[%d/%d]！\n\r",pClientIp,dwDataCount,MEM_ITEM_COUNT);

				////////}
				////////else
				////////{
				////////	lpOperationData=(LPPER_IO_OPERATION_DATA)malloc(sizeof(PER_IO_OPERATION_DATA));
				////////	if(NULL==lpOperationData)
				////////	{
				////////		TracePrint(LOG_INFO, "AuthenCenterService 服务:成功接收到%s的连接,但分配内存失败！\n\r",pClientIp);
				////////		closesocket(clientSocket);
				////////		::Sleep(500);
				////////		continue;
				////////	}
				////////	else
				////////	{
				////////		TracePrint(LOG_INFO, "AuthenCenterService 服务:成功接收到%s的连接,并成功分配内存！\n\r",pClientIp);
				////////	}

				////////}
				////////::LeaveCriticalSection(&csMem);

				////////::memset(lpOperationData,0,sizeof(PER_IO_OPERATION_DATA));
				////////lpOperationData->dataBuf.len =sizeof(MsgHead);
				////////lpOperationData->dataBuf.buf =lpOperationData->buffer;
				////////lpOperationData->dwNeed=sizeof(MsgHead);
				////////lpOperationData->dwRecvBytes=0;
				////////lpOperationData->dwCommand=0;//代表只接收包头
				////////strcpy_s(lpOperationData->szClientIP,32,pClientIp);

				//异步接收---------------------------------------------------------------------------------------------------------------------------------------------
				if(SOCKET_ERROR==WSARecv(clientSocket,&lpOperationData->dataBuf,1,&lpOperationData->BytesRecv,&lpOperationData->dwFlags,(LPWSAOVERLAPPED)lpOperationData,NULL))
				{
					if(WSAGetLastError()!=ERROR_IO_PENDING)
					{
						QNA::TRACE(_T("CSockeHelperIOCP::_ListenThreadProc:WSARecv接收出错！\r\n"));
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
			//接收连接失败
			QNA::TRACE(_T("CSockeHelperIOCP::_ListenThreadProc:接收客户端连接出错!!!\r\n"));
			::Sleep(500);
		}
	}
	return 0;
}


//客户I/O处理是在工作线程_WorkerThreadProc中完成_WorkerThread主要是在完成端口上
//调用GetQueuedCompletionStatus函数等待I/0的完成并且调用HandleIO来处理I/O
DWORD WINAPI CSockeHelperIOCP::_WorkerThreadProc(LPVOID lpParam)
{

#ifdef _DEBUG
	QNA::TRACE("WorkerThread启动...\n");
#endif _DEBUG
	//CIOCPServer *pThis=(CIOCPServer *)lpParam;
	//CIOCPBuffer *pBuffer;
	//DWORD dwKey;
	//DWORD dwTrans;
	//LPOVERLAPPED lpol;
	while(true)
	{//在关联到此完成端口的所有套接字上等待I/O完成
		//BOOL bOK=::GetQueuedCompletionStatus(pThis->m_hCompletion,&dwTrans,(LPDWORD)&dwKey,
		//	(LPOVERLAPPED *)&lpol,WSA_INFINITE);
		if(!g_bExit)//用户通知退出
		{
#ifdef _DEBUG
			QNA::TRACE("WorkThread退出\n");
#endif _DEBUG
			::ExitThread(0);
		}
		//pBuffer=CONTAINING_RECORD(lpol,CIOCPBuffer,ol);
		//int nError=NO_ERROR;
		//if(!bOK)//再此套接字上有错误发生
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
	QNA::TRACE("WorkerThread退出\n");
#endif _DEBUG
	return 0;
}





/*******************************************************************************************************************
内部调用函数 非公用 
*********************************************************************************************************************/
//申请申请大内存块
bool CSockeHelperIOCP::MemoryApply(int iCount)
{
	LPPERIODATA pFirstData = NULL;
	LPPERIODATA pCurrent = NULL;
	::EnterCriticalSection(&m_stuThreadAfferent.csMem);
	m_stuThreadAfferent.pAllMemory = malloc(iCount * sizeof(PER_IO_OPERATION_DATA));
	if(NULL == m_stuThreadAfferent.pAllMemory)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::MemoryApply 申请大内存块失败!\n"));
		return false;
	}

	ZeroMemory(m_stuThreadAfferent.pAllMemory, iCount * sizeof(PER_IO_OPERATION_DATA));
	pFirstData = (LPPERIODATA)m_stuThreadAfferent.pAllMemory;
	for(int i=0; i<iCount; i++)
	{
		pCurrent = NULL;
		pCurrent = pFirstData + i;
		pCurrent->pNext = m_stuThreadAfferent.pstuIoDataUnUsed;
		m_stuThreadAfferent.pstuIoDataUnUsed = pCurrent;		
		m_stuThreadAfferent.dwDataCount++;
	}
	::LeaveCriticalSection(&m_stuThreadAfferent.csMem);
	return true;
}


bool CSockeHelperIOCP::InitIocp(int nPort)
{
	//创建完成端口
	m_stuThreadAfferent.hIoPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(NULL == m_stuThreadAfferent.hIoPort)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::InitIocp:CreateIoCompletionPort failure!\n"));
		return false;
	}

	//创建监听Socket //绑定到端口 //在端口监听
	m_stuThreadAfferent.sockListen =WSASocket(AF_INET,SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_stuThreadAfferent.sockListen)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::InitIocp: create listen socket failure!\n"));
		return false;
	}

	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(nPort);
	localAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if(SOCKET_ERROR == bind(m_stuThreadAfferent.sockListen, (sockaddr *)&localAddr, sizeof(localAddr)))
	{
		QNA::TRACE(_T("CSockeHelperIOCP::InitIocp:  bind socket failure!\n"));
		return false;
	}
	if(SOCKET_ERROR == listen(m_stuThreadAfferent.sockListen, 300))
	{
		QNA::TRACE(_T("CSockeHelperIOCP::InitIocp:  listen socket failure!\n"));
		return false;
	}
	return true;
}


//创建接收数据线程
bool CSockeHelperIOCP::CreateWorkThread()
{
	//创建接收数据线程(按cpu*2个数)
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	m_iThreadCount = si.dwNumberOfProcessors * 2;   //cpu个数*2

	m_pszThreadHandle = (HANDLE*)malloc(sizeof(HANDLE) * m_iThreadCount);
	::ZeroMemory(m_pszThreadHandle, sizeof(HANDLE) * m_iThreadCount);
	if(NULL == m_pszThreadHandle)
	{
		QNA::TRACE(_T("CSockeHelperIOCP::CreateWorkThread malloc thread array failure!\n"));
		return false;
	}
	//创建接收数据处理线程
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