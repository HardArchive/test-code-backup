#ifndef TOOL_C_THREAD_POOL_IMP_H
#define TOOL_C_THREAD_POOL_IMP_H
/*
	�̳߳���
	ʹ�÷���
	class A
	{
	...
	void* fun(void*);
	}
	A a;
	mdk::ThreadPool tp;
	tp.Start(10);
	//linux�±���&A::fun��windows�¿�ʹ��A::fun��A�ຯ���ڲ�ֱ��fun�Կ�
	//Ϊ����ֲ�ԣ����鴫��&A::fun��Bind()
	tp.Accept( mdk::Executor::Bind(&A::fun), &a, (void*)param );
	t.Accept( mdk::Executor::Bind(&A::fun), &a, (void*)param );
*/

#include <vector>
#include <map>

#include "Thread.h"
#include "Lock.h"
#include "Signal.h"
#include <stddef.h>

namespace mdk
{
class MemoryPool;
//�߳���Ϣ
typedef struct THREAD_CONTEXT
{
	Thread thread;	
	bool bIdle;		//�̴߳��ڿ���
	bool bRun;		//���п��Ʊ�־
}THREAD_CONTEXT;
typedef std::map<ThreadID,THREAD_CONTEXT*> threadMaps;

class Task;
class MemoryPool;
class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();
	bool Start(int nMinThreadNum);//�����̳߳�
	void Stop();//�ر������߳�
	//��������
	//methodΪ����Ϊvoid* fun(void*)�ĳ�Ա����
	void Accept( MethodPointer method, void *pObj, void *pParam );
	//��������
	//funΪ����Ϊvoid* fun(void*)�ĺ���
	void Accept( FuntionPointer fun, void *pParam );
	
protected:
	bool CreateThread(unsigned short nNum);//���̳߳��д���n���߳�
	THREAD_CONTEXT* CreateContext();//����һ��������
	void ReleaseContext(THREAD_CONTEXT *pContext);//�ͷ�һ��������
	void* RemoteCall ThreadFunc(void* pParam);//�̺߳���
	Task* CreateTask();//����һ������
	void ReleaseTask(Task *pTask);//�ͷ�һ������
	void PushTask(Task* pTask);//����������̳߳�ִ�С���ע��pTask������new�����Ķ���alloc������
	Task* PullTask();//ȡ��һ������
	//���÷��������޵�������
	void StopIdle();//�رտ����߳�(��֤��С�߳���)
	
protected:
	MemoryPool* m_pTaskPool;//Task�����
	Mutex m_taskPoolMutex;//Task������̰߳�ȫ��
	MemoryPool* m_pContextPool;//THREAD_CONTEXT�����
	Mutex m_contextPoolMutex;//THREAD_CONTEXT������̰߳�ȫ��

	unsigned short m_nMinThreadNum;//�̳߳��б�����ڵ���С�߳���
	unsigned short m_nThreadNum;//�̳߳����������߳���
	threadMaps m_threads;//�̱߳�
	Mutex m_threadsMutex;//�̱߳��̰߳�ȫ��
	std::vector<Task*> m_tasks;//�����
	Mutex m_tasksMutex;//������̰߳�ȫ��
	Signal m_sigNewTask;//�������ź�

	
};

}//namespace mdk

#endif
