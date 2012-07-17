#pragma once

#include <memory.h>

class Yrlib_PixelBuffer
{
public:
    Yrlib_PixelBuffer()
        : _pixel_buffer_data(NULL)
        , _pixel_buffer_data_bytes(0)
        , _pixel_buffer_width(0)
        , _pixel_buffer_height(0)
        , _pixel_buffer_scanline_bytes(0)
    {
    }

    virtual ~Yrlib_PixelBuffer()
    {
        clear();
    }

    bool valid()
    {
        return (NULL != _pixel_buffer_data);
    }

    void clear()
    {
        if (_pixel_buffer_data)
        {
            if (_managed_buffer)
                delete[] _pixel_buffer_data;

            _pixel_buffer_data = NULL;
        }

        _pixel_buffer_data_bytes = 0;
        _pixel_buffer_width = 0;
        _pixel_buffer_height = 0;
    }

    inline static size_t get_buffer_scanline_bytes(size_t width, size_t bytes_per_pixel = 4)
    {
        return (width * bytes_per_pixel + 3) / 4 * 4;
    }

    inline static size_t calc_buffer_bytes(size_t width, size_t height, size_t bytes_per_pixel = 4)
    {
        return get_buffer_scanline_bytes(width, bytes_per_pixel) * height;
    }

    bool attach(unsigned char * buffer_data, size_t buffer_size, size_t width, size_t height, size_t bytes_per_pixel = 4)
    {
        clear();

        _pixel_buffer_scanline_bytes = get_buffer_scanline_bytes(width, bytes_per_pixel);
        _pixel_buffer_data_bytes = calc_buffer_bytes(width, height, bytes_per_pixel);

        if (buffer_size == _pixel_buffer_data_bytes)
            _pixel_buffer_data = buffer_data;

        if (_pixel_buffer_data)
        {
            memset(_pixel_buffer_data, 0, _pixel_buffer_data_bytes);

            _pixel_buffer_width = width;
            _pixel_buffer_height = height;

            _managed_buffer = false;

            return true;
        }
        else
        {
            clear();

            return false;
        }
    }

    bool create(size_t width, size_t height, size_t bytes_per_pixel = 4)
    {
        _pixel_buffer_data_bytes = calc_buffer_bytes(width, height, bytes_per_pixel);
        _pixel_buffer_data = new unsigned char[_pixel_buffer_data_bytes];

        if (!attach(_pixel_buffer_data, _pixel_buffer_data_bytes, width, height, bytes_per_pixel))
            return false;

        _managed_buffer = true;

        return true;
    }

    void* get_data(size_t lineno = 0)
    {
        if (lineno >= _pixel_buffer_height)
            return NULL;

        return _pixel_buffer_data + _pixel_buffer_scanline_bytes * (_pixel_buffer_height - lineno - 1);
    }

    size_t get_width()
    {
        return _pixel_buffer_width;
    }

    size_t get_height()
    {
        return _pixel_buffer_height;
    }

protected:
private:

    bool   _managed_buffer;

    size_t _pixel_buffer_scanline_bytes;
    size_t _pixel_buffer_width;
    size_t _pixel_buffer_height;
    size_t _pixel_buffer_data_bytes;
    unsigned char * _pixel_buffer_data;
};
