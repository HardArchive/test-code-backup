#ifndef MDK_IOBUFFERBLOCK_H
#define MDK_IOBUFFERBLOCK_H

#define BUFBLOCK_SIZE 8192
#include <stddef.h>

namespace mdk
{

class MemoryPool;
class Mutex;

/**
 * ����д���
 * �ض������ͣ�ÿ������д�룬����1������д���Ĵ���
 */
class IOBufferBlock
{
	friend class IOBuffer;
//////////////////////////////////////////////////////////////////////////
//ʹ���Լ����ڴ����
private:
	//�ڴ�أ�������Ϊ�����͵Ķ�������ڴ�
	static MemoryPool* ms_pMemoryPool;
	//�ڴ���̰߳�ȫ��
	static Mutex* ms_pPoolMutex;
public:
	//new���������
	void* operator new(size_t uObjectSize);
	//delete���������
	void operator delete(void* pObject);
//////////////////////////////////////////////////////////////////////////
//ʹ���Լ����ڴ����end

private:
	//˽�й��캯����ֻ��IOBuffer����ɶ�д�뻺�����з���
	IOBufferBlock();
	//˽���������캯������������Զ������ʺϵ�delete
	virtual ~IOBufferBlock();
private:
	/**
	 * IO����
	 * �����BUFBLOCK_SIZE
	 * �����ھ�̬ȷ�����С
	 * ����ʹ��ָ��������ʱ��ָ̬����С���ο��ض���ʹ�����ƶ�
	 */
	unsigned char m_buffer[BUFBLOCK_SIZE];
	//���յ����ݵĳ���
	unsigned short m_uLength;
	//Recv()�����´ζ�ȡ���ݵĿ�ʼλ��
	unsigned short m_uRecvPos;
 
public:
//////////////////////////////////////////////////////////////////////////
//д�뷽��
	/*
		׼��Buffer
		Ϊд��uLength���ȵ�����׼�����壬
		���ʣ�໺�峤�Ȳ��㣬����NULL
		���򷵻ػ����׵�ַ
		
		д�����ʱ�������WriteFinished()��ǿɶ����ݳ���
	*/
	unsigned char* PrepareBuffer( unsigned short uLength );
	/**
	 * д�����
	 * ���д�����д�����ݵĳ���
	 * ������PrepareBuffer()�ɶԵ���
	 */
	void WriteFinished(unsigned short uLength);

//////////////////////////////////////////////////////////////////////////
//��ȡ����
	
	//�建����ȡuLength���ȵ�����
	//����ʵ�ʶ�ȡ�����ݳ���
	unsigned short ReadData( unsigned char *data, unsigned short uLength, bool bDel = true );
};

}//namespace mdk

#endif //MDK_IOBUFFERBLOCK_H
