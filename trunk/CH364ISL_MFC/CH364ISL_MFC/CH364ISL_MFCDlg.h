// CH364ISL_MFCDlg.h : 头文件
//

#pragma once

//#include "Ch366.h"

// CCH364ISL_MFCDlg 对话框
class CCH364ISL_MFCDlg : public CDialog
{
// 构造
public:
	CCH364ISL_MFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CH364ISL_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	//CCh366 m_obj366;
	int m_iRadio;    //1为内网 2为外网
	int m_iCutover;  //重启、休眠、关机
	HANDLE m_hTimer;



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
	afx_msg void OnBnClickedButtonSwitch();
	afx_msg void OnBnClickedRadioIntranet();
	afx_msg void OnBnClickedRadioExtranet();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedRadioReboot();
	afx_msg void OnBnClickedRadioHibernate();
	afx_msg void OnBnClickedRadioShutdown();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
};
