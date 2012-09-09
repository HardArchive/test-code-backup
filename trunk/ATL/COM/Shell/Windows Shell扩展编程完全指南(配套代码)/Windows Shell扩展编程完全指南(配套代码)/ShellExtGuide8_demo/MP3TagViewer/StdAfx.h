// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__2E855542_6BBC_4291_8DC2_9B9B2D1C2A7B__INCLUDED_)
#define AFX_STDAFX_H__2E855542_6BBC_4291_8DC2_9B9B2D1C2A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#define _ATL_APARTMENT_THREADED

#define WINVER       0x0500     // W2K/98
#define _WIN32_WINNT 0x0500     // W2K
#define _WIN32_IE    0x0500     // IE 5+

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

struct CID3v1Tag    // NOTE that strings are fixed-length and might not be 0-terminated!
{
    char szTag[3];
    char szTitle[30];
    char szArtist[30];
    char szAlbum[30];
    char szYear[4];
    char szComment[30];
    char byGenre;
};

#include <string>
#include <list>
typedef std::basic_string<TCHAR> tstring;   // a TCHAR string

struct CID3CacheEntry
{
    tstring   sFilename;
    CID3v1Tag rTag;
};

typedef std::list<CID3CacheEntry> list_ID3Cache;

#define countof(x) (sizeof(x)/sizeof((x)[0]))

#ifdef DEFAULT_UNREACHABLE
#undef DEFAULT_UNREACHABLE  // nuke the lame MS version
#endif
#ifndef _DEBUG
    #if _MSC_VER > 1100
        #define DEFAULT_UNREACHABLE default: __assume(0); break
    #else
        #define DEFAULT_UNREACHABLE default: break
    #endif
#elif defined(ASSERT)
#define DEFAULT_UNREACHABLE default: ASSERT(0); break   // use MFC assert
#else
#define DEFAULT_UNREACHABLE default: _ASSERTE(0); break // use CRT assert
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2E855542_6BBC_4291_8DC2_9B9B2D1C2A7B__INCLUDED)
