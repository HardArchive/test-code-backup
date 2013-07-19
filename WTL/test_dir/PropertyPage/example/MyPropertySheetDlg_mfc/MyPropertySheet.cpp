// MyPropertySheet.cpp : 实现文件
//

#include "stdafx.h"

#include "MyPropertySheet.h"


// CMyPropertySheet
static BOOL bMoved[4] = {FALSE,FALSE,FALSE,FALSE};

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

CMyPropertySheet::CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage), m_nSelectedItem(0), m_clrTextBkSele(RGB(0,132,255)),m_clrSeprator(RGB(0,132,255)),
	m_clrCaption(RGB(92,132,255))
{
	m_szCaption = new TCHAR[128];
		

	//默认16*16，32位色图标
	m_imgList.Create(16,16,ILC_COLOR32, 0, 20);

	m_ftList.CreatePointFont(90,_T("宋体"));

}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage), m_nSelectedItem(0), m_clrTextBkSele(RGB(0,132,255)),m_clrSeprator(RGB(0,132,255)),
	m_clrCaption(RGB(92,132,255))
{
	m_szCaption = new TCHAR[128];

	m_imgList.Create(16,16,ILC_COLOR32, 0, 20);
	m_ftList.CreatePointFont(90,_T("宋体"));

}

CMyPropertySheet::~CMyPropertySheet()
{

	delete [] m_szCaption;
	
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, 0xFFFF, OnNMClick)
	ON_NOTIFY(NM_CUSTOMDRAW,0xFFFF, OnNMCustomDraw)
	
END_MESSAGE_MAP()


// CMyPropertySheet 消息处理程序

BOOL CMyPropertySheet::OnInitDialog()
{
	
	BOOL bResult = CPropertySheet::OnInitDialog();	
 
	//计算属性页的矩形，扩大属性表并将属性页其移至右侧
	CRect rect, rectPage, rectTab;
	GetPage(0)->GetWindowRect(&rectPage);
	
	GetWindowRect(&rect);
	rect.right += 150;
	
    int nWidth = rectPage.Width();
	rectPage.right = rect.right - 20; 
	rectPage.left = rect.right - nWidth;
	ScreenToClient(&rectPage);
	m_rectPage = rectPage;
	MoveWindow(&rect);
	GetPage(0)->MoveWindow(&rectPage);
    
	//隐藏属性页原来的TabControl
	CTabCtrl *pTab = GetTabControl() ;
	pTab->GetWindowRect(&rectTab);
	ScreenToClient(&rectTab);
	if(!pTab->ShowWindow(SW_HIDE))
		return FALSE;

	
	
   
	//创建列表控件并用一个CImageList对象与之关联
	if(!m_wndList.Create(WS_CHILD | WS_VISIBLE |  LVS_REPORT | LVS_NOCOLUMNHEADER , CRect(10 ,rectTab.top,150,rectPage.bottom ),this,0xFFFF))
		return FALSE;
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_wndList.SetImageList(&m_imgList, LVSIL_SMALL);
	
	InitList();

	//这一步是为了扩大行高度
	CFont font;
	font.CreatePointFont(240,_T("宋体"));
	m_wndList.SetFont(&font);
	
	CString strCaption;
	GetPage(0)->GetWindowText(strCaption);	
	_tcscpy(m_szCaption, strCaption.GetBuffer(strCaption.GetLength()));	
	ModifyStyleEx(0,WS_EX_APPWINDOW); 
	return bResult;
}

void CMyPropertySheet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rectList,rectPage;
	m_wndList.GetWindowRect(&rectList);
	GetPage(0)->GetWindowRect(&rectPage);
	ScreenToClient(&rectPage);
	ScreenToClient(&rectList);

	rectList.left = rectList.left -1;
	rectList.right = rectList.right + 1;
	rectList.top = rectList.top - 1;
	rectList.bottom = rectList.bottom + 1;

	rectPage.left -= 1;
	rectPage.right += 1;
	rectPage.top -= 1;
	rectPage.bottom += 1;
	CBrush brush(RGB(141,141,141));
	dc.FrameRect(&rectList,&brush);
	dc.FrameRect(&rectPage, &brush);
	
		
	DrawCaption(&dc, m_clrCaption);
	
	
	
	
}

