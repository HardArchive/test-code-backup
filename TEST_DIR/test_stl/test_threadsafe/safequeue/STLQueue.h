//============================================================  
//                   线程安全队列（Thread Safe Queue Class）  
//  
//                        
//  
// 注意：  
//  1.新申请节点必须通过CTSQueue::Allocate()，否则会出现问题  
//  2.释放节点用CTSQueue::FreeNode()可以利用FreeList，使得效率更佳  
//  3.该类使用前必须用CTSQueue::Initialize()初始化  
//  4.CTSQueue::hEvent是一个指示队列中是否有节点的事件，它可以作为同步锁  
//  5.必须在操作改类的所有线程退出之后才能析构该类  
//============================================================= 

#pragma once    
#include <windows.h>  

#define DeCTSQueueLockMaxTryTimes       400  
#define DeCTSQueueMaxFreeNodes          1024  

template<typename E> class CTSQueue  
{  
 private:  	 
	 // 内部节点定义  
	 struct _QUEUE_NODE  
	 {  
		 E Data;  
		 _QUEUE_NODE* Next;  
	 };  
	 
	 volatile LONG   _Locked;  
	 UINT            _Nodes;                 // 节点的个数  
	 UINT            _FNodes;                // FreeList中节点的个数  
	 _QUEUE_NODE*    _Head;  
	 _QUEUE_NODE*    _Tail;  
	 _QUEUE_NODE*    _FreeList;  
	 
	 VOID    Lock();  
	 VOID    Unlock();  
	 
	 public:   
		 HANDLE hEvent; // 指示队列中有节点事件句柄  
		 CTSQueue();  
         ~CTSQueue();  
         BOOL    Initialize();  
		 E*      Allocate();  
		 BOOL    FreeNode(E**);  
		 BOOL    Inqueue(E**);  
         E*      Dequeue();  
		 UINT    GetSize() { return _Nodes; }  
#ifdef TS_QUEUE_UNSAFE  
		 E*      Front() { return _Head->Data; }  
         E*      Back() { return _Tail->Data; }  
#endif  
};  
   
// 加互斥锁。  
// 这儿加的是自旋锁，在多处理器平台上，  
// 这可以在一定程度上避免线程上下文切换，使性能更出色。  
// 加锁过程在尝试DeCTSQueueLockMaxTry次失败后就换出线程。  
template<typename E>  
inline VOID CTSQueue<E>::Lock()  
{  
#ifndef TS_QUEUE_UNSAFE  
	DWORD dwTried = 0;  
	while(InterlockedExchange(&_Locked, TRUE))  
	{  
		dwTried++;  
		if(dwTried == DeCTSQueueLockMaxTryTimes)  
		{  
#if _WIN32_WINNT >= 0x0400  
			SwitchToThread();  
#else 
			Sleep(0);  
#endif  
			dwTried = 0;  
		}  
	}  
#endif  
 }  

 // 解除互斥锁  
template<typename E>  
inline VOID CTSQueue<E>::Unlock()  
{  
#ifndef TS_QUEUE_UNSAFE  
	InterlockedExchange(&_Locked, FALSE);  
#endif  
}  

    
// 构造函数  
template<typename E>  
CTSQueue<E>::CTSQueue()  
{  
	
	_Locked = FALSE;  
	_Nodes = 0;  
	_FNodes = 0;  
	_Head = NULL;  
	_Tail = NULL;  
	_FreeList = NULL;  
	hEvent = NULL;  
}  

  
// 析构函数  
template<typename E>  
CTSQueue<E>::~CTSQueue()  
{  
	Lock();  
	for(_Tail = _Head; _Tail != NULL; _Tail = _Head)  
	{  
		delete _Tail;  
		if(_Head)  
			_Head = _Head->Next;  
	}  
	
	for(_Tail = _FreeList; _Tail != NULL; _Tail = _FreeList)  
	{  
		delete _Tail;  
		if(_FreeList)  
			_FreeList = _FreeList->Next;  
	}  
	if(hEvent)  
		CloseHandle(hEvent);  
	Unlock();  
}  

// 初始化  
template<typename E>  
BOOL CTSQueue<E>::Initialize()  
{  
	_Head = NULL;  
	_Tail = NULL;  
	_FreeList = NULL;  
	_Nodes = 0;  
	_FNodes = 0;  
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  
	return hEvent != NULL;  
}  
   
// 申请结点  
template<typename E>  
E* CTSQueue<E>::Allocate()  
{  
	_QUEUE_NODE* r = NULL;  
	
	Lock();  
	if(r = _FreeList)  
	{  
		_FreeList = _FreeList->Next;  
		_FNodes--;  
	}  
	else 
	{  
		r = new _QUEUE_NODE;  
		if(r == NULL)  
			return NULL;  
	}  
	Unlock();  
	return (E*)r;  
}  

// 释放结点  
template<typename E>  
BOOL CTSQueue<E>::FreeNode(E** Node)  
{  
	if(Node == NULL || *Node == NULL)  
		return FALSE;  
	Lock();  
	_QUEUE_NODE* e = (_QUEUE_NODE*)(*Node);  
	*Node = NULL;  
	if(_FNodes < DeCTSQueueMaxFreeNodes)  
	{  
		e->Next = _FreeList;  
		_FreeList = e;  
		_FNodes++;  
	}  
	else 
		delete e;  
	Unlock();  
	return TRUE;  
}  
    
// 向队列尾部添加节点  
template<typename E>  
BOOL CTSQueue<E>::Inqueue(E** Node)  
{  
	if(Node == NULL || *Node == NULL)  
		return FALSE;  
	Lock();  
	_QUEUE_NODE* e = (_QUEUE_NODE*)(*Node);  
	*Node = NULL;  
	e->Next = NULL;  
	if(_Head == NULL)  
		_Head = _Tail = e;  
	else 
		_Tail = _Tail->Next = e;  
	if(++_Nodes == 1)  
		SetEvent(hEvent);  
	Unlock();  
	return TRUE;  
}  
   
// 从队列中取出头结点  
template<typename E>  
E* CTSQueue<E>::Dequeue()  
{  
	Lock();  
	E* r = (E*)_Head;  
	if(_Head)  
	{  
		_Head = _Head->Next;  
		if(_Head == NULL)  
			_Tail = NULL;  
		if(--_Nodes < 1)  
			ResetEvent(hEvent);  
	}  
	Unlock();  
	return r;  
}


