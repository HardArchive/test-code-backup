// CreakDLP.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "CreakDLP.h"
#include <Windows.h>
#include "GeneralHelper.h"
#include "Event.h"

#include "ShareMemory.h"

////
////// ���ǵ���������һ��ʾ��
////CREAKDLP_API int nCreakDLP=0;
////
////// ���ǵ���������һ��ʾ����
////CREAKDLP_API int fnCreakDLP(void)
////{
////	return 42;
////}
////
////// �����ѵ�����Ĺ��캯����
////// �й��ඨ�����Ϣ������� CreakDLP.h
////CCreakDLP::CCreakDLP()
////{
////	return;
////}


//���ļ�ȡ���ļ����ݲ����ͳ�ȥ
CREAKDLP_API void SendFile()
//extern "C" __declspec(dllexport) void SendFile()
{
	CEvt clsEventSend(false, false, _T("Global\\IPC_event_send"));
	CEvt clsEventRecv(false, false, _T("Global\\IPC_event_recv"));	
	RG::CShareMemory clsShareMemory;
	clsShareMemory.Open(tstring(_T("Global\\IPC_SHARE")), 0, tstring(_T("D:\\RSCloudClient.h")));
	//ע�⣬��wait��setEvent����client�˵ĳ����෴
	WaitForSingleObject(clsEventRecv.GetHandle(), INFINITE);

	Sleep(5000);
	clsShareMemory.Close();
}