#pragma once

//file DebugTrace.h 
/******************************************************************************* 
* 1、 文件名称： DebugTrace
* 2、 版　　本： Version *.*
* 3、 描    述： 在Debug版本下显示的TRACE，UNICODE和多字节版本通用
				使用时注意要加名称空间TRACE_SYLAR(这是为了避免在和MFC的TRACE发生冲突)
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-2-8 13:55:26
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 
#ifndef __DEBUGTRACE_HEADER_
#define __DEBUGTRACE_HEADER_
namespace TRACE_9CPP
{
#ifdef _DEBUG
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
inline void _trace(TCHAR *fmt,...)
{
#ifdef UNICODE
	WCHAR out[1024];
#else
	char out[1024];
#endif
	va_list body;
	va_start(body,fmt);
#ifdef UNICODE
	vswprintf_s(out,fmt,body);
#else
	vsprintf_s(out,fmt,body);
#endif
	va_end(body);
#ifdef UNICODE
	OutputDebugStringW(out);
#else
	OutputDebugStringA(out);
#endif
}
#define TRACE _trace
#else
inline void _trace(LPCTSTR fmt,...){}
#define TRACE 1 ? (void)0 : _trace
#endif
}
#endif