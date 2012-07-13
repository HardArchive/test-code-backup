// 模版制作：天下我最笨 (QQ:272922032)
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200
#define _CRT_SECURE_NO_WARNINGS		// 禁用 C4996 警告

#include <atlbase.h>
#include <atlwin.h>
#include <atlstr.h>
#include <atltrace.h>

#define _WTL_NO_CSTRING

#include <wtl/atlapp.h>
#include <wtl/atlframe.h>
#include <wtl/atlctrls.h>
#include <wtl/atlctrlx.h>
#include <wtl/atldlgs.h>
#include <wtl/atlcrack.h>
#include <wtl/atlmisc.h>

#include <tinyxml/tinyxml.h>
#include <bkwin/bkdlgview.h>
#include <bkres/bkres.h>