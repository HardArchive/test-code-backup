#ifndef _CLIENTPARSE_H
#define _CLIENTPARSE_H

//�ͻ������ݰ�������
//�м������Э��ܹ�Ϊ (<���� unit32>):��ͷ (<����ID uint16> + <��������ID uint32> + ������ XXX):����
//add by freeeyes 2010-12-24
#include "define.h"

#define MAX_PACKET_HEAD sizeof(uint32)

class CClientParse
{
public:
	CClientParse(void);
	~CClientParse(void);

	void Init();

	uint32 GetPacketHeadLen();
	uint32 GetPacketDataLen();
	uint16 GetPacketCommandID();
	uint32 GetPacketSerialNo();

	bool GetIsHead();

	ACE_Message_Block* GetMessageHead();
	ACE_Message_Block* GetMessageBody();
	bool SetMessageHead(ACE_Message_Block* pmbHead);
	bool SetMessageBody(ACE_Message_Block* pmbBody);

	bool SetPacketHead(char* pData, uint32 u4Len);
	bool SetPacketData(char* pData, uint32 u4Len);

	//ƴ�����ݷ��ذ�
	bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData);
	uint32 MakePacketLength(uint32 u4DataLen);

	void Close();

private:
	uint32 m_u4PacketHead;
	uint32 m_u4PacketData;
	uint16 m_u2PacketCommandID;
	bool   m_blIsHead;

	ACE_Message_Block* m_pmbHead;   //��ͷ����
	ACE_Message_Block* m_pmbBody;   //���岿��
};
#endif
