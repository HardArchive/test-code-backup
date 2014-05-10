// MFCTestSection8Dlg.h : 头文件
//

#pragma once
#include "Resource.h"
#include "loginax.h"
#include "Sink.h"

// CMFCTestSection8Dlg 对话框
class CMFCTestSection8Dlg : public CDialog
{
// 构造
public:
	CMFCTestSection8Dlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CMFCTestSection8Dlg();

// 对话框数据
	enum { IDD = IDD_MFCTESTSECTION8_DIALOG };

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
	CLoginax m_axLogin;
public:
	DECLARE_EVENTSINK_MAP()
public:
	void OnMsg(BSTR bstrMsg);												//组件A的事件处理程序
	void OnLoginLoginax1(LPCTSTR bstrUserName, LPCTSTR bstrPassword);		//组件B的事件处理程序
	

private:
	CComPtr<IPasswordSpObj> m_spPasswordSpObj;		//组件A
	CSink *m_pSink;									//组件A的事件接收类的对象指针

};
