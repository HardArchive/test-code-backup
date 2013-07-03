#pragma once
/***************************************************************************************************
* 1、 File       ： ExpRptHandler
* 2、 Version    ： *.*
* 3、 Description： 让程序崩溃时自动创建minidump 
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-6-13 14:58:58
* 6、 History    ： 
* 7、 Remark     ： minidump的类，然后在类的构造函数中调用SetUnhandledExceptionFilter 来设置异常处理函数。
*                   最后的效果就是只需要我们加入事先写好的.h和.cpp文件，然后什么也不用做，直接编译运行就能达到理想的效果了。
****************************************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <DbgHelp.h>
#include <tchar.h>

#pragma comment(lib, "DbgHelp.lib")

class CExpRptHandler
{
public:
	CExpRptHandler(void);
	~CExpRptHandler(void);

	static LONG WINAPI ExpFilter(PEXCEPTION_POINTERS pExceptionInfo );

	static TCHAR m_tszdmpFileName[MAX_PATH*2];
	static LPTOP_LEVEL_EXCEPTION_FILTER m_pFrontFilter;
};

extern CExpRptHandler g_clsExpRptHandler;