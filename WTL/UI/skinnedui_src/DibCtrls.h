#ifndef __DIBCTRLS_H__
#define __DIBCTRLS_H__

/////////////////////////////////////////////////////////////////////////////
// A couple of DIB related controls
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2001 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#pragma once

#ifndef __cplusplus
  #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
  #error DibCtrls.h requires atlapp.h to be included first
#endif


/////////////////////////////////////////////////////////////////////////////
// Dialog with bitmap background

template< class T, UINT nBmpRes >
class CDialogSkin
{
public:
   BEGIN_MSG_MAP(CDialogSkin)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
      MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
   END_MSG_MAP()

   CBrush m_brBack;

   // Operations

   BOOL SetDlgBitmap(UINT nRes)
   {
      CBitmap bmBack;
      bmBack.LoadBitmap(nRes);
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      m_brBack.CreatePatternBrush(bmBack);
      return TRUE;
   }

   // Message handlers

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      SetDlgBitmap(nBmpRes);
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      ATLASSERT(!m_brBack.IsNull());
      return (LRESULT) m_brBack.m_hBrush;
   }
   LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // STATIC (label) controls need this to shine through...
      ::SetBkMode((HDC) wParam, TRANSPARENT);
      return (LRESULT) ::GetStockObject(NULL_BRUSH); 
   }
};


/////////////////////////////////////////////////////////////////////////////
// A better CDialogSkin class

class CDialogColor
{
public:
   BEGIN_MSG_MAP(CDialogColor)
      MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
      MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
   END_MSG_MAP()

   CBrush m_brBack;
   CBrushHandle m_hbrBack;

   // Operations

   void SetDlgBitmap(UINT nRes)
   {
      CBitmap bmBack;
      bmBack.LoadBitmap(nRes);
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      m_brBack.CreatePatternBrush(bmBack);
      m_hbrBack = m_brBack;
   }
   void SetDlgColor(COLORREF clrBack)
   {
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      m_brBack.CreateSolidBrush(clrBack);
      m_hbrBack = m_brBack;
   }
   void SetDlgColor(HBRUSH hBrush)
   {
      m_hbrBack = hBrush;
   }

   // Message handlers

   LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      ATLASSERT(!m_hbrBack.IsNull());
      return (LRESULT) (HBRUSH) m_hbrBack;
   }
   LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // STATIC (label) controls need this to shine through...
      ATLASSERT(!m_hbrBack.IsNull());
      ::SetBkMode((HDC) wParam, TRANSPARENT);
      return (LRESULT) ::GetStockObject(NULL_BRUSH); 
   }
};


/////////////////////////////////////////////////////////////////////////////
// DIB Bitmap button

