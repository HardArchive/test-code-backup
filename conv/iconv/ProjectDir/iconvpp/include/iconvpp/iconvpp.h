/*
    iconvpp.h ver 0.99

   The iconvpp is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

    author : nanyu, from www.d2school.com
    email : nanyu@sina.com
*/

#include <string>
#include <vector>
#include <exception>

namespace Iconvpp
{
    
struct Utf8Convert
{
    //convert to UCS2 (unicode 2 bytes)
    static bool to_UCS2(std::string const src_utf8, std::vector<unsigned short int>& dst_utf16bytes, size_t const size = std::string::npos);
    //convert from UCS2 (unicode 2 bytes)
    static void from_UCS2(unsigned short int const * const src_ucs2, size_t const size, std::string& dst_utf8str);

    //convert to UC4 (unicode 4 bytes)
    static bool to_UCS4(std::string const src_utf8, std::vector<unsigned int>& dst_utf32bytes, size_t size = std::string::npos);
    //convert from UCS4 (unicode 4 bytes)
    static bool from_UCS4(unsigned int const * const src_ucs4, size_t const size, std::string& dst_utf8str);

    static std::wstring to_wstring(std::string const src_utf8, size_t size = std::string::npos);
    static std::string from_wstring(std::wstring const & src, size_t size = std::string::npos);
};

class bad_convert : public std::exception
{
public:
    bad_convert(char const * descript)
        : _descript(descript)
    {}

    bad_convert(std::string const & descript)
        : _descript(descript)
    {}

    bad_convert()
        : _descript("iconv error: Unexpected.")
    {}

	virtual ~bad_convert() throw()
    {}

	virtual const char* what() const throw()
    {
        return _descript.c_str();
    }
private:
    const std::string _descript;
};

struct Encoding
{
	static std::string gbk_to_utf8 (char const * src, size_t len = (size_t)(-1)) throw (bad_convert);
	static std::string utf8_to_gbk (char const * src, size_t len = (size_t)(-1)) throw (bad_convert);

	static std::string gbk_to_utf8 (const std::string& src) throw (bad_convert);
	static std::string utf8_to_gbk (const std::string& src) throw (bad_convert);

private:
	static std::string convert ( const std::string& src,const std::string fromcode
			, const std::string tocode);
};
    
}
