// Executor.h: interface for the Executor class.
//
//////////////////////////////////////////////////////////////////////
/*
	ִ������
	����ִ�ж���������Ϊvoid* RemoteCall fun(void*)�ĳ�Ա����
	��fun�������麯��
	
    ʹ�÷���
	class A
	{
		...
		void* RemoteCall fun(void*);
	}
	A a;
	//linux�±���&A::fun��windows�¿�ʹ��A::fun��A�ຯ���ڲ�ֱ��fun�Կ�
	//Ϊ����ֲ�ԣ����鴫��&A::fun��Bind()
	mdk::Executor::CallMethod( mdk::Executor::Bind(&A::fun), &a, (void*)param );

 */
#ifndef MDK_EXECUTOR_H
#define MDK_EXECUTOR_H

#include "FixLengthInt.h"
#ifndef NULL
#define NULL 0
#endif

#ifndef WIN32
#define __stdcall 
#endif
#define RemoteCall __stdcall

namespace mdk
{
typedef void* (*pfun)(void*);
typedef pfun FuntionPointer;
typedef uint64 MethodPointer;

class Executor  
{
public:
	Executor();
	virtual ~Executor();

	//��Ա����ָ���ַ
	//��f�������麯��
	template <typename FromType>
	static MethodPointer Bind( FromType f )
	{
		union 
		{
			FromType _f;
			MethodPointer  _t;
		}ut;
		
		ut._f = f;
		return ut._t;
	}

	//ִ������Ϊvoid* fun(void*)�ĳ�Ա����
	static void* CallMethod( MethodPointer method, void *pObj, void *pParam );
};

}//namespace mdk

#endif // MDK_EXECUTOR_H
