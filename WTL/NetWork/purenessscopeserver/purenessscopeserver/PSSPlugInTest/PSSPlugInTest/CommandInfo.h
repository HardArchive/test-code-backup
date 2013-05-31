#pragma once

#include "ConvertBuff.h"

#include <vector>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;

struct _CommaindInfo
{
  char  m_szCommandID[20];
  int   m_nCommandGroup;
  int   m_nCommandSort;
  unsigned char* m_pSendData;
  int   m_nSendDataLen;
  int   m_nSendDataCount;
  int   m_nRecvDataCount;

  _CommaindInfo()
  {
    m_szCommandID[0] = '\0';
    m_nCommandSort   = 0;
    m_nCommandGroup  = 0;
    m_pSendData      = NULL;
    m_nSendDataLen   = 0;
    m_nSendDataCount = 0;
    m_nRecvDataCount = 0;
  }

  void Close()
  {
    if(NULL != m_pSendData)
    {
      delete m_pSendData;
      m_pSendData = NULL;
    }
    m_nSendDataCount = 0;
    m_nRecvDataCount = 0;
  }

  ~_CommaindInfo()
  {
    Close();
  }

  void SetData(const char* pCommandID, int nCommandGroup, int nCommandSort, const char* pSendData)
  {
    Close();
    CConvertBuffer objConvertBuffer;
    m_nSendDataLen = 1024;
    m_pSendData = new unsigned char[1024];
    m_pSendData[0] = '\0';

    sprintf_s(m_szCommandID, 20, "%s", pCommandID);
    m_nCommandGroup = nCommandGroup;
    m_nCommandSort  = nCommandSort;

    int nSecLen = (int)strlen(pSendData);
    objConvertBuffer.Convertstr2charArray(pSendData, nSecLen, m_pSendData, m_nSendDataLen);
  }
};

typedef vector<_CommaindInfo* >     vecCommaindInfo;
typedef map<int, vecCommaindInfo* > mapCommaindInfoGroup;

class CCommandInfo
{
public:
  CCommandInfo(void);
  ~CCommandInfo(void);

  void Close();
  void SetCommandData(const char* pCommandID, int nCommandGroup, int nCommandSort, const char* pSendData);
  void Sort();   //È«²¿ÉýÐòÅÅÁÐ

  int GetCommandGroupCount();
  vecCommaindInfo* GetCommandGroup(int nIndex);

private:
  mapCommaindInfoGroup m_mapCommaindInfoGroup;
};
