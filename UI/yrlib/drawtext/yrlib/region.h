#pragma once

class Yrlib_Region
{
public:
    HRGN _rgn_handle;

    Yrlib_Region(HRGN handle = NULL)
        : _rgn_handle(handle)
    {
    }

    ~Yrlib_Region()
    {
        if(_managed && _rgn_handle != NULL)
            destroy();
    }

    Yrlib_Region& operator =(HRGN handle)
    {
        attach(handle);
        return *this;
    }

    void attach(HRGN handle)
    {
        if(_managed && _rgn_handle != NULL && _rgn_handle != handle)
            ::DeleteObject(_rgn_handle);

        _rgn_handle = handle;
        _managed = false;
    }

    HRGN detach()
    {
        HRGN handle = _rgn_handle;
        _rgn_handle = NULL;
        return handle;
    }

    operator HRGN() const
    {
        return _rgn_handle;
    }

    bool is_valid() const
    {
        return (_rgn_handle != NULL);
    }

    bool is_null() const
    {
        return (_rgn_handle == NULL);
    }

    HRGN create_rect(int x1, int y1, int x2, int y2)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::CreateRectRgn(x1, y1, x2, y2);
        _managed = true;

        return _rgn_handle;
    }

    HRGN create_rect(LPCRECT rect)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::CreateRectRgnIndirect(rect);
        _managed = true;

        return _rgn_handle;
    }

    HRGN create_elliptic(int x1, int y1, int x2, int y2)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::CreateEllipticRgn(x1, y1, x2, y2);
        _managed = true;

        return _rgn_handle;
    }

    HRGN create_elliptic(LPCRECT rect)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::CreateEllipticRgnIndirect(rect);
        _managed = true;

        return _rgn_handle;
    }

    HRGN create_polygon(LPPOINT points, int count, int mode)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::CreatePolygonRgn(points, count, mode);
        _managed = true;

        return _rgn_handle;
    }

    HRGN create_poly_polygon(LPPOINT points, LPINT poly_counts, int count, int poly_fill_mode)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::CreatePolyPolygonRgn(points, poly_counts, count, poly_fill_mode);
        _managed = true;

        return _rgn_handle;
    }

    HRGN create_round_rect(int x1, int y1, int x2, int y2, int x3, int y3)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
        _managed = true;

        return _rgn_handle;
    }

    HRGN create_from_path(HDC hdc)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::PathToRegion(hdc);
        _managed = true;

        return _rgn_handle;
    }

    HRGN create_from_data(const XFORM* xform, int count, const RGNDATA* data)
    {
        if (is_valid())
            return NULL;

        _rgn_handle = ::ExtCreateRegion(xform, count, data);
        _managed = true;

        return _rgn_handle;
    }

    BOOL destroy()
    {
        BOOL bRet = ::DeleteObject(_rgn_handle);
        if(bRet)
        {
            _rgn_handle = NULL;
            _managed = false;
        }

        return bRet;
    }

    void set_rect(int x1, int y1, int x2, int y2)
    {
        ::SetRectRgn(_rgn_handle, x1, y1, x2, y2);
    }

    void set_rect(LPCRECT rect)
    {
        ::SetRectRgn(_rgn_handle, rect->left, rect->top, rect->right, rect->bottom);
    }

    int combine(HRGN src1, HRGN src2, int mode)
    {
        return ::CombineRgn(_rgn_handle, src1, src2, mode);
    }

    int combine(HRGN src, int mode)
    {
        return ::CombineRgn(_rgn_handle, _rgn_handle, src, mode);
    }

    int copy(HRGN src)
    {
        return ::CombineRgn(_rgn_handle, src, NULL, RGN_COPY);
    }

    BOOL equal(HRGN another) const
    {
        return ::EqualRgn(_rgn_handle, another);
    }

    int offset(int x, int y)
    {
        return ::OffsetRgn(_rgn_handle, x, y);
    }

    int offset(POINT point)
    {
        return ::OffsetRgn(_rgn_handle, point.x, point.y);
    }

    int get_box(LPRECT rect) const
    {
        return ::GetRgnBox(_rgn_handle, rect);
    }

    BOOL point_in(int x, int y) const
    {
        return ::PtInRegion(_rgn_handle, x, y);
    }

    BOOL point_in(POINT point) const
    {
        return ::PtInRegion(_rgn_handle, point.x, point.y);
    }

    BOOL rect_in(LPCRECT rect) const
    {
        return ::RectInRegion(_rgn_handle, rect);
    }

    unsigned long get_data(LPRGNDATA data, unsigned long data_size) const
    {
        return (int)::GetRegionData(_rgn_handle, data_size, data);
    }

    // class do_this
    // {
    // public:
    //     bool operator()(LPRECT rect) const
    //     {
    //         return true; // return false for break
    //     }
    // };
    template<class do_this>
    bool for_each_region_data(do_this& func)
    {
        unsigned long data_size = get_data(NULL, 0);
        if (0 == data_size)
            return false;

        LPRGNDATA rgn_data = (LPRGNDATA)new BYTE[data_size];

        data_size = get_data(rgn_data, data_size);
        if (0 == data_size)
        {
            delete[] (LPBYTE)rgn_data;
            return false;
        }

        unsigned long count = rgn_data->rdh.nCount;
        LPRECT rect = (LPRECT)rgn_data->Buffer;

        for (unsigned long i = 0; i < count; i ++)
        {
            if (!func(&(rect[i])))
                break;
        }

        return true;
    }

private:
    bool _managed;
};
