// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

/***************************************************************************************************
��ΪWindows.h����������䣺
#ifndef _MAC
#include <winperf.h>
#include <winsock.h>
#endif
���Ե�����˳�򲻵�(windows.h����winsock2.h)�ͻ���ֺܶ�Ī������Ĵ���
#define WIN32_LEAN_AND_MEAN������䣬���ܹ��������������winsock.h�����������
****************************************************************************************************/
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>


//#include <atlbase.h>
//#include <atlconv.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "../common/com_module/GeneralHelper.h"
