// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "testPrivateHeap.h"

void test_g_heap()
{
	// �����ʹ�� CPrivateHeapBuffer<T> ����װ���ڴ�

	ASSERT(g_heap.IsValid());
	TCHAR* pch    = (TCHAR*)g_heap.Alloc(32 * sizeof(TCHAR));
	lstrcpy(pch, _T("ʧ���˳ɹ�֮ĸ"));
	DWORD size = g_heap.Size(pch);
	g_heap.ReAlloc(pch, 40 * sizeof(TCHAR));
	size = g_heap.Size(pch);
	std::cout << pch << '\n';

	// ��Ҫ��ʽ�ͷŶ��ڴ�
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
