// MFCTestSection6Demo1Dlg.h : 头文件
//

#pragma once
#include "hellocompositecontrol1.h"


// CMFCTestSection6Demo1Dlg 对话框
class CMFCTestSection6Demo1Dlg : public CDialog
{
// 构造
public:
	CMFCTestSection6Demo1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCTESTSECTION6DEMO1_DIALOG };

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
	DECLARE_EVENTSINK_MAP()

//六个按钮的点击事件
private:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();

//复合控件的事件接收
private:
	void OnClickBtnHellocompositecontrol1(long lBtnIndex, long lClickCountSum);

private:
	CHellocompositecontrol1 m_hcc;		//复合控件的包装类的对象
	
};
