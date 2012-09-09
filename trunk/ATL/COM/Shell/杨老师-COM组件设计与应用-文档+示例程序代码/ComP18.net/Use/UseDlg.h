// UseDlg.h : 头文件
//

#pragma once
#include "propertybag.h"


// CUseDlg 对话框
class CUseDlg : public CDialog
{
// 构造
public:
	CUseDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_USE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_str;
	long m_integer;
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedEnd();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedGet();
private:
	// 组件对象的智能指针
	IPropertyPtr m_spObj;
	// 为调用组件 IPersistPropertyBag 接口而在容器中定义的 IPropertyBag 接口对象
	CPropertyBag m_PropertyBag;
};
