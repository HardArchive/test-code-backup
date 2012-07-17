// test1111111111111111111111111.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#define _WTL_NO_CSTRING

#include "test.h"
#include <atlbase.h>
#include <atlstr.h>
// #include <atlimage.h>
#include "..\..\import\wtl\atlapp.h"
#include "..\..\import\wtl\atlgdi.h"
#include "..\..\import\wtl\atlmisc.h"

#include "yrlib/dib.h"
#include "yrlib/region.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST1111111111111111111111111, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST1111111111111111111111111));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST1111111111111111111111111));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TEST1111111111111111111111111);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, WS_POPUP,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//    hWnd = CreateWindowEx(0/*WS_EX_LAYERED*/, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

template<int t_nGray = 4>
class BkTextRender
{
public:
    UINT    uOutlineFormat;
    int     nGrayLevel;
    Yrlib_Dib_Type dib_type;
    Yrlib_Color palette[256];
    Yrlib_Dib dib_glyph;
    Yrlib_Dib_Section dib_draw;

    BkTextRender()
    {
        switch (t_nGray)
        {
        case 1:
            uOutlineFormat = GGO_BITMAP;
            nGrayLevel     = 2;
            dib_type       = BPP1;
            break;
        case 2:
            uOutlineFormat = GGO_GRAY2_BITMAP;
            nGrayLevel     = 5;
            dib_type       = BPP8;
            break;
        case 4:
            uOutlineFormat = GGO_GRAY4_BITMAP;
            nGrayLevel     = 17;
            dib_type       = BPP8;
            break;
        case 8:
            uOutlineFormat = GGO_GRAY8_BITMAP;
            nGrayLevel     = 65;
            dib_type       = BPP8;
            break;
        default:
            return;
        }

        // Prepare DIB palette
        for (int i = 0; i < nGrayLevel; i++)
        {
            unsigned char gray = (unsigned char)(i * 255 / (nGrayLevel - 1));
            palette[i].set_color(0, 0, 0, gray);
        }
    }

    BOOL BkDrawText(HDC hDC, LPCTSTR lpszString, int nLen, LPRECT rcDraw, UINT uFormat)
    {
        LPCTSTR pChar = lpszString;
        GLYPHMETRICS  gm;
        MAT2 mat2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
        TEXTMETRIC tm;
        GetTextMetrics(hDC, &tm);
        int     x = rcDraw->left, y = rcDraw->top + tm.tmAscent;
        COLORREF crForeground = ::GetTextColor(hDC);
        COLORREF crBackground = ::GetBkColor(hDC);
        BOOL bTransparent = ::GetBkMode(hDC) == TRANSPARENT;

//         CDCHandle dc(hDC);
//         
//         dc.FillSolidRect(rcDraw, RGB(0, 0xAA, 0));
// 

        while (_T('\0') != *pChar && 0 != (nLen--))
        {
            UINT uChar = (UINT)*(pChar++);

            DWORD dwSize = GetGlyphOutline(
                hDC, uChar, uOutlineFormat, &gm, 0, NULL, &mat2);
            
            if (0 == dwSize || GDI_ERROR == dwSize)
                return FALSE;

            dib_glyph.create(
                dib_type, 
                gm.gmBlackBoxX, 0 - gm.gmBlackBoxY, 
                NULL, palette, nGrayLevel);

            dib_draw.create(hDC, BPP32_RGBA8888, gm.gmBlackBoxX, gm.gmBlackBoxY);

            GetGlyphOutline(hDC, uChar, uOutlineFormat, &gm, (DWORD)dib_glyph.get_data_size(), dib_glyph.get_data(), &mat2);

            class _pixel_op_src_copy_alpha
            {
            public:
                _pixel_op_src_copy_alpha(COLORREF crBase)
                    : color_base(crBase)
                {
                }

                void operator()(const Yrlib_Color &src, Yrlib_Color &dest) const
                {
                    dest.set_color(
                        GetRValue(color_base) * src.alpha / 255, 
                        GetGValue(color_base) * src.alpha / 255, 
                        GetBValue(color_base) * src.alpha / 255, 
                        src.alpha);
                }

            private:
                COLORREF color_base;
            };

            dib_draw.mix_from(
                dib_glyph, 
                0, 0, 0, 0, gm.gmBlackBoxX, gm.gmBlackBoxY, 
                _pixel_op_src_copy_alpha(crForeground)
                );

            dib_draw.blend(
                hDC, 
                x + gm.gmptGlyphOrigin.x, 
                y - gm.gmptGlyphOrigin.y,
                FALSE);

            x += gm.gmCellIncX;
            y += gm.gmCellIncY;

    //         delete[] pbyDIB;
        }

        return TRUE;
    }

protected:
private:
};

