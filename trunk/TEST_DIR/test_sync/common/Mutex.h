//file Mutex.h
/***************************************************************************************************
* 1、 File       ： Mutex.h
* 2、 Version    ： *.*
* 3、 Description： 封装Win32互斥量内核对象，用于同步
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-28 15:42:35
* 6、 History    ： 
* 7、 Remark     ： 互斥量用于线程的互斥
*					互斥：是指某一资源同时只允许一个访问者对其进行访问，具有唯一性和排它性。但互斥无法限制访问者对资源的访问顺序，即访问是无序的。
*					一个互斥量只能用于一个资源的互斥访问
****************************************************************************************************/
#ifndef __MUTEX_UTIL_H__
#define __MUTEX_UTIL_H__

#ifndef __GENERAL_HELPER_H__
#include "GeneralHelper.h"
#endif

namespace RG
{
	namespace SYNC
	{
		class CMTX
		{
		public:
			//构造函数 lpName对象名  bInitialOwner true为创建线程拥有  pSecurity安全属性
			CMTX(LPCTSTR lpName = NULL, bool bInitialOwner = false, LPSECURITY_ATTRIBUTES pSecurity = NULL)	
			{
				//找出当前系统是否已经存在指定进程的实例。如果没有则创建一个互斥体。CreateMutex（）函数可用来创建一个有名或无名的互斥量对象
				//原型 HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName);
				//参数
				//LPSECURITY_ATTRIBUTES lpMutexAttributes, //指向安全属性的指针
				//BOOL                  bInitialOwner,     //初始化互斥对象的所有者 FALSE表示Mutex不属于任何线程 一个Mutex在不属于任何线程的时候是处于激发态的， 所以处于有信号状态
				//LPCTSTR               lpName             //指向互斥对象名的指针
				//返回值	Long，如执行成功，就返回互斥体对象的句柄；零表示出错。会设置GetLastError。
				//即使返回的是一个有效句柄，但倘若指定的名字已经存在，GetLastError也会设为ERROR_ALREADY_EXISTS 
				__try
				{
					m_hMutex = ::CreateMutex(pSecurity, bInitialOwner, lpName);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					//EXCEPTION_EXECUTE_HANDLER      找到异常处理模块
					//EXCEPTION_CONTINUE_SEARCH      向上一层的try except继续搜索 
					//EXCEPTION_CONTINUE_EXECUTION   忽略这个异常
					int iError = ::GetLastError();
					//在这里应该打印出 错误码				
					//ASSERT(IsValid());       //断言
				}		
			}

			~CMTX(){void Close();}

			//打开一个现有的已命名互斥体对象 lpName对象名 dwAccess完全访问访问属性 bInheritHandle 是否为子进程继承 
			bool Open(LPCTSTR lpName, DWORD dwAccess = MUTEX_ALL_ACCESS, bool bInheritHandle = false)
			{
				Close();

				__try
				{
					//函数功能：为现有的一个已命名互斥体对象创建一个新句柄
					//函数原型： HANDLE OpenMutex(
					//				DWORDdwDesiredAccess, // access
					//				BOOLbInheritHandle, // inheritance option
					//				LPCTSTRlpName // object name
					//				);
					//参数：	dwDesiredAccess：
					//			MUTEX_ALL_ACCESS 请求对互斥体的完全访问
					//			MUTEX_MODIFY_STATE 允许使用 ReleaseMutex 函数
					//			SYNCHRONIZE 允许互斥体对象同步使用
					//bInheritHandle : 如希望子进程能够继承句柄，则为TRUE
					//lpName ：要打开对象的名字
					//返回值：如执行成功，返回对象的句柄；零表示失败。若想获得更多错误信息，请调用GetLastError函数。
					//备注：一旦不再需要，注意一定要用 CloseHandle 关闭互斥体句柄。如对象的所有句柄都已关闭，那么对象也会删除		
					m_hMutex = ::OpenMutex(dwAccess, bInheritHandle, lpName);
				}
				__except(EXCEPTION_EXECUTE_HANDLER) 
				{
					//EXCEPTION_EXECUTE_HANDLER      找到异常处理模块
					//EXCEPTION_CONTINUE_SEARCH      向上一层的try except继续搜索 
					//EXCEPTION_CONTINUE_EXECUTION   忽略这个异常
					int iError = ::GetLastError();
					//在这里应该打印出 错误码	
					//ASSERT(IsValid());
				}	
				return(IsValid());
			}

			//本线程获得了互斥量，所以互斥量由有信号变为没信号
			void Lock(DWORD dwMilliseconds = INFINITE)	{::WaitForSingleObject(m_hMutex, dwMilliseconds);}
			//把互斥量变为有信号
			void Unlock()								{::ReleaseMutex(m_hMutex);}

			HANDLE& GetHandle	() 	{return m_hMutex;}
			operator HANDLE		()	{return m_hMutex;}
			HANDLE* operator &	()	{return &m_hMutex;}
			bool IsValid		()	{return m_hMutex != NULL;}

		private:
			CMTX(const CMTX& mtx);
			CMTX operator = (const CMTX& mtx);
			void Close()
			{
				if(!IsValid()) return;
				VERIFY(::CloseHandle(m_hMutex));
				m_hMutex = NULL;
			}

		private:
			HANDLE m_hMutex;
		};
	}
}
#endif /*__MUTEX_UTIL_H__*/

