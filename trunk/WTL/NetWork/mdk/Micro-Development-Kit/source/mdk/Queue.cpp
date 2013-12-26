// Queue.cpp: implementation of the Queue class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/mdk/Queue.h"
#include "../../include/mdk/atom.h"
#include <stdio.h>
#include <vector>

#ifndef NULL
#define NULL 0
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace mdk
{

Queue::Queue( int nSize )
{
	m_nSize = nSize;
	m_nWriteAbleCount = m_nSize;
	m_nReadAbleCount = 0;
	m_queue = new QUEUE_NODE[m_nSize];
	m_push = 0;
	m_pop = 0;
	Clear();
}

Queue::~Queue()
{
	if ( NULL == m_queue ) return;
	delete[]m_queue;
}

bool Queue::Push( void *pObject )
{
	if ( NULL == pObject ) return false;
	if ( 0 >= m_nWriteAbleCount ) return false;//�ж�����
	if ( 0 >= AtomDec(&m_nWriteAbleCount,1) ) //�Ѿ����㹻���push�������жӲ�ͬλ��д������
	{
		AtomAdd(&m_nWriteAbleCount, 1);
		return false;
	}
	uint32 pushPos = AtomAdd(&m_push, 1);
	pushPos = pushPos % m_nSize;
	/*
		ֻ����NPop��������£���Pop������ɣ���һ��λ�õ�Popδ��ɣ�
		�����Pop���������ʾ�п�λ�������������һ��λ��ʱ����������δ��ȡ��
		��Ϊ����ֻ����1��N�����Ա�Ȼ�ǵ��߳�Push( void *pObject )������������m_push����Ҫ��������
	 */
	if ( !m_queue[pushPos].IsEmpty ) 
	{
		m_push--;
		AtomAdd(&m_nWriteAbleCount,1);
		return false;
	}
	m_queue[pushPos].pObject = pObject;
	m_queue[pushPos].IsEmpty = false;
	AtomAdd(&m_nReadAbleCount,1);
	
	return true;
}

void* Queue::Pop()
{
	if ( 0 >= m_nReadAbleCount ) return NULL;//���ж�
	if ( 0 >= AtomDec(&m_nReadAbleCount,1)) //�Ѿ����㹻���pop������ȡ�жӲ�ͬλ�õ�����
	{
		AtomAdd(&m_nReadAbleCount, 1);
		return NULL;
	}
	uint32 popPos = AtomAdd(&m_pop, 1);
	popPos = popPos % m_nSize;
	/*
		ֻ����NPush��������£���Push������ɣ���һ��λ�õ�Pushδ��ɣ�
		�����Push���������ʾ�����ݣ������������һ��λ��ʱ������û�����ݿɶ�
		��Ϊ����ֻ����1��N����Ȼ�ǵ��߳�Pop()������������m_pop����Ҫ��������
	 */
	if ( m_queue[popPos].IsEmpty )
	{
		m_pop--;
		AtomAdd(&m_nReadAbleCount,1);
		return NULL;
	}
	void *pObject = m_queue[popPos].pObject;
	m_queue[popPos].pObject = NULL;
	m_queue[popPos].IsEmpty = true;
	AtomAdd(&m_nWriteAbleCount,1);

	return pObject;
}

void Queue::Clear()
{
	if ( NULL == m_queue ) return;
	uint32 i = 0;
	m_nWriteAbleCount = m_nSize;
	m_nReadAbleCount = 0;
	for ( i = 0; i < m_nSize; i++ )
	{
		m_queue[i].pObject = NULL;
		m_queue[i].IsEmpty = true;
	}
}

}//MDK_QUEUE_H
