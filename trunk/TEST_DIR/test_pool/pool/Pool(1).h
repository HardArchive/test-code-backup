
/*******************************************************************************
 * Copyright (C), 2008-2010, 李  照
 * FileName: MemoryPool.h
 * Author: 李  照    Version: 1.0    Date: 2010-10-20
 * Description: 内存池管理 头文件
 * Note: 1. 使用时请注意对池对象的内存操作时的溢出检查，如果溢出后，会导致内存池
 *       对象的链表指针错误，无现无法预料的结果。
 *       2. 此内存池对象使用了new申请内存，这样效率虽然低了一点儿，但是可以方便
 *       使用标准库的string等对象的初始化工作。
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. 
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2010-10-20    1.0    build this moudle  
 ******************************************************************************/
#pragma once

#ifndef TCP_BLOCK_POOL_H_
#define TCP_BLOCK_POOL_H_

#include <set>

using namespace std;

template<class T> 
class TPoolItem
{
public:
	~TPoolItem(){  }
	T Item;
	TPoolItem *Next;

	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}
};

template<class T> 
struct TPoolItemHeader
{
	// 项目个数
	int Count;
	// 第一条记录指针
	TPoolItem<T>* First;
	// 最后一条记录指针
	TPoolItem<T>* Last;


	TPoolItemHeader(): Count(0), First(NULL), Last(NULL){}

	// 把数据压进队列
	inline int Push(TPoolItem<T>* item)
	{
		if (NULL == item)
		{
			return this->Count;
		}

		item->Next = NULL;
		this->Count++;
		item->Next = this->First;
		this->First = item;
		if (NULL == this->Last)
		{
			this->Last = item;
		}

		return this->Count;
	}

	// 把数据压进队列
	inline int PushBack(TPoolItem<T>* item)
	{
		if (NULL == item)
		{
			return this->Count;
		}

		item->Next = NULL;
		this->Count++;
		if (NULL == this->Last)
		{
			this->First = item;
		}
		else
		{
			this->Last->Next = item;
		}
		this->Last = item;

		return this->Count;
	}

	// 从队列里弹出数据
	inline TPoolItem<T>* Pop()
	{
		TPoolItem<T>* item = NULL;
		if (this->Count > 0)
		{
			this->Count--;
			item = this->First;
			this->First = this->First->Next;
			item->Next = NULL;
		}

		if (this->First == NULL)
		{
			this->Last = NULL;
		}
		return item;
	}
};

template<class T> 
class TPool
{
public:
	// 构造函数
	TPool(int nMax=40000, int nStep=10000, bool bAllocNow=false);
	// 析构函数
	~TPool();

	// 初始化列表
	int Init(int nMax=1024, int nStep=1024, bool bAllocNow=true);

	// 在池上创建一个对象
	T* New(void);
	// 把对象归还到池
	int Delete(T* item);
	// 对象池是否已满
	bool IsFull(void) const;
	// 释放池资源
	int Free();

	// 打印当前池的使用情况，并记录到“d:\p1_%d.log”和“d:\p2_%d.log”文件
	int TracePool(const int nSeqNo);
	// 统计消息转换为字符串
	int ToString(char* szMsg, const int nBuffSize);
private:
	// 池最大对象数
	int m_Max;
	// 当前池中对象数
	int m_Current;
	////// 已使用对象数
	////int m_UsedCount;
	////// 空闲对象数据
	////int m_UnUsedCount;
	// 当前申请的内存块数
	int m_BlockCount;

	// 申请对象数步长
	int m_Step;

	// 是滞已释放
	bool m_IsFree;
	// 同步变更
	CRITICAL_SECTION m_lock;

	//// 最后一个元素
	//TPoolItem<T>* m_LastUnUsed;
	// 未用元素列表
	TPoolItemHeader<T> m_UnUsed;
	// 正在使用的元素集合
	TPoolItemHeader<T> m_Used;
	// 申请的内存列表
	TPoolItem<T>** m_List;

