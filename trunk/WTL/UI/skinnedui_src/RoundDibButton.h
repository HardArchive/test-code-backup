#if !defined(AFX_ROUNDDIBBUTTON_H__20010716_510F_B73B_FC53_0080AD509054__INCLUDED_)
#define AFX_ROUNDDIBBUTTON_H__20010716_510F_B73B_FC53_0080AD509054__INCLUDED_

#pragma once

#ifndef __DIBCTRLS_H__
  #error This control requires DibCtrls.h to be included first
#endif


class CRoundDibButtonCtrl : public CDibButtonImpl<CRoundDibButtonCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_RoundDibButton"), GetWndClassName())

   void CreateButtons(UINT iImage, UINT iButtonImage, UINT iBackgroundImage)
   {
      RECT rcClient;
      GetClientRect(&rcClient);
      int cx = rcClient.right-rcClient.left;
      int cy = rcClient.bottom-rcClient.top;
      if( GetWindowTextLength() > 0 ) cy -= 10;
      POINT pt;

      CDib24 dibBack;
      dibBack.LoadBitmap(iBackgroundImage);
      CDib24 dibButton;
      dibButton.LoadBitmap(iButtonImage);
      CDib24 dibIcon;
      dibIcon.LoadBitmap(iImage);

      CDib24 dibNormal;
      dibNormal.Create(cx,cy,24);
      CDib24 dibDown;
      dibDown.Create(cx,cy,24);
      CDib24 dibDisabled;
      dibDisabled.Create(cx,cy,24);
      
      CDib24 dibShadow;
      dibButton.CopyTo(&dibShadow);
      CGreyScaleImageFilter::Process(&dibShadow);
      CBrightenImageFilter::Process(&dibShadow, -65);

      COLORREF clrButton = dibButton.GetPixel(0,0);
      COLORREF clrShadow = dibShadow.GetPixel(0,0);
      COLORREF clrIcon = dibIcon.GetPixel(0,0);

      int xoffset = (cx/2)-(dibButton.GetWidth()/2);
      int yoffset = cy - dibButton.GetHeight() - 4;

      // Create NORMAL button
      pt.x = 0; pt.y = 0;
      CCopyImageFilter::Process(&dibNormal, &dibBack, pt);
      pt.x += xoffset + 2; pt.y = yoffset;
      CCopyMaskedImageFilter::Process(&dibNormal, &dibShadow, pt, clrShadow);
      CBlurImageFilter::Process(&dibNormal);
      CBlurImageFilter::Process(&dibNormal);
      dibNormal.CopyTo(&dibBack); // Store background w. shadow
      pt.x -= 2; pt.y += 3;
      CCopyMaskedImageFilter::Process(&dibNormal, &dibButton, pt, clrButton);
      pt.x += 1; pt.y += 1;
      CCopyMaskedImageFilter::Process(&dibNormal, &dibIcon, pt, clrIcon);

      // Create DOWN button
      pt.x = 0; pt.y = 0;
      CCopyImageFilter::Process(&dibDown, &dibBack, pt);
      pt.x += xoffset + 2; pt.y = yoffset;
      pt.x -= 1; pt.y += 2;
      CCopyMaskedImageFilter::Process(&dibDown, &dibButton, pt, clrButton);
      pt.x += 1; pt.y += 1;
      CCopyMaskedImageFilter::Process(&dibDown, &dibIcon, pt, clrIcon);

      // Create DISABLED button
      pt.x = 0; pt.y = 0;
      CCopyImageFilter::Process(&dibDisabled, &dibBack, pt);
      pt.x += xoffset + 2; pt.y = yoffset;
      pt.x -= 1; pt.y += 2;
      CGreyScaleImageFilter::Process(&dibButton);
      clrButton = dibButton.GetPixel(0,0);
      CCopyMaskedImageFilter::Process(&dibDisabled, &dibButton, pt, clrButton);
      pt.x += 1; pt.y += 1;
      CGreyScaleImageFilter::Process(&dibIcon);
      clrIcon = dibIcon.GetPixel(0,0);
      CCopyMaskedImageFilter::Process(&dibDisabled, &dibIcon, pt, clrIcon);
      CBlurImageFilter::Process(&dibDisabled);

      SetImages(&dibNormal, &dibDown, NULL, &dibDisabled);

      TCHAR szText[64];
      GetWindowText(szText, sizeof(szText)/sizeof(TCHAR));
      SetToolTipText(szText);
   }

   void DoPaint(CDCHandle dc)
   {
      if( m_dibNormal.IsEmpty() ) return; // Erh, no bitmap at all?
      DWORD dwStyle = GetStyle();
      RECT rc;      
      GetClientRect(&rc);
      if( (dwStyle & WS_DISABLED) && !m_dibDisabled.IsEmpty() ) {
         m_dibDisabled.Draw(dc,0,0);
      }
      else if( m_fPressed && !m_dibDown.IsEmpty() ) {
         m_dibDown.Draw(dc,0,0);
      }
      else if( m_fMouseOver && !m_dibOver.IsEmpty() ) {
         m_dibOver.Draw(dc,0,0);
      }
      else {
         m_dibNormal.Draw(dc,0,0);
      }
      TCHAR szText[64];
      GetWindowText(szText, sizeof(szText)/sizeof(TCHAR));
      HFONT hOldFont = dc.SelectFont(GetFont());
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(RGB(0,0,0));
      rc.left += 2;
      dc.DrawText(szText, -1, &rc, DT_SINGLELINE|DT_CENTER|DT_BOTTOM|DT_NOPREFIX);
      dc.SelectFont(hOldFont);
   }
};


#endif // !defined(AFX_ROUNDDIBBUTTON_H__20010716_510F_B73B_FC53_0080AD509054__INCLUDED_)

