#pragma once 
#include "define.h"

#ifdef WIN32
#if defined DLLCLASS_EXPORTS
#define DLLCLASS_EXPORTS __declspec(dllexport)
#else
#define DLLCLASS_EXPORTS __declspec(dllimport)
#endif
#else
#define DLLCLASS_EXPORTS
#endif 


class DLLCLASS_EXPORTS CPacketParse
{
public:
	CPacketParse(void);
	~CPacketParse(void);

	void Init();

	uint32 GetPacketHeadLen();                        //�õ��İ�ͷ���ȣ�δ���ܣ�
	uint32 GetPacketDataLen();                        //�õ����峤��(δ����)
	uint16 GetPacketCommandID();                      //�õ����ݰ�������

  const char* GetPacketVersion();                   //�õ�������ģ��İ汾

	bool GetIsHead();                                 //�õ���ͷ���λ�������ǰ�ǰ�ͷ���򷵻�True�����򷵻�False

	ACE_Message_Block* GetMessageHead();              //�õ����ܺõ����ݿ飨��ͷ��
	ACE_Message_Block* GetMessageBody();              //�õ����ܺõ����ݿ飨���壩
	bool SetMessageHead(ACE_Message_Block* pmbHead);  //�������ݿ���Ϣ����ͷ��
	bool SetMessageBody(ACE_Message_Block* pmbBody);  //�������ݿ���Ϣ�����壩

	bool SetPacketHead(char* pData, uint32 u4Len);    //����ԭʼ��ͷ��ڣ������ｫ���ݻ�ԭ��ACE_Message_Block���ܽ��ܺõİ���
	bool SetPacketData(char* pData, uint32 u4Len);    //����ԭʼ������ڣ������ｫ���ݻ�ԭ��ACE_Message_Block���ܽ��ܺõİ���

	uint32 GetPacketHeadSrcLen();                     //�õ����ݰ�ԭʼ��ͷ����
	uint32 GetPacketBodySrcLen();                     //�õ����ݰ�ԭʼ���峤��

	//ƴ�����ݷ��ذ�
	bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData);   //���ص����ݰ�������������������ļ��ܷ���
	uint32 MakePacketLength(uint32 u4DataLen);                                      //��ü��ܺ���ĳ���

	void Close();
};
