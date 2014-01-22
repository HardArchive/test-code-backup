#include "stdafx.h"
#include "ThreadPool.h"


CThreadPool::CThreadPool()
{
	m_iThread = 0;
	m_fFunc = 0;
	m_hThread = 0;
	InitializeCriticalSection(&m_cs); //初始化一个临界区对象
}

CThreadPool::~CThreadPool()
{
	//将一个消息放入（寄送）到指定线程的消息队列里，不等待线程处理消息就返回
	PostThreadMessage(m_iThread, WM_THREADPOOLEND, 0, 0); //线程池退出消息
	WaitForSingleObject(m_hThread, -1); //等待一个内核对象变为已通知状态 INFINITE
	//Windows提供了几种内核对象可以处于已通知状态和未通知状态：进程、线程、作业、文件、控制台输入/输出/错误流、事件、等待定时器、信号量、互斥对象
	CloseHandle(m_hThread);
	LeaveCriticalSection(&m_cs); //解锁 到EnterCriticalSection之间代码资源已经释放了，其他线程可以进行操
	int i;
	for(i = 0; i < m_qRun.size(); i++)
	{
		delete m_qRun[i];
	}
	for(i = 0; i < m_qWait.size(); i++)
	{
		delete m_qWait[i];
	}
}

//ThreadFunc func,线程实现函数 LPVOID lp,线程参数 bool bImme //?????是否为第一次执行
void CThreadPool::AddTask(ThreadFunc func, LPVOID lp, bool bImme)
{
	if(!bImme)
	{
		PostThreadMessage(m_iThread, WM_THREADADD, (WPARAM)func, (LPARAM)lp);  //增加线程---将线程结构放入等待运行的线程vector m_qWait中
	}
	else
	{
		spThread sTh = new sThread;
		sTh->m_dRetval = 0;
		sTh->m_hThread = 0;
		sTh->m_fFunc = func;
		sTh->m_vParam = lp;
		EnterCriticalSection(&m_cs);  //加锁 接下来的代码处理过程中不允许其他线程进行操作，除非遇到LeaveCriticalSection
		m_qWait.insert(m_qWait.begin(), sTh);
		LeaveCriticalSection(&m_cs);
	}
	PostThreadMessage(m_iThread, WM_THREADPOOLIDIL, 0, 0); //?

}

//初始化线程池
void CThreadPool::Init(CallBackFunc func, int count/* =10000 */)
{
	if(func != NULL)  
		m_fFunc = func;
	if(count > 0)
		m_nMaxThreadCount = count;
	m_hEvent = CreateEvent(0, TRUE, FALSE, 0); //打开或创建一个手动复位（参数2TRUE）的初始状态为无信号（参数3）的无名（参数4）事件对象
	m_hThread = CreateThread(0, 0, Thread, this, 0, &m_iThread);
	WaitForSingleObject(m_hEvent, -1);
	CloseHandle(m_hEvent);
}

