// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <afx.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
//#include <Windows.h>
//#include <WinSock2.h>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "Log.h"
using namespace INTERVISION;

extern CLog_T g_objLog;   //��־��
// ������Ϣ
#define LOG_DEBUG 1
// ��ͨ��Ϣ
#define LOG_INFO 2
// ������Ϣ
#define LOG_WARNING 4
// ������Ϣ
#define LOG_ERROR 8
#define LOG_DEFAULT_LEVEL 0xfe

//ȫ�ֱ���
extern char g_szMinitorFolderPah[MAX_PATH];
// ���������Ϣ�������������
void TracePrint(int nLogLevel, LPCTSTR lpstrTrace,...);

