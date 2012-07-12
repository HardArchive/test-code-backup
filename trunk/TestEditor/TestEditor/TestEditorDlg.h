
// TestEditorDlg.h : ͷ�ļ�
//

#pragma once
//#include "ScintillaWnd.h"

// CTestEditorDlg �Ի���
class CTestEditorDlg : public CDialog
{
// ����
public:
	CTestEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	//CScintillaWnd m_ScintillaWnd;  
	HWND m_hScintillaWnd;


// ʵ��
protected:
	HICON m_hIcon;
	
public:
	void UpdateLineNumberWidth(void);
	LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) 
	{
		return ::SendMessage(m_hScintillaWnd/*m_ScintillaWnd*/, Msg, wParam, lParam);
	}

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
