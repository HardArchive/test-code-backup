// ACEMemory.h
// ���˶�һ���ڴ滺��أ���������ACE����֧���ڴ滺��ء�
// �ҵĻ������Ϊ��ACE DLL�����棬��ACE�ڲ������ҵ��ڴ�ز������������Ļ���ء�
// ��������ʱ�䣬���ҵ��ҵ��ڴ�س�ͻ�ı���ԭ��
// Ҫ����ACE֧���ҵ��ڴ�أ��͵ø�ACE��Դ�룬���ҵ��ڴ������DLL���롣�����������İɡ�
// add by freeeyes
// 2009-08-03

#include "ACEMemory.h"

PACKET_DATA_ALLOCTOR* CACEMemory::m_pPacketDataAlloctor = NULL;
CACEMemory::CACEMemory(void)
{
}

CACEMemory::~CACEMemory(void)
{
	OUR_DEBUG((LM_ERROR, "[CACEMemory::~CACEMemory].\n"));
	delete m_pPacketDataAlloctor;
	OUR_DEBUG((LM_ERROR, "[CACEMemory::~CACEMemory]End.\n"));
}

void* CACEMemory::malloc(size_t stSize)
{
	if(NULL == m_pPacketDataAlloctor)
	{
		//return NULL;
		m_pPacketDataAlloctor = new PACKET_DATA_ALLOCTOR();
	}
	
	return m_pPacketDataAlloctor->malloc(stSize);
}

bool CACEMemory::free(void* p)
{
	m_pPacketDataAlloctor->free(p);
	return true;
}
