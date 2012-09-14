//file TraceUtil.h
/*************************************************************************************************** 
* 1、 Class      ： TraceUtil
* 2、 Version    ： 0.5
* 3、 Description： 在Debug版本下显示的TRACE，UNICODE和多字节版本通用
*				    使用时注意要加名称空间QNA(这是为了避免在和MFC的TRACE发生冲突)
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-2-8 13:55:26
* 6、 History    ： 1、2011-12-20 13:54使在Debug下支持用TRACEA、TRACEW版本， 但在release下不起作用
*					2、2012-09-07 15.17使在release下有限支持TRACE(ANSI、UNICODE)
*					3、增加可变长度度定义 通过TRACE_MAX_LEN可指定最大长度避免空间浪费,或缺少空间
*					   增强了容错处理  如果发生溢出在开发阶段就很容易发现
*					   优化了代码采用TCHAR字符串支持ANSI、UNICODE
*					4、增加调试模式时直接输出串到控制台功能  只需定义 RUNINDEBUGMODE宏
* 7、 Remark     ： 只有在release下不定义#define USES_RELEASE_TRACE TRACE才会不起作用
*					注意：				
*					1、#define USES_RELEASE_TRACE 定义需在#include <TraceUtil.h>之前	
*					本来想实现类似于ATL中W2A、A2W一样针对局部的功能但是太麻烦了 ，只能针对宏函数，
*					内联函数跟本实现不了
*					2、头文件最好不好放在命名空间里，有时会使STD出现错误
*					3、调试模式需在在#include <TraceUtil.h>之前定义RUNINDEBUGMODE
****************************************************************************************************/ 
#ifndef __TRACE_UTIL_H__
#define __TRACE_UTIL_H__


//调试模式时直接输出串到控制台
//#define RUNINDEBUGMODE

//windows.h必须包含 因为WCHAR在windows中定义
#ifndef _WINDOWS_
#include <windows.h>
#endif
//如果没有包含tchar则包含
#ifndef _INC_TCHAR
#include <tchar.h>
#endif
#include <locale>
namespace QNA
{

//如果没有指定输入字符数组长度则自动指定为1024
#ifndef TRACE_MAX_LEN
#define TRACE_MAX_LEN 0x400   //最大TRACE长度为1024
#endif

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

//在RUNINDEBUGMODE模式下宽字符输出需要setlocale函数 需要包括locale
#ifdef RUNINDEBUGMODE
#ifndef _INC_LOCALE
#include <locale>
#endif
#endif

//如果没有定义USES_RELEASE_TRACE则执行空函数
	inline void TRACE(TCHAR* ptszEnter, ...)
	{
#ifdef USES_RELEASE_TRACE
		int iRet = 0;
		va_list args;                            //参数列表
		TCHAR tszOutput[TRACE_MAX_LEN] = {0};

		va_start(args, ptszEnter);               //获取参数列表中的参数

		//_vstprintf _vsctprintf送格式化输出到串中 正常情况下返回生成字串的长度(除去\0),错误情况返回负值		
		iRet = _vsctprintf(ptszEnter, args); 
		if (iRet<0)	throw _T("TRACE _vsctprintf ptszEnter is short!!!\r\n");		
		iRet = _vstprintf_s(tszOutput, ptszEnter, args);	
		
		if (iRet>TRACE_MAX_LEN ||iRet<0)	throw _T("TRACE _vstprintf tszOutput is short!!!\r\n");		

		va_end(args);                           //结束
		//在调试模式下直接输出到控制台
#ifdef RUNINDEBUGMODE
		setlocale(LC_CTYPE, "");  //C语言的全局locale设置为本地语言,但这会导致cout和wcout不能输出中文
		_tprintf(tszOutput);
		setlocale(LC_CTYPE, "C"); //将C语言的全局locale恢复
#else
		OutputDebugStr(tszOutput);
#endif
#endif
	}
}
#endif /*__TRACE_UTIL_H__*/

