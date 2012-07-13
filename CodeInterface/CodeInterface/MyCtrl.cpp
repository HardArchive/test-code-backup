#include "StdAfx.h"
#include "MyCtrl.h"

CMyCtrl::CMyCtrl(void)
{
	m_receiver = NULL;
}

CMyCtrl::~CMyCtrl(void)
{
	m_receiver = NULL;
}

void CMyCtrl::OnClick() 
{ 
	m_receiver->OnClick();
	//m_receiver -> OnMyCtrlClick(); 
} 

BEGIN_MESSAGE_MAP(CMyCtrl, CDialog)
	//{{AFX_MSG_MAP(CMyCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CMyCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnClick();

	CDialog::OnLButtonDown(nFlags, point);
}

void CMyCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnClick();

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CMyCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	OnClick();
	CDialog::OnMouseMove(nFlags, point);
}
