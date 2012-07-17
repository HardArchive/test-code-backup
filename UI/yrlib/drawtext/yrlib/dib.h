#pragma once

#include "pixel_buffer.h"
#include "region.h"

enum Yrlib_Dib_Type
{
    BPP1,           //   2 color image, palette-based
    BPP2,           //   4 color image, palttte-based
    BPP4,           //  16 color image, palette-based
    BPP4_RLE,       //  16 color image, palette-based, RLE compressed
    BPP8,           // 256 color image, palette-based 
    BPP8_RLE,       // 256 color image, palette-based, RLE compressed
    BPP16_RGB555,   // 15 bit RGB color image, 5-5-5
    BPP16_RGB565,   // 16 bit RGB color image, 5-6-5, 1 bit unused
    BPP16_BF,       // 16 bit RGB color image, non-standard bit masks, NT-only
    BPP24_RGB888,   // 24 bit RGB color image, 8-8-8
    BPP32_RGB888,   // 32 bit RGB color image, 8-8-8, 8 bit unused
    BPP32_RGBA8888, // 32 bit ARGB color image, 8-8-8-8
    BPP32_BF,       // 32 bit RGB color image, non-standard bit masks, NT-only
    Invalid,
};

struct Yrlib_Dib_Type_Info 
{
    Yrlib_Dib_Type  type;
    bool            support_create;
    bool            use_palette;
    WORD            bit_counts;
    WORD            max_palette_size;
    bool            force_use_bitfields;
    DWORD           mask_red;
    DWORD           mask_green;
    DWORD           mask_blue;
    DWORD           mask_alpha;
};

const Yrlib_Dib_Type_Info Yrlib_Dib_Type_Info_Table[] = {
//  ---------------------------------------------------------------------------------------------------------------
//   type            support    use         bit     max palette force use   bit mask
//   name            create     palette     counts  size        bit fields  r           g           b           a
//  ---------------------------------------------------------------------------------------------------------------
    {BPP1,           true,      true,       1,      2,          false,      0,          0,          0,          0},
    {BPP2,           true,      true,       2,      4,          false,      0,          0,          0,          0},
    {BPP4,           true,      true,       4,      8,          false,      0,          0,          0,          0},
    {BPP4_RLE,       false,     true,       4,      8,          false,      0,          0,          0,          0},
    {BPP8,           true,      true,       8,      256,        false,      0,          0,          0,          0},
    {BPP8_RLE,       false,     true,       8,      256,        false,      0,          0,          0,          0},
    {BPP16_RGB555,   false,     false,      16,     0,          false,      0x00007C00, 0x000003E0, 0x0000001F, 0},
    {BPP16_RGB565,   false,     false,      16,     0,          true,       0x0000F800, 0x000007E0, 0x0000001F, 0},
    {BPP16_BF,       false,     false,      16,     0,          true,       0,          0,          0,          0},
    {BPP24_RGB888,   true,      false,      24,     0,          false,      0x00FF0000, 0x0000FF00, 0x000000FF, 0},
    {BPP32_RGB888,   false,     false,      32,     0,          false,      0x00FF0000, 0x0000FF00, 0x000000FF, 0},
    {BPP32_RGBA8888, true,      false,      32,     0,          false,      0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000},
    {BPP32_BF,       false,     false,      32,     0,          true,       0,          0,          0,          0},
//  ---------------------------------------------------------------------------------------------------------------
};

struct Yrlib_Dib_Bitfields
{
    DWORD bitfield_red;
    DWORD bitfield_green;
    DWORD bitfield_blue;
};

class Yrlib_Pixel_Operation
{
public:
    void operator()(Yrlib_Color &clr) const
    {
        __asm int 3;
    }
};

class Yrlib_Pixel_Mix_Operation
{
public:
    void operator()(const Yrlib_Color &src, Yrlib_Color &dest) const
    {
        __asm int 3;
    }
};

class Yrlib_Pixel_Mix_Op_SrcCopy
{
public:
    void operator()(const Yrlib_Color &src, Yrlib_Color &dest) const
    {
        dest = src;
    }
};

class Yrlib_Pixel_Mix_Op_AlphaBlend
{
public:
    void operator()(const Yrlib_Color &src, Yrlib_Color &dest) const
    {
        switch (src.alpha)
        {
        case 0:
            break;
        case 255:
            dest = src;
            break;
        default:

            dest.red = src.red * src.alpha / 255 + dest.red * (255 - src.alpha) / 255;
            dest.green = src.green * src.alpha / 255 + dest.green * (255 - src.alpha) / 255;
            dest.blue = src.blue * src.alpha / 255 + dest.blue * (255 - src.alpha) / 255;
            dest.alpha = src.alpha + dest.alpha * (255 - src.alpha) / 255;
            break;
        }
    }
};

