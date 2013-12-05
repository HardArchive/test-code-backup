#pragma once
#include "stdafx.h"

#include "WorkItem.h"
#include "RemotePolicy.h"
#include "Communication.h"
//#include "WorkProc.h"
//#include "PoolWork.h"

namespace WorkItem
{
	
	DWORD g_nEventTime;				// ͨ��Socket�¼��ĵȴ�ʱ��
	//ȫ���̹߳�������
	CArray<THREADDATA,THREADDATA&> g_arrayWorking;

	int g_nJobMaxNum;				// �̳߳�������߳���
//	int g_nJobGuardNum;				// �̳߳ط�æ�ľ�����Ŀ
//	int g_nAutoDisconnectNum;		// �����ﾯ����Ŀ֮��ǰg_nAutoDisconnectNum����߳������ر�����
	bool g_AllExit = false;

	DWORD WINAPI ClientWorkItemThread(LPVOID lpParameter)
	{
		return ClientWorkFunction(lpParameter);
	}

	// Windows NT ʹ��
	UINT ClientWorkThread(LPVOID lpParameter)
	{
		return ClientWorkFunction(lpParameter);
	}

	//Client������
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
			
			//Add this thread item to therad pool.�����߳�������̳߳�
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
			strncpy(szbuf,"���",sizeof(szbuf) - 1);
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

	// ��ʼ���̳߳ع��������͹�������У�CPoolThread::InitInstance()������
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
				//�ݶ����߳�Ϊ100����ͬʱ����100��worksocket
				g_nJobMaxNum = 100;//pApp->m_RegParam.dwRecvJobMaxNum;
				g_nEventTime =  3;//��
				bReturn = true;
			}
		}

		ReleaseMutex(hPoolMutex);
		CloseHandle(hPoolMutex);
        g_AllExit = false;
		return bReturn;
	}

	// �����̳߳ع�������У�CPoolThread::ExitInstance()������
	void ClearPool()
	{
		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		if(hPoolMutex == NULL)
		{
			TRACE(_T("ClearPool() -- OpenMutex faild!\n"));
		}
		WaitForSingleObject(hPoolMutex, INFINITE);

		//�����̹߳�����
		g_arrayWorking.RemoveAll();
	
		ReleaseMutex(hPoolMutex);
		CloseHandle(hPoolMutex);
		g_AllExit = true;
	}

	// �ж��̳߳��Ƿ����������������ܻ��Ǿܾ��¼�������������CPoolThread::Accept()������
	bool IsPoolFull()
	{
		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		
		//��ʱ���޷������̶߳���
		if(WaitForSingleObject(hPoolMutex, 2000) == WAIT_TIMEOUT)
		{
			ReleaseMutex(hPoolMutex);
			CloseHandle(hPoolMutex);
			TRACE(_T("IsPoolFull() --> can't receive the PoolObject\n"));
			return true;
		}

		//�̳߳�����
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

	// Add work item.���빤����
	void AddItemRef(THREADDATA &ThreadData)
	{
		CString str;
		str.Format(_T("NDIdentity: Entering  AddItemRef() %d\n"), ThreadData.dwThreadID);
		TRACE(str);

		//�򿪻�����
		HANDLE hPoolMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpstrPoolMutexName);
		WaitForSingleObject(hPoolMutex, INFINITE);

		//���빤��������(�̳߳�)
		g_arrayWorking.Add(ThreadData);

		str.Format(_T("NDIdentity: Leaving AddItemRef() %d\n"), ThreadData.dwThreadID);
		TRACE(str);

		//�رջ�����
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

		//ɾ��������
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

	// �жϹ������Ƿ�Ӧ�������Ͽ����ӣ��ڲ�ʹ�ã�
	// ֻ�е�ϵͳ������ھ�����Ŀ���󣬲��Ҹù�����ڶ���ǰAutoDisconnectNum��Ż᷵��TRUE
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
