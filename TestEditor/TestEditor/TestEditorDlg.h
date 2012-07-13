
// TestEditorDlg.h : 头文件
//

#pragma once
//#include "ScintillaWnd.h"

// CTestEditorDlg 对话框
class CTestEditorDlg : public CDialog
{
// 构造
public:
	CTestEditorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	//CScintillaWnd m_ScintillaWnd;  
	HWND m_hScintillaWnd;


// 实现
protected:
	HICON m_hIcon;
	
public:
	void UpdateLineNumberWidth(void);
	LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) 
	{
		return ::SendMessage(m_hScintillaWnd/*m_ScintillaWnd*/, Msg, wParam, lParam);
	}

	// 生成的消息映射函数
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