template< class T, class TBase = CButton, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CDibButtonImpl : public CWindowImpl< T, TBase, TWinTraits >
{
public:
   DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

   typedef CDibButtonImpl< T, TBase, TWinTraits > thisClass;

   enum
   {
      _nImageNormal = 0,
      _nImagePushed,
      _nImageHover,
      _nImageDisabled,
      _nImageCount = 4,
   };

   DWORD m_dwExtendedStyle;
   CToolTipCtrl m_tip;

   // Internal states
   bool m_fMouseOver;
   bool m_fFocus;
   bool m_fPressed;

   CDib24 m_dibDown;
   CDib24 m_dibOver;
   CDib24 m_dibNormal;
   CDib24 m_dibDisabled;

   CDibButtonImpl() : 
      m_dwExtendedStyle(0), m_fMouseOver(false), m_fFocus(false), m_fPressed(false)
   {
   }

   BOOL SubclassWindow(HWND hWnd)
   {
      BOOL bRet = CWindowImpl< T, TBase, TWinTraits>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   // Attributes

   DWORD GetDibButtonExtendedStyle() const
   {
      return m_dwExtendedStyle;
   }

   DWORD SetDibButtonExtendedStyle(DWORD dwExtendedStyle, DWORD dwMask=0)
   {
      DWORD dwPrevStyle = m_dwExtendedStyle;
      if( dwMask==0 )
         m_dwExtendedStyle = dwExtendedStyle;
      else
         m_dwExtendedStyle = (m_dwExtendedStyle & ~dwMask) | (dwExtendedStyle & dwMask);
      return dwPrevStyle;
   }

   BOOL SetToolTipText(LPCTSTR lpstrText)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( lpstrText==NULL) {
         if( m_tip.IsWindow() ) m_tip.Activate(FALSE);
         return TRUE;
      }
      m_tip.Activate(TRUE);
      m_tip.AddTool(m_hWnd, lpstrText);
      return TRUE;
   }

   // Operations

   void SetImages(CDib24* dibNormal = NULL, CDib24* dibPushed = NULL, CDib24* dibHover = NULL, CDib24* dibDisabled = NULL)
   {
      if( dibNormal ) m_dibNormal = *dibNormal;
      if( dibPushed ) m_dibDown = *dibPushed;
      if( dibHover ) m_dibOver = *dibHover;
      if( dibDisabled ) m_dibDisabled = *dibDisabled;
   }

   void SetImages(UINT iNormal, UINT iPushed = 0, UINT iHover = 0, UINT iDisabled = 0)
   {
      ATLASSERT(iNormal!=0);
      m_dibNormal.LoadBitmap(iNormal);
      if( iPushed!=0 ) m_dibDown.LoadBitmap(iPushed);
      if( iHover!=0 ) m_dibOver.LoadBitmap(iHover);
      if( iDisabled!=0 ) m_dibDisabled.LoadBitmap(iDisabled);
   }

   // Overrideables
   
   void DoPaint(CDCHandle dc)
   {
      if( m_dibNormal.IsEmpty() ) return; // Erh, no bitmap at all?
      DWORD dwStyle = GetStyle();
      RECT rc;      
      GetClientRect(&rc);
      if( dwStyle & WS_DISABLED && !m_dibDisabled.IsEmpty() ) {
         m_dibDisabled.Stretch(dc,rc);
      }
      else if( m_fPressed && !m_dibDown.IsEmpty() ) {
         m_dibDown.Stretch(dc,rc);
      }
      else if( m_fMouseOver && !m_dibOver.IsEmpty() ) {
         m_dibOver.Stretch(dc,rc);
      }
      else {
         m_dibNormal.Stretch(dc,rc);
      }
   }

   // Message map and handlers

   BEGIN_MSG_MAP(thisClass)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
      MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
      MESSAGE_HANDLER(WM_KILLFOCUS, OnFocus)
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
      MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
      MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
      MESSAGE_HANDLER(WM_ENABLE, OnEnable)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
      MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {      
      Init();
      bHandled = FALSE;
      return 1;
   }

   LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
      MSG msg = { m_hWnd, uMsg, wParam, lParam };
      if(m_tip.IsWindow()) m_tip.RelayEvent(&msg);
      bHandled = FALSE;
      return 1;
   }

   LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1;   // no background needed
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      T* pT = static_cast<T*>(this);
      if(wParam != NULL)
      {
         pT->DoPaint((HDC)wParam);
      }
      else
      {
         CPaintDC dc(m_hWnd);
         pT->DoPaint(dc.m_hDC);
      }
      return 0;
   }

   LRESULT OnFocus(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      m_fFocus = (uMsg == WM_SETFOCUS) ? 1 : 0;
      Invalidate();
      UpdateWindow();
      bHandled = FALSE;
      return 1;
   }

   LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LRESULT lRet = 0;
      if( IsHoverMode() )
         SetCapture();
      else
         lRet = DefWindowProc(uMsg, wParam, lParam);
      if( ::GetCapture()==m_hWnd ) {
         m_fPressed = true;
         Invalidate();
         UpdateWindow();
      }
      return lRet;
   }

   LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LRESULT lRet = 0;
      if( !IsHoverMode() ) lRet = DefWindowProc(uMsg, wParam, lParam);
      if( ::GetCapture() != m_hWnd ) SetCapture();
      if( !m_fPressed ) {
         m_fPressed = true;
         Invalidate();
         UpdateWindow();
      }
      return lRet;
   }

   LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LRESULT lRet = 0;
      bool bHover = IsHoverMode();
      if(!bHover)   lRet = DefWindowProc(uMsg, wParam, lParam);
      if(::GetCapture() == m_hWnd) {
         if( bHover && m_fPressed )   ::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
         ::ReleaseCapture();
      }
      return lRet;
   }

   LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if( m_fPressed ) {
         m_fPressed = false;
         Invalidate();
         UpdateWindow();
      }
      bHandled = FALSE;
      return 1;
   }

   LRESULT OnEnable(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      Invalidate();
      UpdateWindow();
      bHandled = FALSE;
      return 1;
   }

   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
   {
      if(::GetCapture() == m_hWnd) {
         POINT ptCursor = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
         ClientToScreen(&ptCursor);
         RECT rect;
         GetWindowRect(&rect);
         bool uPressed = ::PtInRect(&rect, ptCursor)==TRUE;
         if( m_fPressed != uPressed ) {
            m_fPressed = uPressed;
            Invalidate();
            UpdateWindow();
         }
      }
      else if( IsHoverMode() && !m_fMouseOver )   {
         m_fMouseOver = true;
         Invalidate();
         UpdateWindow();
         StartTrackMouseLeave();
      }
      bHandled = FALSE;
      return 1;
   }

   LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( m_fMouseOver ) {
         m_fMouseOver = false;
         Invalidate();
         UpdateWindow();
      }
      return 0;
   }

   LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if(wParam == VK_SPACE && IsHoverMode()) return 0;   // ignore if in hover mode
      if( wParam == VK_SPACE && !m_fPressed ) {
         m_fPressed = true;
         Invalidate();
         UpdateWindow();
      }
      bHandled = FALSE;
      return 1;
   }

   // Implementation
   
   void Init()
   {
      // We need this style to prevent Windows from painting the button
      ModifyStyle(0, BS_OWNERDRAW|BS_NOTIFY);
      // Create a tool tip
      m_tip.Create(m_hWnd);
      ATLASSERT(m_tip.IsWindow());
   }

   BOOL StartTrackMouseLeave() const
   {
      TRACKMOUSEEVENT tme;
      tme.cbSize = sizeof(tme);
      tme.dwFlags = TME_LEAVE;
      tme.hwndTrack = m_hWnd;
      return _TrackMouseEvent(&tme);
   }

   bool IsHoverMode() const
   {
      return !m_dibOver.IsEmpty();
   }
};

