#include "ThreadPool.h"

//�̺߳��������ǰ�߳�ID
DWORD WINAPI t(LPVOID l);
DWORD WINAPI t1(LPVOID l);

void call(UINT m, CThreadPool::spThread sp);


int testThreadPool();