class Yrlib_Pixel_Op_PreMultiply
{
public:
    void operator()(Yrlib_Color &clr) const
    {
        switch (clr.alpha)
        {
        case 0:
            clr.red = clr.green = clr.blue = 0;
            break;
        case 255:
            break;
        default:
            clr.red     = (unsigned char)((long)clr.red * clr.alpha / 255);
            clr.green   = (unsigned char)((long)clr.green * clr.alpha / 255);
            clr.blue    = (unsigned char)((long)clr.blue * clr.alpha / 255);
            break;
        }
    }
};

class Yrlib_Pixel_Op_SetColor
{
public:
    Yrlib_Pixel_Op_SetColor(Yrlib_Color clr_set)
        : _clr_set(clr_set)
    {

    }
    void operator()(Yrlib_Color &clr) const
    {
        clr = _clr_set;
    }

private:
    Yrlib_Color _clr_set;
};

class Yrlib_Dib
{
public:
    Yrlib_Dib()
        : _managed(false)
        , _is_dib_section(false)
        , _dib_info(NULL)
    {
    }

    virtual ~Yrlib_Dib()
    {
        destroy();
    }

    bool is_valid()
    {
        return (NULL != _dib_info);
    }

    void destroy()
    {
        if (_managed)
        {
            delete[] (LPBYTE)_dib_info;
        }

        _pixel_buffer.destroy();

        _managed        = false;
        _is_dib_section = false;
        _dib_info       = NULL;
    }

    bool load_resource(
        UINT        res_id, 
        HINSTANCE   module_handle   = (HINSTANCE)&__ImageBase, 
        LPCWSTR     res_type        = RT_BITMAP, 
        bool        has_file_header = false)
    {
        return load_resource(MAKEINTRESOURCE(res_id), module_handle, res_type, has_file_header);
    }

    bool load_resource(
        LPCWSTR     res_name, 
        HINSTANCE   module_handle   = (HINSTANCE)&__ImageBase, 
        LPCWSTR     res_type        = RT_BITMAP, 
        bool        has_file_header = false)
    {
        BITMAPINFO *dib_info_and_data = (BITMAPINFO *)copy_dib_data_from_buffer(
            res_name, module_handle, res_type, has_file_header);
        if (NULL == dib_info_and_data)
            return false;

        bool ret_code = attach(dib_info_and_data);
        if (!ret_code)
        {
            delete[] (LPBYTE)dib_info_and_data;

            return false;
        }

        _managed = true;

        return true;
    }

    static LPBYTE copy_dib_data_from_buffer(
        LPCWSTR     res_name, 
        HINSTANCE   module_handle   = (HINSTANCE)&__ImageBase, 
        LPCWSTR     res_type        = RT_BITMAP,
        bool        has_file_header = false)
    {
        HRSRC res_handle = ::FindResource(module_handle, res_name, res_type);
        if (NULL == res_handle)
            return NULL;

        DWORD res_size = ::SizeofResource(module_handle, res_handle); 
        if (0 == res_size)
            return NULL;

        HGLOBAL data_handle = ::LoadResource(module_handle, res_handle);
        if (NULL == data_handle)
            return NULL;

        LPVOID res_data_buffer = ::LockResource(data_handle);
        if (NULL == res_data_buffer)
        {
            ::FreeResource(data_handle);

            return NULL;
        }

        size_t file_header_size = has_file_header ? sizeof(BITMAPFILEHEADER) : 0;

        LPBYTE buffer = new BYTE[res_size - file_header_size];
        memcpy(buffer, ((LPBYTE)res_data_buffer) + file_header_size, res_size - file_header_size);

        ::FreeResource(data_handle);

        return buffer;
    }

    bool attach(BITMAPINFO* dib_info, LPVOID pixel_buffer = NULL)
    {
        _dib_info = dib_info;

        if (NULL == pixel_buffer)
        {
            pixel_buffer = ((LPBYTE)dib_info) + get_bm_info_size(dib_info);
        }

        bool ret = _pixel_buffer.attach((LPBYTE)pixel_buffer, 0, &dib_info->bmiHeader);
        if (ret)
        {
            _dib_info       = dib_info;
            _managed        = false;
            _is_dib_section = false;
        }

        return ret;
    }

    bool create(
        Yrlib_Dib_Type              type,
        int                         width,
        int                         height,
        const Yrlib_Dib_Bitfields*  bitfields       = NULL,
        const Yrlib_Color*          palette         = NULL,
        const size_t                palette_size    = 0)
    {
        destroy();

        _dib_info = create_dib_info(type, width, height, bitfields, palette, palette_size);
        if (NULL == _dib_info)
            return false;

        bool ret = _pixel_buffer.create(width, abs(height), _dib_info->bmiHeader.biBitCount);
        if (!ret)
        {
            destroy();

            return false;
        }

        _managed        = true;
        _is_dib_section = false;

        return true;
    }

