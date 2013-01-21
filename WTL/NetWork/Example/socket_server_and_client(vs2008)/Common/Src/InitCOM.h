/******************************************************************************
Module:  initcom.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: 用于简单封装CoInitializeEx和CoUninitialize.
Desc:
******************************************************************************/
#pragma once

#ifndef _WIN32_WINNT			//允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0500		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif	

#ifndef _AFX
	#include <windows.h>
	#include <objbase.h>
#endif

class CInitCOM
{
public:
	CInitCOM(DWORD dwCoInit = COINIT_APARTMENTTHREADED, void * pvReserved = NULL) {::CoInitializeEx(pvReserved, dwCoInit);}
	~CInitCOM() {::CoUninitialize();}
};
