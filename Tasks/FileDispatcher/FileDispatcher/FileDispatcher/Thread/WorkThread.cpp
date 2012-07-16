/*******************************************************************************
 * Copyright (C), 2011-2020, 李  照
 * FileName: WorkThread.cpp
 * Author: 李  照    Version: 1.0    Date: 2011-02-22
 * Description: 工作线程基类
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. 
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2011-02-22    1.0    build this moudle  
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

// 开始一个线程
int CWorkThread::Start(void* pParam/*=NULL*/)
{
	if(m_hThread!=NULL)
	{
		return -1;
	}

	m_pParam = pParam;

	// 开始线程
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



	// 如果创建线程失败，则返回错误信息
	if(m_hThread==NULL)
	{
		return errno;
	}

	return 0;
}

// 停止线程
void CWorkThread::Stop(bool bSameWithStartThread/*=true*/)
{
	DoStop();

	if (!bSameWithStartThread)
	{
		// 如果正在运行，继续等待
		while(m_bIsRunning)
		{
			Sleep(100);
		}
	}
}

// 终止线程
void CWorkThread::Terminate()
{
	if(m_hThread!=NULL)
	{
		if(TerminateThread(m_hThread,-1)==FALSE) return;
		m_hThread = NULL;
		m_nThreadID = 0;
	}
}

// 线程主过程
// 主线程中运行
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
