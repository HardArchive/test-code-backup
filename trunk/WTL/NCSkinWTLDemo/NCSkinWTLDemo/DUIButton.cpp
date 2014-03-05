#include "StdAfx.h"
#include "DUIButton.h"


// ���ư�ť
void CDUIButton::DrawButton( Gdiplus::Graphics&  graphics)
{
	// ��ȡ��ťͼƬ��Ϣ
	UINT iCount = m_nImageCount;
	int	iButtonIndex = 0;
	if(m_bDisabled && iCount >= 4) iButtonIndex = 3;
	else if(m_bPressed && iCount >= 3)iButtonIndex = 2;
	else if(m_bHovering && iCount >= 2)iButtonIndex = 1;
	else iButtonIndex = 0;

	// ��ָ��λ�û��ư�ť
	int iWidth = m_pImage->GetWidth()/iCount;
	int iHeight = m_pImage->GetHeight();
	RectF grect;
	grect.X=(Gdiplus::REAL)m_rcRect.left;
	grect.Y=(Gdiplus::REAL)m_rcRect.top;
	grect.Width = (Gdiplus::REAL)m_rcRect.Width();
	grect.Height = (Gdiplus::REAL)m_rcRect.Height();

	graphics.DrawImage(m_pImage, grect, (Gdiplus::REAL)iWidth*iButtonIndex,0,(Gdiplus::REAL)iWidth,(Gdiplus::REAL)iHeight, UnitPixel);

	StringFormat stringFormat;

	if (m_pIcon)
	{
		PointF ptIcon(m_ptIcon.x,m_ptIcon.y);
		graphics.DrawImage(m_pIcon,ptIcon);

		grect.X=(Gdiplus::REAL)m_ptIcon.x + m_pIcon->GetWidth() + 2;
		grect.Width = (Gdiplus::REAL)m_rcRect.Width() - m_pIcon->GetWidth() - 2;

		stringFormat.SetFormatFlags( StringFormatFlagsDirectionVertical);
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentNear);
	}
	else
	{
		//stringFormat.SetFormatFlags( StringFormatFlagsDirectionVertical);
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
	}

	if (!m_strCaption.IsEmpty())
	{
		//��������
		FontFamily fontFamily(L"����");
		Gdiplus::Font font(&fontFamily, 10, FontStyleRegular, UnitPoint);


		CStringW strTitle(m_strCaption);
		SolidBrush brush((ARGB)Color::White);
		if (m_bDisabled)
		{
			brush.SetColor((ARGB)Color::Gray);
		}

		graphics.DrawString(strTitle, strTitle.GetLength(), &font,grect,&stringFormat, &brush);
	}

}

// ������ť
void CDUIButton::CreateButton(LPCTSTR lpszCaption,LPRECT lpRect,HWND hParentWnd, Image* pImage, UINT nCount/* = 4*/, UINT nID/* = 0*/)
{
	if (lpszCaption)
	{
		m_strCaption = lpszCaption;
	}
	m_hParentWnd = hParentWnd;
	m_rcRect = lpRect;
	m_pImage = pImage;
	m_nImageCount = nCount;
	m_nID = nID;
}

//���ð�ťͼ����Ϣ
void  CDUIButton::SetIconInfo(Image *pIcon,CPoint ptPos/* = CPoint(5,3)*/,BOOL bRedraw/*=FALSE*/)
{
    m_pIcon = pIcon;
	m_ptIcon = CPoint(m_rcRect.left+ptPos.x,m_rcRect.top+ptPos.y);
	if (bRedraw)
	{
		InvalidateRect(m_hParentWnd,&m_rcRect,FALSE);
	}
}

void CDUIButton::SetRect(LPCRECT lpRect)
{
	CPoint ptIconOrg(m_ptIcon.x - m_rcRect.left,m_ptIcon.y - m_rcRect.top);
	m_rcRect = lpRect;
	m_ptIcon = CPoint(m_rcRect.left+ptIconOrg.x,m_rcRect.top+ptIconOrg.y);
}