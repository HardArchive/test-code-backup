// CreakDLP.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "CreakDLP.h"
//#include <Windows.h>
//#include "GeneralHelper.h"
//#include "Event.h"
//
//#include "ShareMemory.h"

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

#include "HandleCreakDLP.h"
//�������߳� ���������������ͨ��
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
//���ļ�ȡ���ļ����ݲ����ͳ�ȥ
CREAKDLP_API void SendFile()
//extern "C" __declspec(dllexport) void SendFile()
{
	//::MessageBox(NULL, "DLL�Ѿ�����", "SendFile", MB_OK);
	//_asm int 3;
	CEvt clsEventDll(true, false, _T("Global\\IPC_event_dll"));
	CEvt clsEventExe(true, false, _T("Global\\IPC_event_exe"));	
	RG::CShareMemory clsShareMemory;
//_asm int 3;
	if (!clsShareMemory.Create(_T("Global\\IPC_SHARE"), 0, _T("F:\\DLP\\RSCloudClient.h")))
	{
		::MessageBox(NULL, "��ӳ���ļ�ʧ�ܣ�����", "SendFile", MB_OK);
		return;
	}
	
	//Sleep(5000);
	 
	clsEventDll.Set();
	clsEventExe.Wait();
	//::MessageBox(NULL, "DLL�ļ��Ѿ���ȡ���", "SendFile", MB_OK);
	
	RG::TRACE(_T("�ļ��Ѿ���ȡ���"));	
	clsShareMemory.Close();
}
}*/