// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <afx.h>


// TODO: 在此处引用程序需要的其他头文件
//#include <Windows.h>
//#include <WinSock2.h>
// TODO: 在此处引用程序需要的其他头文件
#include "Log.h"
using namespace INTERVISION;

extern CLog_T g_objLog;   //日志类
// 调试信息
#define LOG_DEBUG 1
// 普通信息
#define LOG_INFO 2
// 警告信息
#define LOG_WARNING 4
// 错误信息
#define LOG_ERROR 8
#define LOG_DEFAULT_LEVEL 0xfe

//全局变量
extern char g_szMinitorFolderPah[MAX_PATH];
// 输出调试信息，可以输出参数
void TracePrint(int nLogLevel, LPCTSTR lpstrTrace,...);

