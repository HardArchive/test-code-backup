#pragma once
/***************************************************************************************************
* 1�� File       �� ExpRptHandler
* 2�� Version    �� *.*
* 3�� Description�� �ó������ʱ�Զ�����minidump 
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-6-13 14:58:58
* 6�� History    �� 
* 7�� Remark     �� minidump���࣬Ȼ������Ĺ��캯���е���SetUnhandledExceptionFilter �������쳣��������
*                   ����Ч������ֻ��Ҫ���Ǽ�������д�õ�.h��.cpp�ļ���Ȼ��ʲôҲ��������ֱ�ӱ������о��ܴﵽ�����Ч���ˡ�
****************************************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <DbgHelp.h>
#include <tchar.h>

#pragma comment(lib, "DbgHelp.lib")

class CExpRptHandler
{
public:
	CExpRptHandler(void);
	~CExpRptHandler(void);

	static LONG WINAPI ExpFilter(PEXCEPTION_POINTERS pExceptionInfo );

	static TCHAR m_tszdmpFileName[MAX_PATH*2];
	static LPTOP_LEVEL_EXCEPTION_FILTER m_pFrontFilter;
};

extern CExpRptHandler g_clsExpRptHandler;