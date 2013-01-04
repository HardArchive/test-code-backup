// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "testPrivateHeap.h"

void test_g_heap()
{
	// 如果不使用 CPrivateHeapBuffer<T> 来封装堆内存

	ASSERT(g_heap.IsValid());
	TCHAR* pch    = (TCHAR*)g_heap.Alloc(32 * sizeof(TCHAR));
	lstrcpy(pch, _T("失败乃成功之母"));
	DWORD size = g_heap.Size(pch);
	g_heap.ReAlloc(pch, 40 * sizeof(TCHAR));
	size = g_heap.Size(pch);
	std::cout << pch << '\n';

	// 需要显式释放堆内存
	g_heap.Free(pch);
}

int _tmain(int argc, _TCHAR* argv[])
{
	test_g_heap();

	MyClass::test_sm_eap();

	MyClass c;
	c.test_m_eap();

	return 0;
}
