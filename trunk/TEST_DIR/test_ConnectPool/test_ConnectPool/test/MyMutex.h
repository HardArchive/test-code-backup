#ifndef __MY_MUTEX_H
#define __MY_MUTEX_H
/******************************************************************************* 
* 1、 文件名称： MyMutex
* 2、 版　　本： Version *.*
* 3、 描    述： 仿MFC中类CCriticalSection，具体使用参考CCriticalSection类
				类CCriticalSection的对象表示一个“临界区”，它是一个用于同步的对象，同一时刻只允许一个线程存取资源或代码区。
				临界区在控制一次只有一个线程修改数据或其它的控制资源时非常有用。
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-2-20 14:09:47
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 
#include <process.h>
#include <windows.h>

namespace QNA
{
	class CCriticalSection
	{
	public:
		CCriticalSection()
		{
			::InitializeCriticalSection(&fMutex);
		}

		~CCriticalSection()
		{
			::DeleteCriticalSection(&fMutex);
		}

		//在CCriticalSection对象上加锁
		void Lock()
		{
			::EnterCriticalSection(&fMutex);
		}

		//释放CCriticalSection对象上的锁
		void Unlock()
		{
			::LeaveCriticalSection(&fMutex);
		}

	private:
		CRITICAL_SECTION fMutex;
	};
}

#endif