void CMyPropertySheet::DrawCaption(CDC * pDC, const COLORREF clrCaption)
{
	CDC dcBuf;
	dcBuf.CreateCompatibleDC(pDC);
	CBitmap bmp;
	
	CRect rectCap, rectList, rectPage,rectSheet;
	m_wndList.GetWindowRect(&rectList);
	ScreenToClient(&rectList);
	GetPage(0)->GetWindowRect(&rectPage);
	ScreenToClient(&rectPage);

	rectCap = rectPage;
	rectCap.top = rectList.top -1;
	rectCap.left -= 1;
	rectCap.right += 1;
	rectCap.bottom =rectPage.top -1 ;


	GetClientRect(&rectSheet);
	
	rectCap.bottom +=1;
	
	
	
    bmp.CreateCompatibleBitmap(pDC, rectCap.right , rectSheet.Height());

    
	dcBuf.SelectObject(bmp);

	//起始颜色
	int clrBBase = clrCaption>>16 & 0x000000FF;
	int clrGBase = clrCaption>>8 & 0x000000FF;
	int clrRBase = clrCaption & 0x000000FF;	

	//过渡中颜色
	int clrRCurr = clrRBase;
	int clrGCurr = clrGBase;
	int clrBCurr = clrBBase;
	
	//色彩增量
	const double nRClrInc = (double)(255 - clrRBase) / (double)rectCap.Width() ;	
	const double nGClrInc = (double)(255 - clrGBase) / (double)rectCap.Width() ;
	const double nBClrInc = (double)(255 - clrBBase) / (double)rectCap.Width() ;



	//画渐进色标题

	CRect drawRect = rectCap;
	
	for (int nLeft = rectCap.left, nRight = rectCap.left + 1 ; nLeft < rectCap.right; nLeft ++, nRight ++)
	{

		drawRect.left = nLeft;
		drawRect.right = nRight;

		dcBuf.FillSolidRect(&drawRect, RGB(clrRCurr,clrGCurr,clrBCurr));

		clrRCurr = (int)((nLeft - rectCap.left) * nRClrInc + clrRBase);
		clrGCurr = (int)((nLeft - rectCap.left) * nGClrInc + clrGBase);
		clrBCurr = (int)((nLeft - rectCap.left) * nBClrInc + clrBBase);
		
	}

	

	dcBuf.SetBkMode(TRANSPARENT);
	CFont font;
	

	
   
	font.CreatePointFont(110,_T("宋体"),pDC);
	dcBuf.SelectObject(&font);
	

	
	dcBuf.SetTextColor(RGB(0,0,0));
	dcBuf.TextOut(rectCap.left + 26, rectCap.top +5,m_szCaption, (int)_tcslen(m_szCaption));
	dcBuf.SetTextColor(RGB(255,255,255));
	dcBuf.TextOut(rectCap.left + 25, rectCap.top + 4, m_szCaption, (int)_tcslen(m_szCaption));

	::DrawIconEx(dcBuf,rectCap.left + 4, rectCap.top + 3, m_imgList.ExtractIcon(m_nSelectedItem),16, 16, NULL,NULL, DI_NORMAL);


		
		
		

	pDC->BitBlt(rectCap.left,rectCap.top,rectCap.Width()+rectCap.Width(),rectCap.Height(),&dcBuf,rectCap.left,rectCap.top,SRCCOPY);
	
}

void CMyPropertySheet::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE lpItem = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nSelectedItem = lpItem->iItem ;

   if (lpItem->iItem  >= 0 && lpItem->iItem < m_wndList.GetItemCount())
   {
	   m_nSelectedItem = lpItem->iItem;
	   CString strCaption = m_wndList.GetItemText(lpItem->iItem,0);
	_tcscpy(m_szCaption, strCaption);
	
	

	SetActivePage(m_nSelectedItem);
	
	Invalidate();
	
	
	GetPage(m_nSelectedItem)->MoveWindow(&m_rectPage);

	m_wndList.SetFocus();
   }
	
	
}
void CMyPropertySheet::InitList(void)
{
	LVITEM lvi;
	::ZeroMemory(&lvi, sizeof(lvi));
	
	
	CHeaderCtrl *pHeader = m_wndList.GetHeaderCtrl();
	
	
	pHeader->ShowWindow(SW_HIDE);

	CRect rectList;
	m_wndList.GetWindowRect(&rectList);
	ScreenToClient(&rectList);
	//报表头不会显示，但是是必需的
	m_wndList.InsertColumn(0,_T("设置"), LVCFMT_CENTER, rectList.Width(), 0);
	

	CString strCaption;

	CTabCtrl *pTab = GetTabControl();
	TCITEM tci;
	::ZeroMemory(&tci,sizeof(tci));
	tci.mask = TCIF_TEXT;
	tci.cchTextMax = 256;
	TCHAR szBuf[256] = {0};
	tci.pszText = szBuf;
	
	for (int idxPge = 0; idxPge < GetPageCount(); idxPge ++)
	{
		if(pTab->GetItem(idxPge, &tci))

		{
			lvi.iItem = idxPge;
			lvi.iSubItem = 0;
			lvi.iImage = idxPge;
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;   
			lvi.pszText = tci.pszText ;
			m_wndList.InsertItem(&lvi);
		}
	}




}

