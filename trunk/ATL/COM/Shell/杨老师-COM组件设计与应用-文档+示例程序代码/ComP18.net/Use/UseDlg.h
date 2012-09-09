// UseDlg.h : ͷ�ļ�
//

#pragma once
#include "propertybag.h"


// CUseDlg �Ի���
class CUseDlg : public CDialog
{
// ����
public:
	CUseDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_USE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_str;
	long m_integer;
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedEnd();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedGet();
private:
	// ������������ָ��
	IPropertyPtr m_spObj;
	// Ϊ������� IPersistPropertyBag �ӿڶ��������ж���� IPropertyBag �ӿڶ���
	CPropertyBag m_PropertyBag;
};
