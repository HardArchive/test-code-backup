// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200
//

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <atlbase.h>        // ������ATL��

#pragma comment(lib, "atlthunk.lib")
#include <atlwin.h>         // ATL������

extern CComModule _Module;  // ȫ��_Module



#include "resource.h"

//����������ϣ�
//����ʵ��console��windowsģʽ�Ļ�ϣ����Դﵽ������DOS���ڵ�Ч����Ҳ���Դﵽ��Windows�����������̨���printf��Ϣ��
#pragma comment( linker, "/subsystem:windows /entry:WinMainCRTStartup" )
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
//
//#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup" )
//#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup" )

