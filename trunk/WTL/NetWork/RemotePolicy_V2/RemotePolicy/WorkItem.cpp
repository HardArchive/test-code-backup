#pragma once
#include "stdafx.h"

#include "WorkItem.h"
#include "RemotePolicy.h"
#include "Communication.h"
//#include "WorkProc.h"
//#include "PoolWork.h"

namespace WorkItem
{
	
	DWORD g_nEventTime;				// 通信Socket事件的等待时间
	//全局线程工作数组
	CArray<THREADDATA,THREADDATA&> g_arrayWorking;

	int g_nJobMaxNum;				// 线程池最大工作线程数
//	int g_nJobGuardNum;				// 线程池繁忙的警戒数目
//	int g_nAutoDisconnectNum;		// 当到达警戒数目之后，前g_nAutoDisconnectNum项的线程主动关闭连接
	bool g_AllExit = false;

	DWORD WINAPI ClientWorkItemThread(LPVOID lpParameter)
	{
		return ClientWorkFunction(lpParameter);
	}

	// Windows NT 使用
	UINT ClientWorkThread(LPVOID lpParameter)
	{
		return ClientWorkFunction(lpParameter);
	}

	//Client处理函数
	DWORD ClientWorkFunction(LPVOID lpParameter)
	{
		THREADDATA ThreadData;
		SOCKET WorkSocket = (SOCKET)lpParameter;
		try
		{
			if(WorkSocket == 0)
			{
				closesocket(WorkSocket);
				return DWORD(-1);
			}
			
			//Add this thread item to therad pool.将此线程项加入线程池
			ThreadData.dwThreadID = GetCurrentThreadId();
			ThreadData.socket = WorkSocket;
			AddItemRef(ThreadData);

			char szbuf[1460] = {0};
			int iLen = 1460;
			Communicate::CCommunication com(WorkSocket,10000,szbuf,iLen);
			if (!com.PendingReceive(10))
			{
				TRACE("recv err!\n");
				com.CloseSocket();
			}

//com.SetBuffer();
//com.GetBuffer()
			strncpy(szbuf,"你好",sizeof(szbuf) - 1);
			if (!com.PendingSend(strlen(szbuf)))
				TRACE("send err!\n");

/*			CPoolWork workproc(WorkSocket);
			if(!workproc.Communicate())
			{
				TRACE(_T("ClientWorkFunction() >> Communicate() failed\n"));
			}

			if(!workproc.SendEndTalk())
			{
				TRACE(_T("ClientWorkFunction() >> SendEndTalk() failed\n"));
			}

*/			closesocket(WorkSocket);

			ReleaseItem(ThreadData);		
			return 0;
		}
		catch(...)
		{
			ReleaseItem(ThreadData);
			closesocket(WorkSocket);
			return DWORD(-1);
		}
	}

	// 初始化线程池工作参数和工作项队列，CPoolThread::InitInstance()处调用
	bool InitPool()
	{
		bool bReturn = false;
		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		WaitForSingleObject(hPoolMutex, INFINITE);

		g_arrayWorking.RemoveAll();
		CRemotePolicyApp* pApp = (CRemotePolicyApp*)AfxGetApp();
		if(pApp != NULL)
		{
			if(pApp->m_pWorkThread != NULL)
			{
				//暂定满线程为100，即同时处理100个worksocket
				g_nJobMaxNum = 100;//pApp->m_RegParam.dwRecvJobMaxNum;
				g_nEventTime =  3;//秒
				bReturn = true;
			}
		}

		ReleaseMutex(hPoolMutex);
		CloseHandle(hPoolMutex);
        g_AllExit = false;
		return bReturn;
	}

	// 清理线程池工作项队列，CPoolThread::ExitInstance()处调用
	void ClearPool()
	{
		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		if(hPoolMutex == NULL)
		{
			TRACE(_T("ClearPool() -- OpenMutex faild!\n"));
		}
		WaitForSingleObject(hPoolMutex, INFINITE);

		//清理线程工作项
		g_arrayWorking.RemoveAll();
	
		ReleaseMutex(hPoolMutex);
		CloseHandle(hPoolMutex);
		g_AllExit = true;
	}

