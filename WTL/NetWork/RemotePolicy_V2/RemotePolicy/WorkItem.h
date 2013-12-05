#pragma once

namespace WorkItem
{
	typedef struct rawThreadData
	{
		DWORD dwThreadID;
		SOCKET socket;
	}THREADDATA;

	extern DWORD g_nEventTime;									// 通信Socket事件的等待时间
	extern CArray<THREADDATA,THREADDATA&> g_arrayWorking;		// 存放线程池线程工作线程ID数组

	extern int g_nJobMaxNum;				// 线程池最大工作线程数
//	extern int g_nJobGuardNum;				// 线程池繁忙的警戒数目
//	extern int g_nAutoDisconnectNum;		// 当到达警戒数目之后，前g_nAutoDisconnectNum项的线程主动关闭连接
	extern bool g_AllExit;
	

	//Operation
	// 线程池工作项线程，供Win2000或更新版使用，由QueueUserWorkItem()调用
	DWORD WINAPI ClientWorkItemThread(LPVOID lpParameter);	

	// 通信工作线程，供WinNT使用，由AfxBeginThread调用
	UINT ClientWorkThread(LPVOID lpParameter);
	// 通信工作项实现函数
	DWORD ClientWorkFunction(LPVOID lpParameter);	

	// 初始化线程池状态，CPoolThread实例启动时调用
	bool InitPool();
	// 清理线程池状态，CPoolThread实例结束时调用
	void ClearPool();

	// 判断线程池是否已满，CListenSocket接受下级管理中心连接请求是调用
	bool IsPoolFull();

	//添加工作线程到g_arrayWorking
	void AddItemRef(THREADDATA &ThreadData);	

	//从g_arrayWorking中删除工作线程
	void ReleaseItem(THREADDATA &ThreadData);

	//判断是否要主动关闭连接
//	bool AutoDisconnect(THREADDATA &ThreadData);
//	bool CloseSocket(SOCKET &WorkSocket);
	//bool ComOfWork(CWorkProc &workproc,void *pData,long lHostIP);
}