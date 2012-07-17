// test_dib_layer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "test_dib_layer.h"

#include "yrlab/dib.h"

class CDemoWindow
    : public CWindowImpl<CDemoWindow>
{
public:
protected:

    CDC layer_dc;
    int layer_width;
    int layer_height;

    Yrlab_DIBSection layer_dib;

    void _Update_Layer()
    {
        BLENDFUNCTION blfunc = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
        CPoint ptdest(0, 0), ptsrc(0, 0);
        CSize sizedest(layer_width, layer_height);

        ::UpdateLayeredWindow(
            m_hWnd, NULL, &ptdest, &sizedest, layer_dc, &ptsrc, 0, &blfunc, ULW_ALPHA);
    }

    int OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        layer_width = 1440;
        layer_height = 900;

        CWindowDC dc(m_hWnd);

        layer_dc.CreateCompatibleDC(dc);

        layer_dib.create(dc, layer_width, layer_height);

        layer_dc.SelectBitmap(layer_dib);

        layer_dib.fill_solid_rectangle(Yrlib_Color(255, 0, 0), 0, 0, layer_width, layer_height);

        _Update_Layer();

        return 0;
    }

    void OnClose()
    {
        ::PostQuitMessage(0);
    }

    void OnLButtonDown(UINT nFlags, CPoint point)
    {
        layer_dib.fill_solid_rectangle(Yrlib_Color(0, 0, 0, 0), 200, 300, 400, 500);

        Gdiplus::Graphics g(layer_dc);

        g.DrawString(
            L"This is a string", -1, 
            &Gdiplus::Font(L"Î¢ÈíÑÅºÚ", 50, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
            Gdiplus::PointF(10, 10), &Gdiplus::SolidBrush(Gdiplus::Color::Blue)
            );

        _Update_Layer();
    }

private:

    BEGIN_MSG_MAP_EX(CDemoWindow)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
    END_MSG_MAP()
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    ULONG_PTR m_gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    {
        CMessageLoop loop;

        CDemoWindow wnd;

        wnd.Create(
            NULL, 
            CRect(100, 100, 700, 600), 
            L"Demo Window", 
            WS_POPUP | WS_VISIBLE,
            WS_EX_APPWINDOW | WS_EX_LAYERED | WS_EX_TOPMOST
            );

        loop.Run();

        wnd.DestroyWindow();
    }

    Gdiplus::GdiplusShutdown(m_gdiplusToken);

    return 0;
}
