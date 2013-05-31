#ifndef _SERVERPARSE_H
#define _SERVERPARSE_H

//�����м���������ݰ���������
//�û������������޸��м�������������
//���һ�㲻�ܵĻ�����Ĺ������<��ͷ����>��4�ֽڣ�+<����>
//Create by freeeyes
//2010-12-21

#include "define.h"

class CServerParse
{
public:
	CServerParse(void);
	~CServerParse(void);

	void Init();

	uint32 GetPacketHeadLen();
	uint32 GetPacketDataLen();
	uint16 GetPacketCommandID();

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
	uint16 m_u4PacketCommandID;
	bool   m_blIsHead;

	ACE_Message_Block* m_pmbHead;   //��ͷ����
	ACE_Message_Block* m_pmbBody;   //���岿��
};

#endif
