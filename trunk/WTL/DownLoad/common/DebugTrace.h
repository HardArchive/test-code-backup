//file DebugTrace.h 
/******************************************************************************* 
* 1�� �ļ����ƣ� DebugTrace
* 2�� �桡������ Version *.*
* 3�� ��    ���� ��Debug�汾����ʾ��TRACE��UNICODE�Ͷ��ֽڰ汾ͨ��
				ʹ��ʱע��Ҫ�����ƿռ�TRACE_9CPP(����Ϊ�˱����ں�MFC��TRACE������ͻ)
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-2-8 13:55:26
* 6�� �� �� �ˣ� ��ë
* 7�� �޸����ڣ� 2012-12-20 13:54
* 8�� �޸����ݣ� 1��ʹ��Debug��֧����TRACEA��TRACEW�汾�� ����release�²�������
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

