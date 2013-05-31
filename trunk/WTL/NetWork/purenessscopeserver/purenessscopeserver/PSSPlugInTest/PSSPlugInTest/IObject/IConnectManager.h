#ifndef _ICONNECTMANAGER_H
#define _ICONNECTMANAGER_H

#include "IBuffPacket.h"
#include <vector>

class IConnectManager
{
public:
	virtual ~IConnectManager() {};
  //�첽����ָ�������ݰ�(����PacketParse�����Ͱ�) IBuffPacket �����ڲ�������������
	virtual bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)                    = 0;  
  //�첽����ָ�������ݰ�(������PacketParse�����Ͱ�) pData�����治�������٣���Ҫ���߼�ģ�����д���
  virtual bool PostMessage(uint32 u4ConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)          = 0; 
  //�첽Ⱥ��ָ�������ݰ�(����PacketParse�����Ͱ�) IBuffPacket �����ڲ�������������
	virtual bool PostMessage(vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)           = 0;  
  //�첽Ⱥ��ָ�������ݰ�(������PacketParse�����Ͱ�) pData�����治�������٣���Ҫ���߼�ģ�����д���
  virtual bool PostMessage(vector<uint32> vecConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true) = 0;  
  //�첽ȫ��ָ�������ݰ�(����PacketParse�����Ͱ�) IBuffPacket �����ڲ�������������
  virtual bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)                                     = 0;  
  //�첽ȫ��ָ�������ݰ�(������PacketParse�����Ͱ�) pData�����治�������٣���Ҫ���߼�ģ�����д���
  virtual bool PostMessageAll(const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)                           = 0;  
  virtual bool CloseConnect(uint32 u4ConnectID)                                                                = 0;
	virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID)                                                    = 0;
	virtual int  GetCount()                                                                                      = 0;
};

#endif