	// 判断线程池是否已满，来决定接受还是拒绝下级管理中心请求，CPoolThread::Accept()处调用
	bool IsPoolFull()
	{
		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		
		//超时，无法接收线程对象
		if(WaitForSingleObject(hPoolMutex, 2000) == WAIT_TIMEOUT)
		{
			ReleaseMutex(hPoolMutex);
			CloseHandle(hPoolMutex);
			TRACE(_T("IsPoolFull() --> can't receive the PoolObject\n"));
			return true;
		}

		//线程池已满
		bool bFull = false;
		int nWorkCount = g_arrayWorking.GetSize();
		if(g_arrayWorking.GetSize() >= g_nJobMaxNum)
		{
			TRACE(_T("IsPoolFull() --> the receive pool is full\n"));
			bFull = true;
		}

		ReleaseMutex(hPoolMutex);
		CloseHandle(hPoolMutex);

		return bFull;
	}

	// Add work item.加入工作项
	void AddItemRef(THREADDATA &ThreadData)
	{
		CString str;
		str.Format(_T("NDIdentity: Entering  AddItemRef() %d\n"), ThreadData.dwThreadID);
		TRACE(str);

		//打开互斥区
		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		WaitForSingleObject(hPoolMutex, INFINITE);

		//加入工作项数组(线程池)
		g_arrayWorking.Add(ThreadData);

		str.Format(_T("NDIdentity: Leaving AddItemRef() %d\n"), ThreadData.dwThreadID);
		TRACE(str);

		//关闭互斥区
		ReleaseMutex(hPoolMutex);

		CloseHandle(hPoolMutex);
	}

	// release Item.
	void ReleaseItem(THREADDATA &ThreadData)
	{
		CString str;
		str.Format(_T("NDIdentity: Entering ReleaseItem() %d\n"), ThreadData.dwThreadID);
		TRACE(str);

		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		WaitForSingleObject(hPoolMutex, INFINITE);

		//删除工作项
		for(int i = 0; i<g_arrayWorking.GetSize(); i++)
		{
			if(ThreadData.dwThreadID == g_arrayWorking.GetAt(i).dwThreadID)
			{
				g_arrayWorking.RemoveAt(i);
				break;
			}
		}

		str.Format(_T("NDIdentity: Leaving  ReleaseItem() %d\n"), ThreadData.dwThreadID);
		TRACE(str);

		ReleaseMutex(hPoolMutex);
		CloseHandle(hPoolMutex);
	}

	// 判断工作项是否应该主动断开连接，内部使用；
	// 只有当系统处理大于警戒数目请求，并且该工作项处于队列前AutoDisconnectNum项，才会返回TRUE
/*	bool AutoDisconnect(THREADDATA &ThreadData)
	{
		if(g_AllExit)
			return true;

		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		WaitForSingleObject(hPoolMutex, INFINITE);

		bool bAutoDisconnect = false;
		CString strOut;

		if(g_arrayWorking.GetSize() > g_nJobGuardNum)
		{
			for(int i = 0; i < g_arrayWorking.GetSize(); i++)
			{
				if(i >= g_nAutoDisconnectNum)
				{
					break;
				}
				else
				{
					if(ThreadData.dwThreadID == g_arrayWorking.GetAt(i).dwThreadID)
					{
						bAutoDisconnect = true;
						strOut.Format(_T("Thread id = %d and socketID = %d must be disconnected!\n"),ThreadData.dwThreadID,ThreadData.socket);
						TRACE(strOut);
						break;
					}
				}
			}
		}

		ReleaseMutex(hPoolMutex);
		CloseHandle(hPoolMutex);

		return bAutoDisconnect;
	}

	bool CloseSocket(SOCKET &WorkSocket)
	{
		WSAEVENT evtSent;
		evtSent = WSACreateEvent();
		if(evtSent == WSA_INVALID_EVENT)
		{
			return false;
		}

		if(WSAEventSelect(WorkSocket,evtSent,FD_CLOSE) == SOCKET_ERROR)
		{
			closesocket(WorkSocket);	
			return false;
		}
		shutdown(WorkSocket, SD_SEND);
		if(WaitForSingleObject(evtSent, g_nEventTime * 1000) != WAIT_OBJECT_0)
		{
			closesocket(WorkSocket);
			WSACloseEvent(evtSent);
			return false;
		}
		shutdown(WorkSocket, SD_BOTH);
		closesocket(WorkSocket);
		WSACloseEvent(evtSent);
		return true;
	}
*/
}//~WorkItem.
