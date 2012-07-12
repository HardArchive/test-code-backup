// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

#include <tchar.h>

// TODO: 在此处引用程序需要的其他头文件
#include <wininet.h>
#include <UrlHist.h>
#include <Shlwapi.h>
#include <ShlObj.h>
#include <shobjidl.h>

//#pragma Wininet.lib 
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Wininet.lib")

typedef struct INK_INFO
{
	WCHAR wszPath[MAX_PATH];                //绝对路径
	WCHAR wszUrlName[MAX_PATH];             //网页名
	WCHAR wszUrl[2*MAX_PATH];               //网址

}INKINFO, *PINKINFO;