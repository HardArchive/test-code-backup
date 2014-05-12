
// MFCtestCom_Lean4DemoDlg.h : 头文件
//

#pragma once


// CMFCtestCom_Lean4DemoDlg 对话框
class CMFCtestCom_Lean4DemoDlg : public CDialog
{
// 构造
public:
	CMFCtestCom_Lean4DemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCTESTCOM_LEAN4DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
