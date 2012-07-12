//#include "../stdafx.h"
#pragma once
#include <memory.h>

template<class T>      
class CQueueEx
{  
protected:   int head;     //队头指针   
			 int tail;     //队尾指针 
			 int qsize;    //队列长度
			 T *elements;  //存放队列元素的数组   
			 int maxsize;  //队列最大可容纳元素个数   
			 HANDLE    m_hPushEvent;                       //控制入列事件对象
			 HANDLE    m_hPopEvent;                        //控制出列事件对象
			 CRITICAL_SECTION m_lsGS;				       //临界变量
public:   CQueueEx(int i = 32);			//构造函数   
		  ~CQueueEx();					//析构函数   

		  void PushTail(T);				//将新元素插入在队尾   
		  bool PopFront(T&);			//从队头取一个元素   

		  bool IsFull()const			//判队列是否为满   
		  {
			  return (tail+1) % maxsize == head;
		  } 

		  bool IsEmpty()				//判队列是否为空   
		  {
			  return head == tail;
		  }

		  bool append();				//当空间满时追加分配空间

		  int get_maxsize()
		  {
			  return maxsize-1;
		  }

		  int get_qsize()
		  {
			  return qsize;
		  }
};



//构造函数
template <class T>
CQueueEx<T>::CQueueEx(int i)
{
	m_hPushEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hPopEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	InitializeCriticalSection(&m_lsGS);

	EnterCriticalSection(&m_lsGS);

	head = tail = 0;		//将队列设置为空队列  
	maxsize = i<10?10:i;		//设置队列的最大长度，最小为 10  
	qsize = 0;  
	elements = new T[maxsize];//给队列分配内存空间 
	if(elements == 0)			//失败，警告
	{
		OutputDebugString("CQueueEx内存分配失败");
	}

	LeaveCriticalSection(&m_lsGS);
}

//析构函数
template <class T>
CQueueEx<T>::~CQueueEx()
{
	SetEvent(m_hPushEvent);//设置入队列为有信号量
	SetEvent(m_hPopEvent); //设置出队列为有信号量
	EnterCriticalSection(&m_lsGS);
	delete []elements;
	DeleteCriticalSection(&m_lsGS);

} 

//将新元素插入在队尾
template <class T>
void CQueueEx<T>::PushTail(T element)
{
	if(IsFull())                //若队满，追加空间
	{
		append();
	}
	//if (IsFull())
	//{
	//	ResetEvent(m_hPushEvent);//设置无信号量
	//}

	WaitForSingleObject(m_hPushEvent, INFINITE);

	EnterCriticalSection(&m_lsGS);
	tail = (tail+1) % maxsize;  //尾指针加 1  
	elements[tail] = element;   //给队尾赋值
	qsize++;                    //队长加 1
	LeaveCriticalSection(&m_lsGS);
	SetEvent(m_hPopEvent);//设置出队列为有信号量
}

//从队头取一个元素
template <class T>
bool CQueueEx<T>::PopFront(T &element)
{	
	if(IsEmpty())
	{
		//cout<<"队列为空"<<endl;    //若队空，警告，出错处理
		ResetEvent(m_hPopEvent);   //把队列置为无信号状态
	}
	//TRACE("取出队列中数据，进行等待\n");
	WaitForSingleObject(m_hPopEvent, INFINITE);
	//TRACE("取出队列中数据，等待完毕\n");
	EnterCriticalSection(&m_lsGS);
	head = (head+1) % maxsize;    //头指针加 1  
	element =  elements[head];    //取出队头元素  
	qsize--;                      //队长减 1  
	LeaveCriticalSection(&m_lsGS);
	SetEvent(m_hPushEvent);//设置入队列为有信号量
	return true; 
}

//当空间满时追加分配空间
template <class T>
bool CQueueEx<T>::append()
{
	T *p,*temp;
	p = new T[maxsize+10];        //每次追加10个内存空间
	if(!p)
	{
		//cout<<"内存追加失败"<<endl;
		return false;
	}
	EnterCriticalSection(&m_lsGS);
	memmove(p, elements, sizeof(T)*maxsize);
	temp = elements;
	elements = p;
	delete []temp;
	maxsize = maxsize + 10;
	LeaveCriticalSection(&m_lsGS);
	return true;
}