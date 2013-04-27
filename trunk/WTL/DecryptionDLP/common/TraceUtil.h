//file TraceUtil.h
/*************************************************************************************************** 
* 1�� Class      �� TraceUtil
* 2�� Version    �� 1.6
* 3�� Description�� ��Debug�汾����ʾ��TRACE��UNICODE�Ͷ��ֽڰ汾ͨ��
*				    ʹ��ʱע��Ҫ�����ƿռ�QNA(����Ϊ�˱����ں�MFC��TRACE������ͻ)
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2012-2-8 13:55:26
* 6�� History    �� 1��2011-12-20 13:54ʹ��Debug��֧����TRACEA��TRACEW�汾�� ����release�²�������
*					2��2012-09-07 15.17ʹ��release������֧��TRACE(ANSI��UNICODE)
*					3�����ӿɱ䳤�ȶȶ��� ͨ��TRACE_MAX_LEN��ָ����󳤶ȱ���ռ��˷�,��ȱ�ٿռ�
*					   ��ǿ���ݴ���  �����������ڿ����׶ξͺ����׷���
*					   �Ż��˴������TCHAR�ַ���֧��ANSI��UNICODE
*					4�����ӵ���ģʽʱֱ�������������̨����  ֻ�趨�� RUNINDEBUGMODE��
*					5�����Ӵ�ӡ���ļ�����
* 7�� Remark     �� ������ʹ��ʱ����������ע��(�粻ȥ�����ֻ����ͨTRACE����<֧��ANSI��UNICODE>)��
*					1��USES_RELEASE_TRACE  �˺�����Release��ʵ��TRACE����
*					2��RUNINDEBUGMODE �˺���ʵ�ֽ�TRACE��¼��ӡ������̨
*					3��TRACE_LOG_FILE_PATH �˺궨����־����λ�ã�����¼��ӡΪ�ı��ļ�
*					ע�⣺				
*					1������������ֻ��#include <TraceUtil.h>֮ǰ����Ż�������
*					2�����������RUNINDEBUGMODE�꣬��TRACE_LOG_FILE_PATH��������
*ע���궨����Է��������ռ�֮�⣬��Ϊ���ڱ���׶�������
*	 ͷ�ļ���ò��÷��������ռ����ʱ��ʹSTD���ִ���
*	 ��ӡ��־Ϊ�ļ���������д�����־���ӡ����ò�Ҫ������ΪƵ���򿪹ر��ļ���ռ�ô�����Դ	
*	 �˺���ֻ����C++��ʹ�ã�C���Բ�֧�������ռ�
****************************************************************************************************/ 
#ifndef __TRACE_UTIL_H__
#define __TRACE_UTIL_H__

//windows.h������� ��ΪWCHAR��windows�ж���
#ifndef _WINDOWS_
#include <windows.h>
#endif

//���û�а���tchar�����
#ifndef _INC_TCHAR
#include <tchar.h>
#endif

//���û��ָ�������ַ����鳤�����Զ�ָ��Ϊ1024
#ifndef TRACE_MAX_LEN
#define TRACE_MAX_LEN 0x400   //���TRACE����Ϊ1024
#endif

//��debug�� USES_RELEASE_TRACE ֱ�Ӵ�ӡ��־
#ifdef _DEBUG
#ifndef USES_RELEASE_TRACE
#define USES_RELEASE_TRACE
#endif
#endif

//���û�ж���USES_RELEASE_TRACE�򲻰���ͷ��Ҳ�����ӡ
#ifdef USES_RELEASE_TRACE
#include <stdio.h>
#include <stdarg.h>
#endif

//��������˴�ӡ������̨ģʽ�������ӡ���ļ�
#ifndef RUNINDEBUGMODE
//���δ�����������̨ģʽ�Ҷ�������־����·����������ļ�ģʽ
//���������־����·�����ӡ��־���ļ�
#ifdef TRACE_LOG_FILE_PATH
#ifndef TRACE_LOG_FILE
#define TRACE_LOG_FILE
#endif
#endif 

//�������Ľ���־���浽�ļ����������������ģʽ
//�ڴ�ģʽ��release�汾Ҳ�ᱣ���ļ�
#ifdef TRACE_LOG_FILE
#ifndef RUNINDEBUGMODE
#define RUNINDEBUGMODE
#endif
#endif 
#endif

//��RUNINDEBUGMODEģʽ�¿��ַ������Ҫsetlocale���� ��Ҫ����locale
#ifdef RUNINDEBUGMODE
#ifndef _INC_LOCALE
#include <locale.h>
#endif
#endif

namespace RG
{
//���û�ж���USES_RELEASE_TRACE��ִ�пպ���
	inline void TRACE(TCHAR* ptszEnter, ...)
	{
#ifdef USES_RELEASE_TRACE
		int iRet = 0;
		va_list args;                            //�����б�		
		TCHAR tszOutput[TRACE_MAX_LEN] = {0};		
#ifdef RUNINDEBUGMODE		
		TCHAR* ptOldLocale = NULL;
#endif
		va_start(args, ptszEnter);               //��ȡ�����б��еĲ���

		//_vstprintf _vsctprintf�͸�ʽ����������� ��������·��������ִ��ĳ���(��ȥ\0),����������ظ�ֵ		
		iRet = _vsctprintf(ptszEnter, args); 
		if (iRet<0)	throw _T("TRACE _vsctprintf ptszEnter is short!!!\r\n");		
		iRet = _vstprintf_s(tszOutput, ptszEnter, args);	
		
		if (iRet>TRACE_MAX_LEN ||iRet<0)	throw _T("TRACE _vstprintf tszOutput is short!!!\r\n");		

		va_end(args);                           //����
		//�ڵ���ģʽ��ֱ�����������̨
#ifdef RUNINDEBUGMODE
		//���ļ���־ģʽ�±��浽�ļ�
#ifdef TRACE_LOG_FILE
		FILE* pFile = 0;
		if(!( pFile = _tfreopen(TRACE_LOG_FILE_PATH, _T("a+") , stdout)));
#endif
		ptOldLocale = _tcsdup(_tsetlocale(LC_CTYPE, NULL));  //��ȡ�������Ա���
		_tsetlocale(LC_CTYPE, _T(""));  //C���Ե�ȫ��locale����Ϊ��������,����ᵼ��cout��wcout�����������
		_tprintf(tszOutput);
		_tsetlocale(LC_CTYPE, ptOldLocale); //��C���Ե�ȫ��locale�ָ�
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

