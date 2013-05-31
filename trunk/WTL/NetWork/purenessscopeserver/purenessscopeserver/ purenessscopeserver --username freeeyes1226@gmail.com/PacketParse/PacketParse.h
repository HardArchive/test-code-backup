#pragma once 
#include "define.h"

#ifdef WIN32
#if defined PACKETPARSE_BUILD_DLL
#define PACKETPARSE_BUILD_DLL __declspec(dllexport)
#else
#define PACKETPARSE_BUILD_DLL __declspec(dllimport)
#endif
#else
#define PACKETPARSE_BUILD_DLL
#endif 

#ifdef WIN32
class PACKETPARSE_BUILD_DLL CPacketParse
#else
class CPacketParse
#endif 
{
public:
	CPacketParse(void);
	~CPacketParse(void);

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

  const char* GetPacketVersion();

	uint32 GetPacketHeadSrcLen();                     //�õ����ݰ�ԭʼ��ͷ����
	uint32 GetPacketBodySrcLen();                     //�õ����ݰ�ԭʼ���峤��

	//ƴ�����ݷ��ذ�
	bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData);
	uint32 MakePacketLength(uint32 u4DataLen);

	void Close();

private:
	uint32 m_u4PacketHead;
	uint32 m_u4PacketData;
	uint32 m_u4HeadSrcSize;
	uint32 m_u4BodySrcSize;
	uint16 m_u2PacketCommandID;
	bool   m_blIsHead;
  char   m_szPacketVersion[MAX_BUFF_20];   //���������汾

	ACE_Message_Block* m_pmbHead;   //��ͷ����
	ACE_Message_Block* m_pmbBody;   //���岿��

};
