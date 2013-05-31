#ifndef _ICLIENTMANAGER_H
#define _ICLIENTMANAGER_H

#include "ClientMessage.h"
#include "ClientUDPMassage.h"

//��������������ͨѶ�����ݹ���
class IClientManager
{
public:
	virtual ~IClientManager() {};

  //����TCP���Ӳ�����pClientMessageΪԶ�����ݵ��ﴦ���ࡣ
	virtual bool Connect(int nServerID, const char* pIP, int nPort, IClientMessage* pClientMessage)          = 0;    
  //����UDP���Ӳ�����pClientUDPMessageΪԶ�����ݵ��ﴦ���ࡣ
	virtual bool ConnectUDP(int nServerID, const char* pIP, int nPort, IClientUDPMessage* pClientUDPMessage) = 0;
  //�ر�ĳһ��ServerID��Ӧ��TCP����
	virtual bool Close(int nServerID)                                                                        = 0;
  //�ر�ĳһ��ServerID��Ӧ��UDP����
	virtual bool CloseUDP(int nServerID)                                                                     = 0;
  //����һ��TCP�����ݰ�������������blIsDelete�������Ƿ��ɿ�ܻ��գ������߼����գ�����ʹ��CBuffPacket����Ϊ���ڴ�أ��������ﲻ��ɾ��
	virtual bool SendData(int nServerID, const char* pData, int nSize, bool blIsDelete = true)               = 0;
  //����һ��UDP�����ݰ�������������blIsDelete�������Ƿ��ɿ�ܻ��գ������߼����գ�����ʹ��CBuffPacket����Ϊ���ڴ�أ��������ﲻ��ɾ��
	virtual bool SendDataUDP(int nServerID, const char* pIP, int nPort, const char* pMessage, uint32 u4Len)  = 0;
  //���Ӵ���飬������������ڲ��������ݰ���ʱ��Ͽ��ˣ�����Զ��ؽ�
	virtual bool StartConnectTask(int nIntervalTime)                                                         = 0;   
  //�ر����Ӵ����
	virtual void CancelConnectTask()                                                                         = 0;                                                                  
  //�ر����ж������Ӱ���TCP��UDP
    virtual void Close()                                                                                     = 0;
	//�ر����ж������Ӱ���TCP��UDP
	virtual bool GetConnectState(int nServerID)                                                              = 0;
};

#endif
