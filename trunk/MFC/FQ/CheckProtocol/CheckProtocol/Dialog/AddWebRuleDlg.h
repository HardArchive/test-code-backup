#pragma once
#include "../stdafx.h"
#include "afxcmn.h"
#include "afxwin.h"
// CAddWebRuleDlg �Ի���

class CAddWebRuleDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddWebRuleDlg)

public:
	CAddWebRuleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddWebRuleDlg();
    virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_DIALOG_ADD_WEBRULE };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:	
	BOOL         AddListItem();
	void         AddListInfo(LoginFileContentInfo *pLoginFileInfo,int nRow);
	BOOL         CheckEnterInfo(LoginFileContentInfo *pLoginFileInfo);
	void         ClearControlInfo();
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnMenuWebfile();
	afx_msg void OnBnClickedButtonWebDelete();
	afx_msg void OnBnClickedButtonWebModify();
	afx_msg void OnBnClickedButtonWebQuit();
	afx_msg void OnBnClickedButtonWebAdd();
	afx_msg void OnNMDblclkListWeb(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CComboBox    m_ComboBox;
	CListCtrl    m_ListCtrl;	
};
