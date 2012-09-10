// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200
//

// TODO: 在此处引用程序需要的其他头文件
#include <atlbase.h>        // 基本的ATL类

#pragma comment(lib, "atlthunk.lib")
#include <atlwin.h>         // ATL窗口类

extern CComModule _Module;  // 全局_Module



#include "resource.h"

//以下四种组合，
//可以实现console和windows模式的混合，可以达到不弹出DOS窗口的效果，也可以达到在Windows程序中向控制台输出printf信息了
#pragma comment( linker, "/subsystem:windows /entry:WinMainCRTStartup" )
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
//
//#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup" )
//#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup" )

