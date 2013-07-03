/******************************************************************************
Module:  initcom.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: ���ڼ򵥷�װCoInitializeEx��CoUninitialize.
Desc:
******************************************************************************/
#pragma once

#ifndef _WIN32_WINNT			//����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0500		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif	

#ifndef _AFX
	#include <windows.h>
	#include <objbase.h>
#endif

class CInitCOM
{
public:
	CInitCOM(DWORD dwCoInit = COINIT_APARTMENTTHREADED, void * pvReserved = NULL) {::CoInitializeEx(pvReserved, dwCoInit);}
	~CInitCOM() {::CoUninitialize();}
};
