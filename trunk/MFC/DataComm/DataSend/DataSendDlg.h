// DataSendDlg.h : ͷ�ļ�
//

#pragma once


// CDataSendDlg �Ի���
class CDataSendDlg : public CDialog
{
// ����
public:
	CDataSendDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DATASEND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	CWnd* m_pWnd;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonUsermsgSend();
	afx_msg void OnBnClickedButtonRegmsgSend();
	afx_msg void OnBnClickedButtonCopydataSend();
	afx_msg void OnBnClickedButtonMemSend();
	afx_msg void OnBnClickedButtonFilemapSend();
	afx_msg void OnBnClickedButtonClipboardSend();
};
