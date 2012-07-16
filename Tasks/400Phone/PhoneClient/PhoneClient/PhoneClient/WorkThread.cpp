#include "stdafx.h"
#include "WorkThread.h"

CWorkThread::CWorkThread()
{
	m_hThread = NULL;
	m_nThreadID = 0;
	m_bIsRunning = false;
	m_bQuit = true;
	m_pParam = NULL;
}

CWorkThread::CWorkThread(void* pParam )
{
	CWorkThread();
	m_pParam = pParam;
}
CWorkThread::~CWorkThread()
{
	Terminate();
}

// ��ʼһ���߳�
int CWorkThread::Start()
{
	if(m_hThread!=NULL)
	{
		return -1;
	}

	// ��ʼ�߳�
	DWORD dwFlag=0;
	m_hThread = CreateThread( 
		NULL,						// default security attributes 
		0,							// use default stack size  
		MainThreadProc,				// thread function 
		this,						// argument to thread function 
		dwFlag,						// use default creation flags 
		(LPDWORD)&m_nThreadID);		// returns the thread identifier 

	// ��������߳�ʧ�ܣ��򷵻ش�����Ϣ
	if(m_hThread==NULL)
	{
		return ::GetLastError();
	}

	return 0;
}

// ֹͣ�߳�
void CWorkThread::Stop()
{
	this->m_bQuit=TRUE;
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
DWORD CWorkThread::MainThreadProc( LPVOID lpParam ) 
{
	CWorkThread *pThread = (CWorkThread *)lpParam;
	pThread->InitInstance();

	pThread->m_bQuit = false;
	pThread->m_bIsRunning = true;
	int nExitCode = pThread->DoWork();
	pThread->m_bIsRunning = false;

	CloseHandle(pThread->m_hThread);
	pThread->m_hThread = NULL;
	pThread->m_nThreadID = 0;

	pThread->ExitInstance();

	return nExitCode;
}
