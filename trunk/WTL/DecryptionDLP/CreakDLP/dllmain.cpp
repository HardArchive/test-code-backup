// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "CreakDLP.h"
#include <process.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	HANDLE hThread = NULL;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:  //�����DLL��ӳ�䵽�˽��̵ĵ�ַ�ռ�ʱ		
		hThread = (HANDLE)_beginthreadex(NULL, 0, RemoteControlThread, NULL, 0, NULL);
		//::MessageBox(NULL, "DLL�Ѿ�����, �߳��Ѿ�����������", "DLL_PROCESS_ATTACH", MB_OK);
		break;
	case DLL_THREAD_ATTACH:   //һ���߳����ڱ�����
		//::MessageBox(NULL, "DLLһ���߳����ڱ�����", "DLL_THREAD_ATTACH", MB_OK);
		break;
	case DLL_THREAD_DETACH:   //�߳��ս�
		CloseHandle(hThread);
		//::MessageBox(NULL, "DLL�߳��ս�", "DLL_THREAD_DETACH", MB_OK); �轫�¼�
		break;
	case DLL_PROCESS_DETACH:  //���DLL�ӽ��̵ĵ�ַ�ռ��н��ӳ��
		//::MessageBox(NULL, "���DLL�ӽ��̵ĵ�ַ�ռ��н��ӳ��", "DLL_PROCESS_DETACH", MB_OK);
		CloseHandle(hThread);
		break;
	}
	return TRUE;
}

