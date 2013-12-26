// Signal.h: interface for the Signal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_SINGLE_H
#define MDK_SINGLE_H

/*
	�ź���
	Ч��
	���û���߳��ڵȴ�
	���Notify��ֻ�ᱣ����1����Ч��֮��ȫ����ʧ
*/
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif


namespace mdk
{
	
class Signal  
{
public:
	Signal();
	virtual ~Signal();

	bool Wait( unsigned long nMillSecond = (unsigned long)-1 );
	bool Notify();
	
private:
#ifdef WIN32
	HANDLE m_signal;
#else
	int m_waitCount;
	sem_t m_signal;			
#endif
};

}//namespace mdk

#endif // MDK_SINGLE_H
