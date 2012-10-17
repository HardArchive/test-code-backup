#ifndef _ThreadPool_H_
#define _ThreadPool_H_
#pragma once
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#include <cassert>
#include <vector>
#include <queue>
#include <windows.h>
using namespace std;

class ThreadJob  //工作基类
{
public:
	//供线程池调用的纯虚函数
	virtual void DoJob(void *pPara) = 0;
};

class ThreadPool
{
public:
	//构造函数 dwNum 线程池规模  
	ThreadPool(DWORD dwNum = 4) : _lThreadNum(0), _lRunningNum(0)
	{
		InitializeCriticalSection(&_csThreadVector); //初始化一个临界资源对象
		InitializeCriticalSection(&_csWorkQueue);
		_EventComplete = CreateEvent(0, false, false, NULL); //创建一个自动复原，初始状态为无信号的事件对象
		_EventEnd = CreateEvent(0, true, false, NULL);       //创建一个手动复原，初始状态为无信号的事件对象
		_SemaphoreCall = CreateSemaphore(0, 0,  0x7FFFFFFF, NULL); //创建一个新的信号机
		_SemaphoreDel =  CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
		assert(_SemaphoreCall != INVALID_HANDLE_VALUE);
		assert(_EventComplete != INVALID_HANDLE_VALUE);
		assert(_EventEnd != INVALID_HANDLE_VALUE);
		assert(_SemaphoreDel != INVALID_HANDLE_VALUE);
		AdjustSize(dwNum <= 0 ? 4 : dwNum);
	}
	~ThreadPool()
	{
		DeleteCriticalSection(&_csWorkQueue);
		CloseHandle(_EventEnd);
		CloseHandle(_EventComplete);
		CloseHandle(_SemaphoreCall);
		CloseHandle(_SemaphoreDel);
		vector<ThreadItem*>::iterator iter;
		for(iter = _ThreadVector.begin(); iter != _ThreadVector.end(); iter++)
		{
			if(*iter)
				delete *iter;
		}
		DeleteCriticalSection(&_csThreadVector);
	}
	//调整线程池规模
	int AdjustSize(int iNum)
	{
		if(iNum > 0)
		{
			ThreadItem *pNew;
			EnterCriticalSection(&_csThreadVector);
			for(int _i=0; _i<iNum; _i++)
			{
				_ThreadVector.push_back(pNew = new ThreadItem(this));
				assert(pNew);
				pNew->_Handle = CreateThread(NULL, 0, DefaultJobProc, pNew, 0, NULL);
				assert(pNew->_Handle);
			}
			LeaveCriticalSection(&_csThreadVector);
		}
		else
		{
			iNum *= -1;
			ReleaseSemaphore(_SemaphoreDel,  iNum > _lThreadNum ? _lThreadNum : iNum, NULL);
		}
		return (int)_lThreadNum;
	}
	//调用线程池
	void Call(void (*pFunc)(void  *), void *pPara = NULL)
	{
		assert(pFunc);
		EnterCriticalSection(&_csWorkQueue);
		_JobQueue.push(new JobItem(pFunc, pPara));
		LeaveCriticalSection(&_csWorkQueue);
		ReleaseSemaphore(_SemaphoreCall, 1, NULL);
	}
	//调用线程池
	inline void Call(ThreadJob * p, void *pPara = NULL)
	{
		Call(CallProc, new CallProcPara(p, pPara));
	}
	//结束线程池, 并同步等待
	bool EndAndWait(DWORD dwWaitTime = INFINITE)
	{
		SetEvent(_EventEnd);
		return WaitForSingleObject(_EventComplete, dwWaitTime) == WAIT_OBJECT_0;
	}
	//结束线程池
	inline void End()
	{
		SetEvent(_EventEnd);
	}
	inline DWORD Size()
	{
		return (DWORD)_lThreadNum;
	}
	inline DWORD GetRunningSize()
	{
		return (DWORD)_lRunningNum;
	}
	bool IsRunning()
	{
		return _lRunningNum > 0;
	}
protected:
	//工作线程
	static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL)
	{
		ThreadItem *pThread = static_cast<ThreadItem*>(lpParameter);
		assert(pThread);
		ThreadPool *pThreadPoolObj = pThread->_pThis;
		assert(pThreadPoolObj);
		InterlockedIncrement(&pThreadPoolObj->_lThreadNum);
		HANDLE hWaitHandle[3];
		hWaitHandle[0] = pThreadPoolObj->_SemaphoreCall;
		hWaitHandle[1] = pThreadPoolObj->_SemaphoreDel;
		hWaitHandle[2] = pThreadPoolObj->_EventEnd;
		JobItem *pJob;
		bool fHasJob;
		for(;;)
		{
			DWORD wr = WaitForMultipleObjects(3, hWaitHandle, false, INFINITE);
			//响应删除线程信号
			if(wr == WAIT_OBJECT_0 + 1) 
				break;
			//从队列里取得用户作业
			EnterCriticalSection(&pThreadPoolObj->_csWorkQueue);
			if(fHasJob = !pThreadPoolObj->_JobQueue.empty())
			{
				pJob = pThreadPoolObj->_JobQueue.front();
				pThreadPoolObj->_JobQueue.pop();
				assert(pJob);
			}
			LeaveCriticalSection(&pThreadPoolObj->_csWorkQueue);
			//受到结束线程信号确定是否结束线程(结束线程信号&& 是否还有工作)
			if(wr == WAIT_OBJECT_0 + 2 && !fHasJob) 
				break;
			if(fHasJob && pJob)
			{
				InterlockedIncrement(&pThreadPoolObj->_lRunningNum);
				pThread->_dwLastBeginTime = GetTickCount();
				pThread->_dwCount++;
				pThread->_fIsRunning = true;
				pJob->_pFunc(pJob->_pPara); //运行用户作业
				delete pJob;
				pThread->_fIsRunning = false;
				InterlockedDecrement(&pThreadPoolObj->_lRunningNum);
			}
		}
		//删除自身结构
		EnterCriticalSection(&pThreadPoolObj->_csThreadVector);
		pThreadPoolObj->_ThreadVector.erase(find(pThreadPoolObj->_ThreadVector.begin(), pThreadPoolObj->_ThreadVector.end(), pThread));
		LeaveCriticalSection(&pThreadPoolObj->_csThreadVector);
		delete pThread;
		InterlockedDecrement(&pThreadPoolObj->_lThreadNum);
		if(!pThreadPoolObj->_lThreadNum)  //所有线程结束
			SetEvent(pThreadPoolObj->_EventComplete);
		return 0;
	}
	//调用用户对象虚函数
	static void CallProc(void *pPara)
	{
		CallProcPara *cp = static_cast<CallProcPara *>(pPara);
		assert(cp);
		if(cp)
		{
			cp->_pObj->DoJob(cp->_pPara);
			delete cp;
		}
	}
	//用户对象结构
	struct CallProcPara 
	{
		ThreadJob* _pObj;//用户对象
		void *_pPara;    //用户参数
		CallProcPara(ThreadJob* p, void *pPara) : _pObj(p), _pPara(pPara) { };
	};
	//用户函数结构
	struct JobItem
	{
		void (*_pFunc)(void  *);//函数
		void *_pPara; //参数
		JobItem(void (*pFunc)(void  *) = NULL, void *pPara = NULL) : _pFunc(pFunc), _pPara(pPara) { };
	};
	//线程池中的线程结构
	struct ThreadItem
	{
		HANDLE _Handle; //线程句柄
		ThreadPool *_pThis;  //线程池的指针
		DWORD _dwLastBeginTime; //最后一次运行开始时间
		DWORD _dwCount; //运行次数
		bool _fIsRunning;
		ThreadItem(ThreadPool *pthis) : _pThis(pthis), _Handle(NULL), _dwLastBeginTime(0), _dwCount(0), _fIsRunning(false) { };
		~ThreadItem()
		{
			if(_Handle)
			{
				CloseHandle(_Handle);
				_Handle = NULL;
			}
		}
	};

	std::queue<JobItem *> _JobQueue;           //工作队列
	std::vector<ThreadItem *>  _ThreadVector;  //线程数据
	CRITICAL_SECTION _csThreadVector;      //工作队列临界
	CRITICAL_SECTION _csWorkQueue;         //线程数据临界
	HANDLE _EventEnd;         //结束通知
	HANDLE _EventComplete;    //完成事件
	HANDLE _SemaphoreCall;    //工作信号
	HANDLE _SemaphoreDel;     //删除线程信号
	long _lThreadNum;      //线程数,
	long _lRunningNum;     //运行的线程数
};
#endif //_ThreadPool_H_