#pragma once
/***************************************************************************************************
* 1、 File       ： Pool
* 2、 Version    ： 1.0
* 3、 Description： 私有堆管理类
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-3 17:03:09
* 6、 History    ： 
* 7、 Remark     ： 		 1. CPrivateHeap:		自动创建和销毁进程私有堆
*							每一个该类的对象都代表一个私有堆, 所以该类对象的特点是: 一般声明周期都比较长
*							通常作为全局对象, 其他类的静态成员对象或者一些长生命周期类对象的成员对象
*						 2. CPrivateHeapBuffer: 在私有堆中自动分配和释放指定大小的内存
*							一般用于在函数体内分配和释放局部作用域的堆内存
*							从而避免对 CPrivateHeap::Alloc() 和 CPrivateHeap::Free() 的调用
*					DWORD 第一个参数fdwOptions用于修改如何在堆栈上执行各种操作。你可以设定 0、HEAP_NO_SERIALIZE、HEAP_GENERATE_EXCEPTIONS或者是这两个标志的组合。
					#define HEAP_NO_SERIALIZE               0x00000001     //禁用线程安全访问（互斥访问）功能 一般不使用
																		   // (例外
																		   // 1、只使用一个线程
																		   // 2、使用多个线程，但是只有单个线程访问该堆栈
																		   // 3、用多个线程，但是它设法使用其他形式的互斥机制，
																		   //	  如关键代码段、互斥对象和信标（第8、9章中介绍） ，以便设法自己访问堆栈)   
					#define HEAP_GROWABLE                   0x00000002      
					#define HEAP_GENERATE_EXCEPTIONS        0x00000004     //在堆中分配或重新分配内存块失败时抛出异常 
					#define HEAP_ZERO_MEMORY                0x00000008     //返回前内存清零 
					#define HEAP_REALLOC_IN_PLACE_ONLY      0x00000010      
					#define HEAP_TAIL_CHECKING_ENABLED      0x00000020      
					#define HEAP_FREE_CHECKING_ENABLED      0x00000040      
					#define HEAP_DISABLE_COALESCE_ON_FREE   0x00000080      
					#define HEAP_CREATE_ALIGN_16            0x00010000      
					#define HEAP_CREATE_ENABLE_TRACING      0x00020000      
					#define HEAP_CREATE_ENABLE_EXECUTE      0x00040000     //分配的内存允许用于代码执行“数据保护执行特性”如果不设置此标志，则在堆内存块中执行代码时，系统会抛出异常 
					#define HEAP_MAXIMUM_TAG                0x0FFF              
					#define HEAP_PSEUDO_TAG_FLAG            0x8000              
					#define HEAP_TAG_SHIFT                  18         
****************************************************************************************************/
#include <Windows.h>
class CPrivateHeap
{
public:
	//创建堆使用 
	enum EnCreateOptions
	{
		CO_DEFAULT				= 0,
		CO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,             //禁用线程安全访问（互斥访问）功能 
		CO_GENERATE_EXCEPTIONS	= HEAP_GENERATE_EXCEPTIONS,
		CO_NOSERIALIZE_GENERATEEXCEPTIONS
								= HEAP_NO_SERIALIZE	|
								  HEAP_GENERATE_EXCEPTIONS
	};

	//分配内存块使用
	enum EnAllocOptions
	{
		AO_DEFAULT				= 0,
		AO_ZERO_MEMORY			= HEAP_ZERO_MEMORY,              //返回前内存清零
		AO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,             //禁用线程安全访问（互斥访问）功能 一般不使用
																 // (例外
																 // 1、只使用一个线程
																 // 2、使用多个线程，但是只有单个线程访问该堆栈
																 // 3、用多个线程，但是它设法使用其他形式的互斥机制，
																 //	   如关键代码段、互斥对象和信标（第8、9章中介绍） ，以便设法自己访问堆栈)
		AO_GENERATE_EXCEPTIONS	= HEAP_GENERATE_EXCEPTIONS,      //在堆中分配或重新分配内存块失败时抛出异常
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

	//改变内存块大小
	enum EnReAllocOptions
	{
		RAO_DEFAULT					= 0,
		RAO_ZERO_MEMORY				= HEAP_ZERO_MEMORY,             //返回前内存清零
		RAO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,            //禁用线程安全访问（互斥访问）功能
		RAO_GENERATE_EXCEPTIONS		= HEAP_GENERATE_EXCEPTIONS,     //如果无法分配内存块时抛出异常
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
		{m_hHeap = ::HeapCreate(options, initsize, maxsize);}

