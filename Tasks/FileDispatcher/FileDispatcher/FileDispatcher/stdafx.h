// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <ATLComTime.h>



// ������Ϣ
#define LOG_DEBUG 1
// ��ͨ��Ϣ
#define LOG_INFO 2
// ������Ϣ
#define LOG_WARNING 4
// ������Ϣ
#define LOG_ERROR 8
// Ĭ�������־�ȼ�
#define LOG_DEFAULT_LEVEL 0xff

// ���������Ϣ�������������
void TracePrint(int nLogLevel, LPCTSTR lpstrTrace,...);

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�



