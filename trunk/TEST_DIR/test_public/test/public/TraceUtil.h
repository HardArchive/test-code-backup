//file TraceUtil.h
/*************************************************************************************************** 
* 1�� Class      �� TraceUtil
* 2�� Version    �� 0.5
* 3�� Description�� ��Debug�汾����ʾ��TRACE��UNICODE�Ͷ��ֽڰ汾ͨ��
*				    ʹ��ʱע��Ҫ�����ƿռ�QNA(����Ϊ�˱����ں�MFC��TRACE������ͻ)
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-2-8 13:55:26
* 6�� History    �� 1��2011-12-20 13:54ʹ��Debug��֧����TRACEA��TRACEW�汾�� ����release�²�������
*					2��2012-09-07 15.17ʹ��release������֧��TRACE(ANSI��UNICODE)
*					3�����ӿɱ䳤�ȶȶ��� ͨ��TRACE_MAX_LEN��ָ����󳤶ȱ���ռ��˷�,��ȱ�ٿռ�
*					   ��ǿ���ݴ���  �����������ڿ����׶ξͺ����׷���
*					   �Ż��˴������TCHAR�ַ���֧��ANSI��UNICODE
*					4�����ӵ���ģʽʱֱ�������������̨����  ֻ�趨�� RUNINDEBUGMODE��
* 7�� Remark     �� ֻ����release�²�����#define USES_RELEASE_TRACE TRACE�Ż᲻������
*					ע�⣺				
*					1��#define USES_RELEASE_TRACE ��������#include <TraceUtil.h>֮ǰ	
*					������ʵ��������ATL��W2A��A2Wһ����Ծֲ��Ĺ��ܵ���̫�鷳�� ��ֻ����Ժ꺯����
*					������������ʵ�ֲ���
*					2��ͷ�ļ���ò��÷��������ռ����ʱ��ʹSTD���ִ���
*					3������ģʽ������#include <TraceUtil.h>֮ǰ����RUNINDEBUGMODE
****************************************************************************************************/ 
#ifndef __TRACE_UTIL_H__
#define __TRACE_UTIL_H__


//����ģʽʱֱ�������������̨
//#define RUNINDEBUGMODE

//windows.h������� ��ΪWCHAR��windows�ж���
#ifndef _WINDOWS_
#include <windows.h>
#endif
//���û�а���tchar�����
#ifndef _INC_TCHAR
#include <tchar.h>
#endif
#include <locale>
namespace QNA
{

//���û��ָ�������ַ����鳤�����Զ�ָ��Ϊ1024
#ifndef TRACE_MAX_LEN
#define TRACE_MAX_LEN 0x400   //���TRACE����Ϊ1024
#endif

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

//��RUNINDEBUGMODEģʽ�¿��ַ������Ҫsetlocale���� ��Ҫ����locale
#ifdef RUNINDEBUGMODE
#ifndef _INC_LOCALE
#include <locale>
#endif
#endif

//���û�ж���USES_RELEASE_TRACE��ִ�пպ���
	inline void TRACE(TCHAR* ptszEnter, ...)
	{
#ifdef USES_RELEASE_TRACE
		int iRet = 0;
		va_list args;                            //�����б�
		TCHAR tszOutput[TRACE_MAX_LEN] = {0};

		va_start(args, ptszEnter);               //��ȡ�����б��еĲ���

		//_vstprintf _vsctprintf�͸�ʽ����������� ��������·��������ִ��ĳ���(��ȥ\0),����������ظ�ֵ		
		iRet = _vsctprintf(ptszEnter, args); 
		if (iRet<0)	throw _T("TRACE _vsctprintf ptszEnter is short!!!\r\n");		
		iRet = _vstprintf_s(tszOutput, ptszEnter, args);	
		
		if (iRet>TRACE_MAX_LEN ||iRet<0)	throw _T("TRACE _vstprintf tszOutput is short!!!\r\n");		

		va_end(args);                           //����
		//�ڵ���ģʽ��ֱ�����������̨
#ifdef RUNINDEBUGMODE
		setlocale(LC_CTYPE, "");  //C���Ե�ȫ��locale����Ϊ��������,����ᵼ��cout��wcout�����������
		_tprintf(tszOutput);
		setlocale(LC_CTYPE, "C"); //��C���Ե�ȫ��locale�ָ�
#else
		OutputDebugStr(tszOutput);
#endif
#endif
	}
}
#endif /*__TRACE_UTIL_H__*/