    static BITMAPINFO* create_dib_info(
        Yrlib_Dib_Type              type,
        int                         width           = 0,
        int                         height          = 0,
        const Yrlib_Dib_Bitfields*  bitfields       = NULL,
        const Yrlib_Color*          palette         = NULL,
        const size_t                palette_size    = 0)
    {
        const Yrlib_Dib_Type_Info &type_info = Yrlib_Dib_Type_Info_Table[type];

        if (!type_info.support_create || palette_size > type_info.max_palette_size)
            return NULL;

        if ((0 != palette_size && NULL == palette) || (0 == palette_size && NULL != palette))
            return NULL;

        if (BPP16_RGB565 == type && NULL == bitfields)
        {
            const Yrlib_Dib_Bitfields _def_rgb565 = {
                Yrlib_Dib_Type_Info_Table[BPP16_RGB565].mask_red, 
                Yrlib_Dib_Type_Info_Table[BPP16_RGB565].mask_green, 
                Yrlib_Dib_Type_Info_Table[BPP16_RGB565].mask_blue};
            bitfields = &_def_rgb565;
        }

        size_t total_size = sizeof(BITMAPINFOHEADER)
                          + palette_size * sizeof(Yrlib_Color)
                          + (bitfields ? sizeof(Yrlib_Dib_Bitfields) : 0);

        LPBYTE buff_data = new BYTE[total_size];
        
        if (palette)
        {
            memcpy(
                buff_data + sizeof(BITMAPINFOHEADER), 
                palette, palette_size * sizeof(Yrlib_Color));
        }

        if (bitfields)
        {
            memcpy(
                buff_data + sizeof(BITMAPINFOHEADER) + palette_size * sizeof(Yrlib_Color), 
                bitfields, sizeof(Yrlib_Dib_Bitfields));
        }

        BITMAPINFOHEADER* dib_header = (BITMAPINFOHEADER *)buff_data;

        dib_header->biSize          = sizeof(BITMAPINFOHEADER);
        dib_header->biWidth         = width;
        dib_header->biHeight        = height;
        dib_header->biPlanes        = 1;
        dib_header->biBitCount      = type_info.bit_counts;
        dib_header->biCompression   = bitfields ? BI_BITFIELDS : BI_RGB;
        dib_header->biSizeImage     = 0;
        dib_header->biXPelsPerMeter = 0;
        dib_header->biYPelsPerMeter = 0;
        dib_header->biClrUsed       = (DWORD)palette_size;
        dib_header->biClrImportant  = 0;

        return (BITMAPINFO *)buff_data;
    }

    LONG get_width() const 
    {
        if (NULL == _dib_info)
            return 0;

        return _dib_info->bmiHeader.biWidth;
    }

    LONG get_height() const 
    {
        if (NULL == _dib_info)
            return 0;

        return _dib_info->bmiHeader.biHeight;
    }

    BITMAPINFO* get_bmi() const 
    {
        return _dib_info;
    }

    size_t get_bmi_size() const 
    {
        return get_bm_info_size(_dib_info);
    }

    static size_t get_bm_info_size(BITMAPINFO* dib_info) 
    {
        if (NULL == dib_info)
            return 0;

        size_t size_total = sizeof(BITMAPINFOHEADER)                                // header
                          + dib_info->bmiHeader.biClrUsed * sizeof(Yrlib_Color)     // palette
                          + ((dib_info->bmiHeader.biCompression & BI_BITFIELDS) ? sizeof(Yrlib_Dib_Bitfields) : 0);

        return size_total;
    }

    Yrlib_Color* get_palette()
    {
        if (NULL == _dib_info)
            return NULL;

        if (0 == _dib_info->bmiHeader.biClrUsed)
            return NULL;

        return (Yrlib_Color*)_dib_info->bmiColors;
    }

    DWORD get_palette_size()
    {
        if (NULL == _dib_info)
            return 0;

        return _dib_info->bmiHeader.biClrUsed;
    }

    void* get_data(size_t lineno = 0, bool bottom_to_top = true) const
    {
        return _pixel_buffer.get_data(lineno, bottom_to_top);
    }

    size_t get_data_size() const
    {
        return _pixel_buffer.get_size();
    }

    Yrlib_Region& get_clip()
    {
        return _clip_region;
    }

    bool draw(Yrlib_Dib& dest, int x, int y, int width, int height, int srcx, int srcy)
    {
        if (NULL == _dib_info)
            return false;

        mix_to(dest, x, y, srcx, srcy, width, height, Yrlib_Pixel_Mix_Op_SrcCopy());

        return true;
    }

    bool blend(Yrlib_Dib& dest, int x, int y, int width, int height, int srcx, int srcy)
    {
        if (NULL == _dib_info)
            return false;

        if (32 == _dib_info->bmiHeader.biBitCount)
        {
            mix_to(dest, x, y, srcx, srcy, width, height, Yrlib_Pixel_Mix_Op_AlphaBlend());
        }
        else
        {
            mix_to(dest, x, y, srcx, srcy, width, height, Yrlib_Pixel_Mix_Op_SrcCopy());
        }

        return true;
    }

