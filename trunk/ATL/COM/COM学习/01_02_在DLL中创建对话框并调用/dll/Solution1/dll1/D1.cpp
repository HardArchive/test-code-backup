// D1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "dll1.h"
#include "D1.h"
#include ".\d1.h"


// CD1 �Ի���

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


// CD1 ��Ϣ�������

void CD1::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_valueEdit1="˳ʱ�������ѵ����";
	UpdateData(0);
}
