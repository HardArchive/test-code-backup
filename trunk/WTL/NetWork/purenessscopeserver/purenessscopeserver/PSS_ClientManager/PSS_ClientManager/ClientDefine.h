#ifndef _CLIENTDEFINE_H
#define _CLIENTDEFINE_H

#define MESSAGE_SENDERROR      "���͸�����������ʧ��"
#define MESSAGE_TITLE_ERROR    "������Ϣ"
#define MESSAGE_TITLE_SUCCESS  "��������Ϣ" 
#define MESSAGE_TITLE_CONNECT  "���÷�������ַ�ɹ�"
#define MESSAGE_RESULT_SUCCESS "����ִ�гɹ�"
#define MESSAGE_RESULT_FAIL    "����ִ��ʧ��"
#define MESSAGE_INSERT_NULL    "�������������Ϣ���ܽ��д˲���"

struct _ClientConnectInfo
{
  char       m_szIP[50];            //IP��ַ��Ϣ
  int        m_nConnectID;          //���ӵ�ID
  int        m_nRecvCount;          //���հ�����
  int        m_nSendCount;          //���Ͱ�����
  int        m_nAllRecvSize;        //�����ֽ���
  int        m_nAllSendSize;        //�����ֽ���
  int        m_nBeginTime;          //���ӽ���ʱ��
  int        m_nAliveTime;          //���ʱ������
  int        m_nRecvQueueCount;     //�����߼�������ĸ���
  long long  m_nRecvQueueTimeCost;  //�����߼��������ʱ������
  long long  m_nSendQueueTimeCost;  //����������ʱ������

  _ClientConnectInfo()
  {
    m_szIP[0]            = '\0';
    m_nConnectID         = 0;
    m_nRecvCount         = 0;
    m_nSendCount         = 0;
    m_nAllRecvSize       = 0;
    m_nAllSendSize       = 0;
    m_nBeginTime         = 0;
    m_nAliveTime         = 0;
    m_nRecvQueueCount    = 0;
    m_nRecvQueueTimeCost = 0;
    m_nSendQueueTimeCost = 0;
  }
};


#endif