    bool draw(HDC hdc, int x, int y, int width, int height, int srcx, int srcy, int srcwidth, int srcheight)
    {
        if (NULL == _dib_info)
            return false;

        srcwidth = min(srcwidth, _dib_info->bmiHeader.biWidth);
        srcheight = min(srcheight, abs(_dib_info->bmiHeader.biHeight));
        srcy = (_dib_info->bmiHeader.biHeight > 0) ? (_dib_info->bmiHeader.biHeight - srcy - srcheight) : srcy;

        if (srcwidth == width && srcheight == height)
        {
            ::SetDIBitsToDevice(
                hdc, 
                x, y, width, height, srcx, srcy, 
                0, abs(_dib_info->bmiHeader.biHeight), 
                _pixel_buffer.get_data(), _dib_info, DIB_RGB_COLORS);
        }
        else
        {
            ::StretchDIBits(
                hdc, 
                x, y, width, height, srcx, srcy, srcwidth, srcheight,
                _pixel_buffer.get_data(), _dib_info, DIB_RGB_COLORS, SRCCOPY);
        }

        return true;
    }

    bool blend(HDC hdc, int x, int y, int width, int height, int srcx, int srcy, int srcwidth, int srcheight, bool need_pre_multiply = false);

    bool draw(HDC hdc, int x, int y)
    {
        return draw(
            hdc, 
            x, y, _dib_info->bmiHeader.biWidth, abs(_dib_info->bmiHeader.biHeight), 
            0, 0, _dib_info->bmiHeader.biWidth, abs(_dib_info->bmiHeader.biHeight));
    }

    bool draw(HDC hdc, int x, int y, int width, int height)
    {
        return draw(
            hdc, 
            x, y, width, height, 
            0, 0, width, height);
    }

    bool draw(HDC hdc, int x, int y, int width, int height, int srcx, int srcy)
    {
        return draw(
            hdc, 
            x, y, width, height, 
            srcx, srcy, width, height);
    }

    bool draw(Yrlib_Dib& dest, int x, int y)
    {
        if (!is_valid() || !dest.is_valid())
            return false;

        return draw(
            dest, 
            x, y, _dib_info->bmiHeader.biWidth, abs(_dib_info->bmiHeader.biHeight), 
            0, 0);
    }

    bool draw(Yrlib_Dib& dest, int x, int y, int width, int height)
    {
        return draw(
            dest, 
            x, y, width, height, 
            0, 0);
    }

    bool tile(Yrlib_Dib& dest, int x, int y, int width, int height, int srcx, int srcy, int srcwidth, int srcheight)
    {
        for (int posx = x; posx < x + width; posx += srcwidth)
        {
            for (int posy = y; posy < y + height; posy += srcheight)
            {
                draw(dest, posx, posy, srcwidth, srcheight, srcx, srcy);
            }
        }

        return true;
    }

    bool blend(HDC hdc, int x, int y, bool need_pre_multiply = false)
    {
        return blend(
            hdc, 
            x, y, _dib_info->bmiHeader.biWidth, abs(_dib_info->bmiHeader.biHeight), 
            0, 0, _dib_info->bmiHeader.biWidth, abs(_dib_info->bmiHeader.biHeight),
            need_pre_multiply);
    }

    bool blend(HDC hdc, int x, int y, int width, int height, bool need_pre_multiply = false)
    {
        return blend(
            hdc, 
            x, y, width, height, 
            0, 0, width, height,
            need_pre_multiply);
    }

    bool blend(HDC hdc, int x, int y, int width, int height, int srcx, int srcy, bool need_pre_multiply = false)
    {
        return blend(
            hdc, 
            x, y, width, height, 
            srcx, srcy, width, height,
            need_pre_multiply);
    }

    bool blend(Yrlib_Dib& dest, int x, int y)
    {
        return blend(
            dest, 
            x, y, _dib_info->bmiHeader.biWidth, abs(_dib_info->bmiHeader.biHeight), 
            0, 0);
    }

    template<class mix_op>
    void mix_from(
        Yrlib_Dib& src, 
        size_t x_dest, size_t y_dest, size_t x_src, size_t y_src, 
        size_t width, size_t height, 
        const mix_op& mix_operation)
    {
        if (get_clip().is_null())
        {
            no_clip_mix_from(src, x_dest, y_dest, x_src, y_src, width, height, mix_operation);
        }
        else
        {
            Yrlib_Region rgn_draw;

            rgn_draw.create_rect((int)x_dest, (int)y_dest, (int)(x_dest + width), (int)(y_dest + height));

            int combine_ret = rgn_draw.combine(get_clip(), RGN_AND);
            if (NULLREGION == combine_ret)
                return;

            class mix_func_per_rect
            {
            public:
                mix_func_per_rect(Yrlib_Dib& dest, Yrlib_Dib& src, int x_add, int y_add, const mix_op& mix_operation)
                    : _dest(dest)
                    , _src(src)
                    , _x_add(x_add)
                    , _y_add(y_add)
                    , _mix_operation(mix_operation)
                {
                }
                bool operator()(LPRECT rect) const
                {
                    _dest.no_clip_mix_from(
                        _src, 
                        rect->left, rect->top, 
                        rect->left + _x_add, rect->top + _y_add, 
                        rect->right - rect->left - 1, rect->bottom - rect->top - 1, 
                        _mix_operation);

                    return true;
                }
            protected:
                Yrlib_Dib& _dest;
                Yrlib_Dib& _src;
                size_t _x_add;
                size_t _y_add;
                const mix_op& _mix_operation;
            };

            rgn_draw.for_each_region_data(
                mix_func_per_rect(*this, src, (int)x_src - (int)x_dest, (int)y_src - (int)y_dest, mix_operation));
        }
    }

