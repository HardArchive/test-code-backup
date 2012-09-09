#pragma once


// CD1 对话框

class CD1 : public CDialog
{
	DECLARE_DYNAMIC(CD1)

public:
	CD1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CD1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_valueEdit1;
	afx_msg void OnBnClickedButton1();
};
