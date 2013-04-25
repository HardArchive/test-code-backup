// DataRecvDlg.h : ͷ�ļ�
//

#pragma once

#define WM_COMM      WM_USER+100       //�û��Զ�����Ϣ

// CDataRecvDlg �Ի���
class CDataRecvDlg : public CDialog
{
// ����
public:
	CDataRecvDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DATARECV_DIALOG };

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
	afx_msg void OnBnClickedButtonFilemapRead();
	afx_msg void OnBnClickedButtonClipoardRead();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

public:
	//�Զ�����Ϣ
	afx_msg LRESULT OnUserReceiveMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRegReceiveMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRegMemMsg(WPARAM wParam,LPARAM lParam);
};