	// 申请内容，如果申请成功，则返回为第一项，如果申请失败，则返回NULL
	TPoolItem<T>* Malloc(void);
	// 
	TPoolItem<T>* FindItem(TPoolItem<T>* &itFirst, const T* item);
	// 
	TPoolItem<T>* PopItem(TPoolItemHeader<T> &header, const T* item);
};

template<class T>
int TPool<T>::TracePool( const int nSeqNo)
{
	char szFileUsed[MAX_PATH], szFileUnUsed[MAX_PATH];
	sprintf_s(szFileUsed, MAX_PATH, "d:\\p1_%d.log", nSeqNo);
	sprintf_s(szFileUnUsed, MAX_PATH, "d:\\p2_%d.log", nSeqNo);

	FILE* fp = fopen(szFileUnUsed, "wb");
	if (NULL != fp && NULL != m_UnUsed.First)
	{
		TPoolItem<T>* p = m_UnUsed.First;
		for(int i=0; i<m_UnUsed.Count; i++)
		{
			char szMsg[256]={0};
			int nLen = sprintf_s(szMsg, 256, "%08X,%08X,%08X, ", p, p->Next, &p->Item);
			fwrite(szMsg, nLen, 1, fp );
			p = p->Next;
		}
		fclose(fp);
	}

	fp = fopen(szFileUsed, "wb");
	if (NULL != fp && NULL != m_Used.First)
	{
		TPoolItem<T>* p = m_Used.First;
		for(int i=0; i<m_Used.Count; i++)
		{
			char szMsg[256]={0};
			int nLen = sprintf_s(szMsg, 256, "%08X,%08X,%08X", p, p->Next, &p->Item);
			fwrite(szMsg, nLen, 1, fp );
			p = p->Next;
		}
		fclose(fp);
	}

	return 0;
}


template<class T>
TPoolItem<T>* TPool<T>::FindItem( TPoolItem<T>* &itFirst, const T* item )
{
	TPoolItem<T>* it = NULL;
	TPoolItem<T>* p = itFirst;
	while(NULL != p)
	{
		if (&p->Item == item)
		{
			it = p;
			break;
		}

		p = p->Next;
	}
	return it;
}

template<class T>
TPoolItem<T>* TPool<T>::PopItem( TPoolItemHeader<T> &header, const T* item )
{
	TPoolItem<T>* it = NULL;
	TPoolItem<T> *pPre = NULL, *p = header.First;
	while(NULL != p)
	{
		if (&p->Item == item)
		{
			if (NULL == pPre)
			{
				header.First = p->Next;
			}
			else
			{
				pPre->Next = p->Next;
			}
			it = p;
			header.Count--;
			break;
		}

		pPre = p;
		p = p->Next;
	}
	return it;
}

template<class T>
int TPool<T>::ToString( char* szMsg, const int nBuffSize )
{
	int nUnused = 0;
	TPoolItem<T>* p = m_UnUsed.First;
	while(NULL != p)
	{
		nUnused ++;

		p = p->Next;
	}
	return sprintf_s(szMsg, nBuffSize, "pool: %lu, used: %lu, unused: %lu, check: %lu", m_Current, m_Used.Count, m_UnUsed.Count, nUnused);
}

template<class T>
int TPool<T>::Init( int nMax/*=1024*/, int nStep/*=1024*/, bool bAllocNow/*=true*/ )
{
	this->m_Max = nMax;
	this->m_Step = nStep;
	m_IsFree = false;

	if (bAllocNow)
	{
		int iMax = nMax / m_Step + 1;
		m_List = new(nothrow) TPoolItem<T>*[iMax];
		if (m_List == NULL)
		{
			// 对象不属于此池
			TracePrint(LOG_ERROR, "pool malloc item list error!");
		}
		else
		{
			for (int i=0;i<iMax;i++)
			{
				m_List[i] = NULL;
			}
		}
	}

	return 0;
}

