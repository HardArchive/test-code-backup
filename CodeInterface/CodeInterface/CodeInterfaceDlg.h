
// CodeInterfaceDlg.h : 头文件
//

#pragma once
#include "ClickReceiver.h"
#include "MyCtrl.h"
// CCodeInterfaceDlg 对话框
class CCodeInterfaceDlg : public CDialog, public ClickReceiver
{
// 构造
public:
	CCodeInterfaceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CODEINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public: 
	virtual void OnClick()
	{
		::MessageBox(NULL, L"无关联响应", L"成功", MB_OK);
	}
public:
	CMyCtrl m_Static;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnStnClickedStaticTest();
};
