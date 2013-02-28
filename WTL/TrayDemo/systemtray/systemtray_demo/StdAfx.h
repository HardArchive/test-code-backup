// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DB628087_D595_11D1_97E7_00A0243D1382__INCLUDED_)
#define AFX_STDAFX_H__DB628087_D595_11D1_97E7_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500    // enable shell v5 features
#elif _WIN32_IE < 0x0500
#undef _WIN32_IE
#define _WIN32_IE 0x0500    // enable shell v5 features
#endif

// NOTE: Take out these lines if you have the latest Platform SDK.  If you
// just drop in the new shellapi.h, you will need to leave them here to
// have DWORD_PTR defined.
#ifndef DWORD_PTR
#define DWORD_PTR DWORD
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DB628087_D595_11D1_97E7_00A0243D1382__INCLUDED_)