class CDibButtonCtrl : public CDibButtonImpl<CDibButtonCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_DibButton"), GetWndClassName())
};


/////////////////////////////////////////////////////////////////////////////
// DIB Static

template< class T, class TBase = CStatic, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CDibStaticImpl : public CWindowImpl< T, TBase, TWinTraits >
{
public:
   DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

   typedef CDibStaticImpl< T, TBase, TWinTraits > thisClass;

   CDib24 m_dib;
   HPALETTE m_hPalette;

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      BOOL bRet = CWindowImpl< T, TBase, TWinTraits>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   BOOL SetBitmap(CDib24* dib)
   {
      if( dib->IsEmpty() ) return FALSE;
      return dib->CopyTo(&m_dib);
   }
   void SetPalette(HPALETTE hPalette)
   {
      m_hPalette = hPalette;
   }

   // Overridables

   void DoPaint(CDCHandle dc)
   {
      if( m_dib.IsEmpty() ) return; // Erh, no bitmap at all?
      HPALETTE hOldPalette = NULL;
      if( m_hPalette ) {
         hOldPalette = dc.SelectPalette(m_hPalette, FALSE);
         dc.RealizePalette();
      }
      RECT rc;
      GetClientRect(&rc);
      m_dib.Stretch(dc, rc);
      if( m_hPalette ) dc.SelectPalette(hOldPalette, FALSE);
   }

   // Implementation

   void Init()
   {
      // We need this style to prevent Windows from painting the label
      ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SS_OWNERDRAW|SS_NOTIFY);
      ModifyStyleEx(WS_EX_STATICEDGE, 0);
      m_hPalette = NULL;
   }

   // Message map and handlers

   BEGIN_MSG_MAP(thisClass)
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

   LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1;   // no background needed
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      T* pT = static_cast<T*>(this);
      if( wParam != NULL )
      {
         pT->DoPaint((HDC)wParam);
      }
      else
      {
         CPaintDC dc(m_hWnd);
         pT->DoPaint(dc.m_hDC);
      }
      return 0;
   }
};

class CDibStaticCtrl : public CDibStaticImpl<CDibStaticCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_DibStatic"), GetWndClassName())
};


#endif // !__DIBCTRLS_H__

