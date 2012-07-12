#pragma once

//file DebugTrace.h 
/******************************************************************************* 
* 1�� �ļ����ƣ� DebugTrace
* 2�� �桡������ Version *.*
* 3�� ��    ���� ��Debug�汾����ʾ��TRACE��UNICODE�Ͷ��ֽڰ汾ͨ��
				ʹ��ʱע��Ҫ�����ƿռ�TRACE_SYLAR(����Ϊ�˱����ں�MFC��TRACE������ͻ)
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-2-8 13:55:26
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
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