// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>



// TODO: 在此处引用程序需要的其他头文件
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#include <ATLComTime.h>

#pragma warning (disable:4996)  //全部关闭 strcpy
#pragma warning (disable:4800)  //BOOL  bool
//#pragma warning (once:4996)  //仅显示一个
//#define RUNINDEBUGMODE
#include "../global/TraceUtil.h"
using namespace RG;