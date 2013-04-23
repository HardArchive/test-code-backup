#pragma once
#include "afxcmn.h"


// CCaptruePackInfo �Ի���

class CCaptruePackInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CCaptruePackInfoDlg)

public:
	CCaptruePackInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCaptruePackInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CAPTURE_PACKINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	BOOL AddListItem();
	static void AddListInfo(RADE_FILE *readFile,int nRow);
	static void LoadIntercepterInfo();
    void static __stdcall CCaptruePackInfoDlg::UpdateNotice();

	static CCaptruePackInfoDlg **GetInstance()
	{
		return &m_pThis;
	}
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
public:
	CListCtrl m_ListCtrl;
	static CCaptruePackInfoDlg *m_pThis;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonRefresh();
};