    template<class mix_op>
    void no_clip_mix_from(
        Yrlib_Dib& src, 
        size_t x_dest, size_t y_dest, size_t x_src, size_t y_src, 
        size_t width, size_t height, 
        const mix_op& mix_operation)
    {
        if (_dib_info->bmiHeader.biBitCount <= 8)
            return; // not implemented

        Yrlib_Color clr_src, clr_dest;

        size_t bottom_dest  = min(_pixel_buffer.get_height() - 1, y_dest + height);
        size_t bottom_src   = min(src._pixel_buffer.get_height() - 1, y_src + height);
        size_t right_dest   = min(_pixel_buffer.get_width() - 1, x_dest + width);
        size_t right_src    = min(src._pixel_buffer.get_width() - 1, x_src + width);

        if (bottom_dest - y_dest > bottom_src - y_src)
        {
            bottom_dest = y_dest + bottom_src - y_src;
        }
//         else // bottom_src not used any more
//         {
//             bottom_src = y_src + bottom_dest - y_dest;
//         }

        if (right_dest - x_dest > right_src - x_src)
        {
            right_dest = x_dest + right_src - x_src;
        }
//         else // right_src not used any more
//         {
//             right_src = x_src + right_dest - x_dest;
//         }

        size_t ppb_dest = _pixels_per_byte(_dib_info->bmiHeader.biBitCount);
        size_t ppb_src  = _pixels_per_byte(src._dib_info->bmiHeader.biBitCount);
        size_t left_bytes_dest  = _pixel_offset_bytes(x_dest, _dib_info->bmiHeader.biBitCount);
        size_t left_bytes_src   = _pixel_offset_bytes(x_src, src._dib_info->bmiHeader.biBitCount);
        size_t left_ppb_dest    = x_dest % ppb_dest;
        size_t left_ppb_src     = x_src % ppb_src;
        size_t right_bytes_dest = _pixel_offset_bytes(right_dest, _dib_info->bmiHeader.biBitCount);
        size_t right_ppb_dest   = right_dest % ppb_dest;
//         size_t right_bytes_src  = _pixel_offset_bytes(right_src, src._dib_info->bmiHeader.biBitCount);
//         size_t right_ppb_src    = right_src % ppb_src;

        // for every scan line
        for (; y_dest <= bottom_dest; ++y_dest, ++y_src)
        {
            LPBYTE scan_line_dest       = (LPBYTE)_pixel_buffer.get_data(y_dest, _dib_info->bmiHeader.biHeight < 0);
            LPBYTE scan_line_src        = (LPBYTE)src.get_data(y_src, src._dib_info->bmiHeader.biHeight < 0);
            LPBYTE scan_line_pos_dest   = scan_line_dest + left_bytes_dest;
            LPBYTE scan_line_pos_src    = scan_line_src + left_bytes_src;
            size_t ppb_pos_dest         = left_ppb_dest;
            size_t ppb_pos_src          = left_ppb_src;

            // for every pixel in scan line
            while (TRUE)
            {
                if (scan_line_pos_dest > scan_line_dest + right_bytes_dest)
                    break;
                else if (scan_line_pos_dest == scan_line_dest + right_bytes_dest && ppb_pos_dest > right_ppb_dest)
                    break;

                // TODO: 16bit dib color transform
                // get pixel color or index
                ULONG clr_or_index_dest = _pixel_buffer.get_pixel(scan_line_pos_dest, ppb_pos_dest);
                ULONG clr_or_index_src = src._pixel_buffer.get_pixel(scan_line_pos_src, ppb_pos_src);

                if (_dib_info->bmiHeader.biBitCount <= 8)
                { // indexed color
                    clr_dest = _dib_info->bmiColors[clr_or_index_dest];
                }
                else
                { // original color
                    clr_dest = clr_or_index_dest;
                }

                if (src._dib_info->bmiHeader.biBitCount <= 8)
                { // indexed color
                    clr_src = src._dib_info->bmiColors[clr_or_index_src];
                }
                else
                { // original color
                    clr_src = clr_or_index_src;
                }

                // mix
                mix_operation(clr_src, clr_dest);

                // TODO: pixel to index
                clr_or_index_dest = (DWORD)clr_dest._value;

                // write pixel
                _pixel_buffer.set_pixel(scan_line_pos_dest, ppb_pos_dest, clr_or_index_dest);

                // next pixel
                ++ ppb_pos_dest;
                if (ppb_pos_dest == ppb_dest)
                {
                    ppb_pos_dest = 0;
                    scan_line_pos_dest += max(1, _dib_info->bmiHeader.biBitCount >> 3);
                }
                ++ ppb_pos_src;
                if (ppb_pos_src == ppb_src)
                {
                    ppb_pos_src = 0;
                    scan_line_pos_src += max(1, src._dib_info->bmiHeader.biBitCount >> 3);
                }
            }
        }
    }

