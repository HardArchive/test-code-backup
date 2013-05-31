#pragma once

#include "MessageManager.h"
#include "PacketParse.h"
#include "BuffPacketManager.h"
#include "MessageManager.h"
#include "LoadModule.h"
#include "IObject.h"
#include "ModuleMessageManager.h"

#include "CommandInfo.h"

//*******************************************
//ģ��ProactorConnect��ʵ�ʲ�ʵ���κζ�����ֻ��Ϊ�˵���ʵ�֣��������ܡ�
//����������������Ҫ������������Ӵ��룬�����ÿ������Ĵ������滻֮����������Ϳ���޹ص��߼����ܡ�
//add by freeeyes
class CProClientManager : public IClientManager
{
public:
  CProClientManager() {};
  virtual ~CProClientManager() {};

  bool Connect(int nServerID, const char* pIP, int nPort, IClientMessage* pClientMessage)          
  {
    delete pClientMessage;
    return true; 
  };
  bool ConnectUDP(int nServerID, const char* pIP, int nPort, IClientUDPMessage* pClientUDPMessage) 
  {
    delete pClientUDPMessage;
    return true; 
  };
  bool Close(int nServerID)                                                                        { return true; };
  bool CloseUDP(int nServerID)                                                                     { return true; };
  bool SendData(int nServerID, const char* pData, int nSize, bool blIsDelete = true)               { return true; };
  bool SendDataUDP(int nServerID, const char* pIP, int nPort, const char* pMessage, uint32 u4Len)  
  {
    delete pMessage;
    return true; 
  };
  bool StartConnectTask(int nIntervalTime)                                                         { return true; };                            
  void CancelConnectTask()                                                                         {};                                                                 
  void Close()                                                                                     {};
  bool GetConnectState(int nServerID)                                                              { return true; };
};

class CLogManager : public ILogManager
{
public:
  CLogManager() {};
  virtual ~CLogManager() {};
  int WriteLog(int nLogType, const char* fmt, ...) { return 0; }
};

class CProConnectManager : public IConnectManager
{
public:
  CProConnectManager() {};
  virtual ~CProConnectManager() {};

  bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)
  {
    if(false == App_BuffPacketManager::instance()->Delete(pBuffPacket))
    {
      delete pBuffPacket;
      return true;
    }
    else
    {
      return true;
    }
  };

  bool PostMessage(uint32 u4ConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)
  {
    delete pData;
    return true;
  };

  bool PostMessage(vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)
  {
    if(false == App_BuffPacketManager::instance()->Delete(pBuffPacket))
    {
      delete pBuffPacket;
      return true;
    }
    else
    {
      return true;
    }
  }

  bool PostMessage(vector<uint32> vecConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)
  {
    delete pData;
    return true;
  }

  bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)
  {
    if(false == App_BuffPacketManager::instance()->Delete(pBuffPacket))
    {
      delete pBuffPacket;
      return true;
    }
    else
    {
      return true;
    }
  }

  bool PostMessageAll(const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true)
  {
    delete pData;
    return true;
  }

  bool CloseConnect(uint32 u4ConnectID)
  {
    return true;
  };

  _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID)
  {
    _ClientIPInfo objClientIPInfo;
    return objClientIPInfo;
  };

  int  GetCount()
  {
    return 0;
  }
};

class CProUDPConnectManager : public IUDPConnectManager
{
public:
  CProUDPConnectManager() {};
  virtual ~CProUDPConnectManager() {};

  bool SendMessage(int nConnectID, const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead = true, uint16 u2CommandID = 0)
  {
    delete pMessage;
    return true;
  }
};

//*******************************************

class CPlugInOperation
{
public:
  CPlugInOperation(void);
  ~CPlugInOperation(void);

  bool Init();
  bool Start(vecCommaindInfo* pvecCommaindInfo, const char* pModuleString);
  bool Run();
  bool Stop();
  void Close();

  vecCommaindInfo* GetvecCommaindInfo();

private:
  CProClientManager*     m_pProClientManager;
  CLogManager*           m_pLogManager;
  CProConnectManager*    m_pProConnectManager;
  CProUDPConnectManager* m_pProUDPConnectManager;
  ActiveTimer*           m_pActiveTimer;
  CModuleMessageManager* m_pModuleMessageManager;
  vecCommaindInfo*       m_pvecCommaindInfo;
  bool                   m_blIsRun;
};
