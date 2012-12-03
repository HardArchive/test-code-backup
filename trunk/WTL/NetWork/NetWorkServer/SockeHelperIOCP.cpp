#include "StdAfx.h"
#include "SockeHelperIOCP.h"
bool g_bExit = false;   //全局退出函数

CSockeHelperIOCP::CSockeHelperIOCP(void)
{
	g_bExit = true;
}

CSockeHelperIOCP::~CSockeHelperIOCP(void)
{
	g_bExit = false;
}

//监听端口线程
DWORD WINAPI CSockeHelperIOCP::_ListenThreadProc(LPVOID lpParam)
{

	//下面进入无限循环，处理事件对象数组中的事件
	while(true)
	{
	}
	return 0;
}


//客户I/O处理是在工作线程_WorkerThreadProc中完成_WorkerThread主要是在完成端口上
//调用GetQueuedCompletionStatus函数等待I/0的完成并且调用HandleIO来处理I/O
DWORD WINAPI CSockeHelperIOCP::_WorkerThreadProc(LPVOID lpParam)
{

#ifdef _DEBUG
	::OutputDebugString("WorkerThread启动...\n");
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
			::OutputDebugString("WorkThread退出\n");
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
		//	if(!::WSAGetOverlappedResult(s,&pBuffer->ol,&dwTrans,FALSE,&dwFlags))
		//	{
		//		nError=::WSAGetLastError();
		//	}
		//}
		//pThis->HandleIO(dwKey,pBuffer,dwTrans,nError);
	}
#ifdef _DEBUG
	::OutputDebugString("WorkerThread退出\n");
#endif _DEBUG
	return 0;
}