    template<class mix_op>
    void mix_to(
        Yrlib_Dib& src, 
        size_t x_dest, size_t y_dest, size_t x_src, size_t y_src, 
        size_t width, size_t height, 
        const mix_op& mix_operation)
    {
        src.mix_from(*this, x_dest, y_dest, x_src, y_src, width, height, mix_operation);
    }

    template<class pixel_op>
    void per_pixel_operation(
        size_t x, size_t y, 
        size_t width, size_t height, 
        const pixel_op& mix_operation)
    {
        if (_dib_info->bmiHeader.biBitCount <= 8)
            return; // not implemented

        Yrlib_Color clr_pixel;

        size_t bottom  = min(_pixel_buffer.get_height() - 1, y + height);
        size_t right   = min(_pixel_buffer.get_width() - 1, x + width);

        size_t ppb          = _pixels_per_byte(_dib_info->bmiHeader.biBitCount);
        size_t left_bytes   = _pixel_offset_bytes(x, _dib_info->bmiHeader.biBitCount);
        size_t left_ppb     = x % ppb;
        size_t right_bytes  = _pixel_offset_bytes(right, _dib_info->bmiHeader.biBitCount);
        size_t right_ppb    = right % ppb;

        // for every scan line
        for (; y <= bottom; ++y)
        {
            LPBYTE scan_line        = (LPBYTE)_pixel_buffer.get_data(y, _dib_info->bmiHeader.biHeight < 0);
            LPBYTE scan_line_pos    = scan_line + left_bytes;
            size_t ppb_pos          = left_ppb;

            // for every pixel in scan line
            while (TRUE)
            {
                if (scan_line_pos > scan_line + right_bytes)
                    break;
                else if (scan_line_pos == scan_line + right_bytes && ppb_pos > right_ppb)
                    break;

                // TODO: 16bit dib color transform
                // get pixel color or index
                ULONG clr_or_index = _pixel_buffer.get_pixel(scan_line_pos, ppb_pos);

                if (_dib_info->bmiHeader.biBitCount <= 8)
                { // indexed color
                    clr_pixel = _dib_info->bmiColors[clr_or_index];
                }
                else
                { // original color
                    clr_pixel = clr_or_index;
                }

                // mix
                mix_operation(clr_pixel);

                // TODO: pixel to index
                clr_or_index = (DWORD)clr_pixel._value;

                // write pixel
                _pixel_buffer.set_pixel(scan_line_pos, ppb_pos, clr_or_index);

                // next pixel
                ++ ppb_pos;
                if (ppb_pos == ppb)
                {
                    ppb_pos = 0;
                    scan_line_pos += max(1, _dib_info->bmiHeader.biBitCount >> 3);
                }
            }
        }
    }

    Yrlib_Color get_pixel(int x, int y)
    {
        Yrlib_Color ret_color;

        if (!is_valid())
            return ret_color;

        size_t ppb          = _pixels_per_byte(_dib_info->bmiHeader.biBitCount);
        size_t left_bytes   = _pixel_offset_bytes(x, _dib_info->bmiHeader.biBitCount);
        LPBYTE scan_line    = (LPBYTE)_pixel_buffer.get_data(y, _dib_info->bmiHeader.biHeight < 0);

        if (NULL == scan_line)
            return ret_color;

        // TODO: 16bit dib color transform
        // get pixel color or index
        ULONG clr_or_index = _pixel_buffer.get_pixel(scan_line + left_bytes, x % ppb);

        if (_dib_info->bmiHeader.biBitCount <= 8)
        { // indexed color
            ret_color = _dib_info->bmiColors[clr_or_index];
        }
        else
        { // original color
            ret_color = clr_or_index;
        }

        return ret_color;
    }

    bool rectangle(int x, int y, int width, int height, Yrlib_Color clr)
    {
        if (!is_valid())
            return false;

        per_pixel_operation(x, y, width, height, Yrlib_Pixel_Op_SetColor(clr));

        return true;
    }

