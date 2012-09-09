// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__3FCEF006_09A4_11D4_8D3B_D12F9D3D8B02__INCLUDED_)
#define AFX_STDAFX_H__3FCEF006_09A4_11D4_8D3B_D12F9D3D8B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _WIN32_IE 0x0300

#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <string>
#include <list>
#include <strstream>
typedef std::list<std::basic_string<TCHAR> > string_list;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3FCEF006_09A4_11D4_8D3B_D12F9D3D8B02__INCLUDED)
