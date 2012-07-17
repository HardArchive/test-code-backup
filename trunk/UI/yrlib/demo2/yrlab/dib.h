#pragma once

#include "color.h"
#include "pixel_buffer.h"

class Yrlab_Bitmap
{
public:
    Yrlab_Bitmap()
        : _bitmap_handle(NULL)
    {
    }

    inline bool valid() const
    {
        return (NULL != _bitmap_handle);
    }

    inline HBITMAP attach(HBITMAP bitmap)
    {
        HBITMAP bmp_old = detach();

        _bitmap_handle = bitmap;

        return bmp_old;
    }

    inline HBITMAP detach()
    {
        HBITMAP bmp_old = _bitmap_handle;

        _bitmap_handle = NULL;

        return bmp_old;
    }

    inline void clear()
    {
        if (valid())
            ::DeleteObject(detach());
    }

    operator HBITMAP() const
    {
        return _bitmap_handle;
    }

protected:
private:
    HBITMAP _bitmap_handle;
};

class Yrlab_DIBSection
    : public Yrlab_Bitmap
{
public:
    Yrlab_DIBSection()
        : _pixel_bits(NULL)
        , _dib_width(0)
        , _dib_height(0)
    {

    }

    bool create(HDC hdc, size_t width, size_t height)
    {
        if (valid())
            return false;

        BITMAPINFO bi;

        memset(&bi, 0, sizeof(BITMAPINFO));

        bi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth        = width;
        bi.bmiHeader.biHeight       = height;
        bi.bmiHeader.biPlanes       = 1;
        bi.bmiHeader.biBitCount     = 32;
        bi.bmiHeader.biCompression  = BI_RGB;

        HBITMAP dib_handle = ::CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, (void **)&_pixel_bits, NULL, 0);
        if (NULL == dib_handle)
            return false;

        attach(dib_handle);

        _pixel_buffer.attach(
            (unsigned char *)_pixel_bits, 
            Yrlib_PixelBuffer::calc_buffer_bytes(width, height), 
            width, height);

        _dib_width  = width;
        _dib_height = height;

        return true;
    }

    bool fill_solid_rectangle(const Yrlib_Color& clr, int l, int t, int r, int b)
    {
        if (!valid())
            return false;

        if (l > r) { int temp = l; l = r; r = temp; };
        if (t > b) { int temp = t; t = b; b = temp; };

        l = max(0, l); t = max(0, t); r = min(r, (int)_dib_width - 1); b = min(b, (int)_dib_height - 1);

        for (int y = t; y <= b; y ++)
        {
            Yrlib_Color *pixel_data = (Yrlib_Color *)_pixel_buffer.get_data(y);

            for (int x = l; x <= r; x ++)
            {
                pixel_data[x].set_color(clr);
            }
        }

        return true;
    }

protected:
private:
    Yrlib_PixelBuffer _pixel_buffer;
    void *_pixel_bits;
    size_t _dib_width;
    size_t _dib_height;
};