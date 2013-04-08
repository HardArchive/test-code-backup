
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
*  LZ		2012-12-03	  1.0	 
* 1. TPool.PopItem add time limit val.
*  LZ		2013-02-19	  1.0	 
* 1. TPool.PopItem fix last item bug
* 2. 
******************************************************************************/
#pragma once

#ifndef TCP_BLOCK_POOL_H_
#define TCP_BLOCK_POOL_H_
//#include "stdafx.h"
#include "lztempl.h"
#include "Trace.h"
#include "ThreadLock.h"

using namespace std;

//申请的内存链表
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

	// 数据复位
	inline void Reset()
	{
		ZeroMemory(this, sizeof(*this));
	}

	// 链接实际长度
	inline int Length()
	{
		int nLen = 0;
		TPoolItem<T>* p = this->First;
		while(NULL != p)
		{
			nLen ++;
			p = p->Next;
		}
		return nLen;
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
			this->First = item->Next;
			item->Next = NULL;
		}

		if (0 == this->Count || this->First == NULL)
		{
			this->Reset();
		}
		return item;
	}
};

template<class T> 
class TPool
{
public:
	// 构造函数
	TPool(int nMax=1024, int nStep=1024, bool bAllocNow=false);
	// 析构函数
	~TPool();

	// 初始化列表
	int Init(int nMax=1024, int nStep=1024, bool bAllocNow=true);

	// 在池上创建一个对象
	T* New();
	// 在池上创建一个对象
	int New(TListHeader<T> &header);
	// 把对象归还到池
	int Delete(T* item);
	// 把对象归还到池
	int Delete(TListHeader<T> &header);
	// 对象池是否已满
	bool IsFull(void) const;
	// 释放池资源
	int Free();

	// 内存池复位
	void Reset();

	// 打印当前池的使用情况，并记录到“d:\p1_%d.log”和“d:\p2_%d.log”文件
	int TracePool(const int nSeqNo);
	// 统计消息转换为字符串
	int ToString(char* szMsg, const int nBuffSize);
	// 检验链表
	inline int Check();

	// 当前已用长度
	inline int Length() const;
	// 当前容量
	inline int Capacity() const;
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
	CCriticalSection m_lock;

	//// 最后一个元素
	//TPoolItem<T>* m_LastUnUsed;
	
	// 未用元素列表
	TPoolItemHeader<T> m_UnUsed;
	// 正在使用的元素集合
	TPoolItemHeader<T> m_Used;
	// 所有已经申请的内存列表
	TPoolItem<T>** m_List;

	// 申请内容，如果申请成功，则返回为第一项，如果申请失败，则返回NULL
	TPoolItem<T>* Malloc(void);
	// 
	TPoolItem<T>* FindItem(TPoolItem<T>* &itFirst, const T* item);
	// 
	TPoolItem<T>* PopItem(TPoolItemHeader<T> &header, const T* item);
	// 
	int PopItems(TPoolItemHeader<T> &header, TListHeader<T> &objHeader);
	// 
	int PushItems(TPoolItemHeader<T> &header, TListHeader<T> &objHeader);

};

template<class T>
int TPool<T>::Check()
{
	int nUnused = m_UnUsed.Length(), nUsed = m_Used.Length();
	if (m_Used.Count != nUsed || m_UnUsed.Count != nUnused)
	{
		TracePrint(LOG_ERROR, "pool check error[%08x]: %lu, used: %lu, unused: %lu, check: (%lu, %lu)", (u_int)this, m_Current, m_Used.Count, m_UnUsed.Count, nUsed, nUnused);
	}
}

template<class T>
void TPool<T>::Reset()
{
	m_lock.Lock();
	for (int i=0;i<m_Used.Count; i++)
	{
		m_UnUsed.PushBack(m_Used.Pop());
	}
	m_lock.Unlock();
}

template<class T>
int TPool<T>::Capacity() const
{
	return m_Current;
}
template<class T>
int TPool<T>::Length() const
{
	return m_Used.Count;
}

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
	int i = 0;

	////TracePrint(LOG_DEBUG, "pool pop item start...");
	////this->Check();
	while(NULL != p && i<header.Count)
	{
		if (&p->Item == item)
		{
			if (NULL == pPre)
			{
				// 第一个元素处理
				header.First = p->Next;
			}
			else if(p == header.Last)
			{
				// 最后一个元素处理
				pPre->Next = NULL;
				header.Last = pPre;	
			}
			else
			{
				// 中间元素
				pPre->Next = p->Next;
			}

			it = p;
			p->Next = NULL;
			header.Count--;

			// 强制复位一次，应该是不需要做的处理
			if (0 == header.Count)
			{
				header.Reset();
			}
			break;
		}

		i++;
		pPre = p;
		p = p->Next;
	}
	////this->Check();
	////TracePrint(LOG_DEBUG, "pool pop item end.");

	return it;
}


template<class T>
int TPool<T>::PopItems( TPoolItemHeader<T> &header, TListHeader<T> &objHeader )
{
	int nCount = objHeader.Count;
	TPoolItem<T>* it = NULL;
	TPoolItem<T> *pPre = NULL, *p = header.First;
	int i = 0;
	while(NULL != p && i<header.Count)
	{
		if (&p->Item == objHeader.First)
		{
			p = FindItem(p, objHeader.Last);

			if (NULL == pPre)
			{
				header.First = p ? p->Next : NULL;
			}
			else
			{
				pPre->Next = p ? p->Next : NULL;
			}

			header.Count -= objHeader.Count;
			break;
		}

		i++;
		pPre = p;
		p = p->Next;
	}

	return 0;
}


