// MFCTestSection6Demo1Dlg.h : ͷ�ļ�
//

#pragma once
#include "hellocompositecontrol1.h"


// CMFCTestSection6Demo1Dlg �Ի���
class CMFCTestSection6Demo1Dlg : public CDialog
{
// ����
public:
	CMFCTestSection6Demo1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCTESTSECTION6DEMO1_DIALOG };

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
	DECLARE_EVENTSINK_MAP()

//������ť�ĵ���¼�
private:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();

//���Ͽؼ����¼�����
private:
	void OnClickBtnHellocompositecontrol1(long lBtnIndex, long lClickCountSum);

private:
	CHellocompositecontrol1 m_hcc;		//���Ͽؼ��İ�װ��Ķ���
	
};