    void pre_multiply()
    {
        BYTE* pixel_data = (LPBYTE)get_data();
        int right = get_width(), bottom = abs(get_height());

        for (int pos_y = 0; pos_y < bottom; pos_y ++)
        {
            for (int pos_x = right; pos_x > 0; pos_x --)
            {
                BYTE alpha  = pixel_data[3];

                pixel_data[0] = (BYTE)((pixel_data[0] * alpha) / 255);
                pixel_data[1] = (BYTE)((pixel_data[1] * alpha) / 255);
                pixel_data[2] = (BYTE)((pixel_data[2] * alpha) / 255);

                pixel_data += 4;
            }
        }
    }


    bool dump(const wchar_t *file_name)
    {
        if (NULL == _dib_info)
            return false;

        FILE* file_handle = _tfopen(file_name, L"wb+");
        if (NULL == file_handle)
            return false;

        DWORD size_bmi = (DWORD)get_bmi_size();
        DWORD size_data = (DWORD)get_data_size();

        BITMAPFILEHEADER bmh = {0x4d42, sizeof(BITMAPFILEHEADER) + size_bmi + size_data, 0, 0, size_bmi + sizeof(BITMAPFILEHEADER)};

        fwrite(&bmh, sizeof(BITMAPFILEHEADER), 1, file_handle);
        fwrite(get_bmi(), size_bmi, 1, file_handle);
        fwrite(get_data(), size_data, 1, file_handle);

        fclose(file_handle);

        return true;
    }

protected:
    BITMAPINFO*         _dib_info;
    Yrlib_PixelBuffer   _pixel_buffer;
    bool                _is_dib_section;
    bool                _managed;
    Yrlib_Region        _clip_region;

    inline size_t _pixels_per_byte(const WORD bitcount)
    {
        if (bitcount >= 8)
            return 1;
        else
            return 8 / bitcount;
    }

    inline size_t _pixel_offset_bytes(size_t pixel, const WORD bitcount)
    {
        if (bitcount >= 8)
            return pixel * (bitcount >> 3);
        else
            return pixel * bitcount / 8;
    }

private:
};

