// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <afx.h>
#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )


// TODO: 在此处引用程序需要的其他头文件

// 调试信息
#define LOG_DEBUG 1
// 普通信息
#define LOG_INFO 2
// 警告信息
#define LOG_WARNING 4
// 错误信息
#define LOG_ERROR 8
#define LOG_DEFAULT_LEVEL 0xfe

// 输出调试信息，可以输出参数
void TracePrint(int nLogLevel, LPCTSTR lpstrTrace,...);