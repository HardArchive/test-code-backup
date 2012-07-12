
// StandbyDemoDlg.h : ͷ�ļ�
//

#pragma once

#include "TipDialog.h"
// CStandbyDemoDlg �Ի���
class CStandbyDemoDlg : public CDialog
{
// ����
public:
	CStandbyDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STANDBYDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CTipDialog* m_pTipDialog ;
	HANDLE hTimer;	      //����ʱ����

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChildActivate();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton1();
};
