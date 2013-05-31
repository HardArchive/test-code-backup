// Signal.cpp: implementation of the Signal class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/mdk/Signal.h"
#include "../../include/mdk/atom.h"

namespace mdk
{
	
Signal::Signal()
{
#ifdef WIN32
	m_signal = CreateEvent( NULL, false, false, NULL );
#else
	m_waitCount = 0;
	sem_init( &m_signal, 1, 0 );
#endif	
}

Signal::~Signal()
{
#ifdef WIN32
	if (NULL != m_signal) CloseHandle(m_signal);
#else
	sem_destroy(&m_signal);
#endif
}

bool Signal::Wait( unsigned long lMillSecond )
{
	bool bHasSingle = true;
#ifdef WIN32
	int nObjIndex = WaitForSingleObject( m_signal, lMillSecond );
	if ( WAIT_TIMEOUT == nObjIndex ||
		(WAIT_ABANDONED_0 <= nObjIndex && nObjIndex <= WAIT_ABANDONED_0 + 1)
		) bHasSingle = false;
#else
	AtomAdd(&m_waitCount, 1);
	unsigned long notimeout = -1;
	if ( notimeout == lMillSecond )
	{
		sem_wait( &m_signal );//�ȴ�����
	}
	else
	{
		int nSecond = lMillSecond / 1000;
		int nNSecond = (lMillSecond - nSecond * 1000) * 1000;
		timespec timeout;
		timeout.tv_sec=time(NULL) + nSecond;         
		timeout.tv_nsec=nNSecond;
		if ( 0 != sem_timedwait(&m_signal, &timeout) ) bHasSingle = false;
	}
	/*
		windows��Ϊ����û��waitʱ��notify n�� ֮�����ж��wait��ֻ��ͨ��һ��
		linux��Ϊ����û��waitʱ��notify n�� ֮�����ж��wait����ͨ��n������n+1����ʼ����
		��˵����windows�ϵ�2~n��notify���źŶ�ʧ��

		Ϊ�˺�windows��Ϊһ�£����ȴ��߳�Ϊ0ʱ����������źŶ���
	 */
	if ( 1 == AtomDec(&m_waitCount, 1) )
	{
		sem_init( &m_signal, 1, 0 );
	}
#endif
	
	return bHasSingle;
}

bool Signal::Notify()
{
#ifdef WIN32
	SetEvent( m_signal );
#else
	sem_post(&m_signal);	
#endif
	
	return true;
}

}//namespace mdk
