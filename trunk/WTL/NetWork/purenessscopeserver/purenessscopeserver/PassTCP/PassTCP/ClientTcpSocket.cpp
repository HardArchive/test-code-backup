#include "StdAfx.h"
#include "ClientTcpSocket.h"

CClientTcpSocket::CClientTcpSocket(void)
{
  m_pSocket_Info       = NULL;
  m_pSocket_State_Info = NULL;
  m_blRun              = false;
}

CClientTcpSocket::~CClientTcpSocket(void)
{
  Close();
}

void CClientTcpSocket::Close()
{
  if(NULL != m_pSocket_Info)
  {
    delete m_pSocket_Info;
    m_pSocket_Info = NULL;
  }

  if(NULL != m_pSocket_State_Info)
  {
    delete m_pSocket_State_Info;
    m_pSocket_State_Info = NULL;
  }
}

void CClientTcpSocket::SetSocketThread( _Socket_Info* pSocket_Info, _Socket_State_Info* pSocket_State_Info )
{
  Close();

  m_pSocket_Info       = pSocket_Info;
  m_pSocket_State_Info = pSocket_State_Info;
}

_Socket_State_Info* CClientTcpSocket::GetStateInfo()
{
  return m_pSocket_State_Info;
}

void CClientTcpSocket::Stop()
{
  m_blRun = false;
}

void CClientTcpSocket::Run()
{

  m_blRun = true;
  SOCKET sckClient;

  if(m_pSocket_Info == NULL || m_pSocket_State_Info == NULL)
  {
    m_blRun = false;
    return;
  }

  //看看是否是长连接，如果是长连接，则只处理一次。
  bool blIsConnect = false;

  //socket创建的准备工作
  struct sockaddr_in sockaddr;
  
  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port   = htons(m_pSocket_Info->m_nPort);
  sockaddr.sin_addr.S_un.S_addr = inet_addr(m_pSocket_Info->m_szSerevrIP);

  while(m_blRun)
  {
    if(blIsConnect == false)
    {
      sckClient = socket(AF_INET, SOCK_STREAM, 0);

      DWORD TimeOut = (DWORD)m_pSocket_Info->m_nRecvTimeout;
      ::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

      DWORD dwSleepTime = (DWORD)m_pSocket_Info->m_nDelaySecond;
      if(m_pSocket_Info->m_blIsRadomaDelay == true)
      {
        //如果是随机的，则从1-1000之间随机一个时间
        dwSleepTime = (DWORD)RandomValue(1, 1000);
      }

      //挂起指定的时间
      Sleep(dwSleepTime);

      //连接远程服务器
      int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
      if(0 != nErr)
      {
        m_pSocket_State_Info->m_nFailConnect++;
        m_pSocket_State_Info->m_nCurrectSocket = 0;
      }
      else
      {
        //成功连接
        m_pSocket_State_Info->m_nSuccessConnect++;
        m_pSocket_State_Info->m_nCurrectSocket = 1;
        blIsConnect = true;
      }
    }

    if(blIsConnect == true)
    {
      //发送数据
      char szSendBuffData[MAX_BUFF_1024 * 100] = {'\0'};
      char szRecvBuffData[MAX_BUFF_1024 * 100] = {'\0'};

      char* pSendData   = NULL;
      int nSendLen      = 0;
      int nTotalRecvLen = 0;
      //如果数据为随机数据包
      if(m_pSocket_Info->m_blIsSendCount == true)
      {
        int nSendCount = RandomValue(1, 10);
        for(int i = 0; i < nSendCount; i++)
        {
          memcpy(&szSendBuffData[i * m_pSocket_Info->m_nSendLength], m_pSocket_Info->m_pSendBuff, m_pSocket_Info->m_nSendLength);
        }

        //发送数据
        pSendData     = (char* )szSendBuffData;
        nSendLen      = m_pSocket_Info->m_nSendLength * nSendCount;
        nTotalRecvLen = m_pSocket_Info->m_nRecvLength * nSendCount;
      }
      else
      {
        //发送数据
        pSendData     = (char* )m_pSocket_Info->m_pSendBuff;
        nSendLen      = m_pSocket_Info->m_nSendLength;
        nTotalRecvLen = m_pSocket_Info->m_nRecvLength;
      }

      //记录应收字节总数
      int nRecvAllSize = nTotalRecvLen;

      //如果需要记录日志，则将数据计入日志
      if(m_pSocket_Info->m_blIsWriteFile == true)
      {
        WriteFile_SendBuff(pSendData, nSendLen);
      }

      int nTotalSendLen = nSendLen;
      int nBeginSend    = 0;
      int nCurrSendLen  = 0;
      bool blSendFlag   = false;
      int nBeginRecv    = 0;
      int nCurrRecvLen  = 0;
      bool blRecvFlag   = false;
      while(true)
      {
        nCurrSendLen = send(sckClient, pSendData + nBeginSend, nTotalSendLen, 0);
        if(nCurrSendLen <= 0)
        {
          m_pSocket_State_Info->m_nFailSend++;
          closesocket(sckClient);
          m_pSocket_State_Info->m_nCurrectSocket = 0;
          blIsConnect = false;
          break;
        }
        else
        {
          nTotalSendLen -= nCurrSendLen;
          if(nTotalSendLen == 0)
          {
            //发送完成
            m_pSocket_State_Info->m_nSuccessSend++;
            blSendFlag = true;
            break;
          }
          else
          {
            nBeginSend += nCurrSendLen;
          }
        }
      }

      //接收数据
      if(blSendFlag == true)
      {
        while(true)
        {
          //如果发送成功了，则处理接收数据
          nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
          if(nCurrRecvLen <= 0)
          {
            m_pSocket_State_Info->m_nFailSend++;
            closesocket(sckClient);
            m_pSocket_State_Info->m_nCurrectSocket = 0;
            blIsConnect = false;
            break;
          }
          else
          {
            nTotalRecvLen -= nCurrRecvLen;
            if(nTotalRecvLen == 0)
            {
              //接收完成
              m_pSocket_State_Info->m_nSuccessRecv++;
              blRecvFlag = true;

              //如果需要记录日志，则将数据计入日志
              if(m_pSocket_Info->m_blIsWriteFile == true)
              {
                WriteFile_RecvBuff(szRecvBuffData, nRecvAllSize);
              }

              break;
            }
            else
            {
              nBeginRecv += nCurrRecvLen;
            }
          }
        }
      }

      //如果是长连接，则不关闭连接
      if(m_pSocket_Info->m_blIsAlwayConnect == false)
      {
        closesocket(sckClient);
        m_pSocket_State_Info->m_nCurrectSocket = 0;
        blIsConnect = false;
      }
    }

	//如果只发送一次，在这里退出
	if(m_pSocket_Info->m_blIsSendOne == true)
	{
		m_blRun = false;
	}
  }

  //如果连接没断，则断开
  if(blIsConnect == true)
  {
    closesocket(sckClient);
    m_pSocket_State_Info->m_nCurrectSocket = 0;
    blIsConnect = false;
  }
}

