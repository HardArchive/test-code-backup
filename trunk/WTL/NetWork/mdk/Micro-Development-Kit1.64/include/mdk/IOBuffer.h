// IOBuffer.h: interface for the IOBuffer class.
//
//////////////////////////////////////////////////////////////////////
/*
 	����io���壨�����жӣ���֤˳���ԣ������ԣ�һ���ԣ�
	�����жӸ��ͬʱ������������2���̣߳�1��д1����������Ҫ�������ж�

	������д�߳�>2ʱ������˳�򻺳���˵���������ǲ�����ģ�
	��Ϊ�޷���֤˳��д/�����Ӳ��������޷��õ���ȷ�����
	���������жӾ���ָ1��1д����Ҫ�������жӣ�����Ҫ֧��nдn��
 */
#ifndef MDK_IOBUFFER_H
#define MDK_IOBUFFER_H

#include "FixLengthInt.h"
#include "IOBufferBlock.h"
#include "Lock.h"
#include <vector>

namespace mdk
{

class IOBuffer  
{
public:
	IOBuffer();
	virtual ~IOBuffer();

private:
	/**
	 * ����
	 * std::vector<IOBufferBlock*>����
	 */
	std::vector<IOBufferBlock*> m_recvBufferList;
	/**
	 * ���������ݳ���
	 * ʹ��ԭ�Ӳ����޸�
	 */
	uint32 m_uDataSize;
	/**
	 * ��ǰ�ȴ�д�����ݵĿ��л����
	 */
	IOBufferBlock* m_pRecvBufferBlock;
	Mutex m_mutex;

public:
	//�򻺳���д��һ������
	bool WriteData( char *data, int nSize );
	/*
	 *	�ӻ���������һ�����ȵ�����
	 *	���ݳ����㹻��ɹ�������true
	 *	����ʧ�ܣ�����false
	 */
	bool ReadData( unsigned char *data, int uLength, bool bDel = true );
	//��ջ���
	void Clear();

	/*
		׼��Buffer
		Ϊд��uLength���ȵ�����׼�����壬
		�����ǰ�����(m_pRecvBufferBlock)���пռ䲻�㣬�򴴽�һ���»���
		
		д�����ʱ�������WriteFinished()��ǿɶ����ݳ���
	*/
	unsigned char* PrepareBuffer(unsigned short uRecvSize);
	/**
	 * д�����
	 * ���д�����д�����ݵĳ���
	 * ������PrepareBuffer()�ɶԵ���
	 */
	void WriteFinished(unsigned short uLength);

	uint32 GetLength();

protected:
	//����һ�黺���
	void AddBuffer();	
};

}//namespace mdk

#endif // MDK_IOBUFFER_H
