// DataRecvDlg.h : 头文件
//

#pragma once

#define WM_COMM      WM_USER+100       //用户自定义消息

// CDataRecvDlg 对话框
class CDataRecvDlg : public CDialog
{
// 构造
public:
	CDataRecvDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DATARECV_DIALOG };

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
	afx_msg void OnBnClickedButtonFilemapRead();
	afx_msg void OnBnClickedButtonClipoardRead();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

public:
	//自定义消息
	afx_msg LRESULT OnUserReceiveMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRegReceiveMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRegMemMsg(WPARAM wParam,LPARAM lParam);
};
