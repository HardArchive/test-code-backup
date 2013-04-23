#pragma once

#include "../Thread/PackThread.h"

// CFindCaptureRuleDlg �Ի���
class CFindCaptureRuleDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindCaptureRuleDlg)

public:
	CFindCaptureRuleDlg(CWnd* pParent = NULL);            // ��׼���캯��
	virtual ~CFindCaptureRuleDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FIND_PACKRUEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);      // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFinddlgStartcapture();
	afx_msg void OnBnClickedButtonFinddlgFilter();
	afx_msg void OnBnClickedButtonFinddlgStop();
	afx_msg void OnBnClickedButtonFinddlgQuite();
	afx_msg void OnBnClickedButtonFinddlgSave();
public:
	bool         AddListItem();
	void         AddListInfo(int nRow,PACKRULEFILE *pPackRuleInfo);
	void         LoadRulePackInfoToList();
public:
	void static __stdcall _CallStopCapture();
	void static __stdcall _CallAddPackRuleInfo(PackRuleFile *pPackRuleFile);
public:
	int                                     m_packServiceType;
	CString			                        m_strError;	
	CComboBox                               m_ComboBox;
	CPackThread                             m_packThread;
	multiset<string>                        m_setStrPackHead;							//�洢��ͷ���ڹ���
	static CFindCaptureRuleDlg             *m_pThis;
	CListCtrl                               m_ListFindPack;
	afx_msg void OnBnClickedButtonClear();
};
