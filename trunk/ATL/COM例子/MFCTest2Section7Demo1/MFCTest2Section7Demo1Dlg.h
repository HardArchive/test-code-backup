// MFCTest2Section7Demo1Dlg.h : ͷ�ļ�
//

#pragma once
#include "Resource.h"
#include "Sink.h"


// CMFCTest2Section7Demo1Dlg �Ի���
class CMFCTest2Section7Demo1Dlg : public CDialog
{
// ����
public:
	CMFCTest2Section7Demo1Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CMFCTest2Section7Demo1Dlg();

// �Ի�������
	enum { IDD = IDD_MFCTEST2SECTION7DEMO1_DIALOG };

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

//�¼�
public:
	//�ӷ�������
	void OnAddResult(LONG lResult);

private:
	//�¼�������
	CSink *m_pSink;

};
