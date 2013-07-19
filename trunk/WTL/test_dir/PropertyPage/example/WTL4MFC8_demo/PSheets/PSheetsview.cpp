// PSheetsView.cpp : implementation of the CPSheetsView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PSheetsView.h"

CPSheetsView::CPSheetsView() : m_nColor(0), m_nPicture(0)
{
}

CPSheetsView::~CPSheetsView()
{
}

BOOL CPSheetsView::PreTranslateMessage(MSG* pMsg)
{
    pMsg;
    return FALSE;
}

BOOL CPSheetsView::OnEraseBkgnd ( HDC hdc )
{
CDCHandle dc = hdc;
CDC dcMem;
CBitmap bmpMem;
CRect rcClient;
CBitmapHandle bmpOld, bmpOld2;

    if ( m_bmpBkgnd.IsNull() )
        {
        switch ( m_nPicture )
            {
            case 0: m_bmpBkgnd.LoadBitmap ( IDB_ALYSON );    break;
            case 1: m_bmpBkgnd.LoadBitmap ( IDB_THECHEAT );  break;
            case 2: m_bmpBkgnd.LoadBitmap ( IDB_STRONGBAD ); break;
            }

        if ( m_bmpBkgnd )
            m_bmpBkgnd.GetSize ( m_szBkgndBitmap );
        }

    GetClientRect ( rcClient );

    dcMem.CreateCompatibleDC ( dc );
    bmpMem.CreateCompatibleBitmap ( dc, rcClient.Width(), rcClient.Height() );

    bmpOld = dcMem.SelectBitmap ( bmpMem );

    switch ( m_nColor )
        {
        case 0: dcMem.FillSolidRect ( rcClient, RGB(0,0,128) ); break;
        case 1: dcMem.FillSolidRect ( rcClient, RGB(255,153,0) ); break;

        case 2:
            {
            TRIVERTEX atv[] = {
                { rcClient.left, rcClient.top, 0xff00, 0, 0 },    // 0 top left (red)
                { rcClient.right, rcClient.bottom, 0, 0, 0xff00 } // 1 bottom right (blue)
                };
            GRADIENT_RECT gt = { 0, 1 };

            dcMem.GradientFill ( atv, countof(atv), &gt, 1, GRADIENT_FILL_RECT_H );
            }
        break;
        }

    if ( m_bmpBkgnd )
        {
        CDC dcPicture;
        int x, y, cx, cy;

        dcPicture.CreateCompatibleDC ( dc );
        bmpOld2 = dcPicture.SelectBitmap ( m_bmpBkgnd );

        cx = m_szBkgndBitmap.cx;
        cy = m_szBkgndBitmap.cy;

        x = rcClient.Width() - cx;
        y = rcClient.Height() - cy;
        
        dcMem.BitBlt ( x, y, cx, cy, dcPicture, 0, 0, SRCCOPY );

        dcPicture.SelectBitmap ( bmpOld2 );
        }

    dc.BitBlt ( 0, 0, rcClient.Width(), rcClient.Height(), dcMem, 0, 0, SRCCOPY );
    dc.SelectBitmap ( bmpOld );
    return TRUE;
}

void CPSheetsView::SetBackgroundOptions ( int nColor, int nPicture )
{
    if ( m_nColor != nColor )
        m_nColor = nColor;

    if ( m_nPicture != nPicture )
        {
        m_nPicture = nPicture;

        if ( m_bmpBkgnd )
            m_bmpBkgnd.DeleteObject();
        }

    RedrawWindow();
}

