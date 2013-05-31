// Lock.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////
#include "../../include/mdk/Lock.h"

namespace mdk
{

//////////////////////////////////////////////////////////////////////////
//ʵ��
AutoLock::AutoLock( Mutex *pLock )
:m_pMutex(pLock), m_bLocked(true)
{
	m_pMutex->Lock();
}
AutoLock::~AutoLock()
{
	Unlock();
}

void AutoLock::Unlock()
{
	if ( m_bLocked )
	{
		m_pMutex->Unlock();
		m_bLocked = false;
	}
}

Mutex::Mutex()
{
#ifdef WIN32
	InitializeCriticalSection( &m_mutex );
#else
	int kind = PTHREAD_MUTEX_FAST_NP;
	pthread_mutexattr_t mutexattr;//����������
	
	int nError = 0;
	if ( 0 != (nError = pthread_mutexattr_init( &mutexattr )) ) return;
	if ( 0 != (nError = pthread_mutexattr_settype( &mutexattr, kind )) ) return;
	if ( 0 != (nError = pthread_mutex_init( &m_mutex,  &mutexattr )) ) return;
	if ( 0 != (nError = pthread_mutexattr_destroy( &mutexattr )) ) return;
#endif
}

Mutex::~Mutex()
{
#ifdef WIN32
	DeleteCriticalSection( &m_mutex );
#else
	pthread_mutex_destroy( &m_mutex );
#endif
}
 
void Mutex::Lock()
{
#ifdef WIN32
	EnterCriticalSection( &m_mutex );
#else
	pthread_mutex_lock( &m_mutex );
#endif
}

void Mutex::Unlock()
{
#ifdef WIN32
	LeaveCriticalSection( &m_mutex );
#else
	pthread_mutex_unlock( &m_mutex );
#endif
}

}//namespace mdk
