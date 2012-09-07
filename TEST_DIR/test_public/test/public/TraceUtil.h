//file TraceUtil.h
/*************************************************************************************************** 
* 1、 Class      ： TraceUtil
* 2、 Version    ： 0.3
* 3、 Description： 在Debug版本下显示的TRACE，UNICODE和多字节版本通用
*				    使用时注意要加名称空间QNA(这是为了避免在和MFC的TRACE发生冲突)
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-2-8 13:55:26
* 6、 History    ： 1、2011-12-20 13:54使在Debug下支持用TRACEA、TRACEW版本， 但在release下不起作用
*					2、2012-09-07 15.17使在release下有限支持TRACE(ANSI、UNICODE)
* 7、 Remark     ： 只有在release下不定义#define USES_RELEASE_TRACE TRACE才会不起作用
*					注意：				
*					#define USES_RELEASE_TRACE 定义需在#include <TraceUtil.h>之前	
*					本来想实现类似于ATL中W2A、A2W一样针对局部的功能但是太麻烦了 ，只能针对宏函数，
*					内联函数跟本实现不了
*					头文件最好不好放在命名空间里，有时会使STD出现错误
****************************************************************************************************/ 
#ifndef __TRACE_UTIL_H_
#define __TRACE_UTIL_H_

//windows.h必须包含 因为WCHAR在windows中定义
#include <windows.h>

//在debug下 USES_RELEASE_TRACE
#ifdef _DEBUG
#ifndef USES_RELEASE_TRACE
#define USES_RELEASE_TRACE
#endif
#endif
//如果没有定义USES_RELEASE_TRACE则不包含头
#ifdef USES_RELEASE_TRACE
#include <stdio.h>
#include <stdarg.h>
#endif

namespace QNA
{
//如果没有定义USES_RELEASE_TRACE则执行空函数
#ifndef UNICODE
	inline void TRACEA(char *fmt,...)
	{
#ifdef USES_RELEASE_TRACE
		char out[1024] = {0};
		va_list body = NULL;
		va_start(body, fmt);
		vsprintf_s(out, fmt, body);
		va_end(body);
		OutputDebugStringA(out);
#endif
	}
#else 
	inline void TRACEW(WCHAR *fmt,...)
	{
#ifdef USES_RELEASE_TRACE
		WCHAR out[1024] = {0};
		va_list body = NULL;
		va_start(body, fmt);
		vswprintf_s(out, fmt, body);
		va_end(body);
		OutputDebugStringW(out);
#endif
	}
#endif 

#ifndef UNICODE
#define TRACE  TRACEA
#else
#define TRACE  TRACEW
#endif 
}
#endif

