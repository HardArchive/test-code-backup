/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.1.1
 * Author	: Bruce Liang
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: http://qun.qq.com/#jointhegroup/gid/75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/******************************************************************************
Module:  initcom.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: ���ڼ򵥷�װCoInitializeEx��CoUninitialize.
Desc:
******************************************************************************/
#pragma once

#ifndef _WIN32_WINNT			// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0500		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif	

#ifndef _AFX
	#include <windows.h>
	#include <objbase.h>
#endif

class CInitCOM
{
public:
	CInitCOM(DWORD dwCoInit = COINIT_APARTMENTTHREADED, void * pvReserved = nullptr)
		{::CoInitializeEx(pvReserved, dwCoInit);}
	~CInitCOM()
		{::CoUninitialize();}
};
