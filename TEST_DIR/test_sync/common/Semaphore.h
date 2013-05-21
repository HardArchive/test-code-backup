//file Semaphore.h
/***************************************************************************************************
* 1、 File       ： Semaphore.h
* 2、 Version    ： 1.0
* 3、 Description： 信号量处理类
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-5-21 14:30:23
* 6、 History    ： 
* 7、 Remark     ： 	信号量用于线程的同步
*					同步：是指在互斥的基础上（大多数情况），通过其它机制实现访问者对资源的有序访问。
*						在大多数情况下，同步已经实现了互斥，特别是所有写入资源的情况必定是互斥的。少数情况是指可以允许多个访问者同时访问资源
*						信号量可以实现多个同类资源的多线程互斥和同步。当信号量为单值信号量时，也可以完成一个资源的互斥访问。
*					信号量的特性如下：信号量是一个非负整数（车位数），所有通过它的线程/进程（车辆）都会将该整数减一（通过它当然是为了使用资源），
*						当该整数值为零时，所有试图通过它的线程都将处于等待状态。在信号量上我们定义两种操作： Wait（等待） 和 Release（释放）。
*						当一个线程调用Wait操作时，它要么得到资源然后将信号量减一，要么一直等下去（指放入阻塞队列），直到信号量大于等于一时。
*						Release（释放）实际上是在信号量上执行加操作，对应于车辆离开停车场，该操作之所以叫做“释放”是因为释放了由信号量守护的资源。
*					对信号量只能实施三种操作：
*						1. 初始化（initialize），也叫做建立（create）
*						2. 等信号（wait），也可叫做挂起（suspend）
*						3. 给信号（signal）或发信号（post）
*					CSEM使用构造函数初始化  Wait函数等待-1  Release函数释放(给信号)+1 使用跟CMTX类似
****************************************************************************************************/
#ifndef __SEMAPHORE_UTIL_H__
#define __SEMAPHORE_UTIL_H__

#ifndef __GENERAL_HELPER_H__
#include "GeneralHelper.h"
#endif

namespace RG
{
	class CSEM
	{
	public:
		//构造函数 lMaximumCount信号量的最大计数 lInitialCount设置信号量的初始计数 lpName对象名  bInitialOwner true为创建线程拥有  pSecurity安全属性
		CSEM(LONG lMaximumCount, LONG lInitialCount = 0, LPCTSTR lpName = NULL, LPSECURITY_ATTRIBUTES pSecurity = NULL)
		{
			//作用 创建一个新的信号量
			//返回值	Long，成功 返回信号量对象句柄；零 出错,会设置GetLastError。即使返回一个有效句柄，倘若它指出同名的一个信号量已存在，则GetLastError也会返回ERROR_ALREADY_EXISTS
			//参数
			//lpSemaphoreAttributes SECURITY_ATTRIBUTES,定义信号量的安全特性;指定一个SECURITY_ATTRIBUTES结构,或传递零值（将参数声明为ByVal As Long，并传递零值—表示采用不允许继承的默认描述符）
			//lInitialCount Long,                       设置信号量的初始计数。可设置零到lMaximumCount之间的一个值
			//lMaximumCount Long,                       设置信号量的最大计数
			//lpName String,                            指定信号量对象的名称。用vbNullString可创建一个未命名的信号量对象。如果已经存在拥有这个名字的一个信号量，就直接打开现成的信号量。这个名字可能不与一个现有的互斥体、事件、可等待计时器或文件映射的名称相符
			//注解
			//一旦不再需要，一定记住用CloseHandle关闭信号量的句柄。它的所有句柄都关闭以后，对象自己也会删除
			//一旦值大于零，信号量就会触发（发出信号）。ReleaseSemaphore函数的作用是增加信号量的计数。如果成功，就调用信号量上的一个等待函数来减少它的计数			
			__try
			{
				m_hsem = ::CreateSemaphore(pSecurity, lInitialCount, lMaximumCount, lpName);
			}
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				int iError = ::GetLastError();
				//在这里应该打印出 错误码	
				//ASSERT(IsValid());
			}				
			ASSERT(IsValid());
		}

		~CSEM(){Close();}

		//打开在其他进程(线程)中创建的信号量  lpName对象名 dwAccess完全访问访问属性 bInheritHandle 是否为子进程继承 
		bool Open(LPCTSTR lpName, DWORD dwAccess = SEMAPHORE_ALL_ACCESS, bool bInheritHandle = false)
		{
			Close();
			__try
			{
				//说明 为现有的一个已命名信号机对象创建一个新句柄。
				//返回值 Long，如执行成功，返回对象句柄；零表示失败。会设置GetLastError。
				//参数
				//dwDesiredAccess Long，下述常数之一：
				//		SEMAPHORE_ALL_ACCESS 要求对事件对象的完全访问；
				//		SEMAPHORE_MODIFY_STATE 允许使用ReleaseSemaphore函数；
				//		SYNCHRONIZE 允许同步使用信号机对象。
				//bInheritHandle Long，如果允许子进程继承句柄，则设为TRUE。
				//lpName String，指定要打开的对象的名字。
				//注解 一旦不再需要，注意一定要用CloseHandle关闭信号机句柄。如对象的所有句柄都已关闭，那么对象也会删除。
				m_hsem = ::OpenSemaphore(dwAccess, bInheritHandle, lpName); 
			}
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				int iError = ::GetLastError();
				//在这里应该打印出 错误码	
				//ASSERT(IsValid());
			}		
			
			return(IsValid());
		}

		//等待信号量>0 当信号量计数大于零时 不需要等待 调用WaitForSingleObject后信号量计数减1
		void Wait(DWORD dwMilliseconds = INFINITE)
		{
			::WaitForSingleObject(m_hsem, dwMilliseconds);
		}

		//信号量++
		bool Release(LONG lReleaseCount = 1, LPLONG lpPreviousCount = NULL)
		{
			//递增信号量的当前资源计数
			return ::ReleaseSemaphore(m_hsem, lReleaseCount, lpPreviousCount);
		}

		HANDLE& GetHandle	() 	{return m_hsem;}
		operator HANDLE		()	{return m_hsem;}
		HANDLE* operator &	()	{return &m_hsem;}
		bool IsValid		()	{return m_hsem != NULL;}

	private:
		CSEM(const CSEM& sem);
		CSEM operator = (const CSEM& sem);
		void Close()
		{
			if(!IsValid()) return;
			VERIFY(::CloseHandle(m_hsem));
			m_hsem = NULL;
		}
	private:
		HANDLE m_hsem;
	};
}


#endif /*__SEMAPHORE_UTIL_H__*/