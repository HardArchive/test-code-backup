#include "StdAfx.h"
#include "SockeHelperIOCP.h"
bool g_bExit = false;   //ȫ���˳�����

CSockeHelperIOCP::CSockeHelperIOCP(void)
{
	g_bExit = true;
}

CSockeHelperIOCP::~CSockeHelperIOCP(void)
{
	g_bExit = false;
}

//�����˿��߳�
DWORD WINAPI CSockeHelperIOCP::_ListenThreadProc(LPVOID lpParam)
{

	//�����������ѭ���������¼����������е��¼�
	while(true)
	{
	}
	return 0;
}


//�ͻ�I/O�������ڹ����߳�_WorkerThreadProc�����_WorkerThread��Ҫ������ɶ˿���
//����GetQueuedCompletionStatus�����ȴ�I/0����ɲ��ҵ���HandleIO������I/O
DWORD WINAPI CSockeHelperIOCP::_WorkerThreadProc(LPVOID lpParam)
{

#ifdef _DEBUG
	::OutputDebugString("WorkerThread����...\n");
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
			::OutputDebugString("WorkThread�˳�\n");
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
		//	if(!::WSAGetOverlappedResult(s,&pBuffer->ol,&dwTrans,FALSE,&dwFlags))
		//	{
		//		nError=::WSAGetLastError();
		//	}
		//}
		//pThis->HandleIO(dwKey,pBuffer,dwTrans,nError);
	}
#ifdef _DEBUG
	::OutputDebugString("WorkerThread�˳�\n");
#endif _DEBUG
	return 0;
}