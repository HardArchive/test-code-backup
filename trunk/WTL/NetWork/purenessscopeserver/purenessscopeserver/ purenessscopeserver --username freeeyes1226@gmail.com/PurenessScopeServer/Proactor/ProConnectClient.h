#ifndef _PROCONNECTCLIENT_H
#define _PROCONNECTCLIENT_H

//�ṩ�ͻ��˷������ӵĽ������
//�û������ֻ࣬�����ݵ��շ����Զ������������ϲ�ָ��
//add by freeeyes 2010-12-24

#include "ace/Recursive_Thread_Mutex.h"
#include "ace/INET_Addr.h"
#include "ace/Asynch_IO.h"
#include "ace/Mutex.h" 
#include "ace/Thread_Mutex.h"

#include "MainConfig.h"
#include "ClientParsePool.h"
#include "ClientMessage.h"
#include "AceProactorManager.h"

#define MAX_BUFF_1024 1024
#define MAX_IP_LEN    16

class CProConnectClient : public ACE_Service_Handler
{
public:
	CProConnectClient(void);
	~CProConnectClient(void);

	virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                               //�û�����һ������
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);                     //�����û�����
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);                   //�����û�����
	virtual void addresses(const ACE_INET_Addr &remote_address, const ACE_INET_Addr &local_address);   //������ӵ�ַ

	void Close();                                          //���ӹر�
	void ClientClose();                                    //�ͻ������ҹر�
	void SetServerID(int nServerID);                       //���õ�ǰ��ServerID
	void SetClientMessage(IClientMessage* pClientMessage); //������Ϣ���մ�����
	int  GetServerID();                                    //��õ�ǰServerID
	bool SendData(ACE_Message_Block* pmblk);               //��������

	_ClientConnectInfo GetClientConnectInfo();             //�õ���ǰ������Ϣ

private:
	bool RecvData(uint32 u4PacketLen);              //��������
	bool DoMessage();                               //������յ�������������

private:
	ACE_INET_Addr               m_AddrRemote;        //����Զ�̷������ĵ�ַ
	ACE_Message_Block*          m_mbRecv;            //��ȡ���ݻ���
	ACE_Asynch_Read_Stream      m_Reader;            //��ȡ��
	ACE_Asynch_Write_Stream     m_Writer;            //д����
	CClientParse*               m_pClientParse;      //���յ����ݰ���������
	IClientMessage*             m_pClientMessage;    //������Ϣ���յ���
	int                         m_nIOCount;          //��ǰ���ø���
  uint32                      m_u4MaxPacketSize;   //�����հ���
  int                         m_nServerID;         //��ǰServerID
	ACE_Recursive_Thread_Mutex  m_ThreadWritrLock;   //�߳���

	ACE_Time_Value              m_atvBegin;          //���ӽ���ʱ��
	uint32                      m_u4SendSize;        //�����ֽ���
	uint32                      m_u4SendCount;       //�������ݰ���  
	uint32                      m_u4RecvSize;        //�����ֽ���
	uint32                      m_u4RecvCount;       //�������ݰ���
	uint32                      m_u4CostTime;        //��Ϣ������ʱ��
};
#endif 