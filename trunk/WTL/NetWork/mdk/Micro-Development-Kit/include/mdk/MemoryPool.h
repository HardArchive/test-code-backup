#ifndef MDK_MEMORY_POOL_H
#define MDK_MEMORY_POOL_H

#include "FixLengthInt.h"

#include <stdio.h>
#include <stddef.h>

#ifndef NULL
#define NULL 0
#endif
/*
 *	������(n��nд����,�������)
 * 	�ڴ�ؽṹ
 * 	n�����������
 * 
 * 	�ؽṹ
 * 	ÿ���ض���һ�������ڴ汣����char������
 * 	0~7byteΪ�ص�ַ��������n�����ڹ̶����ȷ�����ڴ�飨���ڴ���Ƕ����ģ�
 * 
 * 	�ڴ��ṹ
 * 	״̬4byte+����2byte+�ڴ������2byte
 * 	����һ���ڴ�������Ա���Ķ���(�ڴ��)����Ϊunsigned short�����ֵ65535
 *
 *	״̬��2��(����/δ����)��1��byte�Ϳ��Ա�ʾ��ʹ��4byte��Ϊ��ʹ��ԭ�Ӳ���ʵ��lock-free����ԭ�Ӳ�����������4byte��ַ
 *	2byte���գ���Ϊ�˱�֤�������ڷ�����ڴ��ص�ַ����4byte����
 *  ��Ϊ��ʵnew�����Ķ����׵�ַ�Ǳ�֤���ģ�
 *  ��IOCPͶ�ݻ���struct��ҲҪ���׵�ַ���룬���򷵻�10014����
 */
#define MEMERY_INFO 8

namespace mdk
{

/**
 * �ڴ����
 * 
 */
class MemoryPool
{	
private:
	//��һ���ڴ��
	MemoryPool* m_pNext;
	//ָ��Ԥ������Ĺ�������ڴ��
	unsigned char* m_pMemery;
	//Alloc()����ÿ�η����ȥ�ĵ�ַָ���ڴ�ռ�Ĵ�С
	unsigned short m_uMemorySize;
	//������ڴ���
	unsigned short m_uMemoryCount;
	//δ�����ڴ���
	int32 m_uFreeCount;
	void *m_resizeCtrl;
	
public:
	MemoryPool();
	MemoryPool( unsigned short uMemorySize, unsigned short uMemoryCount );
	//�����������ݹ���ͷ��������������ڴ��
	~MemoryPool();
	//��ʼ���ڴ��
	bool Init(unsigned short uMemorySize, unsigned short uMemoryCount);

	//�����ڴ�(������)
	void* Alloc();
	
	//�����ڴ�(������)
	void Free(void* pObj);

private:
	//�����ڴ�(��㷽��)
	void* AllocMethod();

	//�����ڴ�(��㷽��)
	void FreeMethod(unsigned char* pObject);	

	//ȡ�õ�ַָ���ڴ������ڴ�صĵ�ַ(������)
	MemoryPool* GetMemoryBlock(unsigned char* pObj);

	//ȡ�õ�ַָ���ڴ��ڿ������(������)
	unsigned short GetMemoryIndex(unsigned char* pObj);

	
};

}//namespace mdk

#endif //MDK_MEMORY_POOL_H
