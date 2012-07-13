
// StandbyDemoDlg.h : 头文件
//

#pragma once

#include "TipDialog.h"
// CStandbyDemoDlg 对话框
class CStandbyDemoDlg : public CDialog
{
// 构造
public:
	CStandbyDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_STANDBYDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CTipDialog* m_pTipDialog ;
	HANDLE hTimer;	      //唤醒时间句柄

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChildActivate();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton1();
};
