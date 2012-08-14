//file DebugTrace.h 
/******************************************************************************* 
* 1、 文件名称： DebugTrace
* 2、 版　　本： Version *.*
* 3、 描    述： 在Debug版本下显示的TRACE，UNICODE和多字节版本通用
				使用时注意要加名称空间TRACE_9CPP(这是为了避免在和MFC的TRACE发生冲突)
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-2-8 13:55:26
* 6、 修 改 人： 阿毛
* 7、 修改日期： 2012-12-20 13:54
* 8、 修改内容： 1、使在Debug下支持用TRACEA、TRACEW版本， 但在release下不起作用
********************************************************************************/ 
#ifndef __DEBUGTRACE_HEADER_
#define __DEBUGTRACE_HEADER_
namespace QNA
{
#ifdef _DEBUG
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

	inline void TRACEA(char *fmt,...)
	{
		char out[1024];
		va_list body;
		va_start(body,fmt);
		vsprintf_s(out,fmt,body);
		va_end(body);
		OutputDebugStringA(out);
	}

	inline void TRACEW(WCHAR *fmt,...)
	{
		WCHAR out[1024];
		va_list body;
		va_start(body,fmt);
		vswprintf_s(out,fmt,body);
		va_end(body);
		OutputDebugStringW(out);
	}

#ifdef UNICODE
#define TRACE  TRACEW
#else
#define TRACE  TRACEA
#endif 

#else
	inline void TRACEA(char *fmt,...){}	
	inline void TRACEW(WCHAR *fmt,...){}

#ifdef UNICODE
#define TRACE  TRACEW
#else
#define TRACE  TRACEA
#endif 

#endif
}
#endif

