// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "CreakDLP.h"
#include <process.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:  //当这个DLL被映射到了进程的地址空间时		
		_beginthreadex(NULL, 0, RemoteControlThread, NULL, 0, NULL);
		//::MessageBox(NULL, "DLL已经加载, 线程已经创建！！！", "DLL_PROCESS_ATTACH", MB_OK);
		break;
	case DLL_THREAD_ATTACH:   //一个线程正在被创建
		//::MessageBox(NULL, "DLL一个线程正在被创建", "DLL_THREAD_ATTACH", MB_OK);
		break;
	case DLL_THREAD_DETACH:   //线程终结
		//::MessageBox(NULL, "DLL线程终结", "DLL_THREAD_DETACH", MB_OK);
		break;
	case DLL_PROCESS_DETACH:  //这个DLL从进程的地址空间中解除映射
		//::MessageBox(NULL, "这个DLL从进程的地址空间中解除映射", "DLL_PROCESS_DETACH", MB_OK);
		break;
	}
	return TRUE;
}