//
DWORD WINAPI CThreadPool::Thread(LPVOID lp)
{
	CThreadPool *pThPool = (CThreadPool *)lp;
	DWORD dwCurId = GetCurrentThreadId();
	MSG msg;
	PeekMessage(&msg, 0, WM_USER, WM_USER, PM_NOREMOVE);
	//函数功能：该函数为一个消息检查线程消息队列，并将该消息（如果存在）放于指定的结构。
	//函数原型：BOOL PeekMessage（LPMSG IpMsg，HWND hWnd，UINT wMSGfilterMin，UINT wMsgFilterMax，UINT wRemoveMsg）；
	//lpMsg：接收消息信息的MSG结构指针。
	//hWnd：其消息被检查的窗口的句柄。
	//wMsgFilterMin：指定被检查的消息范围里的第一个消息。
	//wMsgFilterMax：指定被检查的消息范围里的最后一个消息。
	//wRemoveMsg：确定消息如何被处理。此参数可取下列值之一：
	//PM_NOREMOVE：PeekMessage处理后，消息不从队列里除掉。
	//PM_REMOVE：PeekMessage处理后，消息从队列里除掉。
	//可将PM_NOYIELD随意组合到PM_NOREMOVE或PM_REMOVE。此标志使系统不释放等待调用程序空闲的线程。
	//缺省地，处理所有类型的消息。若只处理某些消息，指定一个或多个下列值：
	//PM_QS_INPUT：Windows NT5.0和Windows 98：处理鼠标和键盘消息。
	//PM_QS_PAINT：Windows NT 5.0和Windows 98：处理画图消息。
	//PM_QS_POSTMESSAGE：Windows NT 5.0和Windows 98：处理所有被寄送的消息，包括计时器和热键。
	//PM_QS_SENDMESSAGE：Windows NT 5.0和Windows 98：处理所有发送消息。
	//返回值：如果消息可得到，返回非零值；如果没有消息可得到，返回值是零。
	SetEvent(pThPool->m_hEvent);  //设置事件为有信号
	while (TRUE)
	{
		Sleep(1);

		if(PeekMessage(&msg, 0, WM_THREADSTART, WM_THREADPOOLIDIL, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_THREADPOOLEND: 
				return true;
			case WM_THREADSTART: //创建线程
				{
					spThread sTh = (spThread)msg.wParam;
					sTh->m_hThread = CreateThread(0, 0, sTh->m_fFunc, sTh->m_vParam, 0, 0);
					if(pThPool->m_fFunc) 
						pThPool->m_fFunc(WM_THREADSTART, sTh);
					break;
				}
			case WM_THREADEND: //关闭线程
				{
					spThread sTh = (spThread)msg.wParam;
					if(pThPool->m_fFunc)  pThPool->m_fFunc(WM_THREADEND, sTh);
					delete sTh;
					CloseHandle(sTh->m_hThread);
					break;
				}
			case WM_THREADADD:
				{//增加线程---将线程结构放入等待运行的线程vector m_qWait中
					CThreadPool::spThread sTh = new CThreadPool::sThread;
					sTh->m_dRetval = 0;
					sTh->m_hThread = 0;
					sTh->m_fFunc = (ThreadFunc)msg.wParam;
					sTh->m_vParam = (LPVOID)msg.lParam;
					EnterCriticalSection(&(pThPool->m_cs));
					pThPool->m_qWait.push_back(sTh);
					LeaveCriticalSection(&(pThPool->m_cs));
					break;
				}
			case WM_THREADPOOLIDIL:
				{
					goto IDIL;
				}
			}
		}
		else
		{
IDIL:
			vector<CThreadPool::spThread> &vObj = pThPool->m_qRun;
			for(int i = 0; i < vObj.size(); i++)
			{
				DWORD iExitCode;
				GetExitCodeThread(vObj[i]->m_hThread, &iExitCode);
				if(iExitCode != STILL_ACTIVE)
				{
					vObj[i]->m_dRetval = iExitCode;
					PostThreadMessage(dwCurId, WM_THREADEND, (WPARAM)vObj[i], 0);
					vObj.erase(vObj.begin() + i);
					i--;
				}
			}
			if(vObj.size() < pThPool->m_nMaxThreadCount)
			{
				int nCount = pThPool->m_nMaxThreadCount - vObj.size();
				while(nCount)
				{
					if(pThPool->m_qWait.size() > 0)
					{
						EnterCriticalSection(&(pThPool->m_cs));
						vObj.push_back(pThPool->m_qWait[0]);
						pThPool->m_qWait.erase(pThPool->m_qWait.begin());
						LeaveCriticalSection(&(pThPool->m_cs));
						PostThreadMessage(dwCurId, WM_THREADSTART, (WPARAM)vObj[vObj.size() - 1], 0);
						while(1)
						{
							MSG msgtemp;
							PeekMessage(&msgtemp, 0, WM_THREADSTART, WM_THREADPOOLIDIL, PM_NOREMOVE);
							if(msgtemp.message != WM_THREADSTART)
							{
								PeekMessage(&msgtemp, 0, WM_THREADSTART, WM_THREADPOOLIDIL, PM_REMOVE);
								PostThreadMessage(dwCurId, msgtemp.message, msgtemp.wParam, msgtemp.lParam);
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						break;
					}
					nCount--;
				}
			}

		}
	}
	return 0;
}

DWORD CThreadPool::GetRunThreadCount()
{
	return m_qRun.size();
}

CThreadPool::spThread CThreadPool::GetRunThreadItem(int nIndex)
{
	return m_qRun[nIndex];
}
DWORD CThreadPool::GetWaitThreadCount()
{
	return m_qWait.size();
}

CThreadPool::spThread CThreadPool::GetWaitThreadItem(int nIndex)
{
	return m_qWait[nIndex];
}

