// Executor.cpp: implementation of the Executor class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/mdk/Executor.h"

namespace mdk
{
	
	//__thiscall���÷�ʽ��Ա����,���̺߳���,ָ�붨��
	typedef void* (__stdcall *ThisCallFun)(void*);
	//__stdcall���÷�ʽ��Ա����,���̺߳���,ָ�붨��
	typedef void*(__stdcall *StdCallFun)(void*, void*);
	
	//ִ������Ϊvoid* fun(void*)�ĳ�Ա����
	void* WinCall( MethodPointer uMethod, void *pObj, void *pParam )
	{
		ThisCallFun method = (ThisCallFun)uMethod;
		unsigned long This = (unsigned long)pObj;
#ifdef WIN32
		__asm //׼��thisָ��.
		{
			mov ecx, This;
		}
#else//linux�»�������δʵ��
		return NULL;
#endif
		return method(pParam);
	}
	
	//ִ������Ϊvoid* _stdcall fun(void*)�ĳ�Ա����
	void* LinuxCall( MethodPointer uMethod, void *pObj, void *pParam )
	{
		StdCallFun method = (StdCallFun)uMethod;
		return method(pObj, pParam);
	}
	
Executor::Executor()
{
}

Executor::~Executor()
{
}

//ִ������Ϊvoid* fun(void*)�ĳ�Ա����
void* Executor::CallMethod( MethodPointer method, void *pObj, void *pParam )
{
#ifdef WIN32
	return LinuxCall( method, pObj, pParam );
#else
	return LinuxCall( method, pObj, pParam );
#endif
}


}//namespace mdk
