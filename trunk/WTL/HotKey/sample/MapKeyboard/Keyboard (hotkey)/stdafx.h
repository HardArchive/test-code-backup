// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__00F51DA6_2D5A_434C_9D6B_F240E9B629FA__INCLUDED_)
#define AFX_STDAFX_H__00F51DA6_2D5A_434C_9D6B_F240E9B629FA__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0500
#define _RICHEDIT_VER	0x0100

#define _WTL_FORWARD_DECLARE_CSTRING
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlmisc.h> // for CString
#include <atlctrlx.h> // CHyperLink

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__00F51DA6_2D5A_434C_9D6B_F240E9B629FA__INCLUDED_)
