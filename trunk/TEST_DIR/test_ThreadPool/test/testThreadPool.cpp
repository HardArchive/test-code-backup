#include "stdafx.h"
#include "testThreadPool.h"

//线程函数输出当前线程ID
DWORD WINAPI t(LPVOID l)
{
	cout << (int)l+1<<"、"<<GetCurrentThreadId() << endl;
	return 0;
}

DWORD WINAPI t1(LPVOID l)
{
	cout << "!!!" << endl;
	return 0;
}
void call(UINT m, CThreadPool::spThread sp)
{
	if(m == WM_THREADSTART)
	{
		//  cout<<1<<endl;
	}
}


int testThreadPool()
{
	CThreadPool c;
	c.Init(0, 5);
	DWORD i = 1000;
	while(i--)
	{
		c.AddTask(t, (PVOID)i, 0);  //第一次执行---创建线程
	}
	c.AddTask(t1, 0, 1);

	return 1;
}