#ifndef _BUFFPACKET_H
#define _BUFFPACKET_H

#include "ace/Thread_Mutex.h"

#include "ACEMemory.h"
#include "../IObject/IBuffPacket.h"

#define DEFINE_PACKET_SIZE 1024
#define DEFINE_PACKET_ADD  1024

#define USER_PACKET_MEMORY_POOL 1   //����ʹ��ACE�ڴ�ط���

class CBuffPacket : public IBuffPacket
{
public:
	CBuffPacket(int nSize = DEFINE_PACKET_SIZE);
	~CBuffPacket(void);

	uint32 GetPacketSize();    //�õ����ݰ��ĸ�ʽ������
	uint32 GetPacketLen();     //�õ����ݰ���ʵ�ʳ���
	uint32 GetReadLen();       //�õ�����ȡ�ĳ���
	uint32 GetWriteLen();      //�õ���д��ĳ���
	uint32 GetHeadLen();       //�õ����ݰ�ͷ�ĳ���
	uint32 GetPacketCount();   //�õ��������ݰ��ĸ���
	const char* GetData();     //�õ���ǰ����ָ��

	bool Init(int nSize);
	bool Close();              //ɾ���Ѿ�ʹ�õ��ڴ�
	bool Clear();              //������еı�־λ������ɾ���ڴ档

	bool WriteStream(const char* szData, uint32 u4Len);
	bool ReadStream(char*& pData, uint32 u4MaxLen, uint32 u4Len);

	void SetReadPtr(uint32 u4Pos);              //���ö�ָ���λ��
	void SetPacketCount(uint32 u4PacketCount);  //���û������ݰ��ĸ���
	bool RollBack(uint32 u4Len);                //��ȡ��������ɾ��������������ݼ���
	bool AddBuffPacket(uint32 u4Size);          //����Packet�İ���С
	char* ReadPtr();                            //��ö�ָ��
	char* WritePtr();                           //���дָ��
	void ReadBuffPtr(uint32 u4Size);
	void WriteBuffPtr(uint32 u4Size);

private:
	bool AddBuff(uint32 u4Size);
	void ReadPtr(uint32 u4Size);
	void WritePtr(uint32 u4Size);


public:
	//��ȡ
	CBuffPacket& operator >> (uint8& u1Data);
	CBuffPacket& operator >> (uint16& u2Data);
	CBuffPacket& operator >> (uint32& u4Data);
	CBuffPacket& operator >> (uint64 &u8Data);

	CBuffPacket& operator >> (float32& f4Data);
	CBuffPacket& operator >> (float64& f8Data);

	CBuffPacket& operator >> (VCHARS_STR& str);
	CBuffPacket& operator >> (VCHARM_STR& str);
	CBuffPacket& operator >> (VCHARB_STR& str);

	//д��
	CBuffPacket& operator << (uint8 u1Data);
	CBuffPacket& operator << (uint16 u2Data);
	CBuffPacket& operator << (uint32 u4Data);
	CBuffPacket& operator << (uint64 u8Data);

	CBuffPacket& operator << (float32 f4Data);
	CBuffPacket& operator << (float64 f8Data);
	
	CBuffPacket& operator << (VCHARS_STR &str);
	CBuffPacket& operator << (VCHARM_STR &str);
	CBuffPacket& operator << (VCHARB_STR &str);

private:
	char*                     m_szData;
	uint32                    m_u4ReadPtr;         //������λ��
	uint32                    m_u4WritePtr;        //д����λ��
	uint32                    m_u4PacketLen;       //���ܳ���
	uint32                    m_u4PacketCount;     //��ǰ���ݰ��ĸ���

	char                       m_szError[MAX_BUFF_500];

	ACE_Recursive_Thread_Mutex m_ThreadLock;

public:
	void* operator new(size_t stSize)
	{
		return App_ACEMemory::instance()->malloc(stSize);
	};

	void operator delete(void* p)
	{
		App_ACEMemory::instance()->free(p);
	};
};
#endif
