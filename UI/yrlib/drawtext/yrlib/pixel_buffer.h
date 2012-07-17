#pragma once

#include <memory.h>
#include "color.h"

class Yrlib_PixelBuffer
{
public:
    Yrlib_PixelBuffer()
        : _managed_buffer(false)
        , _pixel_buffer_data(NULL)
        , _pixel_buffer_data_bytes_used(0)
        , _pixel_buffer_data_bytes_alloc(0)
        , _pixel_buffer_width(0)
        , _pixel_buffer_height(0)
        , _pixel_buffer_scanline_bytes(0)
        , _pixel_buffer_bits_per_pixel(0)
    {
    }

    virtual ~Yrlib_PixelBuffer()
    {
        destroy();
    }

    bool valid()
    {
        return (NULL != _pixel_buffer_data);
    }

    void destroy()
    {
        if (_pixel_buffer_data)
        {
            if (_managed_buffer)
                delete[] _pixel_buffer_data;

            _pixel_buffer_data = NULL;
        }

        _pixel_buffer_data_bytes_used = 0;
        _pixel_buffer_data_bytes_alloc = 0;
        _pixel_buffer_width = 0;
        _pixel_buffer_height = 0;
    }

    inline static size_t get_buffer_scanline_bytes(size_t width, size_t bits_per_pixel = 32)
    {
        return (width * bits_per_pixel + 31) / 32 * 4;
    }

    inline static size_t calc_buffer_bytes(size_t width, size_t height, size_t bits_per_pixel = 32)
    {
        return get_buffer_scanline_bytes(width, bits_per_pixel) * height;
    }

    bool attach(unsigned char * buffer_data, size_t buffer_size, const BITMAPINFOHEADER *header)
    {
        return attach(buffer_data, buffer_size, header->biWidth, abs(header->biHeight), header->biBitCount);
    }

    bool attach(unsigned char * buffer_data, size_t buffer_size, size_t width, size_t height, size_t bits_per_pixel = 32)
    {
        destroy();

        _pixel_buffer_scanline_bytes = get_buffer_scanline_bytes(width, bits_per_pixel);
        _pixel_buffer_data_bytes_used = calc_buffer_bytes(width, height, bits_per_pixel);

        if (buffer_size == _pixel_buffer_data_bytes_used || buffer_size == 0)
            _pixel_buffer_data = buffer_data;

        if (_pixel_buffer_data)
        {
            _pixel_buffer_width = width;
            _pixel_buffer_height = height;
            _pixel_buffer_bits_per_pixel = bits_per_pixel;

            _managed_buffer = false;

            return true;
        }
        else
        {
            destroy();

            return false;
        }
    }

    bool create(size_t width, size_t height, size_t bits_per_pixel = 32)
    {
        _pixel_buffer_data_bytes_used = calc_buffer_bytes(width, height, bits_per_pixel);
        _pixel_buffer_scanline_bytes = get_buffer_scanline_bytes(width, bits_per_pixel);

        _realloc_buffer(_pixel_buffer_data_bytes_used);

        _managed_buffer = true;

        if (_pixel_buffer_data)
        {
            memset(_pixel_buffer_data, 0, _pixel_buffer_data_bytes_used);

            _pixel_buffer_width = width;
            _pixel_buffer_height = height;
            _pixel_buffer_bits_per_pixel = bits_per_pixel;

            return true;
        }
        else
        {
            destroy();

            return false;
        }
    }

    void* get_data(size_t lineno = 0, bool bottom_to_top = true) const
    {
        if (lineno >= _pixel_buffer_height)
            return NULL;

        return _pixel_buffer_data + _pixel_buffer_scanline_bytes * (bottom_to_top ? lineno : _pixel_buffer_height - lineno - 1);
    }

    size_t get_width() const
    {
        return _pixel_buffer_width;
    }

    size_t get_height() const
    {
        return _pixel_buffer_height;
    }

    size_t get_size() const
    {
        return _pixel_buffer_data_bytes_used;
    }

    inline unsigned long get_pixel(void *pixel_data, size_t pixel_add) const
    {
        unsigned long data = 0;

        switch (_pixel_buffer_bits_per_pixel)
        {
        case 1:
            // abcdefgh
            data = (*((unsigned char *)pixel_data) >> (7 - pixel_add)) & 1;
        	break;
        case 2:
            // aabbccdd
            data = (*((unsigned char *)pixel_data) >> (6 - pixel_add * 2)) & 3;
            break;
        case 4:
            // aaaabbbb
            data = (*((unsigned char *)pixel_data) >> (4 - pixel_add * 4)) & 15;
            break;
        case 8:
            data = *((unsigned char *)pixel_data);
            break;
        case 16:
            data = *((unsigned short *)pixel_data);
            break;
        case 24:
            data = *((unsigned long *)pixel_data) & 0xFFFFFF;
            break;
        case 32:
            data = *((unsigned long *)pixel_data);
            break;
        }

        return data;
    }

    inline void set_pixel(void *pixel_data, size_t pixel_add, unsigned long data)
    {
        switch (_pixel_buffer_bits_per_pixel)
        {
        case 1:
            // abcdefgh
            *((unsigned char *)pixel_data) &= 1 << (7 - pixel_add);
            *((unsigned char *)pixel_data) |= (unsigned char)(data & 1) << (7 - pixel_add);
            break;
        case 2:
            // aabbccdd
            *((unsigned char *)pixel_data) &= 3 << (6 - pixel_add * 2);
            *((unsigned char *)pixel_data) |= (unsigned char)(data & 3) << (6 - pixel_add * 2);
            break;
        case 4:
            // aaaabbbb
            *((unsigned char *)pixel_data) &= 15 << (4 - pixel_add * 4);
            *((unsigned char *)pixel_data) |= (unsigned char)(data & 15) << (4 - pixel_add * 4);
            break;
        case 8:
            *((unsigned char *)pixel_data) = (unsigned char)(data & 0xFF);
            break;
        case 16:
            *((unsigned short *)pixel_data) = (unsigned short)(data & 0xFFFF);
            break;
        case 24:
            *((unsigned char *)pixel_data) = (unsigned char)(data);
            *((unsigned char *)pixel_data + 1) = (unsigned char)(data >> 8);
            *((unsigned char *)pixel_data + 2) = (unsigned char)(data >> 16);
            break;
        case 32:
            *((unsigned long *)pixel_data) = data;
            break;
        }
    }

protected:
private:

    bool   _managed_buffer;

    size_t _pixel_buffer_scanline_bytes;
    size_t _pixel_buffer_width;
    size_t _pixel_buffer_height;
    size_t _pixel_buffer_data_bytes_used;
    size_t _pixel_buffer_data_bytes_alloc;
    size_t _pixel_buffer_bits_per_pixel;
    unsigned char * _pixel_buffer_data;

    void _realloc_buffer(size_t new_size)
    {
        if (new_size > _pixel_buffer_data_bytes_alloc)
        {
            if (NULL != _pixel_buffer_data && _managed_buffer)
                delete[] _pixel_buffer_data;
            
            _pixel_buffer_data = new unsigned char[_pixel_buffer_data_bytes_used];
            
            _pixel_buffer_data_bytes_alloc = new_size;
        }
    }
};
