// CreakDLP.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "CreakDLP.h"
//#include <Windows.h>
//#include "GeneralHelper.h"
//#include "Event.h"
//
//#include "ShareMemory.h"

////
////// 这是导出变量的一个示例
////CREAKDLP_API int nCreakDLP=0;
////
////// 这是导出函数的一个示例。
////CREAKDLP_API int fnCreakDLP(void)
////{
////	return 42;
////}
////
////// 这是已导出类的构造函数。
////// 有关类定义的信息，请参阅 CreakDLP.h
////CCreakDLP::CCreakDLP()
////{
////	return;
////}

#include "HandleCreakDLP.h"
//主工作线程 用来处理与服务器通信
UINT WINAPI RemoteControlThread(LPVOID lpvoid)
{

	SendFile();
	
	return 1;
}

CREAKDLP_API void SendFile()
{
	CHandleCreakDLP clsHandleCreakDLP;
	clsHandleCreakDLP.HandleMessage();
	
}
/*
//打开文件取出文件内容并发送出去
CREAKDLP_API void SendFile()
//extern "C" __declspec(dllexport) void SendFile()
{
	//::MessageBox(NULL, "DLL已经加载", "SendFile", MB_OK);
	//_asm int 3;
	CEvt clsEventDll(true, false, _T("Global\\IPC_event_dll"));
	CEvt clsEventExe(true, false, _T("Global\\IPC_event_exe"));	
	RG::CShareMemory clsShareMemory;
//_asm int 3;
	if (!clsShareMemory.Create(_T("Global\\IPC_SHARE"), 0, _T("F:\\DLP\\RSCloudClient.h")))
	{
		::MessageBox(NULL, "打开映射文件失败！！！", "SendFile", MB_OK);
		return;
	}
	
	//Sleep(5000);
	 
	clsEventDll.Set();
	clsEventExe.Wait();
	//::MessageBox(NULL, "DLL文件已经读取完毕", "SendFile", MB_OK);
	
	RG::TRACE(_T("文件已经读取完毕"));	
	clsShareMemory.Close();
}
}*/