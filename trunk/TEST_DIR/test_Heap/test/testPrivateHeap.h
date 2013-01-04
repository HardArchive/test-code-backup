#pragma once
#include <iostream>
#include "PrivateHeap.h"


// 全局可见的 Heap
CPrivateHeap g_heap;

class MyClass
{
private:
	// 与类实例生命周期一致的 Heap
	CPrivateHeap m_heap;

	// 仅类内部可见的 Heap
	static CPrivateHeap sm_clsheap;

public:
	void test_m_eap()
	{
		// 无需显式释放堆内存
		CPrivateHeapStrBuffer buff(m_heap, 32);
		lstrcpy(buff, _T("失败乃成功之母"));
		DWORD size = buff.Size();
		buff.ReAlloc(40 * sizeof(TCHAR));
		size = buff.Size();
		std::cout << (TCHAR*)buff << '\n';
	}

	static void test_sm_eap()
	{
		CPrivateHeapStrBuffer buff(sm_clsheap, 32);
		lstrcpy(buff, _T("失败乃成功之母"));
		DWORD size = buff.Size();
		buff.ReAlloc(40 * sizeof(TCHAR));
		size = buff.Size();
		std::cout << (TCHAR*)buff << '\n';
	}
};
//定义静态成员 初始化
CPrivateHeap MyClass::sm_clsheap;