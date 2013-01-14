/******************************************************************************
Module:  PrivateHeap.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: �������˽�ж�.
Desc:
		 1. CPrivateHeap:		�Զ����������ٽ���˽�ж�
								ÿһ������Ķ��󶼴���һ��˽�ж�, ����
								���������ص���: һ���������ڶ��Ƚϳ�
								ͨ����Ϊȫ�ֶ���, ������ľ�̬��Ա����
								����һЩ���������������ĳ�Ա����
		 2. CPrivateHeapBuffer: ��˽�ж����Զ�������ͷ�ָ����С���ڴ�
								һ�������ں������ڷ�����ͷžֲ�������Ķ��ڴ�
								�Ӷ������ CPrivateHeap::Alloc() �� 
								CPrivateHeap::Free() �ĵ���

Examples:
			CPrivateHeap g_hpPrivate;

			int _tmain(int argc, _TCHAR* argv[])
			{
				CPrivateHeapStrBuffer buff(g_hpPrivate, 32);
				lstrcpy(buff, _T("ʧ���˳ɹ�֮ĸ"));
				DWORD size = buff.Size();
				buff.ReAlloc(40 * sizeof(TCHAR));
				size = buff.Size();
				std::cout << (TCHAR*)buff << '\n';
				// OR
				// ASSERT(g_hpPrivate.IsValid());
				// TCHAR* pch	= (TCHAR*)g_hpPrivate.Alloc(32 * sizeof(TCHAR));
				// lstrcpy(pch, _T("ʧ���˳ɹ�֮ĸ"));
				// DWORD size = g_hpPrivate.Size(pch);
				// g_hpPrivate.ReAlloc(pch, 40 * sizeof(TCHAR));
				// size = g_hpPrivate.Size(pch);
				// std::cout << pch << '\n';
				// g_hpPrivate.Free(pch);
				// 
				return 0;
			}

******************************************************************************/

#pragma once

class CPrivateHeap
{
public:
	enum EnCreateOptions
	{
		CO_DEFAULT				= 0,
		CO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,
		CO_GENERATE_EXCEPTIONS	= HEAP_GENERATE_EXCEPTIONS,
		CO_NOSERIALIZE_GENERATEEXCEPTIONS
								= HEAP_NO_SERIALIZE	|
								  HEAP_GENERATE_EXCEPTIONS
	};

	enum EnAllocOptions
	{
		AO_DEFAULT				= 0,
		AO_ZERO_MEMORY			= HEAP_ZERO_MEMORY,
		AO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,
		AO_GENERATE_EXCEPTIONS	= HEAP_GENERATE_EXCEPTIONS,
		AO_ZEROMEMORY_NOSERIALIZE
								= HEAP_ZERO_MEMORY	|
								  HEAP_NO_SERIALIZE,
		AO_ZEROMEMORY_GENERATEEXCEPTIONS
								= HEAP_ZERO_MEMORY	|
								  HEAP_GENERATE_EXCEPTIONS,
		AO_NOSERIALIZE_GENERATESEXCEPTIONS
								= HEAP_NO_SERIALIZE	|
								  HEAP_GENERATE_EXCEPTIONS,
		AO_ZEROMEMORY_NOSERIALIZE_GENERATESEXCEPTIONS
								= HEAP_ZERO_MEMORY	|
								  HEAP_NO_SERIALIZE	|
								  HEAP_GENERATE_EXCEPTIONS
	};

