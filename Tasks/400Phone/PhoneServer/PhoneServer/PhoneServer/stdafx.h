// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <afx.h>
#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

// ������Ϣ
#define LOG_DEBUG 1
// ��ͨ��Ϣ
#define LOG_INFO 2
// ������Ϣ
#define LOG_WARNING 4
// ������Ϣ
#define LOG_ERROR 8
#define LOG_DEFAULT_LEVEL 0xfe

// ���������Ϣ�������������
void TracePrint(int nLogLevel, LPCTSTR lpstrTrace,...);