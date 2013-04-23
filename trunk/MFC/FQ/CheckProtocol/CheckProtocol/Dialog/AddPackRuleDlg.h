#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAddPackDlg �Ի���

class CAddPackRuleDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddPackRuleDlg)

public:
	CAddPackRuleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddPackRuleDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADD_PACKRULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuPackfile();
	virtual BOOL OnInitDialog();

public:
	BOOL AddListItem();
	void AddListInfo(PACKRULEFILE *pPackInfo,int nRow);
	void ClearControlInfo();
	BOOL CheckEnterInfo(PACKRULEFILE *pPackRuleInfo);

public:
	CComboBox m_ComboxBox;
	CListCtrl m_ListCtrl;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnNMDblclkListPackRule(NMHDR *pNMHDR, LRESULT *pResult);
};