	enum EnReAllocOptions
	{
		RAO_DEFAULT					= 0,
		RAO_ZERO_MEMORY				= HEAP_ZERO_MEMORY,
		RAO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,
		RAO_GENERATE_EXCEPTIONS		= HEAP_GENERATE_EXCEPTIONS,
		RAO_REALLOC_IN_PLACE_ONLY	= HEAP_REALLOC_IN_PLACE_ONLY,
		RAO_ZEROMEMORY_NOSERIALIZE
									= HEAP_ZERO_MEMORY			|
									  HEAP_NO_SERIALIZE,
		RAO_ZEROMEMORY_GENERATEEXCEPTIONS
									= HEAP_ZERO_MEMORY			|
									  HEAP_GENERATE_EXCEPTIONS,
		RAO_ZEROMEMORY_REALLOCINPLACEONLY
									= HEAP_ZERO_MEMORY			|
									  HEAP_REALLOC_IN_PLACE_ONLY,
		RAO_NOSERIALIZE_GENERATESEXCEPTIONS
									= HEAP_NO_SERIALIZE			|
									  HEAP_GENERATE_EXCEPTIONS,
		RAO_NOSERIALIZE_REALLOCINPLACEONLY
									= HEAP_NO_SERIALIZE			|
									  HEAP_REALLOC_IN_PLACE_ONLY,
		RAO_GENERATESEXCEPTIONS_REALLOCINPLACEONLY
									= HEAP_GENERATE_EXCEPTIONS	|
									  HEAP_REALLOC_IN_PLACE_ONLY,
		RAO_ZEROMEMORY_NOSERIALIZE_GENERATESEXCEPTIONS
									= HEAP_ZERO_MEMORY			|
									  HEAP_NO_SERIALIZE			|
									  HEAP_GENERATE_EXCEPTIONS,
		RAO_ZEROMEMORY_NOSERIALIZE_REALLOCINPLACEONLY
									= HEAP_ZERO_MEMORY			|
									  HEAP_NO_SERIALIZE			|
									  HEAP_REALLOC_IN_PLACE_ONLY,
		RAO_ZEROMEMORY_GENERATESEXCEPTIONS_REALLOCINPLACEONLY
									= HEAP_ZERO_MEMORY			|
									  HEAP_GENERATE_EXCEPTIONS	|
									  HEAP_REALLOC_IN_PLACE_ONLY,
		RAO_NOSERIALIZE_GENERATESEXCEPTIONS_REALLOCINPLACEONLY
									= HEAP_NO_SERIALIZE			|
									  HEAP_GENERATE_EXCEPTIONS	|
									  HEAP_REALLOC_IN_PLACE_ONLY,
		RAO_ZEROMEMORY_NOSERIALIZE_GENERATESEXCEPTIONS_REALLOCINPLACEONLY
									= HEAP_ZERO_MEMORY			|
									  HEAP_NO_SERIALIZE			|
									  HEAP_GENERATE_EXCEPTIONS	|
									  HEAP_REALLOC_IN_PLACE_ONLY
	};

	enum EnSizeOptions
	{
		SO_DEFAULT		= 0,
		SO_NO_SERIALIZE	= HEAP_NO_SERIALIZE
	};
	
	enum EnFreeOptions
	{
		FO_DEFAULT		= 0,
		FO_NO_SERIALIZE	= HEAP_NO_SERIALIZE
	};

	enum EnCompactOptions
	{
		CPO_DEFAULT		= 0,
		CPO_NO_SERIALIZE	= HEAP_NO_SERIALIZE
	};

public:
	//HeapAlloc����
	//1����������ĺ��ͷŵ��ڴ������ӱ�
	//2��Ѱ��һ�������ڴ��ĵ�ַ��
	//3��ͨ���������ڴ����Ϊ���ѷ��䡱�������ڴ�飻
	//4�������ڴ����Ӹ��ڴ�����ӱ�
	//ע�⣬�ڶ��з�����ڴ��ֻ���ǹ̶����ڴ�飬����GlobalAlloc����һ�����Է�����ƶ����ڴ�顣
	// m_heap���ڱ�ʶ������ڴ�����ԵĶ�ջ�ľ����size���������趨�Ӷ�ջ�з�����ڴ����ֽ���
	//��˽�ж�heap�з����ڴ��  options ���±�ʶ����ϣ�HEAP_NO_SERIALIZE HEAP_GENERATE_EXCEPTIONS HEAP_ZERO_MEMORY
	PVOID Alloc(DWORD size, EnAllocOptions options = AO_DEFAULT)
		{return ::HeapAlloc(m_heap, options, size);}

	//�ı��ڴ��Ĵ�С
	PVOID ReAlloc(PVOID pvmem, DWORD size, EnReAllocOptions options = RAO_DEFAULT)
	{
		//���Ըı����ĳһ���ڴ�Ĵ�С
		return ::HeapReAlloc(m_heap, options, pvmem, size);
	}

	//�����ڴ���ʵ�ʴ�С ����hHeap���ڱ�ʶ��ջ������pvMem����ָ���ڴ��ĵ�ַ������fdwFlags�ȿ�����0��Ҳ������HEAP_NO_SERIALIZE
	DWORD Size(PVOID pvmem, EnSizeOptions options = SO_DEFAULT)
		{return (DWORD)::HeapSize(m_heap, options, pvmem);}