void CMyPropertySheet::OnNMCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW  pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);


	CRect rectRow, rectList;

	m_wndList.GetWindowRect(&rectList);
	ScreenToClient(&rectList);
	m_wndList.GetItemRect(0, &rectRow, LVIR_BOUNDS);
	int iItemHeight = rectRow.Height();
	int iItemTop = rectRow.top ;


	::SelectObject(pLVCD->nmcd.hdc, m_ftList);


	switch (pLVCD->nmcd.dwDrawStage)
	{


	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:


		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
		{
			int iCol = pLVCD->iSubItem ;
			int iRow = (int)pLVCD->nmcd.dwItemSpec;
			CRect rectItem(pLVCD->nmcd.rc), rectIcon;

			//计算每个子项的矩形

			rectItem.top = iItemTop + iRow * iItemHeight;
			rectItem.bottom = rectItem.top + iItemHeight;

			rectItem.left +=3;
			rectItem.right -=3;

			if (iRow == 0)
				rectItem.top +=3;
			CDC *pDC = CDC::FromHandle(pLVCD->nmcd.hdc);			

			LOGFONT lf;
			::ZeroMemory(&lf, sizeof(lf));
			pDC->GetCurrentFont()->GetLogFont(&lf);


			//获得第图标所在的矩形
			m_wndList.GetSubItemRect(iRow,0, LVIR_ICON, rectIcon);

			const COLORREF clrBlack = RGB(0,0,0);
			const COLORREF clrWhite = RGB(255,255,255);

			if ((pLVCD->nmcd.uItemState & (CDIS_FOCUS | CDIS_SELECTED)) == (CDIS_FOCUS | CDIS_SELECTED))
			{
					
					
					pDC->FillSolidRect(&rectItem, m_clrTextBkSele);
					pDC->SetTextColor(clrWhite);
					pDC->TextOut(rectItem.left + rectIcon.Width() + 8, (iRow == 0?(rectItem.top - 3):rectItem.top) + (iItemHeight - abs(lf.lfHeight))/2, m_wndList.GetItemText(iRow, iCol), (int)_tcslen(m_wndList.GetItemText(iRow, iCol)));
					
					::DrawIconEx(*pDC,rectIcon.left, rectIcon.top + (iItemHeight - 16) / 2,m_imgList.ExtractIcon(iRow),16,16,NULL,NULL,DI_NORMAL);						
					
					pDC->SetTextColor(clrBlack);
					
					DrawGradientLine(pDC,m_clrSeprator,CPoint(rectItem.left, rectItem.bottom-1), CPoint(rectItem.right, rectItem.bottom-1));					

			
			}
			
			else
			
			{
				
					pDC->FillSolidRect(&rectItem, clrWhite);

					pDC->TextOut(rectItem.left + rectIcon.Width() + 8, (iRow == 0?(rectItem.top - 3):rectItem.top) + (iItemHeight - abs(lf.lfHeight))/2, m_wndList.GetItemText(iRow, iCol), (int)_tcslen(m_wndList.GetItemText(iRow, iCol)));

					::DrawIconEx(*pDC,rectIcon.left, rectIcon.top + (iItemHeight - 16) / 2, m_imgList.ExtractIcon(iRow),16,16,NULL,NULL,DI_NORMAL);

					DrawGradientLine(pDC,m_clrSeprator,CPoint(rectItem.left, rectItem.bottom-1), CPoint(rectItem.right, rectItem.bottom-1));					

			

			}
			
			
			*pResult = CDRF_SKIPDEFAULT;
			break;
		}


	default:
		*pResult = CDRF_SKIPDEFAULT;
		break;
	}	



}


void CMyPropertySheet::DrawGradientLine(CDC* pDC, COLORREF clrLine, POINT ptStart, POINT ptEnd)
{
//画渐近线，从clrLine的颜色变化至白色

	int clrBBase = clrLine>>16 & 0x000000FF;
	int clrGBase = clrLine>>8 & 0x000000FF;
	int clrRBase = clrLine & 0x000000FF;

	int clrBCurr = 255;
	int clrGCurr = 255;
	int clrRCurr = 255;

	double dRInc = (double)(255 - clrRBase) / (double)(abs(ptEnd.x - ptStart.x));
	double dGInc = (double)(255 - clrGBase) / (double)(abs(ptEnd.x - ptStart.x));
	double dBInc = (double)(255 - clrBBase) / (double)(abs(ptEnd.x - ptStart.x));

	POINT ptCurr = ptStart;

	for (;ptCurr.x < ptEnd.x;ptCurr.x ++)			
	{
		pDC->SetPixel(ptCurr.x, ptCurr.y -1,RGB(clrRCurr,clrGCurr,clrBCurr));
		pDC->SetPixel(ptCurr, RGB(clrRCurr,clrGCurr,clrBCurr));
		clrRCurr = clrRBase + (int)((ptCurr.x - ptStart.x) * dRInc);
		clrGCurr = clrGBase + (int)((ptCurr.x - ptStart.x) * dGInc);
		clrBCurr = clrBBase + (int)((ptCurr.x - ptStart.x) * dBInc);
	}
	

	

}

int CMyPropertySheet::AddIcon(HICON icon)
{
	return m_imgList.Add(icon);
}

void CMyPropertySheet::SetCaptionColor(const COLORREF clrCaption)
{
	m_clrCaption = clrCaption;
}

void CMyPropertySheet::SetSepratorColor(const COLORREF clrSeprator)
{
	m_clrSeprator = clrSeprator;
}

void CMyPropertySheet::SetListFont(CFont * pFont)
{
	m_ftList.Attach(pFont->GetSafeHandle());
}

void CMyPropertySheet::SetSelectedColor(const COLORREF clrSelected)
{
	m_clrTextBkSele = clrSelected;
}
