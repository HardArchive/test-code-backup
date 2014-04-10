//file MessageboxUtil.h 
/***************************************************************************************************
* 1�� Class      �� CMessageboxUtil
* 2�� Version    �� 1.0
* 3�� Description�� �ܼ򵥵Ĺ��� ֻ����Messagebox֧�����޲���
* 4�� Author     �� Ray (http://www.9cpp.com/)
* 5�� Created    �� 2012-9-9 13:34:13
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __MESSAGEBOX_UTIL_H__
#define __MESSAGEBOX_UTIL_H__
namespace RG
{
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
	//���޲�����Messagebox����
	//������˼���׼��һ����ֻ�ǻ��˸�λ��
	int CDECL MessageboxUtil(HWND hWnd, UINT uType, TCHAR * szCaption, TCHAR * szFormat, ...)
	{
		TCHAR szBuffer [1024] = {0};
		va_list pArgList ;
		va_start (pArgList, szFormat);
		_vsntprintf (szBuffer, sizeof (szBuffer) / sizeof (TCHAR), 
			szFormat, pArgList);
		va_end (pArgList) ;
		return MessageBox(hWnd, szBuffer, szCaption, uType);
	}
	
	//������Messagebox����
	int CDECL MessageboxUtil(HWND hWnd,	LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
	{
		return MessageBox(hWnd, lpText, lpCaption, uType);
	}
}
#endif

