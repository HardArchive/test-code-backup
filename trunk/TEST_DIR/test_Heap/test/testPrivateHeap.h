#pragma once
#include <iostream>
#include "PrivateHeap.h"


// ȫ�ֿɼ��� Heap
CPrivateHeap g_heap;

class MyClass
{
private:
	// ����ʵ����������һ�µ� Heap
	CPrivateHeap m_heap;

	// �����ڲ��ɼ��� Heap
	static CPrivateHeap sm_clsheap;

public:
	void test_m_eap()
	{
		// ������ʽ�ͷŶ��ڴ�
		CPrivateHeapStrBuffer buff(m_heap, 32);
		lstrcpy(buff, _T("ʧ���˳ɹ�֮ĸ"));
		DWORD size = buff.Size();
		buff.ReAlloc(40 * sizeof(TCHAR));
		size = buff.Size();
		std::cout << (TCHAR*)buff << '\n';
	}

	static void test_sm_eap()
	{
		CPrivateHeapStrBuffer buff(sm_clsheap, 32);
		lstrcpy(buff, _T("ʧ���˳ɹ�֮ĸ"));
		DWORD size = buff.Size();
		buff.ReAlloc(40 * sizeof(TCHAR));
		size = buff.Size();
		std::cout << (TCHAR*)buff << '\n';
	}
};
//���徲̬��Ա ��ʼ��
CPrivateHeap MyClass::sm_clsheap;