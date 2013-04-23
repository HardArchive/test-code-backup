// ThreadBase.cpp : 实现文件
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "ThreadBase.h"

CThreadBase::CThreadBase()
{
	m_bRun = false;
	m_bStop = false;
	m_hThread = NULL;
	m_hMutex = NULL;
	m_nThreadId = 0;
}

CThreadBase::~CThreadBase()
{
	//Xt：关闭线程
	CloseThread();

	if(this->m_hMutex)
	{
		CloseHandle(this->m_hMutex);
		this->m_hMutex = NULL;
	}	
}

void CThreadBase::CloseThread()
{
	m_bStop = true;
	Sleep(100);
	if(m_bRun)	TerminateThread(m_hThread,0);
	this->m_bRun  = false;
	this->m_bStop = false;
}

bool CThreadBase::OpenThread(LPVOID pParam)
{
	if(m_bRun)
	{
		//sprintf_s(CThreadBase::m_strError,sizeof(CThreadBase::m_strError),);
		CGlobalData::PrintLogInfo("CThreadBase","程序以运行!",0);
		return false;
	}
	m_hMutex = CreateMutexA(NULL,false,"CheckProtocol");
	if(!m_hMutex)
	{
		if(ERROR_ALREADY_EXISTS != GetLastError())
		{
			CGlobalData::PrintLogInfo("CThreadBase","创建互斥体错误!",0); 
			return false;
		}
	}
	m_ThreadParam = pParam;
	m_hThread = CreateThread(NULL,0,&ThreadProc,this,0,&m_nThreadId);
 	if(NULL == m_hThread)	return false;
	Sleep(100);
 	CloseHandle(m_hThread);
	return true;
}

void CThreadBase::ReturnThreadInfo()
{
	
}

DWORD WINAPI CThreadBase::ThreadProc(LPVOID lpParameter)
{
	CThreadBase *pThis = (CThreadBase *)lpParameter;
	pThis->m_bRun = true;
	if(!pThis->Initialization())
		return false;
	pThis->WorkThread(pThis->m_ThreadParam);
	pThis->m_bRun = false;
	return 1;
}


