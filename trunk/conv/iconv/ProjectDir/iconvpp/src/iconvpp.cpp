/*
    iconvpp.cpp, ver 0.99

   The iconvpp is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

    author : nanyu, from www.d2school.com
    email : nanyu@sina.com
*/

#include <iterator>
#include <iconv.h>
#include <errno.h>

#include "iconvpp\iconvpp.h"

namespace Iconvpp
{

using namespace std;

struct BitReader
{
    BitReader()
            : byte (0), bitPos (0)
    {}

    BitReader& reset()
    {
        bitPos =
        byte = (unsigned char) (0);

        return *this;
    }

    BitReader& fillzero (unsigned char const count)
    {
        return read( (unsigned char) 0, (unsigned char) 0, count);
    }

    BitReader& fill (unsigned char const count)
    {
        return read( (unsigned char) 0xFF, (unsigned char) 0, count);
    }

    BitReader& read(unsigned char bits, unsigned char beg, unsigned char count)
    {
        for (unsigned char i = 0 ;i < count; ++i)
        {
            byte <<= 1;
            ++bitPos;

            unsigned char bit = ( (bits >> (7 - beg - i)) & 0x01);
            byte |= bit;
        }

        return *this;
    }

    unsigned char getByte() const
    {
        return byte;
    }

private:
    unsigned char byte;
    unsigned char bitPos;
};


inline unsigned short MakeWORD (unsigned char hiByte, unsigned char lowByte)
{
    return ( (static_cast<unsigned short> (hiByte) << 8) & 0xFF00) |
           (static_cast<unsigned short> (lowByte) & 0x00FF);
}

inline unsigned int MakeDWORD (unsigned short hiWord, unsigned short lowWord)
{
    return ( (static_cast<unsigned int> (hiWord) << 16) & 0xFFFF0000) |
           (static_cast<unsigned int> (lowWord) & 0x0000FFFF);
}

inline unsigned char HiBYTE (unsigned short int word)
{
    return static_cast<unsigned char> ( (word >> 8) & 0x00FF);
}

inline unsigned char LowBYTE (unsigned short int word)
{
    return static_cast<unsigned char> (word & 0x00FF);
}

inline unsigned short int HiWORD (unsigned int dword)
{
    return static_cast<unsigned char> ( (dword >> 16) & 0x0000FFFF);
}

inline unsigned short int LowWORD (unsigned int dword)
{
    return static_cast<unsigned char> (dword & 0x0000FFFF);
}

bool Utf8Convert::to_UCS2 (std::string const src_utf8, std::vector<unsigned short int>& dst_utf16bytes, size_t const size)
{
    /*
    UCS-2 range (hex.) UTF-8 octet sequence (binary)
    0000-007F 0xxxxxxx
    0080-07FF 110xxxxx 10xxxxxx
    0800-FFFF 1110xxxx 10xxxxxx 10xxxxxx
    */

    unsigned char c1, c2, c3;
    unsigned char hiByte, lowByte;
    unsigned short bit16;

    BitReader br;

    size_t size_tmp = (size == std::string::npos) ?  src_utf8.size() : size;

    for (size_t i = 0; i < size_tmp; ++i)
    {
        c1 = src_utf8.at (i);

        if (c1 < 0x80)
        {
            //0000-007F <==> 0xxxxxxx
            dst_utf16bytes.push_back (static_cast<unsigned short> (c1) & 0x007F);
        }
        else if ( (c1 & 0xC0) && ! (c1 & 0x20))
        {
            //0080-07FF <==> 110xxx,xx 10xxxxxx
            hiByte = br.reset().read(c1, 3, 3).getByte();

            if (1 + i >= size_tmp)
            {
                return false;
            }

            c2 = src_utf8.at (++i);

            if (! (c2 & 0x80))
            {
                return false;
            }

            lowByte = br.reset().read(c1, 6, 2).read(c2, 2, 6).getByte();

            bit16 = MakeWORD (hiByte, lowByte);
            dst_utf16bytes.push_back (bit16);
        }
        else if ( (c1 & 0xE0) && ! (c1 & 0x10))
        {
            //0800-FFFF <==> 1110xxxx 10xxxx,xx 10xxxxxx
            if (2 + i >= size_tmp)
            {
                return false;
            }

            c2 = src_utf8.at (++i);
            c3 = src_utf8.at (++i);

            if (! (c2 & 0x80) || ! (c3 & 0x80))
            {
                return false;
            }

            hiByte = br.reset().read(c1, 4, 4).read(c2, 2, 4).getByte();
            lowByte = br.reset().read(c2, 6, 2).read(c3, 2, 6).getByte();

            bit16 = MakeWORD (hiByte, lowByte);
            dst_utf16bytes.push_back (bit16);
        }
        else
        {
            return false;
        }
    }

    return true;
}

//convert to UC4 (unicode 4 bytes)
bool Utf8Convert::to_UCS4 (std::string const src_utf8, std::vector<unsigned int>& dst_utf32bytes, size_t size)
{
    /*
    UCS-4 range (hex.) UTF-8 octet sequence (binary)
    0000 0000-0000 007F 0xxxxxxx
    0000 0080-0000 07FF 110xxxxx 10xxxxxx
    0000 0800-0000 FFFF 1110xxxx 10xxxxxx 10xxxxxx

    0001 0000-001F FFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    0020 0000-03FF FFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    0400 0000-7FFF FFFF 1111110x 10xxxxxx ... 10xxxxxx
    */

    BitReader br;
    unsigned char hiByte, lowByte;
    unsigned char c1, c2(0), c3, c4, c5, c6;
    unsigned short hiWord, lowWord;
    unsigned int bit32;

    size_t size_tmp = (size == std::string::npos) ?  src_utf8.size() : size;

    for (size_t i = 0; i < size_tmp; ++i)
    {
        c1 = src_utf8.at (i);

        if (c1 < 0x80)
        {
            //0000 0000-0000 007F <==> 0xxxxxxx
            dst_utf32bytes.push_back (static_cast<unsigned int> (c1) & 0x00007F);
        }
        else if ( (c1 & 0xC0) && ! (c1 & 0x20))
        {
            //0000 0080-0000 07FF <==> 110xxx,xx 10xx,xxxx
            hiByte = br.reset().read(c1, 3, 3).getByte();

            if (1 + i >= size_tmp)
            {
                return false;
            }

            c2 = src_utf8.at (++i);

            if (! (c2 & 0x80))
            {
                return false;
            }

            lowByte = br.reset().read(c1, 6, 2).read(c2, 2, 6).getByte();

            unsigned int bit32 = ( ( (static_cast<unsigned short> (hiByte) << 8) & 0xFF00) |
                                   (static_cast<unsigned short> (lowByte) & 0x00FF)) & 0x0000FFFF;

            dst_utf32bytes.push_back (bit32);
        }
        else if ( (c1 & 0xE0) && ! (c1 & 0x10))
        {
            //0000 0800-0000 FFFF <==> 1110xxxx 10xxxx,xx 10xx,xxxx
            if (2 + i >= size_tmp)
            {
                return false;
            }

            c2 = src_utf8.at (++i);
            c3 = src_utf8.at (++i);

            if (! (c2 & 0x80) || ! (c3 & 0x80))
            {
                return false;
            }

            hiByte = br.reset().read(c1, 4, 4).read(c2, 2, 4).getByte();
            lowByte = br.reset().read(c2, 6, 2).read(c3, 2, 6).getByte();

            lowWord = MakeWORD (hiByte, lowByte);

            bit32 = MakeDWORD ( (unsigned short) (0), lowWord);

            dst_utf32bytes.push_back (bit32);
        }
        else if ( (c1 & 0xF0) && ! (c2 & 0x08))
        {
            //0001 0000-001F FFFF <==> 11110xxx 10xx,xxxx 10xxxx,xx 10xx,xxxx
            if (3 + i >= size_tmp)
            {
                return false;
            }

            c2 = src_utf8.at (++i);
            c3 = src_utf8.at (++i);
            c4 = src_utf8.at (++i);

            if (! (c2 & 0x80) || ! (c3 & 0x80) || ! (c4 & 0x80))
            {
                return false;
            }

            hiByte = 0;
            lowByte = br.reset().read(c1, 5, 3).read(c2, 2, 2).getByte();
            hiWord = MakeWORD (hiByte, lowByte);

            hiByte = br.reset().read(c2, 4, 4).read(c3, 2, 4).getByte();
            lowByte = br.reset().read(c3, 6, 2).read(c4, 2, 6).getByte();
            lowWord = MakeWORD (hiByte, lowByte);

            bit32 = MakeDWORD (hiWord, lowWord);

            dst_utf32bytes.push_back (bit32);
        }
        else if ( (c1 &0xF8) && ! (c1 & 0x04))
        {
            //0020 0000-03FF FFFF <==> 111110xx 10xxxxxx 10xx,xxxx 10xxxx,xx 10xxxxxx
            if (4 + i >= size_tmp)
            {
                return false;
            }

            c2 = src_utf8.at (++i);
            c3 = src_utf8.at (++i);
            c4 = src_utf8.at (++i);
            c5 = src_utf8.at (++i);

            if (! (c2 & 0x80)
                    || ! (c3 & 0x80) || ! (c4 & 0x80) || ! (c5 & 0x80))
            {
                return false;
            }

            hiByte = br.reset().read(c1, 6, 2).getByte();
            lowByte = br.reset().read(c2, 2, 6).read(c3, 2, 2).getByte();
            hiWord = MakeWORD (hiByte, lowByte);

            hiByte = br.reset().read(c3, 4, 4).read(c4, 2, 4).getByte();
            lowByte = br.reset().read(c4, 6, 2).read(c5, 2, 6).getByte();
            lowWord = MakeWORD (hiByte, lowByte);

            bit32 = MakeDWORD (hiWord, lowWord);
            dst_utf32bytes.push_back (bit32);
        }
        else if ( (c1 & 0xFC) && ! (c1 & 0x02))
        {
            //0400 0000-7FFF FFFF <==> 1111110x 10xxxxxx,10xxxxxx 10xx,xxxx 10xxxx,xx 10xxxxxx
            if (5 + i >= size_tmp)
            {
                return false;
            }

            c2 = src_utf8.at (++i);
            c3 = src_utf8.at (++i);
            c4 = src_utf8.at (++i);
            c5 = src_utf8.at (++i);
            c6 = src_utf8.at (++i);

            if (! (c2 & 0x80) || ! (c3 & 0x80)
                    || ! (c4 & 0x80) || ! (c5 & 0x80) || ! (c6 & 0x80))
            {
                return false;
            }

            hiByte = br.reset().read(c1, 7, 1).read(c2, 2, 6).getByte();
            lowByte = br.reset().read(c3, 2, 6).read(c4, 2, 2).getByte();
            hiWord = MakeWORD (hiByte, lowByte);

            hiByte = br.reset().read(c4, 4, 4).read(c5, 2, 4).getByte();
            lowByte = br.reset().read(c5, 6, 2).read(c6, 2, 6).getByte();
            lowWord = MakeWORD (hiByte, lowByte);

            bit32 = MakeDWORD (hiWord, lowWord);
            dst_utf32bytes.push_back (bit32);
        }
        else
        {
            return false;
        }
    }

    return true;
}

void Utf8Convert::from_UCS2 (unsigned short int const * const src_ucs2, size_t const size, std::string& dst_utf8str)
{
    /*
    UCS-2 range (hex.) UTF-8 octet sequence (binary)
    0000-007F 0xxxxxxx
    0080-07FF 110xxx,xx 10xxxxxx
    0800-FFFF 1110xxxx 10xxxx,xx 10xxxxxx
    */

    for (unsigned int i = 0; i < size; ++i)
    {
        unsigned short s = src_ucs2[i];
        unsigned char lowByte = LowBYTE (s);

        if (s <= 0x7F)
        {
            dst_utf8str.push_back (static_cast<char> (lowByte));
            continue;
        }

        unsigned char hiByte = HiBYTE (s);
        char lowChar = static_cast<char> ( (lowByte & 0x3F) | 0x80);

        if (s <= 0x07FF)
        {
            char hiChar = static_cast<char> ( ( (lowByte >> 6) & 0x03) | (hiByte << 2) | 0xC0);

            dst_utf8str.push_back (hiChar);
            dst_utf8str.push_back (lowChar);
        }
        else //if ( s <= 0xffff )
        {
            char middleChar = static_cast<char> ( ( (lowByte >> 6) & 0x03) | ( (hiByte & 0x0F) << 2) | 0x80);
            char hiChar = static_cast<char> ( ( (hiByte >> 4) & 0x0F) | 0xE0);

            dst_utf8str.push_back (hiChar);
            dst_utf8str.push_back (middleChar);
            dst_utf8str.push_back (lowChar);
        }
    }
}

bool Utf8Convert::from_UCS4 (unsigned int const * const src_ucs4, size_t const size, std::string& dst_utf8str)
{
    /*
    UCS-4 range (hex.) UTF-8 octet sequence (binary)
    0000 0000-0000 007F 0xxxxxxx
    0000 0080-0000 07FF 110xxx,xx 10xxxxxx
    0000 0800-0000 FFFF 1110xxxx 10xxxx,xx 10xxxxxx

    0001 0000-001F FFFF 11110xxx 10xx,xxxx 10xxxx,xx 10xxxxxx
    0020 0000-03FF FFFF 111110xx 10,xxxxxx 10xx,xxxx 10xxxx,xx 10xxxxxx
    0400 0000-7FFF FFFF 1111110x 10xxxxxx ... 10xxxxxx
    */

    unsigned int bit32;
    unsigned short int lowWord, hiWord;
    unsigned char lowByte, hiByte;
    unsigned char ch;
    BitReader br;

    for (unsigned int i = 0; i < size; ++i)
    {
        bit32 = src_ucs4[i];
        lowWord = LowWORD (bit32);

        if (bit32 <= 0x0000007F)
        {
            //0000 0000-0000 007F 0xxxxxxx
            dst_utf8str.push_back (static_cast<char> (lowWord & 0x00FF));
        }
        else if (bit32 <= 0x000007FF)
        {
            //0000 0080-0000 07FF <==> -00000111 11111111 <==> 110xxx,xx 10xxxxxx
            hiByte = HiBYTE (lowWord);
            lowByte = LowBYTE (lowWord);

            ch = br.reset().fill (2).fillzero (1).read(hiByte, 5, 3).read(lowByte, 0, 2).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(lowByte, 2, 6).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));
        }
        else if (bit32 <= 0x0000FFFF)
        {
            //0000 0800-0000 FFFF <==> -11111111 11111111 <==> 1110xxxx 10xxxx,xx 10xxxxxx
            hiByte = HiBYTE (lowWord);
            lowByte = LowBYTE (lowWord);

            ch = br.reset().fill (3).fillzero (1).read(hiByte, 0, 4).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(hiByte, 4, 4).read(lowByte, 0, 2).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(lowByte, 2, 6).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));
        }
        else if (bit32 <= 0x001FFFFF)
        {
            //0001 0000-001F FFFF <==> -00000000 00011111 11111111 <==> 11110xxx 10xx,xxxx 10xxxx,xx 10xxxxxx
            hiWord = HiWORD (bit32);
            lowByte = LowBYTE (hiWord);
            hiByte = HiBYTE (lowWord);

            ch = br.reset().fill (4).fillzero (1).read(lowByte, 3, 3).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(lowByte, 6, 2).read(hiByte, 0, 4).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            lowByte = LowBYTE (lowWord);
            ch = br.reset().fill (1).fillzero (1).read(hiByte, 4, 4).read(lowByte, 0, 2).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(lowByte, 2, 6).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));
        }
        else if (bit32 <= 0x03FFFFFF)
        {
            //0020 0000-03FF FFFF <==> -00000011 11111111 11111111 <==> 111110xx 10,xxxxxx 10xx,xxxx 10xxxx,xx 10xxxxxx
            hiWord = HiWORD (bit32);
            hiByte = HiBYTE (hiWord);
            lowByte = LowBYTE (hiWord);

            ch = br.reset().fill (5).fillzero (1).read(hiByte, 6, 2).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(lowByte, 0 , 6).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            hiByte = HiBYTE (lowWord);
            ch = br.reset().fill (1).fillzero (1).read(lowByte, 6, 2).read(hiByte, 0, 4).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            lowByte = LowBYTE (lowWord);
            ch = br.reset().fill (1).fillzero (1).read(hiByte, 4, 4).read(lowByte, 0 , 2).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(lowByte, 2, 6).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));
        }
        else if (bit32 <= 0x7FFFFFFF)
        {
            //0400 0000-7FFF FFFF <==> -01111111 11111111 11111111 <==>1111110x 10xxxxxx, 10xxxxxx 10xx,xxxx 10xxxx,xx 10xxxxxx
            hiWord = HiWORD (bit32);
            hiByte = HiBYTE (hiWord);
            lowByte = LowBYTE (hiWord);

            ch = br.reset().fill (6).fillzero (1).read(hiByte, 1, 1).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(hiByte, 2, 6).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(lowByte, 0, 6).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            hiByte = HiBYTE (lowWord);
            ch = br.reset().fill (1).fillzero (1).read(lowByte, 6, 2).read(hiByte, 0, 4).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            lowByte = LowBYTE (lowWord);
            ch = br.reset().fill (1).fillzero (1).read(hiByte, 4, 4).read(lowByte, 0, 2).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));

            ch = br.reset().fill (1).fillzero (1).read(lowByte, 4, 4).getByte();
            dst_utf8str.push_back (static_cast<char> (ch));
        }
        else
        {
            return false;
        }
    }

    return true;
}

