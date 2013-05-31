#pragma once

#include <WinSock2.h>
#include <time.h>
#include <string>

using namespace std;

#define MAX_BUFF_20   20
#define MAX_BUFF_100  100
#define MAX_BUFF_1024 1024

//����һ���������
inline void InitRandom()
{
  srand((int)time(NULL));
};

//��һ��ֵ���л�ȡһ�����ֵ
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
    bool blState     = false;   //ת������ַ����Ƿ���Ч
    bool blSrcState  = true;    //Ԫ�ַ����Ƿ���Ч
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
    bool blState     = false;   //ת������ַ����Ƿ���Ч
    bool blSrcState  = true;    //Ԫ�ַ����Ƿ���Ч

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

//�߳�������Ϣ
struct _Socket_Info
{
  char  m_szSerevrIP[MAX_BUFF_20];   //Զ�̷�������IP
  int   m_nPort;                     //Զ�̷������Ķ˿�
  int   m_nThreadID;                 //�߳�ID
  int   m_nRecvTimeout;              //�������ݳ�ʱʱ�䣨��λ�Ǻ��룩
  int   m_nDelaySecond;              //�����Ӽ���ʱ����λ�Ǻ��룩
  int   m_nSendLength;               //�����ַ�������
  int   m_nRecvLength;               //�����ַ��������޶�
  char* m_pSendBuff;                 //�������ݳ���
  bool  m_blIsAlwayConnect;          //�Ƿ�����
  bool  m_blIsRadomaDelay;           //�Ƿ������ʱ
  bool  m_blIsRecv;                  //�Ƿ���ջ�Ӧ��
  bool  m_blIsBroken;                //�Ƿ��������
  bool  m_blIsSendCount;             //�Ƿ�������ݰ���
  bool  m_blIsWriteFile;             //�Ƿ�д���ļ�
  bool  m_blIsSendOne;               //�Ƿ�ֻ��һ��

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

//�߳�����״̬��Ϣ
struct _Socket_State_Info
{
  int m_nSuccessConnect;            //���ӳɹ���
  int m_nSuccessSend;               //���ͳɹ���
  int m_nSuccessRecv;               //���ճɹ���
  int m_nCurrectSocket;             //��ǰ������
  int m_nFailConnect;               //����ʧ����
  int m_nFailSend;                  //����ʧ����
  int m_nFailRecv;                  //����ʧ����

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
