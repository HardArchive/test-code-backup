#ifndef _FORBIDDEN_H
#define _FORBIDDEN_H

//�ܾò������������д�����ˣ���ʵ��һ�����������һ�ּ�֡�����ֻʣ����һ���ˡ�
//�������ã���ʵ����������ң����������ڴ���Ҳ�����ڷ���������һ�ֱ�Ȼ��
//�ǵģ�����������������ô�򵥡�û��ʲô�������Ҽ�֣�Ҳ������ʲô�������ҷ�����
//���ǣ�������ǰ�У����汾Ȼ�ɡ�
//ȫ��xml��������һ�������⣬��л��͹man��
//add by freeeyes
//2013-03-09

#include "define.h"
#include "XmlOpeation.h"
#include "ace/Singleton.h"

#define MAX_IP_SIZE 20

struct _ForbiddenIP
{
  char           m_szClientIP[MAX_IP_SIZE];   //����ֹ��IP
  uint8          m_u1Type;                    //��ֹ�����ͣ�0Ϊ���ý�ֹ��1Ϊʱ�ν�ֹ��
  ACE_Time_Value m_tvBegin;                   //ʱ�ν�ֹ��ʼʱ��
  uint32         m_u4Second;                  //��ֹ������
  uint8          m_u1ConnectType;             //���ӵ����ͣ�0ΪTCP��1ΪUDP  

  _ForbiddenIP()
  {
    m_szClientIP[0] = '\0';
    m_u1Type        = 0;
    m_u4Second      = 0;
    m_u1ConnectType = CONNECT_TCP;            //Ĭ��ΪTCP��
    m_tvBegin       = ACE_OS::gettimeofday(); //Ĭ�Ͽ�ʼʱ��Ϊ��ǰʱ�� 
  }
};

typedef vector<_ForbiddenIP> VecForbiddenIP;

class CForbiddenIP
{
public:
  CForbiddenIP();
  ~CForbiddenIP();

  bool Init(const char* szConfigPath);                                                    //��ʼ���������÷�ͣIP�ļ�
  bool CheckIP(const char* pIP, uint8 u1ConnectType = CONNECT_TCP);                       //���IP�Ƿ�������� 
  bool AddForeverIP(const char* pIP, uint8 u1ConnectType = CONNECT_TCP);                  //������÷�ͣ��IP 
  bool AddTempIP(const char* pIP, uint32 u4Second, uint8 u1ConnectType = CONNECT_TCP);    //�����ʱ��ͣ��IP
  bool DelForeverIP(const char* pIP, uint8 u1ConnectType = CONNECT_TCP);                  //ɾ�����÷�ͣIP
  bool DelTempIP(const char* pIP, uint8 u1ConnectType = CONNECT_TCP);                     //ɾ����ʱ��ͣIP
  VecForbiddenIP* ShowForeverIP() const;                                                  //��ʾ���÷�ͣIP
  VecForbiddenIP* ShowTemoIP() const;                                                     //��ʾ��ʱ��ͣIP

private:
  bool SaveConfig();                                                                      //�洢�����ļ�
  bool ParseTXT(const char* pText, char* pIP, char* pConnectType);                        //���������ļ�����Ϣ
  bool CompareIP(char* pTargetIP, char* pClientIP);                                       //�Ƚ��Ƿ��ڵ�ǰIP��

private:
  CXmlOpeation   m_ForbiddenData;
  VecForbiddenIP m_VecForeverForbiddenIP;           //���÷�ͣ��IP�б�
  VecForbiddenIP m_VecTempForbiddenIP;              //��ʱ��ͣ��IP�б�
};

typedef ACE_Singleton<CForbiddenIP, ACE_Null_Mutex> App_ForbiddenIP;

#endif
