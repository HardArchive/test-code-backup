// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <atlbase.h>
#include <Windows.h>
#include <atlcoll.h>
#include <atlwin.h>
#include <atlstr.h>
//#include <atltrace.h>

//#include "vld.h"
#include "GlobalFunction.h"
#include "DebugTrace.h"
using namespace TRACE_9CPP;



//ȫ�ֱ���
extern char g_szServerIP[32];             //������IP
extern char g_szMysqlUserName[MAX_PATH];  //Mysql�û���
extern char g_szMysqlPassword[MAX_PATH];  //Mysql���� 