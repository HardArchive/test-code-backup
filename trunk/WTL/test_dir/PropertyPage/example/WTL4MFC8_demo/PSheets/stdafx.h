// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__37509651_D12F_4198_9036_2992439EC282__INCLUDED_)
#define AFX_STDAFX_H__37509651_D12F_4198_9036_2992439EC282__INCLUDED_

// Change these values to use different versions
#define WINVER          0x0500
#define _WIN32_WINNT    0x0500
#define _WIN32_IE       0x0501
#define _RICHEDIT_VER   0x0100

#define _WTL_USE_CSTRING
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS

#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>

#include <atlframe.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlddx.h>

#define countof(x) (sizeof(x)/sizeof((x)[0]))

// Pages send this to their parent sheet to center the sheet
#define UWM_CENTER_SHEET WM_APP

#if _ATL_VER < 0x0700
#undef BEGIN_MSG_MAP
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x)
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__37509651_D12F_4198_9036_2992439EC282__INCLUDED_)
