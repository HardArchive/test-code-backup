// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

#include <tchar.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <wininet.h>
#include <UrlHist.h>
#include <Shlwapi.h>
#include <ShlObj.h>
#include <shobjidl.h>

//#pragma Wininet.lib 
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Wininet.lib")

typedef struct INK_INFO
{
	WCHAR wszPath[MAX_PATH];                //����·��
	WCHAR wszUrlName[MAX_PATH];             //��ҳ��
	WCHAR wszUrl[2*MAX_PATH];               //��ַ

}INKINFO, *PINKINFO;