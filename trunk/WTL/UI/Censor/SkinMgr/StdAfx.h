// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#include <Windows.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlapp.h>
#include <atlgdi.h>
#include <atltrace.h>

#include <crtdbg.h>

#ifndef ASSERT
#define ASSERT	_ASSERT
#endif

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#define SKINMGR_MSG_HANDLER(msg, func) \
	if (uMsg == msg) \
	{ \
		return func(uMsg, wParam, lParam); \
	}