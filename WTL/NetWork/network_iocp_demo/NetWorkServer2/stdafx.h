// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

/***************************************************************************************************
因为Windows.h中有以下语句：
#ifndef _MAC
#include <winperf.h>
#include <winsock.h>
#endif
所以当包含顺序不当(windows.h先于winsock2.h)就会出现很多莫名其妙的错误
#define WIN32_LEAN_AND_MEAN这条语句，它能够将上面的语句包含winsock.h的语句消减掉
****************************************************************************************************/
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>


//#include <atlbase.h>
//#include <atlconv.h>

// TODO: 在此处引用程序需要的其他头文件
#include "../common/com_module/GeneralHelper.h"
