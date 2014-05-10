// Section6Demo2Dlg.h : 头文件
//

#pragma once


// CSection6Demo2Dlg 对话框
class CSection6Demo2Dlg : public CDialog
{
// 构造
public:
	CSection6Demo2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SECTION6DEMO2_DIALOG };

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
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButton5();
};
