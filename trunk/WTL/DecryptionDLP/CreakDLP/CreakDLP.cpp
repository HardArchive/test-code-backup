// CreakDLP.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "CreakDLP.h"
#include <Windows.h>
#include "GeneralHelper.h"
#include "Event.h"

#include "ShareMemory.h"

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


//打开文件取出文件内容并发送出去
CREAKDLP_API void SendFile()
//extern "C" __declspec(dllexport) void SendFile()
{
	CEvt clsEventSend(false, false, _T("Global\\IPC_event_send"));
	CEvt clsEventRecv(false, false, _T("Global\\IPC_event_recv"));	
	RG::CShareMemory clsShareMemory;
	clsShareMemory.Open(tstring(_T("Global\\IPC_SHARE")), 0, tstring(_T("D:\\RSCloudClient.h")));
	//注意，先wait再setEvent，与client端的程序相反
	WaitForSingleObject(clsEventRecv.GetHandle(), INFINITE);

	Sleep(5000);
	clsShareMemory.Close();
}