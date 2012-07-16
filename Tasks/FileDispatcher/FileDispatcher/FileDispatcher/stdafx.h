// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <ATLComTime.h>



// 调试信息
#define LOG_DEBUG 1
// 普通信息
#define LOG_INFO 2
// 警告信息
#define LOG_WARNING 4
// 错误信息
#define LOG_ERROR 8
// 默认输出日志等级
#define LOG_DEFAULT_LEVEL 0xff

// 输出调试信息，可以输出参数
void TracePrint(int nLogLevel, LPCTSTR lpstrTrace,...);

// TODO: 在此处引用程序需要的其他头文件