class Yrlib_Dib_Section
    : public Yrlib_Dib
{
public:
    Yrlib_Dib_Section()
        : _handle_ddb(NULL)
        , _managed_ddb_handle(false)
    {
    }

    virtual ~Yrlib_Dib_Section()
    {
        destroy();
    }

    bool load_resource(
        HDC         hdc,
        UINT        res_id, 
        HINSTANCE   module_handle   = (HINSTANCE)&__ImageBase, 
        LPCWSTR     res_type        = RT_BITMAP, 
        bool        has_file_header = false)
    {
        return load_resource(hdc, MAKEINTRESOURCE(res_id), module_handle, res_type, has_file_header);
    }

    bool load_resource(
        HDC         hdc,
        LPCWSTR     res_name, 
        HINSTANCE   module_handle   = (HINSTANCE)&__ImageBase, 
        LPCWSTR     res_type        = RT_BITMAP, 
        bool        has_file_header = false)
    {
        bool ret_code = false;
        HGLOBAL data_handle = NULL;

        HRSRC res_handle = ::FindResource(module_handle, res_name, res_type);
        if (NULL == res_handle)
            goto _tag_exit0;

        DWORD res_size = ::SizeofResource(module_handle, res_handle); 
        if (0 == res_size)
            goto _tag_exit0;

        data_handle = ::LoadResource(module_handle, res_handle);
        if (NULL == data_handle)
            goto _tag_exit0;

        LPVOID res_data_buffer = ::LockResource(data_handle);
        if (NULL == res_data_buffer)
            goto _tag_exit0;

        size_t file_header_size = has_file_header ? sizeof(BITMAPFILEHEADER) : 0;
        BITMAPINFO *pbmi = (BITMAPINFO *)(((LPBYTE)res_data_buffer) + file_header_size);
        LPVOID res_pixel_buffer = (LPBYTE)res_data_buffer + file_header_size + get_bm_info_size(pbmi);
        LPVOID pixel_buffer = NULL;

        ret_code = create(hdc, pbmi, true);
        if (ret_code)
        {
            memcpy(get_data(), res_pixel_buffer, get_data_size());

            _is_dib_section = true;
        }
        else
        {
            destroy();
        }

    _tag_exit0:

        if (data_handle)
        {
            ::FreeResource(data_handle);
            data_handle = NULL;
        }

        return ret_code;
    }

    bool create(
        HDC hdc,
        Yrlib_Dib_Type type, 
        size_t width, size_t height, 
        const Yrlib_Dib_Bitfields*  bitfields       = NULL,
        const Yrlib_Color*          palette         = NULL,
        const size_t                palette_size    = 0,
        bool  scan_line_from_top    = false)
    {
        LPVOID pixel_buffer = NULL;
        BITMAPINFO *pbmi = NULL;
        
        pbmi = Yrlib_Dib::create_dib_info(type, (int)width, (int)(scan_line_from_top ? 0 - height : height), bitfields, palette, palette_size);
        if (NULL == pbmi)
            return false;

        bool ret_code = create(hdc, pbmi);
        if (ret_code)
        {
            _managed        = true;
            _is_dib_section = true;
        }
        else
        {
            delete[] (LPBYTE)pbmi;
            destroy();
        }

        return ret_code;
    }

    bool create(HDC hdc, BITMAPINFO *dib_info, bool copy_bmi = false)
    {
        LPVOID pixel_buffer = NULL;
        BITMAPINFO *pbmi = NULL;

        if (NULL == hdc || NULL == dib_info)
            return false;

        destroy();

        _handle_ddb = ::CreateDIBSection(hdc, dib_info, DIB_RGB_COLORS, &pixel_buffer, NULL, 0);
        if (NULL == _handle_ddb)
            return false;

        if (copy_bmi)
        {
            size_t bmi_size = get_bm_info_size(dib_info);
            
            pbmi = (BITMAPINFO*)new BYTE[bmi_size];

            memcpy(pbmi, dib_info, bmi_size);
        }
        else
        {
            pbmi = dib_info;
        }

        bool ret_code = Yrlib_Dib::attach(pbmi, pixel_buffer);
        if (ret_code)
        {
            _managed_ddb_handle = true;
            _managed        = copy_bmi;
            _is_dib_section = true;
        }
        else
        {
            if (copy_bmi)
                delete[] (LPBYTE)pbmi;
            destroy();
        }

        return ret_code;
    }

    bool clone(HDC hdc, const Yrlib_Dib& from)
    {
        bool ret_code = create(hdc, from.get_bmi(), true);
        if (!ret_code)
            return false;

        memcpy(get_data(), from.get_data(), from.get_data_size());
        
        return true;
    }

    // TODO: not implemented
    bool attach(HBITMAP hdibsec)
    {
        DIBSECTION dib_section_inf;
        int ret_bytes = ::GetObject(hdibsec, sizeof(DIBSECTION), &dib_section_inf);
        if (sizeof(DIBSECTION) != ret_bytes)
            return false;

        size_t bmi_size = get_bm_info_size((BITMAPINFO *)&(dib_section_inf.dsBmih));
        BITMAPINFO *pbmi = pbmi = (BITMAPINFO*)new BYTE[bmi_size];

        memcpy(pbmi, &(dib_section_inf.dsBmih), bmi_size);

        bool ret_code = Yrlib_Dib::attach(pbmi, dib_section_inf.dsBm.bmBits);
        if (ret_code)
        {
            _managed_ddb_handle = false;
            _handle_ddb     = hdibsec;
            _managed        = true;
            _is_dib_section = true;
        }
        else
        {
            delete[] (LPBYTE)pbmi;
            destroy();
        }

        return ret_code;
    }

    HBITMAP detach()
    {
        Yrlib_Dib::destroy();

        HBITMAP ret_handle = _handle_ddb;

        _handle_ddb = NULL;

        return ret_handle;
    }

    void destroy()
    {
        Yrlib_Dib::destroy();

        if (_handle_ddb && _managed_ddb_handle)
        {
            ::DeleteObject(_handle_ddb);
        }

        _handle_ddb = NULL;
    }

    HBITMAP get_hbitmap() const
    {
        return _handle_ddb;
    }

protected:
    HBITMAP _handle_ddb;
    bool    _managed_ddb_handle;

private:
};

bool Yrlib_Dib::blend(
    HDC hdc, int x, int y, int width, int height, 
    int srcx, int srcy, int srcwidth, int srcheight, 
    bool need_pre_multiply/* = false*/)
{
    if (NULL == _dib_info)
        return false;

    if (32 == _dib_info->bmiHeader.biBitCount)
    {
        Yrlib_Dib_Section dib_temp;

        if (_is_dib_section && !need_pre_multiply)
        {
            dib_temp.attach(((Yrlib_Dib_Section *)this)->get_hbitmap());
        }
        else
        {
            bool ret_code = dib_temp.clone(hdc, *this);

            if (need_pre_multiply)
            {
                dib_temp.per_pixel_operation(
                    0, 0, 
                    _dib_info->bmiHeader.biWidth, 
                    abs(_dib_info->bmiHeader.biHeight), 
                    Yrlib_Pixel_Op_PreMultiply());
//                 dib_temp.pre_multiply();
            }
        }

        BLENDFUNCTION bl_func = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
        HDC hdc_mem = ::CreateCompatibleDC(hdc);

        HBITMAP hbmp_old = (HBITMAP)::SelectObject(hdc_mem, dib_temp.get_hbitmap());

        ::AlphaBlend(
            hdc, 
            x, y, width, height, 
            hdc_mem, 
            srcx, srcy, srcwidth, srcheight, bl_func);

        ::DeleteDC(hdc_mem);
    }
    else
    {
        return draw(hdc, x, y, width, height, srcx, srcy, srcwidth, srcheight);
    }

    return true;
}
