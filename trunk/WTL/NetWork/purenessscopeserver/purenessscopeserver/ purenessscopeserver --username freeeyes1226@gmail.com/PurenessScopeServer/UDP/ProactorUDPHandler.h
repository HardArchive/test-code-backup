#ifndef _PROACTORUDPHANDLE_H
#define _PROACTORUDPHANDLE_H

#include "ace/ACE.h"
#include "ace/OS_main.h"   
#include "ace/OS_NS_string.h"
#include "ace/Proactor.h"   
#include "ace/Asynch_IO.h"   
#include "ace/INET_Addr.h"   
#include "ace/SOCK_Dgram.h"

#include "define.h"
#include "MessageBlockManager.h"
#include "PacketParsePool.h"
#include "MakePacket.h"
#include "ForbiddenIP.h"
#include "CommandAccount.h"

class CProactorUDPHandler : public ACE_Service_Handler
{
public:
	CProactorUDPHandler(void);
	~CProactorUDPHandler(void);

	virtual void handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result);

	int  OpenAddress(const ACE_INET_Addr& AddrLocal, ACE_Proactor* pProactor);
	void Close();
	bool SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead = true, uint16 u2CommandID = 0);
	_ClientConnectInfo GetClientConnectInfo();

private:
	bool CheckMessage(ACE_Message_Block* pMbData, uint32 u4Len);     //����������ݰ����������ݶ���

private:
	CPacketParse*           m_pPacketParse;                 //���ݰ�������
	ACE_INET_Addr           m_addrRemote;                   //���ݷ��ͷ���IP��Ϣ
	ACE_SOCK_Dgram          m_skRemote;
	ACE_Asynch_Read_Dgram   m_Read;
	ACE_Asynch_Write_Dgram  m_Write;

	ACE_Time_Value          m_atvInput;                     //���հ���ʱ��
	ACE_Time_Value          m_atvOutput;                    //���Ͱ���ʱ��
	uint32                  m_u4RecvPacketCount;            //�������ݰ�������
	uint32                  m_u4SendPacketCount;            //�������ݰ�������
	uint32                  m_u4RecvSize;                   //�������ݵ��ܴ�С
	uint32                  m_u4SendSize;                   //�������ݵ��ܴ�С
	char                    m_szCompletionkey[MAX_BUFF_20]; //��ɶ˿ڵ�Key
	char                    m_szAct[MAX_BUFF_20];           //����
	_TimeConnectInfo        m_TimeConnectInfo;              //���ӽ��������
};
#endif
