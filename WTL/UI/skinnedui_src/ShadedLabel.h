#if !defined(AFX_SHADEDLABEL_H__20010719_4CCD_3C01_0602_0080AD509054__INCLUDED_)
#define AFX_SHADEDLABEL_H__20010719_4CCD_3C01_0602_0080AD509054__INCLUDED_

#pragma once

#ifndef __ATLCTRLS_H__
  #error This control requires atlctrls.h to be included first
#endif


class CShadedTextCtrl : 
   public CWindowImpl< CShadedTextCtrl, CStatic, CControlWinTraits >
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_ShadedText"), GetWndClassName())

   CFont m_font;
   CBrush m_brBack;
   COLORREF m_clrBackground;
   COLORREF m_clrText;
   bool m_bDrawAntiAlias;

   void CreateText(LPCTSTR pstrFontName,
                   int iFontSize,
                   int iWeight,
                   bool bItalic,
                   bool bDrawAntiAlias,
                   UINT iBackgroundImage,
                   COLORREF clrBackground,
                   COLORREF clrText=RGB(0,0,0))
   {
      LOGFONT lf = { 0 };
      lf.lfWeight = iWeight;
      lf.lfHeight = iFontSize;
      lf.lfItalic = (bItalic==true);
      ::lstrcpy(lf.lfFaceName, pstrFontName);
      m_font.CreateFontIndirect(&lf);
      //
      m_bDrawAntiAlias = bDrawAntiAlias;
      m_clrBackground = clrBackground;
      m_clrText = clrText;
      //
      CBitmap bmBack;
      bmBack.LoadBitmap(iBackgroundImage);      
      m_brBack.CreatePatternBrush(bmBack);
      //
      SetWindowPos(HWND_TOP, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_FRAMECHANGED);
   }

   BOOL SubclassWindow(HWND hWnd)
   {
      BOOL bRet = CWindowImpl< CShadedTextCtrl, CStatic, CControlWinTraits>::SubclassWindow(hWnd);
      if(bRet) Init();
      return bRet;
   }

   // Overrideables

   void DoPaint(CDCHandle dc)
   {
      RECT rc;
      GetClientRect(&rc);
      DWORD dwStyle = GetStyle();

      HFONT hOldFont = dc.SelectFont(m_font);
      UINT iFlags = DT_SINGLELINE;
      if( dwStyle & SS_RIGHT ) iFlags |= DT_RIGHT;
      else if( dwStyle & SS_CENTER ) iFlags |= DT_CENTER;
      if( dwStyle & SS_CENTERIMAGE ) iFlags |= DT_VCENTER;
      
      int len = GetWindowTextLength();
      LPTSTR pstr = (LPTSTR)_alloca(len+1);
      ATLASSERT(pstr);
      GetWindowText(pstr, len+1);

      dc.SetBkMode(TRANSPARENT);
      if( m_bDrawAntiAlias ) {
         COLORREF clrAlias = WeightColor(0.8f, m_clrBackground, m_clrText);
         RECT rcAlias = rc;
         rcAlias.left++;
         rcAlias.top++;
         dc.SetTextColor(clrAlias);
         dc.DrawText(pstr, -1, &rcAlias, iFlags);
      }
      dc.SetTextColor(m_clrText);
      dc.DrawText(pstr, -1, &rc, iFlags);

      dc.SelectFont(hOldFont);
   }

   // Implementation

   void Init()
   {
      // We need this style to prevent Windows from painting the label
      ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SS_OWNERDRAW|SS_NOTIFY);
      ModifyStyleEx(WS_EX_STATICEDGE, 0);
   }

   inline COLORREF WeightColor(float Weight, COLORREF clrBright, COLORREF clrDark) const
   {
      BYTE Red   = (BYTE) (Weight*GetRValue(clrBright) + (1.0f-Weight)*GetRValue(clrDark));
      BYTE Green = (BYTE) (Weight*GetGValue(clrBright) + (1.0f-Weight)*GetGValue(clrDark));
      BYTE Blue  = (BYTE) (Weight*GetBValue(clrBright) + (1.0f-Weight)*GetBValue(clrDark));
      return RGB(Red, Green, Blue);
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CShadedTextCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {      
      Init();
      bHandled = FALSE;
      return 1;
   }

   LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CDCHandle dc((HDC)wParam);
      RECT rc;
      GetClientRect(&rc);
      dc.FillRect(&rc, m_brBack);
      return 1;   // no background needed
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if(wParam != NULL) {
         DoPaint((HDC)wParam);
      }
      else {
         CPaintDC dc(m_hWnd);
         DoPaint(dc.m_hDC);
      }
      return 0;
   }

};


#endif // !defined(AFX_SHADEDLABEL_H__20010719_4CCD_3C01_0602_0080AD509054__INCLUDED_)

