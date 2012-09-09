// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include <atlstr.h>
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS
#include <atldlgs.h>
#include <atlcoll.h>

#undef _WTL_NO_WTYPES
#define _WTL_NO_CSTRING
#include <atlmisc.h>
#include <atlpath.h>
#include <atlddx.h>
#include <atlctrlw.h>
#include <atlctrlx.h>


//#include "..\inc\ArtFrameImpl.h"
#include "MainNotifyObject.h"


extern CMainNotifyObject *g_MainnObject;
