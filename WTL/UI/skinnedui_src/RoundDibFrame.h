#if !defined(AFX_ROUNDDIBFRAME_H__20010717_3DE9_94B0_77F5_0080AD509054__INCLUDED_)
#define AFX_ROUNDDIBFRAME_H__20010717_3DE9_94B0_77F5_0080AD509054__INCLUDED_

#pragma once

#include <math.h>

#ifndef __DIBCTRLS_H__
  #error This control requires DibCtrls.h to be included first
#endif


class CRoundDibFrameCtrl : public CDibStaticImpl<CRoundDibFrameCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_RoundDibFrame"), GetWndClassName())

   void CreateFrame(int iRadius, int iShadow, COLORREF clrBright, COLORREF clrDark, COLORREF clrBack, UINT iBackgroundImage=IDB_BACKGROUND)
   {
      RECT rc;
      GetClientRect(&rc);
      CDib24 dib;
      dib.Create(rc.right-rc.left, rc.bottom-rc.top, 24);
      TileBackground(dib, iBackgroundImage);
      DrawCircle(dib, rc, iRadius, clrBright, clrDark, clrBack);
      DrawShadow(dib, rc, iRadius, iShadow, clrBright, clrDark, clrBack);
      CBlurImageFilter::Process(&dib);
      SetBitmap(&dib);
      SetWindowPos(HWND_BOTTOM, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_FRAMECHANGED);
   }

   void DoPaint(CDCHandle dc)
   {
      if( m_dib.IsEmpty() ) return; // Erh, no bitmap at all?
      m_dib.Draw(dc,0,0);
   }

   // Paint stuff...
   // The round-about-painting stuff comes from the honourable Chris Maunder
   // from www.codeproject.com

   void TileBackground(CDib24& dib, UINT iBackgroundImage)
   {
      CDib24 back;
      back.LoadBitmap(iBackgroundImage);
      for( int y=0; y<(int)dib.GetHeight(); y+= back.GetHeight() ) {
         for( int x=0; x < (int) dib.GetWidth(); x += back.GetWidth() ) {
            POINT pt = { x,y };
            CCopyImageFilter::Process(&dib, &back, pt);
         }
      }
   }

   COLORREF GetColour(float dAngle, COLORREF clrBright, COLORREF clrDark) const
   {
      #define Rad2Deg 180.0f/3.1415f
      #define LIGHT_SOURCE_ANGLE   -0.956f      // -2.356 radians = -135 degrees, i.e. From top left

      ATLASSERT(dAngle > -3.1416f && dAngle < 3.1416f);
      float dAngleDifference = LIGHT_SOURCE_ANGLE - dAngle;

      if( dAngleDifference < -3.1415f ) dAngleDifference = 6.293f + dAngleDifference;
      else if( dAngleDifference > 3.1415f ) dAngleDifference = 6.293f - dAngleDifference;

      float Weight = 0.5f * (float) (cos(dAngleDifference) + 1.0f);
      return WeightColor(Weight, clrBright, clrDark);
   }

   inline COLORREF WeightColor(float Weight, COLORREF clrBright, COLORREF clrDark) const
   {
      BYTE Red   = (BYTE) (Weight*GetRValue(clrBright) + (1.0f-Weight)*GetRValue(clrDark));
      BYTE Green = (BYTE) (Weight*GetGValue(clrBright) + (1.0f-Weight)*GetGValue(clrDark));
      BYTE Blue  = (BYTE) (Weight*GetBValue(clrBright) + (1.0f-Weight)*GetBValue(clrDark));
      return RGB(Red, Green, Blue);
   }

   void DrawHLine(CDib24& dib, int x1, int x2, int y, COLORREF clr)
   {
      ATLASSERT(x2>x1);
      LPBYTE p = dib.GetBits() + (x1*dib.GetPixelWidth()) + (y*dib.GetLineWidth());
      BYTE r = GetRValue(clr);
      BYTE g = GetGValue(clr);
      BYTE b = GetBValue(clr);
      for( int i=0; i<x2-x1; i++ ) {
         *p++ = b;
         *p++ = g;
         *p++ = r;
      }
   }

   void DrawVLine(CDib24& dib, int x, int y1, int y2, COLORREF clr)
   {
      ATLASSERT(y2>y1);
      DWORD dwLineWidth = dib.GetLineWidth();
      LPBYTE p = dib.GetBits() + (x*dib.GetPixelWidth()) + (y1*dwLineWidth);
      BYTE r = GetRValue(clr);
      BYTE g = GetGValue(clr);
      BYTE b = GetBValue(clr);
      for( int i=0; i<y2-y1; i++ ) {
         *p++ = b;
         *p++ = g;
         *p++ = r;
         p += dwLineWidth-3;
      }
   }

   void DrawCircle(CDib24& dib, 
                   RECT rcFrame, LONG lRadius, 
                   COLORREF clrBright, COLORREF clrDark, COLORREF clrBack)
   {
      const float Pi = 3.141592654f, 
                  Pi_on_2 = Pi * 0.5f;
      LONG lError, lXoffset, lYoffset;

      // Set starting values
      lXoffset = lRadius;
      lYoffset = 0;
      lError   = -lRadius;

      ::InflateRect(&rcFrame, -2, -2);
      POINT pt[4];
      pt[0].x = rcFrame.left + lRadius;
      pt[0].y = rcFrame.top + lRadius;
      pt[1].x = rcFrame.right - lRadius;
      pt[1].y = rcFrame.top + lRadius;
      pt[2].x = rcFrame.left + lRadius;
      pt[2].y = rcFrame.bottom - lRadius;
      pt[3].x = rcFrame.right - lRadius;
      pt[3].y = rcFrame.bottom - lRadius;

      COLORREF clrColor;
      float dAngle;
      do {
         dAngle = (float)atan2((double)lYoffset, (double)lXoffset);

         // Draw background
         DrawHLine(dib, pt[0].x - lXoffset, pt[1].x + lXoffset, pt[0].y - lYoffset, clrBack);
         DrawHLine(dib, pt[0].x - lYoffset, pt[1].x + lYoffset, pt[0].y - lXoffset, clrBack);
         DrawHLine(dib, pt[2].x - lXoffset, pt[3].x + lXoffset, pt[3].y + lYoffset, clrBack);
         DrawHLine(dib, pt[2].x - lYoffset, pt[3].x + lYoffset, pt[3].y + lXoffset, clrBack);

         // Draw the current pixel, reflected across all eight arcs
         clrColor = GetColour(dAngle, clrBright, clrDark);
         dib.SetPixel(pt[3].x + lXoffset, pt[3].y + lYoffset, clrColor);

         clrColor = GetColour(Pi_on_2 - dAngle, clrBright, clrDark);
         dib.SetPixel(pt[3].x + lYoffset, pt[3].y + lXoffset, clrColor);

         clrColor = GetColour(Pi_on_2 + dAngle, clrBright, clrDark);
         dib.SetPixel(pt[2].x - lYoffset, pt[2].y + lXoffset, clrColor);

         clrColor = GetColour(Pi - dAngle, clrBright, clrDark);
         dib.SetPixel(pt[2].x - lXoffset, pt[2].y + lYoffset, clrColor);

         clrColor = GetColour(-Pi + dAngle, clrBright, clrDark);
         dib.SetPixel(pt[0].x - lXoffset, pt[0].y - lYoffset, clrColor);

         clrColor = GetColour(-Pi_on_2 - dAngle, clrBright, clrDark);
         dib.SetPixel(pt[0].x - lYoffset, pt[0].y - lXoffset, clrColor);

         clrColor = GetColour(-Pi_on_2 + dAngle, clrBright, clrDark);
         dib.SetPixel(pt[1].x + lYoffset, pt[1].y - lXoffset, clrColor);

         clrColor = GetColour(-dAngle, clrBright, clrDark);
         dib.SetPixel(pt[1].x + lXoffset, pt[1].y - lYoffset, clrColor);

         // Advance the error term and the constant X axis step
         lError += lYoffset++;

         // Check to see if error term has overflowed
         if( (lError += lYoffset) >= 0 ) lError -= --lXoffset * 2;

      } while( lYoffset <= lXoffset ); // Continue until halfway point

      // Fill background
      for( int y = rcFrame.top + lRadius; y < rcFrame.bottom - lRadius; y ++ ) {
         DrawHLine(dib, rcFrame.left + 1, rcFrame.right - 1, y, clrBack);
      }

      // Draw frame-lines at the edge of the arcs
      DrawVLine(dib, rcFrame.left, rcFrame.top + lRadius, rcFrame.bottom - lRadius, clrDark);
      DrawVLine(dib, rcFrame.right, rcFrame.top + lRadius, rcFrame.bottom - lRadius, clrBright);
      DrawHLine(dib, rcFrame.left + lRadius, rcFrame.right - lRadius, rcFrame.top, clrBright);
      DrawHLine(dib, rcFrame.left + lRadius, rcFrame.right - lRadius, rcFrame.bottom, clrDark);
   }

   void DrawShadow(CDib24& dib, 
                   RECT rcFrame, LONG /*lRadius*/, LONG lShadow,
                   COLORREF /*clrBright*/, COLORREF clrDark, COLORREF clrBack)
   {
      if( lShadow<=0 ) return;

      ::InflateRect(&rcFrame, -2, -2);
      DWORD dwPixelWidth = dib.GetPixelWidth();
      DWORD dwLineWidth = dib.GetLineWidth();
      
      LPBYTE p;
      int i;
      BYTE rBack = GetRValue(clrBack);
      BYTE gBack = GetGValue(clrBack);
      BYTE bBack = GetBValue(clrBack);

      float Divisor = 1.0f / (lShadow+2);
      float Weight = 1.0f - (Divisor*2);
      while( lShadow-- > 0 ) {
         COLORREF clrColor = WeightColor(Weight, clrDark, clrBack);

         BYTE rShadow = GetRValue(clrColor);
         BYTE gShadow = GetGValue(clrColor);
         BYTE bShadow = GetBValue(clrColor);
         
         // Horizontal shadow
         p = dib.GetBits() + ((rcFrame.left+1) * dwPixelWidth) + ((rcFrame.bottom-1) * dwLineWidth);
         for( i=0; i<(rcFrame.right-rcFrame.left)-2; i++ ) {
            if( *(p+0)==bBack && *(p+1)==gBack && *(p+2)==rBack ) {
               *p++ = bShadow;
               *p++ = gShadow;
               *p++ = rShadow;
            }
            else {
               p += dwPixelWidth;
            }
         }
         // Vertical shadow
         p = dib.GetBits() + ((rcFrame.left+1) * dwPixelWidth) + ((rcFrame.top+1) * dwLineWidth);
         for( i=0; i<(rcFrame.bottom-rcFrame.top)-2; i++ ) {
            if( *(p+0)==bBack && *(p+1)==gBack && *(p+2)==rBack ) {
               *p++ = bShadow;
               *p++ = gShadow;
               *p++ = rShadow;
               p += dwLineWidth - 3;
            }
            else {
               p += dwLineWidth;
            }
         }
         rcFrame.left++;
         rcFrame.bottom--;
         Weight -= Divisor;
      }
   }
};


#endif // !defined(AFX_ROUNDDIBFRAME_H__20010717_3DE9_94B0_77F5_0080AD509054__INCLUDED_)

