// CH364ISL_MFCDlg.h : ͷ�ļ�
//

#pragma once

//#include "Ch366.h"

// CCH364ISL_MFCDlg �Ի���
class CCH364ISL_MFCDlg : public CDialog
{
// ����
public:
	CCH364ISL_MFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CH364ISL_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	//CCh366 m_obj366;
	int m_iRadio;    //1Ϊ���� 2Ϊ����
	int m_iCutover;  //���������ߡ��ػ�
	HANDLE m_hTimer;



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
	afx_msg void OnBnClickedButtonSwitch();
	afx_msg void OnBnClickedRadioIntranet();
	afx_msg void OnBnClickedRadioExtranet();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedRadioReboot();
	afx_msg void OnBnClickedRadioHibernate();
	afx_msg void OnBnClickedRadioShutdown();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
};