	BOOL Free(PVOID pvmem, EnFreeOptions options = FO_DEFAULT)
		{return ::HeapFree(m_heap, options, pvmem);}

	//�ϲ����еĿ����ڴ�鲢�ͷŲ���ʹ���е��ڴ�ҳ��
	DWORD Comapct(EnCompactOptions options = CPO_DEFAULT)
		{return (DWORD)::HeapCompact(m_heap, options);}

	BOOL IsValid() {return m_heap != NULL;}

public:
	//��ԭ��Ĭ�϶ѵĻ����϶�̬����һ����
	//�����ֵΪ0����ô������һ������չ�Ķѣ��ѵĴ�С���ܿ����ڴ�����ơ�
	//���Ӧ�ó�����Ҫ�������ڴ�飬ͨ��Ҫ���ò�������Ϊ0�����dwMaximumSize����0�����ֵ�޶��˶����ܴ��������ֵ
	//HANDLE HeapCreate(
	//	DWORD flOptions,  //ָ���ѵ����ԣ�
			//HEAP_GENERATE_EXCEPTIONS---ָ������ʧ��ʱ����ֵ����ָ�������־ʱ������ʧ��ʱ����NULL�����򷵻�һ������Ĵ������
			//HEAP_NO_SERIALIZE---���ƶ�˽�жѵķ����Ƿ�Ҫ���ж�ռ�Եļ�⣻ָ�������־ʱ��������ʱ�����ж�ռ�Լ�⣬�����ٶȿ��Ը���
	//	DWORD dwInitialSize,   //������ʱ������ѵ������ڴ棨�ѵ��ڴ治��ʱ�����Զ���չ��
	//	DWORD dwMaximumSize);  //�ܹ���չ������������ڴ�
	CPrivateHeap(EnCreateOptions options = CO_DEFAULT, DWORD initsize = 0, DWORD maxsize = 0)
		{m_heap = ::HeapCreate(options, initsize, maxsize);}

	//HeapDestroy�����ͷ�˽�жѿ����ͷŶ��а����������ڴ�飬Ҳ���Խ���ռ�õ������ڴ�ͱ����ĵ�ַ�ռ�ȫ��������ϵͳ��
	//����������гɹ�������ֵ��TRUE��
	//���ڽ�����ֹʱû�е���HeapDestry������˽�ж��ͷ�ʱ��ϵͳ���Զ��ͷš�
	~CPrivateHeap() {if(IsValid()) ::HeapDestroy(m_heap);}

private:
	CPrivateHeap(const CPrivateHeap&);
	CPrivateHeap operator = (const CPrivateHeap&);

private:
	HANDLE	m_heap;
};

template<class T> 
class CPrivateHeapBuffer
{
public:
	CPrivateHeapBuffer(	CPrivateHeap& heap, 
						DWORD size = 0, 
						CPrivateHeap::EnAllocOptions allocoptions	= CPrivateHeap::AO_DEFAULT, 
						CPrivateHeap::EnFreeOptions freeoptions		= CPrivateHeap::FO_DEFAULT)
	: m_hpPrivate(heap)
	, m_opFree(freeoptions)
	{
		ASSERT(m_hpPrivate.IsValid());
		m_pvMemory = (T*)m_hpPrivate.Alloc(size * sizeof(T), allocoptions);
	}

	~CPrivateHeapBuffer() {m_hpPrivate.Free(m_pvMemory, m_opFree);}

	T* ReAlloc(DWORD size, CPrivateHeap::EnReAllocOptions options = CPrivateHeap::RAO_DEFAULT)
		{return m_pvMemory = (T*)m_hpPrivate.ReAlloc(m_pvMemory, size * sizeof(T), options);}

	DWORD Size(CPrivateHeap::EnSizeOptions options = CPrivateHeap::SO_DEFAULT)
		{return m_hpPrivate.Size(m_pvMemory, options);}

	operator T* () const {return m_pvMemory;}

private:
	CPrivateHeapBuffer(const CPrivateHeapBuffer&);
	CPrivateHeapBuffer operator = (const CPrivateHeapBuffer&);

private:
	CPrivateHeap&				m_hpPrivate;
	T*							m_pvMemory;
	CPrivateHeap::EnFreeOptions m_opFree;
};

typedef CPrivateHeapBuffer<BYTE>	CPrivateHeapByteBuffer;
typedef CPrivateHeapBuffer<TCHAR>	CPrivateHeapStrBuffer;
