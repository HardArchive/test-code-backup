/******************************************************************************
Module:  PrivateHeap.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: 管理进程私有堆.
Desc:
		 1. CPrivateHeap:		自动创建和销毁进程私有堆
								每一个该类的对象都代表一个私有堆, 所以
								该类对象的特点是: 一般声明周期都比较长
								通常作为全局对象, 其他类的静态成员对象
								或者一些长生命周期类对象的成员对象
		 2. CPrivateHeapBuffer: 在私有堆中自动分配和释放指定大小的内存
								一般用于在函数体内分配和释放局部作用域的堆内存
								从而避免对 CPrivateHeap::Alloc() 和 
								CPrivateHeap::Free() 的调用

Examples:
			CPrivateHeap g_hpPrivate;

			int _tmain(int argc, _TCHAR* argv[])
			{
				CPrivateHeapStrBuffer buff(g_hpPrivate, 32);
				lstrcpy(buff, _T("失败乃成功之母"));
				DWORD size = buff.Size();
				buff.ReAlloc(40 * sizeof(TCHAR));
				size = buff.Size();
				std::cout << (TCHAR*)buff << '\n';
				// OR
				// ASSERT(g_hpPrivate.IsValid());
				// TCHAR* pch	= (TCHAR*)g_hpPrivate.Alloc(32 * sizeof(TCHAR));
				// lstrcpy(pch, _T("失败乃成功之母"));
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
	//HeapAlloc函数
	//1）遍历分配的和释放的内存块的链接表；
	//2）寻找一个空闲内存块的地址；
	//3）通过将空闲内存块标记为“已分配”分配新内存块；
	//4）将新内存块添加给内存块链接表；
	//注意，在堆中分配的内存块只能是固定的内存块，不想GlobalAlloc函数一样可以分配可移动的内存块。
	// m_heap用于标识分配的内存块来自的堆栈的句柄，size参数用于设定从堆栈中分配的内存块的字节数
	//从私有堆heap中分配内存块  options 以下标识的组合：HEAP_NO_SERIALIZE HEAP_GENERATE_EXCEPTIONS HEAP_ZERO_MEMORY
	PVOID Alloc(DWORD size, EnAllocOptions options = AO_DEFAULT)
		{return ::HeapAlloc(m_heap, options, size);}

	//改变内存块的大小
	PVOID ReAlloc(PVOID pvmem, DWORD size, EnReAllocOptions options = RAO_DEFAULT)
	{
		//可以改变堆中某一块内存的大小
		return ::HeapReAlloc(m_heap, options, pvmem, size);
	}

	//检索内存块的实际大小 参数hHeap用于标识堆栈，参数pvMem用于指明内存块的地址，参数fdwFlags既可以是0，也可以是HEAP_NO_SERIALIZE
	DWORD Size(PVOID pvmem, EnSizeOptions options = SO_DEFAULT)
		{return (DWORD)::HeapSize(m_heap, options, pvmem);}

	BOOL Free(PVOID pvmem, EnFreeOptions options = FO_DEFAULT)
		{return ::HeapFree(m_heap, options, pvmem);}

	//合并堆中的空闲内存块并释放不在使用中的内存页面
	DWORD Comapct(EnCompactOptions options = CPO_DEFAULT)
		{return (DWORD)::HeapCompact(m_heap, options);}

	BOOL IsValid() {return m_heap != NULL;}

public:
	//在原有默认堆的基础上动态创建一个堆
	//如果该值为0，那么将创建一个可扩展的堆，堆的大小仅受可用内存的限制。
	//如果应用程序需要分配大的内存块，通常要将该参数设置为0。如果dwMaximumSize大于0，则该值限定了堆所能创建的最大值
	//HANDLE HeapCreate(
	//	DWORD flOptions,  //指定堆的属性：
			//HEAP_GENERATE_EXCEPTIONS---指定函数失败时返回值，不指定这个标志时，函数失败时返回NULL、否则返回一个具体的错误代码
			//HEAP_NO_SERIALIZE---控制对私有堆的访问是否要进行独占性的检测；指定这个标志时，建立堆时不进行独占性检测，访问速度可以更快
	//	DWORD dwInitialSize,   //创建堆时分配给堆的物理内存（堆的内存不足时可以自动扩展）
	//	DWORD dwMaximumSize);  //能够扩展到的最大物理内存
	CPrivateHeap(EnCreateOptions options = CO_DEFAULT, DWORD initsize = 0, DWORD maxsize = 0)
		{m_heap = ::HeapCreate(options, initsize, maxsize);}

	//HeapDestroy函数释放私有堆可以释放堆中包含的所有内存块，也可以将堆占用的物理内存和保留的地址空间全部返还给系统。
	//如果函数运行成功，返回值是TRUE；
	//当在进程终止时没有调用HeapDestry函数将私有堆释放时，系统会自动释放。
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
