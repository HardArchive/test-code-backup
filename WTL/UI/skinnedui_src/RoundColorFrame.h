#if !defined(AFX_ROUNDCOLORFRAME_H__20010719_55C1_F125_7696_0080AD509054__INCLUDED_)
#define AFX_ROUNDCOLORFRAME_H__20010719_55C1_F125_7696_0080AD509054__INCLUDED_

#pragma once


class CRoundColorFrameCtrl : 
   public CWindowImpl< CRoundColorFrameCtrl, CStatic, CControlWinTraits >
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_RoundColorFrame"), GetWndClassName())

   CBrush m_brBack;
   CBrush m_brImage;
   CPen m_penBlack;
   POINT m_ptArc;

   void CreateFrame(UINT iBackground,
                    UINT iImage,
                    int iRadius)
   {
      m_ptArc.x = m_ptArc.y = iRadius;
      //
      m_penBlack.CreatePen(PS_SOLID, 1, RGB(0,0,0));
      //
      CBitmap bmImage;
      bmImage.LoadBitmap(iImage);      
      m_brImage.CreatePatternBrush(bmImage);
      //
      CBitmap bmBack;
      bmBack.LoadBitmap(iBackground);      
      m_brBack.CreatePatternBrush(bmBack);
      //
      SetWindowPos(HWND_TOP, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
   }

   BOOL SubclassWindow(HWND hWnd)
   {
      BOOL bRet = CWindowImpl< CRoundColorFrameCtrl, CStatic, CControlWinTraits>::SubclassWindow(hWnd);
      if(bRet) Init();
      return bRet;
   }

   // Overrideables

   void DoPaint(CDCHandle dc)
   {
      RECT rc;
      GetClientRect(&rc);

      HPEN hOldPen = dc.SelectPen(m_penBlack);
      HBRUSH hOldBrush = dc.SelectBrush(m_brImage);
      dc.RoundRect(&rc, m_ptArc);
      dc.SelectBrush(hOldBrush);
      dc.SelectPen(hOldPen);
   }

   // Implementation

   void Init()
   {
      // We need this style to prevent Windows from painting the label
      ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_OWNERDRAW | SS_NOTIFY);
      ModifyStyleEx(WS_EX_STATICEDGE, 0);
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CRoundColorFrameCtrl)
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


#endif // !defined(AFX_ROUNDCOLORFRAME_H__20010719_55C1_F125_7696_0080AD509054__INCLUDED_)

