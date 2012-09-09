// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__A4C13E28_64BA_406B_8B8D_D98AF851C916__INCLUDED_)
#define AFX_STDAFX_H__A4C13E28_64BA_406B_8B8D_D98AF851C916__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER          0x0500
#define _WIN32_WINNT    0x0500
#define _WIN32_IE       0x0500

#define _ATL_APARTMENT_THREADED

// MFC
#include <afxwin.h>
#include <afxdisp.h>
#include <afxole.h>

// ATL
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atlconv.h>

// Win32
#include <comdef.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A4C13E28_64BA_406B_8B8D_D98AF851C916__INCLUDED)
