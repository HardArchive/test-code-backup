
// PassTCPDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ClientTcpSocket.h"
#include "afxwin.h"
#include <afxpriv.h>
#include <vector>

using namespace std;

typedef vector<CClientTcpSocket* > vecClientTcpSocket;

// CPassTCPDlg 对话框
class CPassTCPDlg : public CDialog
{
// 构造
public:
	CPassTCPDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PASSTCP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButton1();

private:
  void InitView();
  void ClearResult();
  void Close();

public:
  CEdit m_txtServerIP;
  CEdit m_txtPort;
  CEdit m_txtThreadCount;
  CEdit m_txtRecvTimeout;
  CEdit m_txtSocketInterval;
  CEdit m_txtRecvLength;
  CEdit m_txtSendData;
  CButton m_chkIsAlwayConnect;
  CButton m_chkRadomaDelay;
  CButton m_chkIsRecv;
  CButton m_ChkIsBroken;
  CButton m_chkRadomSendCount;
  CButton m_chkIsWriteLog;
  CButton m_chkSendOne;
  CEdit m_txtSuccessConnect;
  CEdit m_txtSuccessSend;
  CEdit m_txtSuccessRecv;
  CEdit m_txtCurrConnect;
  CEdit m_txtFailConnect;
  CEdit m_txtFailSend;
  CEdit m_txtFailRecv;
  
  vecClientTcpSocket m_vecClientTcpSocket;
  afx_msg void OnClose();
  afx_msg void OnBnClickedButton2();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnBnClickedButton3();
};
