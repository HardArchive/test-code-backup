#include "StdAfx.h"
#include "TcpClientConnect.h"

CTcpClientConnect::CTcpClientConnect(void)
{
}

CTcpClientConnect::~CTcpClientConnect(void)
{
}

void CTcpClientConnect::Init( const char* pIp, int nPort, const char* pKey )
{
  sprintf_s(m_szServerIP, 20, "%s", pIp);
  sprintf_s(m_szConsoleKey, 50, "%s", pKey);
  m_nServerPort = nPort;
}

bool CTcpClientConnect::SendConsoleMessage( const char* pMessage, int nMessageLen, char* pRevBuff, int& nRecvLen )
{
  SOCKET sckClient;
  char* pBuff = new char[1024*100];
  int nPacketLen = 0;

  //socket������׼������
  struct sockaddr_in sockaddr;

  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port   = htons(m_nServerPort);
  sockaddr.sin_addr.S_un.S_addr = inet_addr(m_szServerIP);

  sckClient = socket(AF_INET, SOCK_STREAM, 0);

  //����Զ�̷�����
  int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
  if(0 != nErr)
  {
    delete[] pBuff;
    closesocket(sckClient);
    return false;
  }

  int nTotalRecvLen = nRecvLen;
  int nTotalSendLen = nMessageLen;
  int nBeginSend    = 0;
  int nCurrSendLen  = 0;
  int nFinishRecv   = 0;
  bool blSendFlag   = false;

  while(true)
  {
    nCurrSendLen = send(sckClient, pMessage + nBeginSend, nTotalSendLen, 0);
    if(nCurrSendLen <= 0)
    {
      delete[] pBuff;
      closesocket(sckClient);
      return false;
    }
    else
    {
      nTotalSendLen -= nCurrSendLen;
      if(nTotalSendLen == 0)
      {
        //�������
        blSendFlag = true;
        break;
      }
      else
      {
        nBeginSend += nCurrSendLen;
      }
    }
  }

  int nCurrRecvLen = 0;
  //��������
  if(blSendFlag == true)
  {
    while(true)
    {
      //��������
      int nLen = recv(sckClient, (char* )&pBuff[nCurrRecvLen], 1024*100 - nCurrRecvLen, 0);
      if(nLen <= 0)
      {
        delete[] pBuff;
        closesocket(sckClient);
        return false;
      }

      if((int)nLen >= 4)
      {
        memcpy(&nPacketLen, pBuff, sizeof(int));
        nFinishRecv = nPacketLen + 4;
      }

      nCurrRecvLen += (int)nLen;

      if(nCurrRecvLen >= nFinishRecv)
      {
        break;
      }
    }
  }

  if(nFinishRecv <= nRecvLen)
  {
    memcpy_s(pRevBuff, nFinishRecv,  pBuff, nFinishRecv);
    nRecvLen = nFinishRecv;
  }

  delete[] pBuff;
  closesocket(sckClient);
  return true;
}

char* CTcpClientConnect::GetKey()
{
  return m_szConsoleKey;
}
