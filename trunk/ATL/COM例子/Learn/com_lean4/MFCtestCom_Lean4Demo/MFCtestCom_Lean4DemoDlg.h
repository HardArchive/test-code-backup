
// MFCtestCom_Lean4DemoDlg.h : ͷ�ļ�
//

#pragma once


// CMFCtestCom_Lean4DemoDlg �Ի���
class CMFCtestCom_Lean4DemoDlg : public CDialog
{
// ����
public:
	CMFCtestCom_Lean4DemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCTESTCOM_LEAN4DEMO_DIALOG };

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
};
