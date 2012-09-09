// D1.cpp : 实现文件
//

#include "stdafx.h"
#include "dll1.h"
#include "D1.h"
#include ".\d1.h"


// CD1 对话框

IMPLEMENT_DYNAMIC(CD1, CDialog)
CD1::CD1(CWnd* pParent /*=NULL*/)
	: CDialog(CD1::IDD, pParent)
	, m_valueEdit1(_T(""))
{
}

CD1::~CD1()
{
}

void CD1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_valueEdit1);
}


BEGIN_MESSAGE_MAP(CD1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CD1 消息处理程序

void CD1::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_valueEdit1="顺时针电脑培训中心";
	UpdateData(0);
}
