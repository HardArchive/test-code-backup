#pragma once
/***************************************************************************************************
* 1�� File       �� Pool
* 2�� Version    �� 1.0
* 3�� Description�� ˽�жѹ�����
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-3 17:03:09
* 6�� History    �� 
* 7�� Remark     �� 		 1. CPrivateHeap:		�Զ����������ٽ���˽�ж�
*							ÿһ������Ķ��󶼴���һ��˽�ж�, ���Ը��������ص���: һ���������ڶ��Ƚϳ�
*							ͨ����Ϊȫ�ֶ���, ������ľ�̬��Ա�������һЩ���������������ĳ�Ա����
*						 2. CPrivateHeapBuffer: ��˽�ж����Զ�������ͷ�ָ����С���ڴ�
*							һ�������ں������ڷ�����ͷžֲ�������Ķ��ڴ�
*							�Ӷ������ CPrivateHeap::Alloc() �� CPrivateHeap::Free() �ĵ���
*					DWORD ��һ������fdwOptions�����޸�����ڶ�ջ��ִ�и��ֲ�����������趨 0��HEAP_NO_SERIALIZE��HEAP_GENERATE_EXCEPTIONS��������������־����ϡ�
					#define HEAP_NO_SERIALIZE               0x00000001     //�����̰߳�ȫ���ʣ�������ʣ����� һ�㲻ʹ��
																		   // (����
																		   // 1��ֻʹ��һ���߳�
																		   // 2��ʹ�ö���̣߳�����ֻ�е����̷߳��ʸö�ջ
																		   // 3���ö���̣߳��������跨ʹ��������ʽ�Ļ�����ƣ�
																		   //	  ��ؼ�����Ρ����������ű꣨��8��9���н��ܣ� ���Ա��跨�Լ����ʶ�ջ)   
					#define HEAP_GROWABLE                   0x00000002      
					#define HEAP_GENERATE_EXCEPTIONS        0x00000004     //�ڶ��з�������·����ڴ��ʧ��ʱ�׳��쳣 
					#define HEAP_ZERO_MEMORY                0x00000008     //����ǰ�ڴ����� 
					#define HEAP_REALLOC_IN_PLACE_ONLY      0x00000010      
					#define HEAP_TAIL_CHECKING_ENABLED      0x00000020      
					#define HEAP_FREE_CHECKING_ENABLED      0x00000040      
					#define HEAP_DISABLE_COALESCE_ON_FREE   0x00000080      
					#define HEAP_CREATE_ALIGN_16            0x00010000      
					#define HEAP_CREATE_ENABLE_TRACING      0x00020000      
					#define HEAP_CREATE_ENABLE_EXECUTE      0x00040000     //������ڴ��������ڴ���ִ�С����ݱ���ִ�����ԡ���������ô˱�־�����ڶ��ڴ����ִ�д���ʱ��ϵͳ���׳��쳣 
					#define HEAP_MAXIMUM_TAG                0x0FFF              
					#define HEAP_PSEUDO_TAG_FLAG            0x8000              
					#define HEAP_TAG_SHIFT                  18         
****************************************************************************************************/
#include <Windows.h>
class CPrivateHeap
{
public:
	//������ʹ�� 
	enum EnCreateOptions
	{
		CO_DEFAULT				= 0,
		CO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,             //�����̰߳�ȫ���ʣ�������ʣ����� 
		CO_GENERATE_EXCEPTIONS	= HEAP_GENERATE_EXCEPTIONS,
		CO_NOSERIALIZE_GENERATEEXCEPTIONS
								= HEAP_NO_SERIALIZE	|
								  HEAP_GENERATE_EXCEPTIONS
	};

	//�����ڴ��ʹ��
	enum EnAllocOptions
	{
		AO_DEFAULT				= 0,
		AO_ZERO_MEMORY			= HEAP_ZERO_MEMORY,              //����ǰ�ڴ�����
		AO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,             //�����̰߳�ȫ���ʣ�������ʣ����� һ�㲻ʹ��
																 // (����
																 // 1��ֻʹ��һ���߳�
																 // 2��ʹ�ö���̣߳�����ֻ�е����̷߳��ʸö�ջ
																 // 3���ö���̣߳��������跨ʹ��������ʽ�Ļ�����ƣ�
																 //	   ��ؼ�����Ρ����������ű꣨��8��9���н��ܣ� ���Ա��跨�Լ����ʶ�ջ)
		AO_GENERATE_EXCEPTIONS	= HEAP_GENERATE_EXCEPTIONS,      //�ڶ��з�������·����ڴ��ʧ��ʱ�׳��쳣
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

	//�ı��ڴ���С
	enum EnReAllocOptions
	{
		RAO_DEFAULT					= 0,
		RAO_ZERO_MEMORY				= HEAP_ZERO_MEMORY,             //����ǰ�ڴ�����
		RAO_NO_SERIALIZE			= HEAP_NO_SERIALIZE,            //�����̰߳�ȫ���ʣ�������ʣ�����
		RAO_GENERATE_EXCEPTIONS		= HEAP_GENERATE_EXCEPTIONS,     //����޷������ڴ��ʱ�׳��쳣
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
		{m_hHeap = ::HeapCreate(options, initsize, maxsize);}

