
// WebPageDownLoadDlg.h : ͷ�ļ�
//

#pragma once


// CWebPageDownLoadDlg �Ի���
class CWebPageDownLoadDlg : public CDialog
{
// ����
public:
	CWebPageDownLoadDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WEBPAGEDOWNLOAD_DIALOG };

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
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonDownload();
	CString m_csSavePath;
	CString m_csEditUrl;
};