template<class T>
int TPool<T>::PushItems( TPoolItemHeader<T> &header, TListHeader<T> &objHeader )
{
	if (NULL == objHeader.First || 0 == objHeader.Count)
	{
		return header.Count;
	}

	objHeader.Last->Next = NULL;
	header.Count += objHeader.Count;
	if (NULL == header.Last)
	{
		header.First = FindItem(header.First, objHeader.First);
	}
	else
	{
		header.Last->Next = FindItem(header.First, objHeader.First);
	}
	header.Last = FindItem(header.First, objHeader.Last);

	return header.Count;

}

template<class T>
int TPool<T>::ToString( char* szMsg, const int nBuffSize )
{
	int nUnused = m_UnUsed.Length(), nUsed = m_Used.Length();
	if (m_Used.Count != nUsed || m_UnUsed.Count != nUnused)
	{
		TracePrint(LOG_ERROR, "pool check error[%08x]: %lu, used: %lu, unused: %lu, check: (%lu, %lu)", (u_int)this, m_Current, m_Used.Count, m_UnUsed.Count, nUsed, nUnused);
	}
	return sprintf_s(szMsg, nBuffSize, "pool: %lu, used: %lu, unused: %lu, check: (%lu, %lu)", m_Current, m_Used.Count, m_UnUsed.Count, nUsed, nUnused);
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

	m_UnUsed.Reset();
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
	m_lock.Init();

	// 初始化内存池
	Init(nMax, m_Step, bAllocNow);
}

template<class T>
TPool<T>::~TPool()
{
	m_lock.Free();
}

template<class T>
int TPool<T>::Free()
{
	if (!m_IsFree)
	{
		m_lock.Lock();
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
		m_lock.Unlock();
	}

	return 0;
}

template<class T>
bool TPool<T>::IsFull( void ) const
{
	bool bIsFull = false;
	m_lock.Lock();
	bIsFull = (m_Current >= m_Max);
	m_lock.Unlock();
	return bIsFull;
}

template<class T>
int TPool<T>::Delete( TListHeader<T> &header )
{
	if (header.Count <= 0)
	{
		return 0;
	}

	T* item = header.First;
	m_lock.Lock();

	int nCount = header.Count;
	TPoolItem<T>* it = NULL;
	TPoolItem<T> *pPre = NULL, *p = m_Used.First;
	int i = 0;
	while(NULL != p && i<m_Used.Count)
	{
		if (&p->Item == header.First)
		{
			if (NULL == m_UnUsed.First)
			{
				m_UnUsed.First = p;
			}
			else
			{
				m_UnUsed.Last->Next = p;
			}

			p = FindItem(m_Used.First, header.Last);

			if (NULL == pPre)
			{
				m_Used.First = p->Next;
			}
			else
			{
				pPre->Next = p->Next;
			}

			m_UnUsed.Last = p;
			p->Next = NULL;
			if (header.Count == m_Used.Count)
			{
				m_Used.Last = NULL;
			}
			m_Used.Count -= header.Count;
			m_UnUsed.Count += header.Count;
			break;
		}

		i++;
		pPre = p;
		p = p->Next;
	}

	m_lock.Unlock();

	return 0;
}

template<class T>
int TPool<T>::Delete( T* item )
{
	m_lock.Lock();

	TPoolItem<T>* it = PopItem(m_Used, item);
	if (NULL != it)
	{
		it->Next = NULL;
		m_UnUsed.PushBack(it);
	}
	else
	{
		TPoolItem<T>* it = FindItem(m_UnUsed.First, item);

		char szMsg[256] = {0};
		this->ToString(szMsg, 255);
		if (NULL != it)
		{
			// 对象不属于此池
			//TracePrint(LOG_ERROR, "item already return to pool.");
		}
		else
		{
			// 对象不属于此池
			TracePrint(LOG_ERROR, "item[%08X] not in the pool.\n%s", (u_int)item, szMsg);
		}
	}
	m_lock.Unlock();

	return 0;
}

template<class T>
int TPool<T>::New( TListHeader<T> &header )
{
	TPoolItem<T>* item = NULL;

	m_lock.Lock();
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
				if (header.Count > m_UnUsed.Count) header.Count = m_UnUsed.Count;

				for (int i=0;i<header.Count;i++)
				{

					item = m_UnUsed.Pop();
					m_Used.PushBack(item);
					header.Push(item->Item);

				}
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
		if (header.Count > m_UnUsed.Count) header.Count = m_UnUsed.Count;

		for (int i=0;i<header.Count;i++)
		{

			item = m_UnUsed.Pop();
			m_Used.PushBack(item);
			header.Push(item->Item);

		}
	}
	m_lock.Unlock();

	return header.Count;
}

template<class T>
T* TPool<T>::New( void )
{
	TPoolItem<T>* item = NULL;

	m_lock.Lock();
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
				m_Used.PushBack(item);
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
		m_Used.PushBack(item);
	}
	m_lock.Unlock();

	return &item->Item;
}
#endif

