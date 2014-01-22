#include "ThreadPool.h"

//线程函数输出当前线程ID
DWORD WINAPI t(LPVOID l);
DWORD WINAPI t1(LPVOID l);

void call(UINT m, CThreadPool::spThread sp);


int testThreadPool();