std::wstring Utf8Convert::to_wstring(std::string const src, size_t size)
{
    std::wstring dst;
    
#if defined(_WIN32_) || defined(__MINGW32__) || defined(_CYGWIN_)
    #define Utf8Convert_TO_UCS(S, D, C) to_UCS2(S, D, C)
    std::vector<unsigned short int> dst_v;
#else
    #define Utf8Convert_TO_UCS(S, D, C) to_UCS4(S, D, C)
    std::vector<unsigned int> dst_v;
#endif

    if (size == std::string::npos)
    {
        size = src.size();
    }

    bool success = Utf8Convert_TO_UCS(src, dst_v, size);
    #undef Utf8Convert_TO_UCS
    
    if (success)
    {
        dst.append(dst_v.begin(), dst_v.end());
    }
        
    return dst;
}

std::string Utf8Convert::from_wstring(std::wstring const & src, size_t size)
{
    std::string dst;
    
#if defined(_WIN32_) || defined(__MINGW32__) || defined(_CYGWIN_)
    #define Utf8Convert_FROM_UCS(S, D, C) from_UCS2(S, D, C)
    std::vector<unsigned short int> src_v(src.begin(), src.end());
#else
    #define Utf8Convert_FROM_UCS(S, D, C) from_UCS4(S, D, C)
    std::vector<unsigned int> src_v(src.begin(), src.end());
#endif

    if (size == std::string::npos)
    {
        size = src.size();
    }

    Utf8Convert_FROM_UCS(&(src_v[0]), size, dst);

    #undef Utf8Convert_FROM_UCS
    
    return dst;
}


