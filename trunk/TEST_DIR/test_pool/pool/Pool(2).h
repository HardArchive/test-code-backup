
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

//������ڴ�����
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

	// ���ݸ�λ
	inline void Reset()
	{
		ZeroMemory(this, sizeof(*this));
	}

	// ����ʵ�ʳ���
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
	// ���캯��
	TPool(int nMax=1024, int nStep=1024, bool bAllocNow=false);
	// ��������
	~TPool();

	// ��ʼ���б�
	int Init(int nMax=1024, int nStep=1024, bool bAllocNow=true);

	// �ڳ��ϴ���һ������
	T* New();
	// �ڳ��ϴ���һ������
	int New(TListHeader<T> &header);
	// �Ѷ���黹����
	int Delete(T* item);
	// �Ѷ���黹����
	int Delete(TListHeader<T> &header);
	// ������Ƿ�����
	bool IsFull(void) const;
	// �ͷų���Դ
	int Free();

	// �ڴ�ظ�λ
	void Reset();

	// ��ӡ��ǰ�ص�ʹ�����������¼����d:\p1_%d.log���͡�d:\p2_%d.log���ļ�
	int TracePool(const int nSeqNo);
	// ͳ����Ϣת��Ϊ�ַ���
	int ToString(char* szMsg, const int nBuffSize);
	// ��������
	inline int Check();

	// ��ǰ���ó���
	inline int Length() const;
	// ��ǰ����
	inline int Capacity() const;
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
	CCriticalSection m_lock;

	//// ���һ��Ԫ��
	//TPoolItem<T>* m_LastUnUsed;
	
	// δ��Ԫ���б�
	TPoolItemHeader<T> m_UnUsed;
	// ����ʹ�õ�Ԫ�ؼ���
	TPoolItemHeader<T> m_Used;
	// �����Ѿ�������ڴ��б�
	TPoolItem<T>** m_List;

	// �������ݣ��������ɹ����򷵻�Ϊ��һ��������ʧ�ܣ��򷵻�NULL
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
				// ��һ��Ԫ�ش���
				header.First = p->Next;
			}
			else if(p == header.Last)
			{
				// ���һ��Ԫ�ش���
				pPre->Next = NULL;
				header.Last = pPre;	
			}
			else
			{
				// �м�Ԫ��
				pPre->Next = p->Next;
			}

			it = p;
			p->Next = NULL;
			header.Count--;

			// ǿ�Ƹ�λһ�Σ�Ӧ���ǲ���Ҫ���Ĵ���
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

	m_UnUsed.Reset();
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
	m_lock.Init();

	// ��ʼ���ڴ��
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
			// �������ڴ˳�
			//TracePrint(LOG_ERROR, "item already return to pool.");
		}
		else
		{
			// �������ڴ˳�
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
				// ��������ʧ��
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
			// ��������
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
				// ��������ʧ��
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
			// ��������
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

