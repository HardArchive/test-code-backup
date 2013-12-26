// Queue.h: interface for the Queue class.
//
//	1��n lock free����
//	����ģʽ��1��nд��n��1д
//	push��pop���Ӷȣ�O(1)
//////////////////////////////////////////////////////////////////////

#ifndef MDK_QUEUE_H
#define MDK_QUEUE_H

#include "FixLengthInt.h"

namespace mdk
{

class Queue  
{
	typedef	struct QUEUE_NODE
	{
		void *pObject;
		bool IsEmpty;
	}QUEUE_NODE;
	
public:
	Queue( int nSize );
	virtual ~Queue();

public:
	bool Push( void *pObject );
	void* Pop();
	void Clear();//�������
protected:
	
private:
	QUEUE_NODE *m_queue;
	uint32 m_push;
	uint32 m_pop;
	uint32 m_nSize;
	int32 m_nWriteAbleCount;
	int32 m_nReadAbleCount;
};

}

#endif // MDK_QUEUE_H
