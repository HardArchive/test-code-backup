//============================================================  
//                   �̰߳�ȫ���У�Thread Safe Queue Class��  
//  
//                        
//  
// ע�⣺  
//  1.������ڵ����ͨ��CTSQueue::Allocate()��������������  
//  2.�ͷŽڵ���CTSQueue::FreeNode()��������FreeList��ʹ��Ч�ʸ���  
//  3.����ʹ��ǰ������CTSQueue::Initialize()��ʼ��  
//  4.CTSQueue::hEvent��һ��ָʾ�������Ƿ��нڵ���¼�����������Ϊͬ����  
//  5.�����ڲ�������������߳��˳�֮�������������  
//============================================================= 

#pragma once    
#include <windows.h>  

#define DeCTSQueueLockMaxTryTimes       400  
#define DeCTSQueueMaxFreeNodes          1024  

template<typename E> class CTSQueue  
{  
 private:  	 
	 // �ڲ��ڵ㶨��  
	 struct _QUEUE_NODE  
	 {  
		 E Data;  
		 _QUEUE_NODE* Next;  
	 };  
	 
	 volatile LONG   _Locked;  
	 UINT            _Nodes;                 // �ڵ�ĸ���  
	 UINT            _FNodes;                // FreeList�нڵ�ĸ���  
	 _QUEUE_NODE*    _Head;  
	 _QUEUE_NODE*    _Tail;  
	 _QUEUE_NODE*    _FreeList;  
	 
	 VOID    Lock();  
	 VOID    Unlock();  
	 
	 public:   
		 HANDLE hEvent; // ָʾ�������нڵ��¼����  
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
   
// �ӻ�������  
// ����ӵ������������ڶദ����ƽ̨�ϣ�  
// �������һ���̶��ϱ����߳��������л���ʹ���ܸ���ɫ��  
// ���������ڳ���DeCTSQueueLockMaxTry��ʧ�ܺ�ͻ����̡߳�  
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

 // ���������  
template<typename E>  
inline VOID CTSQueue<E>::Unlock()  
{  
#ifndef TS_QUEUE_UNSAFE  
	InterlockedExchange(&_Locked, FALSE);  
#endif  
}  

    
// ���캯��  
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

  
// ��������  
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

// ��ʼ��  
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
   
// ������  
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

// �ͷŽ��  
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
    
// �����β����ӽڵ�  
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
   
// �Ӷ�����ȡ��ͷ���  
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


