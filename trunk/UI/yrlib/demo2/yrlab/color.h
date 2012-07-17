#pragma once

#include <pshpack1.h>

class Yrlib_Color
{
public:
    Yrlib_Color()
    {
        _value = 0;
    }

    Yrlib_Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
    {
        alpha   = a;
        red     = r;
        green   = g;
        blue    = b;
    }

    Yrlib_Color(const Yrlib_Color& clr)
    {
        _value = clr._value;
    }

#ifdef COLORREF
    Yrlib_Color(COLORREF clr)
    {
        clr_ref = clr;
        alpha   = 255;
    }
#endif

#ifdef ARGB
    Yrlib_Color(ARGB clr)
    {
        clr_argb = clr;
    }
#endif

    union {
        struct { 
            unsigned char blue;
            unsigned char green;
            unsigned char red;
            unsigned char alpha;
        };
        unsigned long _value;
#ifdef COLORREF
        COLORREF clr_ref;
#endif
#ifdef ARGB
        ARGB clr_argb;
#endif
    };

    void set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
    {
        _value = Yrlib_Color(a, r, g, b)._value;
    }

    void set_color(const Yrlib_Color& clr)
    {
        _value = clr._value;
    }
};

#include <poppack.h>
