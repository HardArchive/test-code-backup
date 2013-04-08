
/*******************************************************************************
 * Copyright (C), 2008-2010, ��  ��
 * FileName: MemoryPool.h
 * Author: ��  ��    Version: 1.0    Date: 2010-10-20
 * Description: �ڴ�ع��� ͷ�ļ�
 * Note: 1. ʹ��ʱ��ע��Գض�����ڴ����ʱ�������飬�������󣬻ᵼ���ڴ��
 *       ���������ָ����������޷�Ԥ�ϵĽ����
 *       2. ���ڴ�ض���ʹ����new�����ڴ棬����Ч����Ȼ����һ��������ǿ��Է���
 *       ʹ�ñ�׼���string�ȶ���ĳ�ʼ��������
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. 
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2010-10-20    1.0    build this moudle  
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
	// ��Ŀ����
	int Count;
	// ��һ����¼ָ��
	TPoolItem<T>* First;
	// ���һ����¼ָ��
	TPoolItem<T>* Last;


	TPoolItemHeader(): Count(0), First(NULL), Last(NULL){}

	// ������ѹ������
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

	// ������ѹ������
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

	// �Ӷ����ﵯ������
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
	// ���캯��
	TPool(int nMax=40000, int nStep=10000, bool bAllocNow=false);
	// ��������
	~TPool();

	// ��ʼ���б�
	int Init(int nMax=1024, int nStep=1024, bool bAllocNow=true);

	// �ڳ��ϴ���һ������
	T* New(void);
	// �Ѷ���黹����
	int Delete(T* item);
	// ������Ƿ�����
	bool IsFull(void) const;
	// �ͷų���Դ
	int Free();

	// ��ӡ��ǰ�ص�ʹ�����������¼����d:\p1_%d.log���͡�d:\p2_%d.log���ļ�
	int TracePool(const int nSeqNo);
	// ͳ����Ϣת��Ϊ�ַ���
	int ToString(char* szMsg, const int nBuffSize);
private:
	// ����������
	int m_Max;
	// ��ǰ���ж�����
	int m_Current;
	////// ��ʹ�ö�����
	////int m_UsedCount;
	////// ���ж�������
	////int m_UnUsedCount;
	// ��ǰ������ڴ����
	int m_BlockCount;

	// �������������
	int m_Step;

	// �������ͷ�
	bool m_IsFree;
	// ͬ�����
	CRITICAL_SECTION m_lock;

	//// ���һ��Ԫ��
	//TPoolItem<T>* m_LastUnUsed;
	// δ��Ԫ���б�
	TPoolItemHeader<T> m_UnUsed;
	// ����ʹ�õ�Ԫ�ؼ���
	TPoolItemHeader<T> m_Used;
	// ������ڴ��б�
	TPoolItem<T>** m_List;

	// �������ݣ��������ɹ����򷵻�Ϊ��һ��������ʧ�ܣ��򷵻�NULL
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
			// �������ڴ˳�
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
	// �����ڴ����
	//item = (TPoolItem<T>*)malloc(nMallocSize);
	item = new(nothrow) TPoolItem<T>[m_Step];
	if (item == NULL)
	{
		// �����ڴ�ʧ�ܣ�ֱ�ӷ��ؿ�
		return NULL;
	}
	if (m_List == NULL)
	{
		// ��ʼ���ڴ��
		Init(m_Max, true);
	}

	m_Current += m_Step;
	m_List[m_BlockCount] = item;
	m_BlockCount ++;

	// �����뵽���ڴ棬�ŵ�δʹ�õ�������
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

	// ��ʼ���ڴ��
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
			// �������ڴ˳�
			//TracePrint(LOG_ERROR, "item already return to pool.");
		}
		else
		{
			// �������ڴ˳�
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
				// ��������ʧ��
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
			// ��������
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
//// * Copyright (C), 2008-2010, ��  ��
//// * FileName: MemoryPool.h
//// * Author: ��  ��    Version: 1.0    Date: 2010-10-20
//// * Description: �ڴ�ع��� ͷ�ļ�
//// * Version: 1.0     // �汾��Ϣ
//// * Function List:   // ��Ҫ�������书��
//// * 1. 
//// * History:         // ��ʷ�޸ļ�¼
//// * <author>   <time>   <version>   <desc>
//// *  ��  ��  2010-10-20    1.0    build this moudle  
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
////	// ���캯��
////	TPool(int nMax=1024, bool bAllocNow=false);
////	// ��������
////	~TPool();
////
////	// ��ʼ���б�
////	int Init(int nMax=1024, bool bAllocNow=true);
////
////	// �ڳ��ϴ���һ������
////	T* New(void);
////	// �Ѷ���黹����
////	int Delete(T* item);
////	// ������Ƿ�����
////	bool IsFull(void) const;
////	// �ͷų���Դ
////	int Free();
////
////	// ͳ����Ϣת��Ϊ�ַ���
////	int ToString(char* szMsg, const int nBuffSize);
////private:
////	// ����������
////	int m_Max;
////	// ��ǰ���ж�����
////	int m_Current;
////	// ��ʹ�ö�����
////	int m_UsedCount;
////	// ���ж�������
////	int m_UnUsedCount;
////	// ��ǰ������ڴ����
////	int m_BlockCount;
////
////	// �������������
////	int m_Step;
////
////	// �������ͷ�
////	bool m_IsFree;
////	// ͬ�����
////	CRITICAL_SECTION m_lock;
////
////	// ���һ��Ԫ��
////	T* m_LastUnUsed;
////	// δ��Ԫ���б�
////	T* m_UnUsed;
////	// ����ʹ�õ�Ԫ�ؼ���
////	std::set<T*> m_Used;
////	// ������ڴ��б�
////	T** m_List;
////
////	// �������ݣ��������ɹ����򷵻�Ϊ��һ��������ʧ�ܣ��򷵻�NULL
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
////			// �������ڴ˳�
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
////	// �����ڴ����
////	item = new(nothrow) T[m_Step];
////	if (item == NULL)
////	{
////		// �����ڴ�ʧ�ܣ�ֱ�ӷ��ؿ�
////		return NULL;
////	}
////	if (m_List == NULL)
////	{
////		// ��ʼ���ڴ��
////		Init(m_Max, true);
////	}
////
////	m_Current += m_Step;
////	m_UnUsedCount += m_Step;
////	m_List[m_BlockCount] = item;
////	m_BlockCount ++;
////
////	// ע������ǰ����û�п����ڴ�ʱ�Ż������µ��ڴ�
////	if (m_Step > 1)
////	{
////		m_UnUsed = &item[0];
////		m_LastUnUsed = m_UnUsed;
////	}
////
////	// �����뵽���ڴ棬�ŵ�δʹ�õ�������
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
////	// ��ʼ���ڴ��
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
////			// �������ڴ˳�
////			TracePrint(LOG_ERROR, "item already return to pool.");
////		}
////		else
////		{
////			// �������ڴ˳�
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
////				// ��������ʧ��
////				TracePrint(LOG_ERROR, "pool malloc memory error!");
////			}
////			else
////			{
////				item = m_UnUsed;
////				m_Used.insert(m_UnUsed);
////				m_UnUsed = m_UnUsed->Next;
////
////				// �޸ļ�����Ϣ
////				m_UsedCount++;
////				m_UnUsedCount--;
////			}
////		}
////		else
////		{
////			// ��������
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
////		// �������һ��Ԫ����
////		if (m_UnUsed == NULL) m_LastUnUsed = NULL;
////
////		// �޸ļ�����Ϣ
////		m_UsedCount++;
////		m_UnUsedCount--;
////	}
////	LeaveCriticalSection(&m_lock);
////
////	return item;
////}
////#endif