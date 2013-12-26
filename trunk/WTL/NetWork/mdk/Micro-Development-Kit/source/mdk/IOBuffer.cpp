// IOBuffer.cpp: implementation of the IOBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/mdk/IOBuffer.h"
#include "../../include/mdk/atom.h"
#include <new>
#include <string.h>
using namespace std;

namespace mdk
{

IOBuffer::IOBuffer()
{
	m_pRecvBufferBlock = NULL;
	m_uDataSize = 0;
	m_recvBufferList.clear();
}

IOBuffer::~IOBuffer()
{
	Clear();
}

//����һ�黺���
void IOBuffer::AddBuffer()
{
	m_pRecvBufferBlock = new IOBufferBlock();//���뻺���
	if ( NULL == m_pRecvBufferBlock ) return;
	AutoLock lock( &m_mutex );
	m_recvBufferList.push_back( m_pRecvBufferBlock ); //���뻺���б�
}

/**
 * ȡ�õ�ǰ���ջ����ַ,���뽫Ҫ���յ����ݳ���
 * ���ڱ����������
 * ������볤��>��ǰ�����ʣ�೤�ȣ��򴴽��»����
 */
unsigned char* IOBuffer::PrepareBuffer(unsigned short uRecvSize)
{
	if ( uRecvSize > BUFBLOCK_SIZE ) return NULL;
	if ( NULL == m_pRecvBufferBlock ) AddBuffer();//������һ�黺���
	unsigned char* pWriteBuf = m_pRecvBufferBlock->PrepareBuffer( uRecvSize );
	//ʣ�໺�岻���Ա���ϣ��д������ݳ��ȣ����ӻ����
	if ( NULL == pWriteBuf ) 
	{
		AddBuffer();
		pWriteBuf = m_pRecvBufferBlock->PrepareBuffer( uRecvSize );
	}
	
	return pWriteBuf;//���ص�һ�������ֽڵ�ַ
}

/**
 * д�����
 * ���д�����д�����ݵĳ���
 * ������PrepareBuffer()�ɶԵ���
 */
void IOBuffer::WriteFinished(unsigned short uLength)
{
	m_pRecvBufferBlock->WriteFinished( uLength );
	AtomAdd(&m_uDataSize, uLength);
}

//����д�뻺��
bool IOBuffer::WriteData( char *data, int nSize )
{
	unsigned char *ioBuf = PrepareBuffer( nSize );
	if ( NULL == ioBuf ) return false;
	memcpy( ioBuf, data, nSize );
	WriteFinished( nSize );
	
	return true;
}

/*
 *	�ӻ�������ȡһ�����ȵ�����
 *	���ݳ����㹻��ɹ�������true
 *	����ʧ�ܣ�����false
 */
bool IOBuffer::ReadData( unsigned char *data, int uLength, bool bDel )
{
	//������m_uDataSize����Ҫԭ�Ӳ���
	//cpu���ڴ�1�ν�����С���Ⱦ���4byte,����С�ڵ���4byte�����͵�ȡֵ/��ֵ������
	//�������ԭ�Ӳ���
	if ( 0 >= uLength || m_uDataSize < uLength ) return false;//��ȡ����С��0�������ݲ�������ִ�ж�ȡ

	//��ȡ����
	IOBufferBlock *pRecvBlock = NULL;
	AutoLock lock( &m_mutex );
	vector<IOBufferBlock*>::iterator it = m_recvBufferList.begin();
	unsigned short uRecvSize = 0;
	unsigned short uStartPos = 0;
	
	for ( ; it != m_recvBufferList.end();  )
	{
		pRecvBlock = *it;
		uRecvSize = pRecvBlock->ReadData( &data[uStartPos], uLength, bDel );
		if ( bDel ) AtomDec(&m_uDataSize, uRecvSize);
		if ( uLength == uRecvSize ) return true;//��ȡ���
		
		//�������ݲ��㹻��ȡ������һ�黺���ȡ
		uStartPos += uRecvSize;
		uLength -= uRecvSize;
		if ( bDel )//ɾ�������
		{
			//ǰ���Ѿ���֤�����㹻�������ܴ����ڵȴ��Ļ�����л����ղ���������
			//���������¼��
			//if ( m_pRecvBufferBlock == pRecvBlock ) m_pRecvBufferBlock = NULL;
			//д����ֻ����m_pRecvBufferBlock��ǰָ����ڴ��Ͻ���
			//ɾ���������Բ���m_pRecvBufferBlock��ǰָ����ڴ��Ͻ���
			//������Ҫô��m_pRecvBufferBlock֮ǰ���ڴ��Ͻ���
			//Ҫô��m_pRecvBufferBlock��ǰָ����ڴ��ǰ�ˣ��Ѿ�д����ɵ�byte�Ͻ���
			//���ۣ�������д��Զ���ᷢ����ͻ������
			delete pRecvBlock;//�ͷŻ����
			m_recvBufferList.erase( it );
			it = m_recvBufferList.begin();//׼������һ��������ж�ȡ
		}
		else it++;
	}

	return true;
}

void IOBuffer::Clear()
{
	
	IOBufferBlock *pRecvBlock = NULL;
	vector<IOBufferBlock*>::iterator it = m_recvBufferList.begin();
	for ( ; it != m_recvBufferList.end(); it++ )
	{
		pRecvBlock = *it;
		delete pRecvBlock;
	}
	m_recvBufferList.clear();
	m_pRecvBufferBlock = NULL;
	m_uDataSize = 0;
}

uint32 IOBuffer::GetLength()
{
	return m_uDataSize;
}

}//namespace mdk
