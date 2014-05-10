// MFCTestSection8Dlg.h : ͷ�ļ�
//

#pragma once
#include "Resource.h"
#include "loginax.h"
#include "Sink.h"

// CMFCTestSection8Dlg �Ի���
class CMFCTestSection8Dlg : public CDialog
{
// ����
public:
	CMFCTestSection8Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CMFCTestSection8Dlg();

// �Ի�������
	enum { IDD = IDD_MFCTESTSECTION8_DIALOG };

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
	CLoginax m_axLogin;
public:
	DECLARE_EVENTSINK_MAP()
public:
	void OnMsg(BSTR bstrMsg);												//���A���¼��������
	void OnLoginLoginax1(LPCTSTR bstrUserName, LPCTSTR bstrPassword);		//���B���¼��������
	

private:
	CComPtr<IPasswordSpObj> m_spPasswordSpObj;		//���A
	CSink *m_pSink;									//���A���¼�������Ķ���ָ��

};
