//���������˺ܶ�����߳����Ķ�������Ϊ�Ҳ������ҵ��ڴ������������ACE�Ľӿڡ�
//һ��ʼ������һ���������࿽��һ��������ʡ�¡����Ƿ���Ҫ��д��̫���ӣ�Ҫ��̫�򵥡�
//��������Ҫ�Լ�����ѽ�������������ҵ���һЩ�ȽϺõ����ӡ�
//���ͣ���Ҫ���˵�����ԣ�ҪŬ��ȥ������Ϊ�Ҽ��š�

#ifndef _THREADLOCK_H
#define _THREADLOCK_H

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

#ifdef WIN32
#define LOCK_MUTEXT CRITICAL_SECTION
#else
#define LOCK_MUTEXT pthread_mutex_t
#endif

#include "ace/OS_main.h"

class CThreadLock
{
public:
	CThreadLock(void)
	{
		Init();
	};

	~CThreadLock()
	{
		Close();
	};

	void Init()
	{
#ifdef _WIN32
		InitializeCriticalSection(&m_lock);
#else
		pthread_mutex_init(&m_lock, NULL);
#endif
	};

	void Close()
	{
#ifdef _WIN32
		DeleteCriticalSection(&m_lock);
#else
		pthread_mutex_destroy(&m_lock);
#endif
	}

	void Lock()
	{
		m_Time = ACE_OS::gettimeofday();
#ifdef _WIN32
		EnterCriticalSection(&m_lock);
#else
		pthread_mutex_lock(&m_lock);
#endif
	};

	void UnLock()
	{
		m_Time = ACE_OS::gettimeofday() - m_Time;

#ifdef _WIN32
		LeaveCriticalSection(&m_lock);
#else
		pthread_mutex_unlock(&m_lock);
#endif
	};

private:
	LOCK_MUTEXT m_lock;
	ACE_Time_Value m_Time;
};

//�Զ���������
class CAutoLock
{
public:
	CAutoLock(CThreadLock* pThreadLock) 
	{ 
		m_pThreadLock = pThreadLock;
		if(NULL != m_pThreadLock)
		{
			m_pThreadLock->Lock();
		}
	};
	~CAutoLock() 
	{
		if(NULL != m_pThreadLock)
		{
			m_pThreadLock->UnLock();
		}
	};

private:
	CThreadLock* m_pThreadLock;
};

#endif
