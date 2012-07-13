// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件
#include <atlbase.h>
#include <Windows.h>
#include <atlcoll.h>
#include <atlwin.h>
#include <atlstr.h>
//#include <atltrace.h>

//#include "vld.h"
#include "GlobalFunction.h"
#include "DebugTrace.h"
using namespace TRACE_9CPP;



//全局变量
extern char g_szServerIP[32];             //服务器IP
extern char g_szMysqlUserName[MAX_PATH];  //Mysql用户名
extern char g_szMysqlPassword[MAX_PATH];  //Mysql密码 