string Encoding::gbk_to_utf8 (char const * src, size_t len) throw (bad_convert)
{
    if (!src || !*src)
    {
        return "";
    }
    
    string ss;
    
    if (len == (size_t)(-1))
    {
        ss = src;
    }
    else
    {
        ss = string(src, src + len);
    }
    
    return gbk_to_utf8(ss);
}

string Encoding::utf8_to_gbk (char const * src, size_t len) throw (bad_convert)
{
    if (!src || !*src)
    {
        return "";
    }
    
    string ss;
    
    if (len == (size_t)(-1))
    {
        ss = src;
    }
    else
    {
        ss = string(src, src + len);
    }
    
    return utf8_to_gbk(ss);
}

string Encoding::gbk_to_utf8 (const string& src) throw (bad_convert)
{
    try
    {
        return convert(src, "GBK", "UTF-8");
    }
    catch (const bad_convert & e)
    {
        throw;
    }
    catch (...)
    {
        throw bad_convert();
    }
}

string Encoding::utf8_to_gbk (const string& src) throw (bad_convert)
{
    try
    {
        return convert(src, "UTF-8", "GBK");
    }
    catch (const bad_convert & e)
    {
        throw;
    }
    catch (...)
    {
        throw bad_convert();
    }
}

string Encoding::convert(const string& src
                         , const string fromcode, const string tocode)
{
    char buff[1024] = "";

    #if _LIBICONV_VERSION >= 0x0109
    const char * ps = src.c_str();
    #else
    char *ps = const_cast<char*>(src.c_str());
    #endif

    size_t len1=strlen(ps),len2;

    int r;

    char *pd;
    string result;

    do
    {
        pd = buff;
        len2=sizeof(buff);

        iconv_t cd=iconv_open(tocode.c_str(),fromcode.c_str());

        if (cd == (void *)-1)
        {
            string s=0;
            if (errno==EINVAL)
                s="iconv_open error: The conversion from fromcode to tocode is not supported by the implementation.";
            else if (errno==EMFILE)
                s="iconv_open error: The number of file descriptors specified by the OPEN_MAX configuration variable is currently open in the calling process.";
            else if (errno==ENFILE)
                s="iconv_open error: Too many files are currently open in the system.";
            else if (errno==ENOMEM)
                s="iconv_open error: Insufficient storage space is available.";
            else
                s="iconv_open error: Unexpected.";

            throw bad_convert(s);
        }
        
        r = iconv(cd, &ps, &len1, &pd, &len2);
        iconv_close(cd);

        if (r == -1 && E2BIG != errno )
        {
            string s;
            if (errno==EILSEQ)
                s="iconv error: An invalid multibyte sequence has been encountered in the input.";
            else if (errno==EINVAL)
                s="iconv error: An incomplete multibyte sequence has been encountered in the input.";
            else
                s="iconv error: Unexpected.";

			throw bad_convert(s);
        }

        int count = sizeof(buff) - len2;

        result.append(buff, buff+count);

    }
    while (len1 > 0);

    return result;
}

}
