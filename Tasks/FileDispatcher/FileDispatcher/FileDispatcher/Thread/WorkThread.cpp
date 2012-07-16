/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: WorkThread.cpp
 * Author: ��  ��    Version: 1.0    Date: 2011-02-22
 * Description: �����̻߳���
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. 
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2011-02-22    1.0    build this moudle  
 ******************************************************************************/
#include "stdafx.h"
#include ".\WorkThread.h"

CWorkThread::CWorkThread(): m_hThread(NULL)
, m_nThreadID(0)
, m_bIsRunning(false)
, m_bQuit(true)
, m_pParam(NULL)
{

}

CWorkThread::~CWorkThread()
{
	Terminate();
}

// ��ʼһ���߳�
int CWorkThread::Start(void* pParam/*=NULL*/)
{
	if(m_hThread!=NULL)
	{
		return -1;
	}

	m_pParam = pParam;

	// ��ʼ�߳�
	DWORD dwFlag=0;
#ifdef C_RUNTIME_THREAD
	m_hThread=(void*)_beginthreadex(NULL,0,(unsigned(__stdcall*)(void*))MainThreadProc,this,dwFlag,&m_nThreadID);
#else
	m_hThread = CreateThread( 
		NULL,						// default security attributes 
		0,							// use default stack size  
		MainThreadProc,				// thread function 
		this,						// argument to thread function 
		dwFlag,						// use default creation flags 
		(LPDWORD)&m_nThreadID);		// returns the thread identifier 
#endif



	// ��������߳�ʧ�ܣ��򷵻ش�����Ϣ
	if(m_hThread==NULL)
	{
		return errno;
	}

	return 0;
}

// ֹͣ�߳�
void CWorkThread::Stop(bool bSameWithStartThread/*=true*/)
{
	DoStop();

	if (!bSameWithStartThread)
	{
		// ����������У������ȴ�
		while(m_bIsRunning)
		{
			Sleep(100);
		}
	}
}

// ��ֹ�߳�
void CWorkThread::Terminate()
{
	if(m_hThread!=NULL)
	{
		if(TerminateThread(m_hThread,-1)==FALSE) return;
		m_hThread = NULL;
		m_nThreadID = 0;
	}
}

// �߳�������
// ���߳�������
#ifdef C_RUNTIME_THREAD
int CWorkThread::MainThreadProc( CWorkThread *pThread )
{
	pThread->InitInstance();

	pThread->m_bQuit = false;
	pThread->m_bIsRunning = true;
	int nExitCode=pThread->DoWork();
	pThread->m_bIsRunning = false;

	CloseHandle(pThread->m_hThread);
	pThread->m_hThread = NULL;
	pThread->m_nThreadID = 0;

	pThread->ExitInstance();

	return nExitCode;
}
#else
DWORD CWorkThread::MainThreadProc( LPVOID lpParam ) 
{
	CWorkThread *pThread = (CWorkThread *)lpParam;
	pThread->InitInstance();

	pThread->m_bQuit = false;
	pThread->m_bIsRunning = true;
	int nExitCode=pThread->DoWork();
	pThread->m_bIsRunning = false;

	CloseHandle(pThread->m_hThread);
	pThread->m_hThread = NULL;
	pThread->m_nThreadID = 0;

	pThread->ExitInstance();

	return nExitCode;
}
#endif
