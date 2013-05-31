// Lock.h: interface for the Thread class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_LOCK_H
#define MDK_LOCK_H

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace mdk
{

#ifdef WIN32
#define OriginalMutex CRITICAL_SECTION
#else
	typedef pthread_mutex_t OriginalMutex;//������
#endif
	
//�Խ�����װ���Ƽ�ʹ���Խ��������������������쳣�����ǽ����Ȳ���ȫ������������
class Mutex;
class AutoLock
{
public:
	AutoLock( Mutex *pLock );
	~AutoLock();
	void Unlock();
	
private:
	Mutex *m_pMutex;
	bool m_bLocked;
};

//������������װ
class Mutex
{
private:
	//������ϵͳ����
	OriginalMutex m_mutex;
public:
	Mutex();
	~Mutex();
	//����
	void Lock();
	//����
	void Unlock();		
};

}//namespace mdk

#endif//MDK_LOCK_H
