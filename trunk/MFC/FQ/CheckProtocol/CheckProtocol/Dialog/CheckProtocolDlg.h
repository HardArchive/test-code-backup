
// CheckProtocolDlg.h : ͷ�ļ�

#pragma once

class CCheckProtocolDlg : public CDialog
{
// ����
public:
	CCheckProtocolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHECKPROTOCOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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
	afx_msg void OnMenuPackfile();
	afx_msg void OnMenuWebfile();
	afx_msg void OnMenuLoginTest();
	afx_msg void OnMenuSendTest();
	afx_msg void OnMenuFindPacp();
	afx_msg void OnMenuDownload();
	afx_msg void OnMenuUpdataDownload();
	afx_msg void OnMenuVoice();
};