bool CClientTcpSocket::WriteFile_SendBuff( const char* pData, int nLen )
{
  FILE* pFile = NULL;
  char szFileName[20];
  sprintf_s(szFileName, "Thread%d.log", m_pSocket_Info->m_nThreadID);
  fopen_s(&pFile, szFileName, "a+");
  if(pFile == NULL)
  {
    return false;
  }

  string strLog;
  strLog = "[SendBuff]";

  for(int i = 0; i < nLen; i++)
  {
    char szChar[20];
    sprintf_s(szChar, 20, " 0x%02X", (unsigned char )pData[i]);
    strLog += szChar;
  }

  strLog += "\n";

  fwrite(strLog.c_str(), strLog.length(), sizeof(char), pFile);

  fclose(pFile);
  return true;
}

bool CClientTcpSocket::WriteFile_RecvBuff( const char* pData, int nLen )
{
  FILE* pFile = NULL;
  char szFileName[20];
  sprintf_s(szFileName, "Thread%d.log", m_pSocket_Info->m_nThreadID);
  fopen_s(&pFile, szFileName, "a+");
  if(pFile == NULL)
  {
    return false;
  }

  string strLog;
  strLog = "[RecvBuff]";

  for(int i = 0; i < nLen; i++)
  {
    char szChar[20];
    sprintf_s(szChar, 20, " 0x%02X", (unsigned char )pData[i]);
    strLog += szChar;
  }

  strLog += "\n";

  fwrite(strLog.c_str(), strLog.length(), sizeof(char), pFile);

  fclose(pFile);
  return true;
}
