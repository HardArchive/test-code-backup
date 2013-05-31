#ifndef _REAACTORUDPHANDLER_H
#define _REAACTORUDPHANDLER_H

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"  

#include "define.h"
#include "MessageBlockManager.h"
#include "PacketParsePool.h"
#include "MakePacket.h"
#include "CommandAccount.h"

class CReactorUDPHander : public ACE_Event_Handler
{
public:
	CReactorUDPHander(void);
	~CReactorUDPHander(void);

	virtual ACE_HANDLE get_handle(void) const;
	virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

	int  OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Reactor* pReactor);
	void Close();
	bool SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead = true, uint16 u2CommandID = 0);
	_ClientConnectInfo GetClientConnectInfo();

private:
	bool CheckMessage(const char* pData, uint32 u4Len);     //����������ݰ����������ݶ���

private:   
	ACE_SOCK_Dgram          m_skRemote;
	ACE_INET_Addr           m_addrRemote;                   //���ݷ��ͷ���IP��Ϣ
	CPacketParse*           m_pPacketParse;                 //���ݰ�������

	ACE_Time_Value          m_atvInput;                     //���հ���ʱ��
	ACE_Time_Value          m_atvOutput;                    //���Ͱ���ʱ��
	uint32                  m_u4RecvPacketCount;            //�������ݰ�������
	uint32                  m_u4SendPacketCount;            //�������ݰ�������
	uint32                  m_u4RecvSize;                   //�������ݵ��ܴ�С
	uint32                  m_u4SendSize;                   //�������ݵ��ܴ�С
};
#endif
