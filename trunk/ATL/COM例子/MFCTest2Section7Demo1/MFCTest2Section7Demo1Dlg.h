// MFCTest2Section7Demo1Dlg.h : 头文件
//

#pragma once
#include "Resource.h"
#include "Sink.h"


// CMFCTest2Section7Demo1Dlg 对话框
class CMFCTest2Section7Demo1Dlg : public CDialog
{
// 构造
public:
	CMFCTest2Section7Demo1Dlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CMFCTest2Section7Demo1Dlg();

// 对话框数据
	enum { IDD = IDD_MFCTEST2SECTION7DEMO1_DIALOG };

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

//事件
public:
	//加法运算结果
	void OnAddResult(LONG lResult);

private:
	//事件接收器
	CSink *m_pSink;

};