		//HeapDestroy�����ͷ�˽�жѿ����ͷŶ��а����������ڴ�飬Ҳ���Խ���ռ�õ������ڴ�ͱ����ĵ�ַ�ռ�ȫ��������ϵͳ��
		//����������гɹ�������ֵ��TRUE��
		//���ڽ�����ֹʱû�е���HeapDestry������˽�ж��ͷ�ʱ��ϵͳ���Զ��ͷš�
		~CPrivateHeap() {Distory();}

public:
	//HeapAlloc����
	//1����������ĺ��ͷŵ��ڴ������ӱ�
	//2��Ѱ��һ�������ڴ��ĵ�ַ��
	//3��ͨ���������ڴ����Ϊ���ѷ��䡱�������ڴ�飻
	//4�������ڴ����Ӹ��ڴ�����ӱ�
	//ע�⣬�ڶ��з�����ڴ��ֻ���ǹ̶����ڴ�飬����GlobalAlloc����һ�����Է�����ƶ����ڴ�顣
	//m_heap���ڱ�ʶ������ڴ�����ԵĶ�ջ�ľ����size���������趨�Ӷ�ջ�з�����ڴ����ֽ���
	//��˽�ж�heap�з����ڴ��  options ���±�ʶ����ϣ�HEAP_NO_SERIALIZE HEAP_GENERATE_EXCEPTIONS HEAP_ZERO_MEMORY
	PVOID Alloc(DWORD size, EnAllocOptions options = AO_DEFAULT)
		{return ::HeapAlloc(m_hHeap, options, 
		size);}

	//�ı��ڴ��Ĵ�С
	PVOID ReAlloc(PVOID pvmem, DWORD size, EnReAllocOptions options = RAO_DEFAULT)
	{
		//���Ըı����ĳһ���ڴ�Ĵ�С
		return ::HeapReAlloc(m_hHeap, options, pvmem, size);
	}

	//�����ڴ���ʵ�ʴ�С ����hHeap���ڱ�ʶ��ջ������pvMem����ָ���ڴ��ĵ�ַ������fdwFlags�ȿ�����0��Ҳ������HEAP_NO_SERIALIZE
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

	//�ϲ����еĿ����ڴ�鲢�ͷŲ���ʹ���е��ڴ�ҳ��
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
* 1�� File       �� Pool
* 2�� Version    �� 1.0
* 3�� Description�� �ڴ�ع���
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-3 16:59:35
* 6�� History    �� 
* 7�� Remark     �� 1. ʹ��ʱ��ע��Գض�����ڴ����ʱ�������飬�������󣬻ᵼ���ڴ��
*					   ���������ָ����������޷�Ԥ�ϵĽ����
*					2. ���ڴ�ض���ʹ����˽�ж�CPrivateHeap�����ڴ� ���������Ӧ�ó�������
****************************************************************************************************/
#include "com_module/PrivateHeap.h"
#include <vector>
using namespace std;
template <class _T>
class TPool
{
public:
	// ���캯��
	TPool(){InitializeCriticalSection(&m_lock);m_pVec.clear();}
	// ��������
	~TPool(){Reset();DeleteCriticalSection(&m_lock);}

public:
	//// �ڳ��ϴ���һ������
	//_T* New(void)
	//{
	//	m_pVec.push_back(m_clsPrivateHeap.Alloc(sizeof(_T)));
	//	return (_T*)m_pVec.back();
	//}

	// �ڳ��ϴ���iCount������
	_T* New(int iCount = 1)
	{
		EnterCriticalSection(&m_lock);
		m_pVec.push_back(m_clsPrivateHeap.Alloc(iCount*sizeof(_T), CPrivateHeap::AO_ZEROMEMORY_GENERATEEXCEPTIONS));
		LeaveCriticalSection(&m_lock);
		return (_T*)m_pVec.back();
	}

	// �Ѷ���黹����
	int Delete(_T* pItem)
	{
		EnterCriticalSection(&m_lock);
		bool bRet = true;
		vector<PVOID>::iterator intiter;    //��intiter����Ϊint���͵���������������		
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

	//�ϲ����еĿ����ڴ�鲢�ͷŲ���ʹ���е��ڴ�ҳ��
	DWORD Comapct()
	{
		EnterCriticalSection(&m_lock);
		return m_clsPrivateHeap.Comapct();
		LeaveCriticalSection(&m_lock);
	}

	//�ͷų������ж���
	bool Free()
	{
		return Reset();
	}

private:
	//�ͷų������ж���
	bool Reset()
	{
		EnterCriticalSection(&m_lock);
		m_pVec.clear();
		return m_clsPrivateHeap.Distory();
		LeaveCriticalSection(&m_lock);
	}
private:
	vector<PVOID> m_pVec;      //ָ������
	CRITICAL_SECTION m_lock;
	CPrivateHeap m_clsPrivateHeap;   
};
