// MFCTestSection4Demo1Dlg.h : ͷ�ļ�
//

#pragma once


// CMFCTestSection4Demo1Dlg �Ի���
class CMFCTestSection4Demo1Dlg : public CDialog
{
// ����
public:
	CMFCTestSection4Demo1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCTESTSECTION4DEMO1_DIALOG };

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
	afx_msg void OnBnClickedButton1();
};
