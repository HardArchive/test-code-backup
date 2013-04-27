// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:  //当这个DLL被映射到了进程的地址空间时

	case DLL_THREAD_ATTACH:   //一个线程正在被创建
	case DLL_THREAD_DETACH:   //线程终结
	case DLL_PROCESS_DETACH:  //这个DLL从进程的地址空间中解除映射
		break;
	}
	return TRUE;
}