Yrlib_Dib_Section bmp3;
Yrlib_Dib_Section bmp;
Yrlib_Dib_Section layer;
CDC dc_layer;

void DrawText2(Yrlib_Dib& dest, LPCTSTR lpchText, int cchText, LPRECT lprc, HFONT font, COLORREF crtext, UINT format)
{
    CRect rcTest = lprc, rcDraw = lprc;

    Yrlib_Dib_Section bmp_tmp;

    CDC dc_tmp;
    HDC hdc_desktop = ::GetDC(NULL);

    dc_tmp.CreateCompatibleDC(hdc_desktop);

    dc_tmp.SelectFont(font);

    DrawText(dc_tmp, lpchText, cchText, rcTest, format | DT_CALCRECT);


    rcDraw.MoveToXY(rcTest.TopLeft());
    rcTest.MoveToXY(0, 0);

    bmp_tmp.create(hdc_desktop, BPP24_RGB888, rcTest.right, rcTest.bottom);

    ::ReleaseDC(NULL, hdc_desktop);

    dc_tmp.SelectBitmap(bmp_tmp.get_hbitmap());

    COLORREF crbg = (~crtext) & 0xFFFFFF;

    dc_tmp.SetTextColor(crtext);
    //dc_tmp.SetBkColor(crbg);
    dc_tmp.SetBkMode(TRANSPARENT);
    dc_tmp.DrawText(lpchText, cchText, rcTest, format);

//     bmp_tmp.pre_multiply();
    bmp_tmp.draw(dest, rcDraw.left, rcDraw.top);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
        {

            CWindowDC dc(hWnd);

            if (0)
            {
                CDC dcmem;
                dcmem.CreateCompatibleDC(dc);

                Yrlib_Dib_Section vert, horz, v2h, h2v;
                vert.load_resource(dc, IDB_BITMAP4);
                horz.load_resource(dc, IDB_BITMAP5);

                v2h.load_resource(dc, IDB_BITMAP5);
                dcmem.SelectBitmap(v2h.get_hbitmap());
                vert.blend(dcmem, 0, 0);
                v2h.dump(L"d:\\v2h.bmp");
                h2v.load_resource(dc, IDB_BITMAP4);
                dcmem.SelectBitmap(h2v.get_hbitmap());
                horz.blend(dcmem, 0, 0);
                h2v.dump(L"d:\\h2v.bmp");
                ExitProcess(0);
            }

            Yrlib_Region rgn;

            //rgn.create_round_rect(0, 0, 100, 100, 5, 5);
            rgn.create_rect(0, 0, 100, 100);

            class _trace_rect
            {
            public:
                bool operator()(LPRECT rect) const
                {
                    return true;
                }
            };

            rgn.for_each_region_data(_trace_rect());

            dc_layer.CreateCompatibleDC(dc);

            layer.create(dc_layer, BPP32_RGBA8888, 300, 300);
            dc_layer.SelectBitmap(layer.get_hbitmap());

            RECT rcDraw = {0, 0, 300, 300};

//             bmp.load_resource(dc, IDB_BITMAP2);
//             bmp.pre_multiply();
// 
//             bmp.draw(layer, 0, 0, 3, 3, 0, 0);
//             bmp.draw(layer, 0, 0, bmp.get_width(), bmp.get_height(), 0, 0);

            bmp.load_resource(dc, IDB_BITMAP2);
            bmp.pre_multiply();

            DWORD dw = ::GetTickCount();
            for (int i = 0;i < 1; i ++)
            {
                LONG left = 6, top = 6;
                LONG width = bmp.get_width();
                LONG height = bmp.get_height();
                LONG bottom = height - top - 1;
                LONG right = width - left - 1;

                // lt
                bmp.draw(layer, rcDraw.left, rcDraw.top, left, top, 0, 0);
                // lb
                bmp.draw(layer, rcDraw.left, rcDraw.bottom - bottom - 1, left, top + 1, 0, top);
                // rt
                bmp.draw(layer, rcDraw.right - right, rcDraw.top, right, top, left + 1, 0);
                // rb
                bmp.draw(layer, rcDraw.right - right, rcDraw.bottom - bottom - 1, right, top + 1, left + 1, top);
                // l
                bmp.tile(layer, rcDraw.left, rcDraw.top + top, left, rcDraw.bottom - rcDraw.top - height, 0, top, left, 1);
                // r
                bmp.tile(layer, rcDraw.right - right, rcDraw.top + top, right, rcDraw.bottom - rcDraw.top - height, left + 1, top, right, 1);
                // t
                bmp.tile(layer, rcDraw.left + left, rcDraw.top, rcDraw.right - rcDraw.left - width + 1, top, left, 0, 1, top);
                // b
                bmp.tile(layer, rcDraw.left + left, rcDraw.bottom - bottom, rcDraw.right - rcDraw.left - width + 1, bottom, left, top + 1, 1, bottom);

                layer.rectangle(
                    rcDraw.left + left, 
                    rcDraw.top + top, 
                    rcDraw.right - rcDraw.left - width + 1, 
                    rcDraw.bottom - rcDraw.top - height + 1, 
                    bmp.get_pixel(left, top));

                bmp3.load_resource(dc, IDB_BITMAP3);
//                 bmp3.pre_multiply();

                bmp3.blend(layer, 10, 60);
                bmp3.blend(dc_layer, 120, 60, true);
                Yrlib_Region& rgn = layer.get_clip();
                
                rgn.create_elliptic(10, 130, 220, 230);
                bmp3.blend(layer, 10, 130);
                rgn.destroy();

                dw = GetTickCount();
                for (int n = 0; n < 1; n ++)
                {
                    dc_layer.SetBkMode(TRANSPARENT);

                    CFontHandle ft((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
                    LOGFONT lf;
                    ft.GetLogFont(&lf);

                    wcscpy_s(lf.lfFaceName, L"微软雅黑");
                    lf.lfItalic = 0;
                    lf.lfWeight = FW_BOLD;

                    ft.Detach();
                    ft.CreateFontIndirect(&lf);

                    dc_layer.SelectFont(ft);

                    dc_layer.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
                    BkTextRender<2>().BkDrawText(dc_layer, L"标题栏哦", -1, CRect(10, 10, 100, 100), DT_SINGLELINE);

                    ft.DeleteObject();

                    lf.lfHeight = -20;
                    ft.CreateFontIndirect(&lf);
                    dc_layer.SelectFont(ft);
                    dc_layer.SetTextColor(RGB(0xFF, 0xAA, 0xAA));
                    BkTextRender<2>().BkDrawText(dc_layer, L"这是一段文字", -1, CRect(10, 30, 100, 200), DT_SINGLELINE);

                    //dc_layer.DrawText(L"这是一段文字", -1, CRect(10, 70, 200, 200), DT_SINGLELINE);
                    //DrawText2(layer, L"这是一段文字", -1, CRect(10, 70, 200, 200), ft, RGB(0xFF, 0xAA, 0xAA), DT_SINGLELINE);

                    ft.DeleteObject();
                }
            }

            dw = GetTickCount() - dw;
            {
                CString str;
                str.Format(L"xxxx %u\n", dw);
//                 ::MessageBox(NULL, str, 0, 0);
//                 ExitProcess(0);
            }

            if (GetWindowLongPtr(hWnd, GWL_EXSTYLE) & WS_EX_LAYERED)
            {
                POINT pt = {100, 100};
                POINT ptsrc = {0, 0};
                SIZE sz = {300, 300};
                BLENDFUNCTION bl_func = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

                ::UpdateLayeredWindow(hWnd, NULL, &pt, &sz, dc_layer, &ptsrc, 0, &bl_func, ULW_ALPHA);
            }
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    case WM_LBUTTONDOWN:
        SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
        break;
    case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

        ::BitBlt(hdc, 0, 0, 300, 300, dc_layer, 0, 0, SRCCOPY);

        if (0)
        {
            CDCHandle dc;
            CFontHandle ft((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
            LOGFONT lf;
            ft.GetLogFont(&lf);

            lf.lfHeight = -20;
            wcscpy_s(lf.lfFaceName, L"幼圆");
            lf.lfItalic = 0;
            lf.lfWeight = FW_BOLD;

            ft.Detach();
            ft.CreateFontIndirect(&lf);

            dc.Attach(hdc);
            dc.SelectFont(ft);
            dc.SetTextColor(RGB(0xFF, 0, 0));

            if (!bmp.is_valid())
            {
            }

            LPCWSTR lpszText = L"这a是b一c段d文e字fghijklmnopqrst";
            //LPCWSTR lpszText = L"b";
            if (0)
            {
                RECT rc = {20, 10, 1000, 40};

                BkTextRender<2> textRender;

                DWORD dw = ::GetTickCount();
                for (int i = 0; i < 1000; i ++)
                {
                    textRender.BkDrawText(hdc, lpszText, -1, &rc, 0);
                }
                dw = GetTickCount() - dw;
                {
                    CString str;
                    str.Format(L"xxxx %u\n", dw);
                    ::OutputDebugString(str);
                    ExitProcess(0);
                }
            }

            if (1)
            {
                RECT rc = {20, 10, 1000, 40};

                BkTextRender<1>().BkDrawText(hdc, lpszText, -1, &rc, 0);
                rc.left += 500;
                DrawText(hdc, lpszText, -1, &rc, 0);
                rc.left -= 500;
                rc.top += 40;
                rc.bottom += 40;
                BkTextRender<2>().BkDrawText(hdc, lpszText, -1, &rc, 0);
                rc.left += 500;
                DrawText(hdc, lpszText, -1, &rc, 0);
                rc.left -= 500;
                rc.top += 40;
                rc.bottom += 40;
                BkTextRender<4>().BkDrawText(hdc, lpszText, -1, &rc, 0);
                rc.left += 500;
                DrawText(hdc, lpszText, -1, &rc, 0);
                rc.left -= 500;
                rc.top += 40;
                rc.bottom += 40;
                BkTextRender<8>().BkDrawText(hdc, lpszText, -1, &rc, 0);
                rc.left += 500;
                DrawText(hdc, lpszText, -1, &rc, 0);
                rc.left -= 500;
                rc.top += 40;
                rc.bottom += 40;
            }
            
            ft.DeleteObject();

            {
                RECT rc = {100 - 2, 200 - 2, 300 + 2, 400 + 2};

                dc.Rectangle(&rc);

            }

//             bmp.blend(hdc, 100, 200, 3, 3, 0, 0, 3, 3);
//             bmp.blend(hdc, 100, 200, 3, 3, 0, 0, 3, 3);
//             bmp.draw(hdc, 100, 20, 30, 30, 0, 0, 30, 30);
//             bmp.draw(hdc, 150, 20, 30, 30, 0, 10, 30, 30);
//             bmp.draw(hdc, 200, 20, 30, 30, 10, 0, 30, 30);
//             bmp.draw(hdc, 250, 20, 60, 60, 0, 0, 30, 30);

            if (0)
            {
                DWORD dw = ::GetTickCount();
                for (int i = 0; i < 1000; i ++)
                {
                    bmp.blend(hdc, 100, 20, false);
                }
                dw = GetTickCount() - dw;
                {
                    CString str;
                    str.Format(L"xxxx %u\n", dw);
                    ::OutputDebugString(str);
                    ExitProcess(0);
                }
            }
        }

		EndPaint(hWnd, &ps);
		break;
	case WM_NCHITTEST:
		return HTCLIENT;
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
