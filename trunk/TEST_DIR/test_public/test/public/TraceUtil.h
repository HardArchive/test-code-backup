//file TraceUtil.h
/*************************************************************************************************** 
* 1、 Class      ： TraceUtil
* 2、 Version    ： 1.6
* 3、 Description： 在Debug版本下显示的TRACE，UNICODE和多字节版本通用
*				    使用时注意要加名称空间QNA(这是为了避免在和MFC的TRACE发生冲突)
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2012-2-8 13:55:26
* 6、 History    ： 1、2011-12-20 13:54使在Debug下支持用TRACEA、TRACEW版本， 但在release下不起作用
*					2、2012-09-07 15.17使在release下有限支持TRACE(ANSI、UNICODE)
*					3、增加可变长度度定义 通过TRACE_MAX_LEN可指定最大长度避免空间浪费,或缺少空间
*					   增强了容错处理  如果发生溢出在开发阶段就很容易发现
*					   优化了代码采用TCHAR字符串支持ANSI、UNICODE
*					4、增加调试模式时直接输出串到控制台功能  只需定义 RUNINDEBUGMODE宏
*					5、增加打印到文件功能
* 7、 Remark     ： 本函数使用时有三条宏需注意(如不去理会则只起普通TRACE功能<支持ANSI、UNICODE>)：
*					1、USES_RELEASE_TRACE  此宏是在Release下实现TRACE功能
*					2、RUNINDEBUGMODE 此宏是实现将TRACE记录打印到控制台
*					3、TRACE_LOG_FILE_PATH 此宏定义日志保存位置，将记录打印为文本文件
*					注意：				
*					1、以上三条宏只在#include <TraceUtil.h>之前定义才会起作用
*					2、如果定义了RUNINDEBUGMODE宏，则TRACE_LOG_FILE_PATH不起作用
*注：宏定义可以放于命名空间之外，因为其在编译阶段起作用
*	 头文件最好不好放在命名空间里，有时会使STD出现错误
*	 打印日志为文件功能如果有大量日志需打印，最好不要启用因为频繁打开关闭文件会占用大量资源	
*	 此函数只能在C++中使用，C语言不支持命名空间
****************************************************************************************************/ 
#ifndef __TRACE_UTIL_H__
#define __TRACE_UTIL_H__

//windows.h必须包含 因为WCHAR在windows中定义
#ifndef _WINDOWS_
#include <windows.h>
#endif

//如果没有包含tchar则包含
#ifndef _INC_TCHAR
#include <tchar.h>
#endif

//如果没有指定输入字符数组长度则自动指定为1024
#ifndef TRACE_MAX_LEN
#define TRACE_MAX_LEN 0x400   //最大TRACE长度为1024
#endif

//在debug下 USES_RELEASE_TRACE 直接打印日志
#ifdef _DEBUG
#ifndef USES_RELEASE_TRACE
#define USES_RELEASE_TRACE
#endif
#endif

//如果没有定义USES_RELEASE_TRACE则不包含头，也不需打印
#ifdef USES_RELEASE_TRACE
#include <stdio.h>
#include <stdarg.h>
#endif

//如果开启了打印到控制台模式则不允许打印到文件
#ifndef RUNINDEBUGMODE
//如果未定义输出控制台模式且定义了日志保存路径则开启输出文件模式
//如果定义日志保存路径则打印日志到文件
#ifdef TRACE_LOG_FILE_PATH
#ifndef TRACE_LOG_FILE
#define TRACE_LOG_FILE
#endif
#endif 

//如果定义的将日志保存到文件则开启输出到控制来模式
//在此模式下release版本也会保存文件
#ifdef TRACE_LOG_FILE
#ifndef RUNINDEBUGMODE
#define RUNINDEBUGMODE
#endif
#endif 
#endif

//在RUNINDEBUGMODE模式下宽字符输出需要setlocale函数 需要包括locale
#ifdef RUNINDEBUGMODE
#ifndef _INC_LOCALE
#include <locale.h>
#endif
#endif

namespace RG
{
//如果没有定义USES_RELEASE_TRACE则执行空函数
	inline void TRACE(TCHAR* ptszEnter, ...)
	{
#ifdef USES_RELEASE_TRACE
		int iRet = 0;
		va_list args;                            //参数列表		
		TCHAR tszOutput[TRACE_MAX_LEN] = {0};		
#ifdef RUNINDEBUGMODE		
		TCHAR* ptOldLocale = NULL;
#endif
		va_start(args, ptszEnter);               //获取参数列表中的参数

		//_vstprintf _vsctprintf送格式化输出到串中 正常情况下返回生成字串的长度(除去\0),错误情况返回负值		
		iRet = _vsctprintf(ptszEnter, args); 
		if (iRet<0)	throw _T("TRACE _vsctprintf ptszEnter is short!!!\r\n");		
		iRet = _vstprintf_s(tszOutput, ptszEnter, args);	
		
		if (iRet>TRACE_MAX_LEN ||iRet<0)	throw _T("TRACE _vstprintf tszOutput is short!!!\r\n");		

		va_end(args);                           //结束
		//在调试模式下直接输出到控制台
#ifdef RUNINDEBUGMODE
		//在文件日志模式下保存到文件
#ifdef TRACE_LOG_FILE
		FILE* pFile = 0;
		if(!( pFile = _tfreopen(TRACE_LOG_FILE_PATH, _T("a+") , stdout)));
#endif
		ptOldLocale = _tcsdup(_tsetlocale(LC_CTYPE, NULL));  //获取本地语言保存
		_tsetlocale(LC_CTYPE, _T(""));  //C语言的全局locale设置为本地语言,但这会导致cout和wcout不能输出中文
		_tprintf(tszOutput);
		_tsetlocale(LC_CTYPE, ptOldLocale); //将C语言的全局locale恢复
		ptOldLocale = NULL;
#ifdef TRACE_LOG_FILE
		fclose(pFile);
#endif 
#else
		OutputDebugString(tszOutput);
#endif
#endif
	}
}
#endif /*__TRACE_UTIL_H__*/

