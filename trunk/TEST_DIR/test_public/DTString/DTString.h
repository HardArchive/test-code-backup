//////////////////////////////////////////////////////////////////////////
// Project:			DTString
// FileNme:			DTString.cpp
// Description:		DTString is a String using the standard C++ coding of the class,
//				in order to be compatible with the MFC code
// Change Time:		2012-2-9 14:13:38
// Code by ccnyou
//
// Change Time:		2012-2-11
// Change Log :		for add some method by ccnyou such as operator +(string)
// Change Time:		2012-2-24 0:49:38
// Change Log :		for add method ReverseFind and explain by ccnyou
//
// Change Log:      2012-2-28
//                  change macro _MACOS => __APPLE_CC__
//////////////////////////////////////////////////////////////////////////


#ifndef _DTSTRING_H
#define _DTSTRING_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdarg.h>

using namespace std;

#ifndef _WINDODW_
//windows.h isn't included
typedef const char* LPCSTR,*LPCTSTR;
typedef char DTCHAR;
#endif

#ifdef WIN32
//Windows
#define snprintf	_snprintf
#define vsnprintf	_vsnprintf
#endif

#define MAX_FORMAT_BUFFER	1024

#ifdef __APPLE_CC__
#import <Foundation/Foundation.h>
typedef NSString *PCNSSTRING;
#endif

class DTString
{
private:
	string m_string;
public:
	void Format( LPCTSTR lpszFormat, ... );
	DTString(const char *pStr);
	DTString();
	~DTString();

	int GetLength();

	int Find( char ch, int nStart = 0) const;
	int Find( LPCSTR pstr, int nStart = 0) const;

	int ReverseFind(char ch) const;
	/*
	*Parameters:
	ch
	The character to search for
	*Return Value£º
	The zero-based index of the last character in this DTString object that matches the
	requested character, or ¨C1 if the character is not found
	*Remarks£º
	The function is similar to the run-time function strrchr.
	*/

	DTString Right( int nCount ) const;
	DTString Left( int nCount ) const;
	/*
	*Parameters:
	nCount
	The number of characters to extract from this DTString object.
	*Return Value£º
	A DTString object containing a copy of the specified range of characters. Note that the returned DTString object may be empty.
	*/

	const DTString &operator = (const char *pStr);
	const DTString &operator += (const char *pStr);
    friend DTString operator+(const DTString &str1,const char *pStr);

	const DTString &operator = (const DTString &Str);
	const DTString &operator+=(const DTString &Str);
	friend DTString operator+(const DTString &Str1,const DTString &Str2);

	const DTString &operator = (const string &Str);
	const DTString &operator += (const string &Str);
    friend DTString operator + (const DTString &Str1,const string &Str);


#ifdef __APPLE_CC__

    DTString(NSString *Str);
    DTString &LoadNSString(NSString *str);
    DTString &operator = (NSString *Str);
    NSString *ns_str() const;

	operator PCNSSTRING() const;

#endif

	operator LPCSTR() const;

	friend istream &operator >>(istream &is,DTString &str);
	friend ostream &operator <<(ostream &os,DTString &str);

};




#endif
