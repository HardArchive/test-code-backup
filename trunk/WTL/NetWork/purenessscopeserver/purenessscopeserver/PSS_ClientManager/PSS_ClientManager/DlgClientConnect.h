#pragma once

#include "ClientDefine.h"
#include "TcpClientConnect.h"
#include "afxwin.h"
#include "afxcmn.h"

#include <vector>

using namespace std;

struct _IPAccount
{
  char           m_szIP[50];   //��ǰ���ӵ�ַ
  int            m_nCount;              //��ǰ���Ӵ���
  int            m_nAllCount;           //ָ��IP���Ӵ����ܺ� 
  int            m_nMinute;             //��ǰ������
  char           m_szDate[50]; //������ʱ��

  _IPAccount()
  {
    m_szIP[0]    = '\0';
    m_nCount     = 0;
    m_nAllCount  = 0;
    m_szDate[0]  = '\0'; 
  }
};

typedef vector<_IPAccount> vecIPAccount;

class CDlgClientConnect : public CDialog
{
	DECLARE_DYNAMIC(CDlgClientConnect)

public:
	CDlgClientConnect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgClientConnect();

// Dialog Data
	enum { IDD = IDD_DIALOG_CLIENTCONNECT };

public:
  CString GetPageTitle();
  void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
  CTcpClientConnect* m_pTcpClientConnect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CEdit m_txtClientConnectCount;
  CEdit m_txtCpu;
  CEdit m_txtMemory;
  CListCtrl m_lcClientConnectHistory;
  afx_msg void OnBnClickedButton1();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton6();
};
