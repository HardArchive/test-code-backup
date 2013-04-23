#include "StdAfx.h"
#include "HandleFile.h"

//HMODULE hModuleKernel32 = NULL;
typedef BOOL WINAPI QUEUEUSERWORKITEM(LPTHREAD_START_ROUTINE, PVOID Context, ULONG Flags);
//extern QUEUEUSERWORKITEM* g_fpQueueUserWorkItem;
HMODULE hModuleKernel32 = NULL;
// Kernel32.dll模块句柄
QUEUEUSERWORKITEM* g_fpQueueUserWorkItem = NULL;
// QueueUserWorkItem函数指针
CHandleFile::CHandleFile(void)
{
	hModuleKernel32 = LoadLibrary("Kernel32");
	if(hModuleKernel32!=NULL)
	{
		g_fpQueueUserWorkItem = (QUEUEUSERWORKITEM*)GetProcAddress(hModuleKernel32, "QueueUserWorkItem");
	}
}

CHandleFile::~CHandleFile(void)
{
}


bool CHandleFile::Start(const PTCHAR ptSourcePath, const PTCHAR ptLinkPath, const int iInterval)
{
	m_bExit = true;
	m_clsDirectoryTraversalFile.InitEx(ptSourcePath, ptLinkPath, iInterval);
	//启动线程  开始遍历 
	//g_fpQueueUserWorkItem((LPTHREAD_START_ROUTINE)WorkerThreadProc, (LPVOID)this, WT_EXECUTELONGFUNCTION);
	//
	QueueUserWorkItem((LPTHREAD_START_ROUTINE)WorkerThreadProc, (LPVOID)this, WT_EXECUTELONGFUNCTION);
	return false;
}

bool CHandleFile::Stop()
{
	m_bExit = false;
	m_clsDirectoryTraversalFile.StopTraversal();

	return false;
}

UINT WINAPI CHandleFile::WorkerThreadProc(LPVOID lpThreadParameter)
{
	CHandleFile* pclsHandleFile = (CHandleFile*)lpThreadParameter;
	return pclsHandleFile->m_clsDirectoryTraversalFile.StartTraversal();	 
}
bool CHandleFile::HandleFile()
{
	
	//DirectoryTraversal(m_strLinkPath)
	return true;

}