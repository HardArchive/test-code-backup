// DataSendDlg.h : 头文件
//

#pragma once


// CDataSendDlg 对话框
class CDataSendDlg : public CDialog
{
// 构造
public:
	CDataSendDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DATASEND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CWnd* m_pWnd;


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
	afx_msg void OnBnClickedButtonUsermsgSend();
	afx_msg void OnBnClickedButtonRegmsgSend();
	afx_msg void OnBnClickedButtonCopydataSend();
	afx_msg void OnBnClickedButtonMemSend();
	afx_msg void OnBnClickedButtonFilemapSend();
	afx_msg void OnBnClickedButtonClipboardSend();
};
