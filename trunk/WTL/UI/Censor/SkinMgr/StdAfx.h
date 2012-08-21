// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

#include <Windows.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlapp.h>
#include <atlgdi.h>
#include <atltrace.h>

#include <crtdbg.h>

#ifndef ASSERT
#define ASSERT	_ASSERT
#endif

// TODO: 在此处引用程序需要的其他头文件

#define SKINMGR_MSG_HANDLER(msg, func) \
	if (uMsg == msg) \
	{ \
		return func(uMsg, wParam, lParam); \
	}