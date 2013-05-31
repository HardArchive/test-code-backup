#pragma once
#include "afxcmn.h"
#include "ClientDefine.h"
#include "TcpClientConnect.h"

#include <vector>
#include <time.h>

using namespace std;

struct _WorkThreadInfo
{
  char                m_szThreadName[50];   //�߳�����
  int                 m_nThreadID;         //�߳�ID
  int                 m_nUpdateTime;       //�߳���������ݵ�ʱ��
  int                 m_nState;            //0Ϊû���������ڴ���1Ϊ���ڴ���������ݡ�
  int                 m_nRecvPacketCount;  //���߳��½��հ��Ĵ������
  int                 m_nSendPacketCount;  //���߳��·��Ͱ��Ĵ������
  int                 m_nCommandID;        //��ǰ���������ID
  int                 m_nPacketTime;       //ƽ�����ݰ�����ʱ��
  int                 m_nCurrPacketCount;  //��λʱ���ڵİ���������

  _WorkThreadInfo()
  {
    m_nThreadID        = 0;
    m_nState           = 0;
    m_nRecvPacketCount = 0;
    m_nSendPacketCount = 0;
    m_nCommandID       = 0;
    m_nPacketTime      = 0;
    m_nCurrPacketCount = 0;
  }
};

typedef vector<_WorkThreadInfo> vecWorkThreadInfo;

class CDlgClientWorkThread : public CDialog
{
	DECLARE_DYNAMIC(CDlgClientWorkThread)

public:
	CDlgClientWorkThread(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgClientWorkThread();

// Dialog Data
	enum { IDD = IDD_DIALOG_WORKTHREAD };

public:
  CString GetPageTitle();
  void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
  CTcpClientConnect* m_pTcpClientConnect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CListCtrl m_lcWorkThreadInfo;
  afx_msg void OnBnClickedButton1();
  virtual BOOL OnInitDialog();
};
