//file TraceUtil.h
/*************************************************************************************************** 
* 1�� Class      �� TraceUtil
* 2�� Version    �� 0.3
* 3�� Description�� ��Debug�汾����ʾ��TRACE��UNICODE�Ͷ��ֽڰ汾ͨ��
*				    ʹ��ʱע��Ҫ�����ƿռ�QNA(����Ϊ�˱����ں�MFC��TRACE������ͻ)
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-2-8 13:55:26
* 6�� History    �� 1��2011-12-20 13:54ʹ��Debug��֧����TRACEA��TRACEW�汾�� ����release�²�������
*					2��2012-09-07 15.17ʹ��release������֧��TRACE(ANSI��UNICODE)
* 7�� Remark     �� ֻ����release�²�����#define USES_RELEASE_TRACE TRACE�Ż᲻������
*					ע�⣺				
*					#define USES_RELEASE_TRACE ��������#include <TraceUtil.h>֮ǰ	
*					������ʵ��������ATL��W2A��A2Wһ����Ծֲ��Ĺ��ܵ���̫�鷳�� ��ֻ����Ժ꺯����
*					������������ʵ�ֲ���
*					ͷ�ļ���ò��÷��������ռ����ʱ��ʹSTD���ִ���
****************************************************************************************************/ 
#ifndef __TRACE_UTIL_H_
#define __TRACE_UTIL_H_

//windows.h������� ��ΪWCHAR��windows�ж���
#include <windows.h>

//��debug�� USES_RELEASE_TRACE
#ifdef _DEBUG
#ifndef USES_RELEASE_TRACE
#define USES_RELEASE_TRACE
#endif
#endif
//���û�ж���USES_RELEASE_TRACE�򲻰���ͷ
#ifdef USES_RELEASE_TRACE
#include <stdio.h>
#include <stdarg.h>
#endif

namespace QNA
{
//���û�ж���USES_RELEASE_TRACE��ִ�пպ���
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

