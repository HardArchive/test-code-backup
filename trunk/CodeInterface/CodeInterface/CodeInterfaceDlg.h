
// CodeInterfaceDlg.h : ͷ�ļ�
//

#pragma once
#include "ClickReceiver.h"
#include "MyCtrl.h"
// CCodeInterfaceDlg �Ի���
class CCodeInterfaceDlg : public CDialog, public ClickReceiver
{
// ����
public:
	CCodeInterfaceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CODEINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public: 
	virtual void OnClick()
	{
		::MessageBox(NULL, L"�޹�����Ӧ", L"�ɹ�", MB_OK);
	}
public:
	CMyCtrl m_Static;

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
	//afx_msg void OnStnClickedStaticTest();
};
