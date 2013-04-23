
// CheckProtocolDlg.h : 头文件

#pragma once

class CCheckProtocolDlg : public CDialog
{
// 构造
public:
	CCheckProtocolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHECKPROTOCOL_DIALOG };

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
	afx_msg void OnMenuPackfile();
	afx_msg void OnMenuWebfile();
	afx_msg void OnMenuLoginTest();
	afx_msg void OnMenuSendTest();
	afx_msg void OnMenuFindPacp();
	afx_msg void OnMenuDownload();
	afx_msg void OnMenuUpdataDownload();
	afx_msg void OnMenuVoice();
};
