#ifndef __Queue_H__
#define __Queue_H__


#include "KernelObject.h"

#include <iostream.h>
#include <assert.h>
#include <fstream.h>

// You can adjust this constant to make program appliable,
// But DO NOT forget to change the constant for semaphore maximum
// wait count in KernelObject.h if it is used
const int constDEFAULTQUEUESIZE =128 ;

template <class T> class TQueue
{
	T **data;
	int size;

	T **head;
	T **tail;
	T **dataend;

	CTSemaphore  pEmptySema;     //Semaphere show that the queue is empty.
	CTSemaphore  pOccupiedSema;  //Semaphere show that the queue is occupied.
//	CTMutex		m_lock;
	CTCriticalSection m_lock;

public:
	TQueue(int aSize = constDEFAULTQUEUESIZE, int isMulti = 1);
	//Constructor, aSize=buffer size, isMulti=true means there are mutliple producer/consumer.
	~TQueue();

	void Put(T &newData);
	//Put a new data into queue, if queue is occupied that block until not occupied.
	void Get(T &buf);
	//Get a data from queue, if queue is empty that block until not empty.
	int	 Remove();
	void RemoveAll() { 	while( GetCount() )  Remove() ; }
	
	int  TryPut(T &newData);
	//Put a new data into queue, if queue is occupied that return -1 else return 0.
	int  TryGet(T &buf);
	//Get a data from queue, if queue is empty that return -1 else return 0.
//	int  TryRemove();	

	int PutHead(T &newData);
	//Put a new data into queue head, if queue is occupied that return -1 else return 0.

	int	 Peek(T &buf);

	int  GetCount();
	int  IsEmpty() { return (GetCount()==0); };
	int  IsOccupied() { return (GetCount()==size); };

private:
	int  MyPut(T &newData);
	void MyGet(T &buf);

};

//  Queue:
//         +----+----+----+----+----+----+----+----+----+
//         |T*  |T*  |    |    |    |    |    |    |    |
//         +----+----+----+----+----+----+----+----+----+
//         ^              ^              ^               ^
//         data           head           tail            dataend
//
//                        <---- count --->
//         <---------------- size ----------------->
//         <-------------- buffer size ----------------->
//
//         IsEmpty: if head=tail
//         IsOccupied: if tail=head-1

template <class T>
TQueue< T >::TQueue(int aSize, int isMulti) :
	pEmptySema(aSize,aSize),
	pOccupiedSema(0,aSize)
{
	size = aSize>0?aSize:1;

//	IsMulti = isMulti;
	typedef T* TP;
	data = new TP [size+1];
	assert(data);
	for(int i=0; i<=size; i++)
		data[i]=0;
	tail = head = data;
	dataend = data+size+1;
}

template <class T>
TQueue< T >::~TQueue()
{
	//WQX's old line: for(int i = 0; i<size;i++)
	for(int i = 0; i<=size; i++)
	{
		delete data[i];
		data[i] = 0 ;
	}
	delete [] data;	
}

template <class T>
void TQueue< T >::Put(T &newData)
{
	pEmptySema.Wait();
	MyPut(newData);
	pOccupiedSema.Post();
}

template <class T>
int  TQueue< T >::PutHead(T &newData)
{
	T *tmpdata ;
	if( IsOccupied() )
		return -1 ;

	tmpdata = new T ;
	if( tmpdata ) 
	{
		*tmpdata = newData;
		pEmptySema.Wait();

		m_lock.Lock();
			if( head == data )
				head = data+size ;
			else 
				head -- ;
		
		//*head = new T;
		//**head = (newData);
		*head = tmpdata;

		m_lock.Unlock();

		pOccupiedSema.Post();
		return 0;
	}

	return -1;	
}


template <class T>
int TQueue< T >::TryPut(T &newData)
{
	int ret = 0;

//	if( IsOccupied() )
		//return -1 ;

	DWORD err = pEmptySema.Wait(100); //try wait 100 ms
	if( err != WAIT_OBJECT_0 )
		return -1;

	ret = MyPut(newData);
	pOccupiedSema.Post();
	return ret;
}


template <class T>
int TQueue< T >::MyPut(T &newData)
{

	T *tmpdata = new T;

	if( tmpdata )
	{
		*tmpdata = newData ;

		m_lock.Lock();
		*tail = tmpdata ;

	/*try
	{
    *tail = new T;
	}
	catch (...)
	{
		ofstream o("queue.err",ios::out|ios::app);
		o << "catch(*tail = new T ) in Put() ERROR!!" << endl;
	}
    **tail = (newData);
*/
		tail++;
		/*
			tail = new <T>(newData) so you must T::T(T&) T::T(T*);
			else
				*tail++ = new <T>;
				memcpy(tail,newData,sizeof(T));
		*/

		if (tail>=dataend)
			tail = data;	
		m_lock.Unlock();
		return 0 ;
	}

	return -1;

}

template <class T>
void TQueue< T >::Get(T &buf)
{
	pOccupiedSema.Wait();
	MyGet(buf);
	pEmptySema.Post();
}

template <class T>
int TQueue< T >::TryGet(T &buf)
{
	//if( IsEmpty() )
		//return -1;
	DWORD  err= pOccupiedSema.Wait(100);
	if( err != WAIT_OBJECT_0 )
		return -1;

	MyGet(buf);
	pEmptySema.Post();

	return 0;
}

template <class T>
void  TQueue< T >::MyGet(T &buf)
{
	m_lock.Lock();
	buf = **head; // operator =
	T* prev = *head;
/*
	try
	{
		delete prev;
	}
	catch (...)
	{
		ofstream o("queue.err",ios::out|ios::app);
		o << "catch(delete prev) in Get() ERROR!!" << endl;
	}
*/

	delete prev;
	*head = NULL;
	head++;
	if (head>=dataend)
		head = data;
	m_lock.Unlock();
}


template <class T>
int TQueue< T >::Peek(T & buf)
{
	m_lock.Lock();
	buf = **head; // T a = b;
	m_lock.Unlock();
	return 0;
}

template <class T>
int TQueue< T >::Remove()
{
	if( GetCount() )  {
		T tmp;
		Get( tmp ) ;
	}
	return 0 ;

}

template <class T>
int  TQueue< T >::GetCount()
{
	int count=0;

	m_lock.Lock();
	if (tail>=head)
		count = tail-head;
	else
		count =size+1-(head-tail);
	
	m_lock.Unlock();		
	
	return count;
}

#endif
