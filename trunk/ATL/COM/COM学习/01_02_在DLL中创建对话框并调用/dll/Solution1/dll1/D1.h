#pragma once


// CD1 �Ի���

class CD1 : public CDialog
{
	DECLARE_DYNAMIC(CD1)

public:
	CD1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CD1();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_valueEdit1;
	afx_msg void OnBnClickedButton1();
};
