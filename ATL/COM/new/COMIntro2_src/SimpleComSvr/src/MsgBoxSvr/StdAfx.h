// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__895FC33E_A2FA_45BD_898E_81F79E700E1B__INCLUDED_)
#define AFX_STDAFX_H__895FC33E_A2FA_45BD_898E_81F79E700E1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <tchar.h>
#include <comdef.h>

extern HINSTANCE g_hinstThisDll;        // Our DLL's HINSTANCE
extern UINT      g_uDllLockCount;       // The module lock count

#ifdef _DEBUG
void TRACE (LPCTSTR lpszFormat, ...);
#else
inline void TRACE(LPCTSTR x, ...) {}
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__895FC33E_A2FA_45BD_898E_81F79E700E1B__INCLUDED_)