template<class T>
TPoolItem<T>* TPool<T>::Malloc( void )
{
	TPoolItem<T>* item = NULL;

	int nMallocSize = sizeof(TPoolItem<T>)*m_Step;
	// 申请内存对象
	//item = (TPoolItem<T>*)malloc(nMallocSize);
	item = new(nothrow) TPoolItem<T>[m_Step];
	if (item == NULL)
	{
		// 申请内存失败，直接返回空
		return NULL;
	}
	if (m_List == NULL)
	{
		// 初始化内存池
		Init(m_Max, true);
	}

	m_Current += m_Step;
	m_List[m_BlockCount] = item;
	m_BlockCount ++;

	// 把申请到的内存，放到未使用的链表中
	for (int i=0;i<m_Step;i++)
	{
		m_UnUsed.PushBack(&item[i]);
	}
	
	return item;
}

template<class T>
TPool<T>::TPool(int nMax/*=1024*/, int nStep/*=1024*/, bool bAllocNow/*=false*/): m_IsFree(false), m_Current(0), 
	m_BlockCount(0), m_List(NULL)
{
	m_Max = nMax;
	m_Step = nStep;
	InitializeCriticalSection(&m_lock);

	// 初始化内存池
	Init(nMax, m_Step, bAllocNow);
}

template<class T>
TPool<T>::~TPool()
{
	DeleteCriticalSection(&m_lock);
}

template<class T>
int TPool<T>::Free()
{
	if (!m_IsFree)
	{
		EnterCriticalSection(&m_lock);
		m_IsFree = true;
		m_Current = 0;
		for (int i=0;i<m_BlockCount;i++)
		{
			TPoolItem<T>* &item = m_List[i];
			delete[] item;
			item = NULL;
		}
		delete[] m_List;
		m_List = NULL;
		m_BlockCount = 0;
		LeaveCriticalSection(&m_lock);
	}

	return 0;
}

template<class T>
bool TPool<T>::IsFull( void ) const
{
	bool bIsFull = false;
	EnterCriticalSection(&m_lock);
	bIsFull = (m_Current >= m_Max);
	LeaveCriticalSection(m_lock);
	return bIsFull;
}

template<class T>
int TPool<T>::Delete( T* item )
{
	EnterCriticalSection(&m_lock);
	TPoolItem<T>* it = PopItem(m_Used, item);
	if (NULL != it)
	{
		it->Next = NULL;
		m_UnUsed.PushBack(it);
	}
	else
	{
		TPoolItem<T>* it = FindItem(m_UnUsed.First, item);
		if (NULL != it)
		{
			// 对象不属于此池
			//TracePrint(LOG_ERROR, "item already return to pool.");
		}
		else
		{
			// 对象不属于此池
			TracePrint(LOG_ERROR, "item not in the pool.");
		}
	}
	LeaveCriticalSection(&m_lock);

	return 0;
}

template<class T>
T* TPool<T>::New( void )
{
	TPoolItem<T>* item = NULL;

	EnterCriticalSection(&m_lock);
	if (NULL == m_UnUsed.First)
	{
		if(m_Current < m_Max)
		{
			item = Malloc();

			if (item == NULL)
			{
				// 申请内容失败
				TracePrint(LOG_ERROR, "pool malloc memory error!");
			}
			else
			{
				item = m_UnUsed.Pop();
				m_Used.Push(item);
			}
		}
		else
		{
			// 缓冲区满
			TracePrint(LOG_ERROR, "pool is full!");
		}
	}
	else
	{
		item = m_UnUsed.Pop();
		m_Used.Push(item);
	}
	LeaveCriticalSection(&m_lock);

	return &item->Item;
}
#endif



