		//HeapDestroy函数释放私有堆可以释放堆中包含的所有内存块，也可以将堆占用的物理内存和保留的地址空间全部返还给系统。
		//如果函数运行成功，返回值是TRUE；
		//当在进程终止时没有调用HeapDestry函数将私有堆释放时，系统会自动释放。
		~CPrivateHeap() {Distory();}

public:
	//HeapAlloc函数
	//1）遍历分配的和释放的内存块的链接表；
	//2）寻找一个空闲内存块的地址；
	//3）通过将空闲内存块标记为“已分配”分配新内存块；
	//4）将新内存块添加给内存块链接表；
	//注意，在堆中分配的内存块只能是固定的内存块，不想GlobalAlloc函数一样可以分配可移动的内存块。
	//m_heap用于标识分配的内存块来自的堆栈的句柄，size参数用于设定从堆栈中分配的内存块的字节数
	//从私有堆heap中分配内存块  options 以下标识的组合：HEAP_NO_SERIALIZE HEAP_GENERATE_EXCEPTIONS HEAP_ZERO_MEMORY
	PVOID Alloc(DWORD size, EnAllocOptions options = AO_DEFAULT)
		{return ::HeapAlloc(m_hHeap, options, 
		size);}

	//改变内存块的大小
	PVOID ReAlloc(PVOID pvmem, DWORD size, EnReAllocOptions options = RAO_DEFAULT)
	{
		//可以改变堆中某一块内存的大小
		return ::HeapReAlloc(m_hHeap, options, pvmem, size);
	}

	//检索内存块的实际大小 参数hHeap用于标识堆栈，参数pvMem用于指明内存块的地址，参数fdwFlags既可以是0，也可以是HEAP_NO_SERIALIZE
	DWORD Size(PVOID pvmem, EnSizeOptions options = SO_DEFAULT)
		{return (DWORD)::HeapSize(m_hHeap, options, pvmem);}

	bool Free(PVOID pvmem, EnFreeOptions options = FO_DEFAULT)
		{return (bool)::HeapFree(m_hHeap, options, pvmem);}

	bool Distory()
	{
		bool bRet = false;
		bRet = IsValid()?(bool)::HeapDestroy(m_hHeap):true;
		m_hHeap = NULL;
		return bRet;
	}

	//合并堆中的空闲内存块并释放不在使用中的内存页面
	DWORD Comapct(EnCompactOptions options = CPO_DEFAULT)
		{return (DWORD)::HeapCompact(m_hHeap, options);}

	bool IsValid() {return m_hHeap != NULL;}


private:
	CPrivateHeap(const CPrivateHeap&);
	CPrivateHeap operator = (const CPrivateHeap&);

private:
	HANDLE	m_hHeap;
};


/***************************************************************************************************
* 1、 File       ： Pool
* 2、 Version    ： 1.0
* 3、 Description： 内存池管理
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-3 16:59:35
* 6、 History    ： 
* 7、 Remark     ： 1. 使用时请注意对池对象的内存操作时的溢出检查，如果溢出后，会导致内存池
*					   对象的链表指针错误，无现无法预料的结果。
*					2. 此内存池对象使用了私有堆CPrivateHeap申请内存 有助于提高应用程序性能
****************************************************************************************************/
#include "com_module/PrivateHeap.h"
#include <vector>
using namespace std;
template <class _T>
class TPool
{
public:
	// 构造函数
	TPool(){InitializeCriticalSection(&m_lock);m_pVec.clear();}
	// 析构函数
	~TPool(){Reset();DeleteCriticalSection(&m_lock);}

public:
	//// 在池上创建一个对象
	//_T* New(void)
	//{
	//	m_pVec.push_back(m_clsPrivateHeap.Alloc(sizeof(_T)));
	//	return (_T*)m_pVec.back();
	//}

	// 在池上创建iCount个对象
	_T* New(int iCount = 1)
	{
		EnterCriticalSection(&m_lock);
		m_pVec.push_back(m_clsPrivateHeap.Alloc(iCount*sizeof(_T), CPrivateHeap::AO_ZEROMEMORY_GENERATEEXCEPTIONS));
		LeaveCriticalSection(&m_lock);
		return (_T*)m_pVec.back();
	}

	// 把对象归还到池
	int Delete(_T* pItem)
	{
		EnterCriticalSection(&m_lock);
		bool bRet = true;
		vector<PVOID>::iterator intiter;    //将intiter声明为int类型的向量容器迭代器		
		for(intiter = m_pVec.begin(); intiter!=m_pVec.end(); intiter++)
		{
			if ((PVOID)pItem == *intiter)
			{
				bRet = m_clsPrivateHeap.Free((PVOID)pItem);
				m_pVec.erase(intiter);
				break;
			}
		}
		LeaveCriticalSection(&m_lock);
		return bRet;
	}

	//合并池中的空闲内存块并释放不在使用中的内存页面
	DWORD Comapct()
	{
		EnterCriticalSection(&m_lock);
		return m_clsPrivateHeap.Comapct();
		LeaveCriticalSection(&m_lock);
	}

	//释放池内所有对象
	bool Free()
	{
		return Reset();
	}

private:
	//释放池内所有对象
	bool Reset()
	{
		EnterCriticalSection(&m_lock);
		m_pVec.clear();
		return m_clsPrivateHeap.Distory();
		LeaveCriticalSection(&m_lock);
	}
private:
	vector<PVOID> m_pVec;      //指针向量
	CRITICAL_SECTION m_lock;
	CPrivateHeap m_clsPrivateHeap;   
};
