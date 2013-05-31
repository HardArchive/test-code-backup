#pragma once

#include <WinSock2.h>
#include <time.h>
#include <string>

using namespace std;

#define MAX_BUFF_20   20
#define MAX_BUFF_100  100
#define MAX_BUFF_1024 1024

//设置一个随机种子
inline void InitRandom()
{
  srand((int)time(NULL));
};

//从一个值域中获取一个随机值
inline int RandomValue(int nMin, int nMax)
{
  return  nMin + (int) ((nMax - nMin) * (rand() / (RAND_MAX + 1.0)));
};

class CConvertBuffer
{
public:
  CConvertBuffer() {};
  ~CConvertBuffer() {};
  int GetBufferSize(const char* pData, int nSrcLen)
  {
    char szData[3] = {'\0'};
    int nPos         = 0;
    int nCurrSize    = 0;
    int nConvertSize = 0;
    bool blState     = false;   //转换后的字符串是否有效
    bool blSrcState  = true;    //元字符串是否有效
    unsigned char cData;

    while(nPos < nSrcLen)
    {
      if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
      {
        if(nPos == nSrcLen - 1)
        {
          szData[nCurrSize++] = pData[nPos];
        }

        szData[nCurrSize] = '\0';
        if(blSrcState == true)
        {
          blState = ConvertStr2char(szData, cData);
          if(blState == true)
          {
            nConvertSize++;
          }
        }
        nCurrSize  = 0;
        blSrcState = true;
        nPos++;
      }
      else
      {
        if(nCurrSize < 2)
        {
          szData[nCurrSize++] = pData[nPos];
        }
        else
        {
          blSrcState = false;
        }
        nPos++;
      }
    }

    return nConvertSize;
  };

  bool Convertstr2charArray(const char* pData, int nSrcLen, unsigned char* pDes, int& nMaxLen)
  {
    char szData[3] = {'\0'};
    int nPos         = 0;
    int nCurrSize    = 0;
    int nConvertSize = 0;
    bool blState     = false;   //转换后的字符串是否有效
    bool blSrcState  = true;    //元字符串是否有效

    while(nPos < nSrcLen)
    {
      if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
      {
        if(nPos == nSrcLen - 1)
        {
          szData[nCurrSize++] = pData[nPos];
        }

        szData[nCurrSize] = '\0';
        if(nConvertSize < nMaxLen && blSrcState == true)
        {
          blState = ConvertStr2char(szData, pDes[nConvertSize]);
          if(blState == true)
          {
            nConvertSize++;
          }
        }
        nCurrSize  = 0;
        blSrcState = true;
        nPos++;
      }
      else
      {
        if(nCurrSize < 2)
        {
          szData[nCurrSize++] = pData[nPos];
        }
        else
        {
          blSrcState = false;
        }
        nPos++;
      }
    }

    nMaxLen = nConvertSize;
    return true;
  };
private:
  bool Get_binary_Char(unsigned char cTag, unsigned char& cDes)
  {
    if(cTag >='A'&&  cTag <='F')
    {
      cDes = cTag - 'A' + 10;
      return true;
    }
    else if(cTag >='a'&&  cTag <='f')
    {
      cDes = cTag - 'a' + 10;
      return true; 
    }
    else if(cTag >= '0'&& cTag<= '9')
    {
      cDes = cTag-'0';
      return true;
    }
    else
    {
      return false;
    }
  }

  bool ConvertStr2char(const char* pData, unsigned char& cData)
  {
    if(pData == NULL || strlen(pData) != 2)
    {
      return false;
    }

    char cFirst = pData[1];
    unsigned char cTemp = 0;
    bool blStste = Get_binary_Char(cFirst, cTemp);
    if(false == blStste)
    {
      return false;
    }
    cData = cTemp;
    char cSecond = pData[0];
    blStste  = Get_binary_Char(cSecond, cTemp);
    if(false == blStste)
    {
      return false;
    }
    cTemp = cTemp << 4;
    cData = cData | cTemp;

    return true;
  }
};

//线程连接信息
struct _Socket_Info
{
  char  m_szSerevrIP[MAX_BUFF_20];   //远程服务器的IP
  int   m_nPort;                     //远程服务器的端口
  int   m_nThreadID;                 //线程ID
  int   m_nRecvTimeout;              //接收数据超时时间（单位是毫秒）
  int   m_nDelaySecond;              //短连接间延时（单位是毫秒）
  int   m_nSendLength;               //发送字符串长度
  int   m_nRecvLength;               //接收字符串长度限定
  char* m_pSendBuff;                 //发送数据长度
  bool  m_blIsAlwayConnect;          //是否长连接
  bool  m_blIsRadomaDelay;           //是否随机延时
  bool  m_blIsRecv;                  //是否接收回应包
  bool  m_blIsBroken;                //是否断线重连
  bool  m_blIsSendCount;             //是否随机数据包数
  bool  m_blIsWriteFile;             //是否写入文件
  bool  m_blIsSendOne;               //是否只发一次

  _Socket_Info()
  {
    m_szSerevrIP[0]    = '\0';
    m_nPort            = 0;
    m_nThreadID        = 0;
    m_nRecvTimeout     = 0;
    m_nDelaySecond     = 0;
    m_nSendLength      = 0;
    m_nRecvLength      = 0;
    m_pSendBuff        = NULL;
    m_blIsAlwayConnect = false;
    m_blIsRadomaDelay  = false;
    m_blIsRecv         = true;
    m_blIsBroken       = true;
    m_blIsSendCount    = false;
    m_blIsWriteFile    = false;
	m_blIsSendOne      = false;
  }

  ~_Socket_Info()
  {
    if(NULL != m_pSendBuff)
    {
      delete[] m_pSendBuff;
      m_pSendBuff = NULL;
    }
  }

  void InitSendSize(int nSize)
  {
    if(NULL != m_pSendBuff)
    {
      delete[] m_pSendBuff;
      m_pSendBuff = NULL;
    }

    m_pSendBuff = new char[nSize];
    m_nSendLength = nSize;
  }
};

//线程运行状态信息
struct _Socket_State_Info
{
  int m_nSuccessConnect;            //连接成功数
  int m_nSuccessSend;               //发送成功数
  int m_nSuccessRecv;               //接收成功数
  int m_nCurrectSocket;             //当前链接数
  int m_nFailConnect;               //连接失败数
  int m_nFailSend;                  //发送失败数
  int m_nFailRecv;                  //接收失败数

  _Socket_State_Info()
  {
    m_nSuccessConnect = 0;
    m_nSuccessSend    = 0;
    m_nSuccessRecv    = 0;
    m_nCurrectSocket  = 0;
    m_nFailConnect    = 0;
    m_nFailSend       = 0;
    m_nFailRecv       = 0;
  }
};

class CClientTcpSocket
{
public:
  CClientTcpSocket(void);
  ~CClientTcpSocket(void);

  void Close();

  void Run();
  void Stop();

  void SetSocketThread(_Socket_Info* pSocket_Info, _Socket_State_Info* pSocket_State_Info);
  _Socket_State_Info* GetStateInfo();

private:
  bool WriteFile_SendBuff(const char* pData, int nLen);
  bool WriteFile_RecvBuff(const char* pData, int nLen);

private:
  _Socket_Info*       m_pSocket_Info;
  _Socket_State_Info* m_pSocket_State_Info;
  bool                m_blRun;
};
