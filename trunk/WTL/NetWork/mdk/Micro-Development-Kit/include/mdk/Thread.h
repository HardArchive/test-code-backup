// Thread.h: interface for the Thread class.
//
//////////////////////////////////////////////////////////////////////
/*
	�߳���
	��fun�������麯��

	ʹ�÷���
	void* g_fun(void*)
	{
	return NULL;
	}
	class A
	{
		...
		void* RemoteCall fun(void*);
	}
	A a;
	
	mdk::Thread t;
	//linux�±���&A::fun��windows�¿�ʹ��A::fun
	//Ϊ����ֲ�ԣ����鴫��&A::fun��Bind
	t.Run( mdk::Executor::Bind(&A::fun), &a, (void*)param );
	mdk::Thread t2;
	t2.Run( g_fun, (void*)param );
*/
#ifndef MDK_THREAD_H
#define MDK_THREAD_H

#include "FixLengthInt.h"
#include "Task.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

namespace mdk
{
	
#ifdef WIN32
	typedef DWORD ThreadID;
#else
	typedef pthread_t ThreadID;
#endif
	
//���߳���
class Thread  
{
public:
	Thread();
	virtual ~Thread();

//////////////////////////////////////////////////////////////////////////
protected://����
	ThreadID m_nID;	
	Task m_task;
	bool m_bRun;

	//�߳��˳���������
#ifdef WIN32
	HANDLE m_hHandle;
#else
	pthread_cond_t m_exit;
	pthread_mutex_t m_exitMutex;
	bool m_bStop;
#endif
	//End�߳��˳���������
	//////////////////////////////////////////////////////////////////////////
public://�ӿ�
	bool Run( MethodPointer method, void *pObj, void *pParam );//�����̣߳�ʹ�����Ա������pParam���ݸ��̺߳����Ĳ���
	bool Run( FuntionPointer fun, void *pParam );//�����̣߳�ʹ��ȫ�ֺ�����pParam���ݸ��̺߳����Ĳ���
	/*
	 	ֹͣ
	 */
	void Stop(long lMillSecond = 0);
	/*
	 	�ȴ��߳�ֹͣ
		ע��linux��һ�����أ�֮���ٵ���WaitStop()��Stop();�����ٵȴ���ֱ�ӷ���
		��ΪWaitStop();����ǰ�ᷢ��1���߳̽����źţ�֪ͨ�������ܴ��ڵȴ����߳�
	 */
	void WaitStop();
	ThreadID GetID();//ȡ���߳�ID

protected:
	bool Run( void *pParam );//�����̣߳�pParam���ݸ��̺߳����Ĳ���
	static void* TMain(void*);
	void* Main();
};

}//namespace mdk

#endif // MDK_THREAD_H
