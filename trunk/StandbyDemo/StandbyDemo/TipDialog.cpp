// TipDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StandbyDemo.h"
#include "TipDialog.h"


// CTipDialog �Ի���
#define ID_TIMER_POP_WINDOW		10
#define ID_TIMER_CLOSE_WINDOW	11
#define ID_TIMER_DISPLAY_DELAY	12
#define WIN_WIDTH	181
#define WIN_HEIGHT	116
IMPLEMENT_DYNAMIC(CTipDialog, CDialog)

CTipDialog::CTipDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTipDialog::IDD, pParent)
{

}

CTipDialog::~CTipDialog()
{
}

void CTipDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTipDialog, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CTipDialog ��Ϣ�������

void CTipDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static int nHeight=0;
	int cy=GetSystemMetrics(SM_CYSCREEN);
	int cx=GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	int y=rect.bottom-rect.top;
	int x=rect.right-rect.left;
	x=x-WIN_WIDTH;

	switch(nIDEvent)
	{
	case ID_TIMER_POP_WINDOW:
		if(nHeight<=WIN_HEIGHT)
		{
			++nHeight;			
			MoveWindow(x,
				y-nHeight,
				WIN_WIDTH,
				WIN_HEIGHT);

			Invalidate(FALSE);
		}
		else
		{
			KillTimer(ID_TIMER_POP_WINDOW);
			SetTimer(ID_TIMER_DISPLAY_DELAY,5000,NULL);
		}
		break;
	case ID_TIMER_CLOSE_WINDOW:
		if(nHeight>=0)
		{
			nHeight--;
			MoveWindow(x,
				y-nHeight,
				WIN_WIDTH,
				nHeight);
		}
		else
		{
			KillTimer(ID_TIMER_CLOSE_WINDOW);
			//SendMessage(WM_CLOSE);
			ShowWindow(SW_HIDE);
		}
		break;
	case ID_TIMER_DISPLAY_DELAY:
		KillTimer(ID_TIMER_DISPLAY_DELAY);
		SetTimer(ID_TIMER_CLOSE_WINDOW,20,NULL);
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CTipDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	//CRect rect;
	//GetClientRect(&rect);

	//CFont font;
	//font.CreatePointFont(90,"Impact");
	//dc.SelectObject(&font);
	//dc.SetTextColor(RGB(0,64,128));
	//dc.SetBkMode(TRANSPARENT);
	//dc.TextOut(30, 10, "�����ܼ���ʾ");
	//rect.top=30;
	//dc.DrawText(m_szMessage, -1, &rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CTipDialog::SetPromptMessage(PCHAR pszMsg)
{
	strcpy(m_szMessage, pszMsg);
	m_bFlag=false;
	SetTimer(ID_TIMER_POP_WINDOW,20,NULL);
	SetDlgItemText(IDC_STATIC_TIP, m_szMessage);
	UpdateData(FALSE);
}
void CTipDialog::PreInitDialog()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	CDialog::PreInitDialog();
}

void CTipDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(&rect);
	if(rect.PtInRect(point))
	{
		m_bFlag=true;
		KillTimer(ID_TIMER_DISPLAY_DELAY);
	}	


	CDialog::OnMouseMove(nFlags, point);
}

void CTipDialog::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	if(m_bFlag)
		SetTimer(ID_TIMER_DISPLAY_DELAY,3000,NULL);
	// TODO: Add your message handler code here

}

void CTipDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::MessageBox(NULL, "��ʾ���Ѿ���˫��������", "TIPTOP��ʾ", MB_OK);

	CDialog::OnLButtonDown(nFlags, point);
}
