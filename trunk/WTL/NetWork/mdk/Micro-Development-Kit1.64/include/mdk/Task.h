#ifndef TOOL_C_TASK_H
#define TOOL_C_TASK_H

#include "Executor.h"

/**
	������
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

	mdk::Task t;
	//linux�±���&A::fun��windows�¿�ʹ��A::fun
	//Ϊ����ֲ�ԣ����鴫��&A::fun��Bind()
	t.Accept( mdk::Executor::Bind(&A::fun), &a, (void*)param );
	t.Execute();

	t.Accept( g_fun, (void*)param );
	t.Execute();
*/

namespace mdk
{
class Task
{
public:
	Task(int i);
	Task();
	virtual ~Task();
	//��������
	//methodΪ����Ϊvoid* fun(void*)�ĳ�Ա����
	void Accept( MethodPointer method, void *pObj, void *pParam );
	//��������
	//funΪ����Ϊvoid* fun(void*)�ĺ���
	void Accept( FuntionPointer fun, void *pParam );
	//ִ������
	void* Execute();
	
private:
	void *m_pParam;
	MethodPointer m_method;
	void *m_pObj;
	FuntionPointer m_fun;
};

}//namespace mdk

#endif