////
////
/////*******************************************************************************
//// * Copyright (C), 2008-2010, 李  照
//// * FileName: MemoryPool.h
//// * Author: 李  照    Version: 1.0    Date: 2010-10-20
//// * Description: 内存池管理 头文件
//// * Version: 1.0     // 版本信息
//// * Function List:   // 主要函数及其功能
//// * 1. 
//// * History:         // 历史修改记录
//// * <author>   <time>   <version>   <desc>
//// *  李  照  2010-10-20    1.0    build this moudle  
//// ******************************************************************************/
////#pragma once
////
////#ifndef TCP_BLOCK_POOL_H_
////#define TCP_BLOCK_POOL_H_
////
////#include <set>
////
////using namespace std;
////
////template<class T> 
////class TPoolItem
////{
////public:
////	~TPoolItem(){  }
////	T Item;
////	TPoolItem *Next;
////};
////
////template<class T> class TPool
////{
////public:
////	// 构造函数
////	TPool(int nMax=1024, bool bAllocNow=false);
////	// 析构函数
////	~TPool();
////
////	// 初始化列表
////	int Init(int nMax=1024, bool bAllocNow=true);
////
////	// 在池上创建一个对象
////	T* New(void);
////	// 把对象归还到池
////	int Delete(T* item);
////	// 对象池是否已满
////	bool IsFull(void) const;
////	// 释放池资源
////	int Free();
////
////	// 统计消息转换为字符串
////	int ToString(char* szMsg, const int nBuffSize);
////private:
////	// 池最大对象数
////	int m_Max;
////	// 当前池中对象数
////	int m_Current;
////	// 已使用对象数
////	int m_UsedCount;
////	// 空闲对象数据
////	int m_UnUsedCount;
////	// 当前申请的内存块数
////	int m_BlockCount;
////
////	// 申请对象数步长
////	int m_Step;
////
////	// 是滞已释放
////	bool m_IsFree;
////	// 同步变更
////	CRITICAL_SECTION m_lock;
////
////	// 最后一个元素
////	T* m_LastUnUsed;
////	// 未用元素列表
////	T* m_UnUsed;
////	// 正在使用的元素集合
////	std::set<T*> m_Used;
////	// 申请的内存列表
////	T** m_List;
////
////	// 申请内容，如果申请成功，则返回为第一项，如果申请失败，则返回NULL
////	T* Malloc(void);
////};
////
////template<class T>
////int TPool<T>::ToString( char* szMsg, const int nBuffSize )
////{
////	int nUnused = 0;
////	T* p = m_UnUsed;
////	while(NULL != p)
////	{
////		nUnused ++;
////
////		p = p->Next;
////	}
////	return sprintf_s(szMsg, nBuffSize, "pool: %lu, used: %lu, unused: %lu, check: %lu", m_Current, m_UsedCount, m_UnUsedCount, nUnused);
////}
////
////template<class T>
////int TPool<T>::Init( int nMax/*=1024*/, bool bAllocNow/*=true*/ )
////{
////	this->m_Max = nMax;
////	m_IsFree = false;
////
////	if (bAllocNow)
////	{
////		m_List = new(nothrow) T*[nMax];
////		if (m_List == NULL)
////		{
////			// 对象不属于此池
////			TracePrint(LOG_ERROR, "pool malloc item list error!");
////		}
////		else
////		{
////			for (int i=0;i<nMax;i++)
////			{
////				m_List[i] = NULL;
////			}
////		}
////	}
////
////	return 0;
////}
////
////template<class T>
////T* TPool<T>::Malloc( void )
////{
////	T* item = NULL;
////
////	// 申请内存对象
////	item = new(nothrow) T[m_Step];
////	if (item == NULL)
////	{
////		// 申请内存失败，直接返回空
////		return NULL;
////	}
////	if (m_List == NULL)
////	{
////		// 初始化内存池
////		Init(m_Max, true);
////	}
////
////	m_Current += m_Step;
////	m_UnUsedCount += m_Step;
////	m_List[m_BlockCount] = item;
////	m_BlockCount ++;
////
////	// 注意这里前提是没有可用内存时才会申请新的内存
////	if (m_Step > 1)
////	{
////		m_UnUsed = &item[0];
////		m_LastUnUsed = m_UnUsed;
////	}
////
////	// 把申请到的内存，放到未使用的链表中
////	for (int i=1;i<m_Step;i++)
////	{
////		m_LastUnUsed->Next = &item[i];
////		m_LastUnUsed = &item[i];
////	}
////	if (m_LastUnUsed != NULL) m_LastUnUsed->Next = NULL;
////
////	
////	return item;
////}
////
////template<class T>
////TPool<T>::TPool(int nMax/*=1024*/, bool bAllocNow/*=false*/): m_IsFree(false), m_Current(0), m_Step(1024), 
////	m_LastUnUsed(NULL), m_UnUsed(NULL), m_UsedCount(0), m_UnUsedCount(0), m_BlockCount(0), m_List(NULL)
////{
////	m_Max = nMax;
////	InitializeCriticalSection(&m_lock);
////
////	// 初始化内存池
////	Init(nMax, bAllocNow);
////}
////
////template<class T>
////TPool<T>::~TPool()
////{
////	DeleteCriticalSection(&m_lock);
////}
////
////template<class T>
////int TPool<T>::Free()
////{
////	if (!m_IsFree)
////	{
////		EnterCriticalSection(&m_lock);
////		m_IsFree = true;
////		m_Current = 0;
////		m_LastUnUsed = NULL;
////		for (int i=0;i<m_BlockCount;i++)
////		{
////			T* &item = m_List[i];
////			delete[] item;
////			item = NULL;
////		}
////		delete[] m_List;
////		m_List = NULL;
////		m_BlockCount = 0;
////		LeaveCriticalSection(&m_lock);
////	}
////
////	return 0;
////}
////
////template<class T>
////bool TPool<T>::IsFull( void ) const
////{
////	bool bIsFull = false;
////	EnterCriticalSection(&m_lock);
////	bIsFull = (m_Current >= m_Max);
////	LeaveCriticalSection(m_lock);
////	return bIsFull;
////}
////
////template<class T>
////int TPool<T>::Delete( T* item )
////{
////	EnterCriticalSection(&m_lock);
////	if (m_Used.count(item) == 1)
////	{
////		m_Used.erase(item);
////		item->Next = NULL;
////		if(m_LastUnUsed == NULL)
////		{
////			m_UnUsed = item;
////		}
////		else
////		{
////			m_LastUnUsed->Next = item;
////		}
////		m_LastUnUsed = item;
////
////		m_UsedCount--;
////		m_UnUsedCount++;
////	}
////	else
////	{
////		bool bHasDeleted = false;
////		T* p = m_UnUsed;
////		for (int i=0;i<m_UnUsedCount && NULL != p;i++)
////		{
////			if (p == item) 
////			{
////				bHasDeleted = true;
////				break;
////			}
////			p = p->Next;
////		}
////		if (bHasDeleted)
////		{
////			// 对象不属于此池
////			TracePrint(LOG_ERROR, "item already return to pool.");
////		}
////		else
////		{
////			// 对象不属于此池
////			TracePrint(LOG_ERROR, "item not in the pool.");
////		}
////	}
////	LeaveCriticalSection(&m_lock);
////
////	return 0;
////}
////
////template<class T>
////T* TPool<T>::New( void )
////{
////	T* item = NULL;
////
////	EnterCriticalSection(&m_lock);
////	if (m_UnUsed == NULL)
////	{
////		if(m_Current < m_Max)
////		{
////			item = Malloc();
////
////			if (item == NULL)
////			{
////				// 申请内容失败
////				TracePrint(LOG_ERROR, "pool malloc memory error!");
////			}
////			else
////			{
////				item = m_UnUsed;
////				m_Used.insert(m_UnUsed);
////				m_UnUsed = m_UnUsed->Next;
////
////				// 修改记数信息
////				m_UsedCount++;
////				m_UnUsedCount--;
////			}
////		}
////		else
////		{
////			// 缓冲区满
////			TracePrint(LOG_ERROR, "pool is full!");
////		}
////	}
////	else
////	{
////		item = m_UnUsed;
////		m_Used.insert(m_UnUsed);
////
////		m_UnUsed = m_UnUsed->Next;
////
////		// 处理到最后一个元素了
////		if (m_UnUsed == NULL) m_LastUnUsed = NULL;
////
////		// 修改记数信息
////		m_UsedCount++;
////		m_UnUsedCount--;
////	}
////	LeaveCriticalSection(&m_lock);
////
////	return item;
////}
////#endif