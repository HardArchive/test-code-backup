#include "../../include/mdk/MemoryPool.h"
#include "../../include/mdk/atom.h"
#include "../../include/mdk/Lock.h"
#include <new>

namespace mdk
{
	
MemoryPool::MemoryPool()
{
	m_resizeCtrl = new Mutex;
}

MemoryPool::MemoryPool( unsigned short uMemorySize, unsigned short uMemoryCount )
{
	m_resizeCtrl = new Mutex;
	if ( !Init( uMemorySize, uMemoryCount ) ) throw;
}
 
MemoryPool::~MemoryPool()
{
	if ( NULL != m_pNext ) delete m_pNext;
	delete[]m_pMemery;
	m_pMemery = NULL;
	m_uFreeCount = 0;
	m_pNext = NULL;
	if ( NULL != m_resizeCtrl )
	{
		Mutex* pMutex = (Mutex*)m_resizeCtrl;
		m_resizeCtrl = NULL;
		delete pMutex;
	}
}

//��ʼ���ڴ��
bool MemoryPool::Init(unsigned short uMemorySize, unsigned short uMemoryCount)
{
	if ( 0 >= uMemoryCount || 0 >= uMemorySize ) return false;//�ڴ������ڴ��С�����0
	unsigned char uAddrSize = sizeof(this);
	m_uMemoryCount = uMemoryCount;
	m_uMemorySize = uMemorySize;
	//Ԥ�����ڴ�=MemoryPool���������ַ��8byte��֧��64λ��Ѱַ
	//			+uMemoryCount���ڴ�鳤��
	//			(�ڴ�鳤��=4byte����Ƿ����+2byte����+2byte���
	//						+ʵ�ʷ�����ⲿʹ�õ��ڴ�ĳ���)
	m_pMemery = new unsigned char[8+(MEMERY_INFO + uMemorySize) * m_uMemoryCount];
	if ( NULL == m_pMemery ) return false;
	unsigned long nPos = 0;
	uint64 uThis = (uint64)this;
	//��¼�����ַ
	//ͷ8���ֽڱ�����������ַ�����ڴ��ڴ��ַ�ҵ��ڴ�ض����ַ
	if ( 8 == uAddrSize )
	{
		m_pMemery[nPos++] = (unsigned char)(uThis >> 56);
		m_pMemery[nPos++] = (unsigned char)(uThis >> 48);
		m_pMemery[nPos++] = (unsigned char)(uThis >> 40);
		m_pMemery[nPos++] = (unsigned char)(uThis >> 32);
	}
	else
	{
		m_pMemery[nPos++] = 0;
		m_pMemery[nPos++] = 0;
		m_pMemery[nPos++] = 0;
		m_pMemery[nPos++] = 0;
	}
	m_pMemery[nPos++] = (unsigned char)(uThis >> 24);
	m_pMemery[nPos++] = (unsigned char)(uThis >> 16);
	m_pMemery[nPos++] = (unsigned char)(uThis >> 8);
	m_pMemery[nPos++] = (unsigned char)uThis;
	//��ʼ���ڴ�
	unsigned short i;
	for ( i = 0; i < m_uMemoryCount; i++ )
	{
		//״̬δ����
		m_pMemery[nPos++] = 0;
		m_pMemery[nPos++] = 0;
		m_pMemery[nPos++] = 0;
		m_pMemery[nPos++] = 0;
		//�����ֽ�
		m_pMemery[nPos++] = 0;
		m_pMemery[nPos++] = 0;
		//�����ڴ����
		m_pMemery[nPos++] = (unsigned char) (i >> 8);
		m_pMemery[nPos++] = (unsigned char) i;
		nPos += uMemorySize;
	}
	m_pNext = NULL;
	//���δ�����ڴ���
	m_uFreeCount = m_uMemoryCount;

	return true;
}

//�����ڴ�
void* MemoryPool::AllocMethod()
{
	void *pObject = NULL;
	int i = 0;
	int nBlockStartPos = 8 + i;
	for ( i = 0; i < m_uMemoryCount; i++ )
	{
		if ( 0 == m_pMemery[nBlockStartPos] )//�����ڴ棬���з���
		{
			if ( 0 == AtomAdd(&m_pMemery[nBlockStartPos], 1) )//�ɹ����Ϊ�ѷ���
			{
				nBlockStartPos += MEMERY_INFO;
				pObject = &(m_pMemery[nBlockStartPos]);
				break;
			}
			//else �ѱ�����߳����ȱ��
		}
		//��λ����һ���ڴ�
		nBlockStartPos += MEMERY_INFO + m_uMemorySize;
	}
	return pObject;
}

void* MemoryPool::Alloc()
{
	//�ҵ��ɷ�����ڴ��
	MemoryPool *pBlock = this;
	for ( ; NULL != pBlock; pBlock = pBlock->m_pNext )
	{
		if ( 0 < (int32)AtomDec(&pBlock->m_uFreeCount, 1) ) break;//�ɷ���
		AtomAdd(&pBlock->m_uFreeCount, 1);
		if ( NULL == pBlock->m_pNext ) //�����ڴ�ض�û�п����ڴ�ɷ��䣬����һ���ڴ��
		{
			AutoLock lock((Mutex*)m_resizeCtrl);
			if ( NULL == pBlock->m_pNext ) 
			{
				pBlock->m_pNext = new MemoryPool( m_uMemorySize, m_uMemoryCount );
			}
		}
	}
	return pBlock->AllocMethod();
}

void MemoryPool::FreeMethod(unsigned char* pObject)
{
	//�ڴ���Ϊδ����
	pObject = pObject - MEMERY_INFO;
	AtomSet(pObject,0);
	AtomSelfAdd(&m_uFreeCount);//�ɷ������+1
	
	return;
}

void MemoryPool::Free(void* pObj)
{
	unsigned char *pObject = (unsigned char*)pObj;
	MemoryPool *pBlock = GetMemoryBlock( pObject );	
	pBlock->FreeMethod(pObject);

	return;
}

MemoryPool* MemoryPool::GetMemoryBlock(unsigned char* pObj)
{
	unsigned short uIndex = GetMemoryIndex( pObj );
	unsigned char *pMemery = pObj - (uIndex * (MEMERY_INFO+m_uMemorySize)) - MEMERY_INFO - 8;
	uint64 pBlock = 0;
	pBlock = pMemery[0];
	pBlock = (pBlock << 8) + pMemery[1];
	pBlock = (pBlock << 8) + pMemery[2];
	pBlock = (pBlock << 8) + pMemery[3];
	pBlock = (pBlock << 8) + pMemery[4];
	pBlock = (pBlock << 8) + pMemery[5];
	pBlock = (pBlock << 8) + pMemery[6];
	pBlock = (pBlock << 8) + pMemery[7];
	return (MemoryPool*)pBlock;
}

unsigned short MemoryPool::GetMemoryIndex(unsigned char* pObj)
{
	unsigned char *pMemory = pObj - MEMERY_INFO;
	unsigned short uIndex = 0;
	uIndex = pMemory[6];
	uIndex = (uIndex << 8) + pMemory[7];

	return uIndex;
}

}//namespace mdk
