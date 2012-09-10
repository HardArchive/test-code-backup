// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2E6B6299_78FC_4514_953C_D3F5DA24A99E__INCLUDED_)
#define AFX_STDAFX_H__2E6B6299_78FC_4514_953C_D3F5DA24A99E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define WINVER 0x0400       // Win 9x, NT 4+
#define _WIN32_IE 0x0400    // IE 4+

#include <afx.h>
#include <wininet.h>        // This has to be included here, since IActiveDesktop uses
                            // constants from this header.  See Q196342 for more.
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2E6B6299_78FC_4514_953C_D3F5DA24A99E__INCLUDED_)
