#include "StdAfx.h"
#include "DecryptDLP.h"
#include "Event.h"
#include "ShareMemory.h"
#include "file.h"
//#pragma check_stack(off)
//static DWORD WINAPI RemoteControlThread(LPVOID lpvoid)
//{
//    try
//    {
//        LPINJECT_DLL lpInject = (LPINJECT_DLL)lpvoid;
//
//        if (!lpInject)
//        {
//            return 0xffffffff;
//        }
//
//        HMODULE hMod	= lpInject->prcLoadLib(lpInject->szLibPath);  //����DLL
//        if (!hMod)
//        {
//            return 0xfffffffe;
//        }
//
//        //		#ifdef _UNICODE
//        //			LPINIT_BTN	Init_Btn = (LPINIT_BTN)lpInject->prcGetProcAddr(hMod, TEXT("RC_InitBtnW"));
//        //		#else
//        LPINIT_BTN	Init_Btn = (LPINIT_BTN)lpInject->prcGetProcAddr(hMod, MAKEINTRESOURCE(1));//TEXT("RC_InitBtnA")); //��ȡ������ַ
//        //		#endif
//
//        if (!Init_Btn)
//        {
//            lpInject->prcFreeLib(hMod); //�ͷ�DLL
//            return 0xfffffffd;
//        }
//		
//		//ִ��DLL����
//        if (!Init_Btn(lpInject->dwIDBtn, lpInject->szCaptionBtn, lpInject->rtBtn, lpInject->hWnd))
//        {
//            lpInject->prcFreeLib(hMod);
//            return 0xfffffffc;
//        }
//
//        /*		while (TRUE){
//        			TCHAR szName[128];
//
//        			wsprintf(szName, TEXT("%08d_%s_zy"), lpInject->hWnd, lpInject->szCaptionBtn);
//
//        			HANDLE hMutex = z_OpenMutex(MUTEX_ALL_ACCESS, FALSE, szName);
//        			if (hMutex){
//        				CloseHandle(hMutex);
//        				lpInject->prcFreeLib(hMod);
//        				lpInject->prcFreeLib(hKernel32);
//        				return 0;
//        			}
//        			Sleep(1000);
//        		}
//        */
//        lpInject->prcFreeLib(hMod);
//        //				lpInject->prcFreeLib(hKernel32);
//        //				return 0;
//    }
//    catch (...)
//    {
//        return 0xfffffffb;
//    }
//    return 0;
//}
//#pragma check_stack
//
//CDecryptDLP::CDecryptDLP(void)
//{
//}
//
//CDecryptDLP::~CDecryptDLP(void)
//{
//}

#include <TlHelp32.h>
//��ȡ��Ӧ��������ID 
DWORD GetSpecifiedProcessId(tstring& tstrProcessName) 
{ 
	DWORD dwID = 0; 
	//���ϵͳ���վ�� (ͨ�׵Ľ�, ���ǵõ���ǰ�����н���) 
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	PROCESSENTRY32 pInfo; //���ڱ��������Ϣ��һ�����ݽṹ 
	pInfo.dwSize = sizeof(pInfo); 
	//�ӿ����л�ȡ�����б� 
	Process32First(hSnapShot, &pInfo) ; //�ӵ�һ�����̿�ʼѭ�� 
	do 
	{ 
		//tstrProcessNameΪ��Ľ������� 				
		if(0 == tstrProcessName.compare(tstring(pInfo.szExeFile))) 
		{ 
			dwID = pInfo.th32ProcessID ; 
			break ; 
		} 
	}while(Process32Next(hSnapShot, &pInfo) != false); 
	return dwID; //dwID ������Ҫ�Ľ���PID ��.. 
}


bool DecryptDLP()
{
	DWORD deProcessID = GetSpecifiedProcessId(tstring(_T("calc.exe")));

	LoadLib(deProcessID, _T("F:\\Visual Studio Projects\\test-code-backup\\trunk\\WTL\\DecryptionDLP\\bin\\CreakDLP.dll"));
	
	CEvt clsEventDll(true, false, _T("Global\\IPC_event_dll"));
	CEvt clsEventExe(true, false, _T("Global\\IPC_event_exe"));	

	clsEventDll.Wait();

	RG::CShareMemory clsShareMemory;
	DWORD dwFileSize = 1643;

	clsShareMemory.Open(_T("Global\\IPC_SHARE"), dwFileSize);
	
	RG::CFile clsFile;         //�ļ��򿪾��
	clsFile.Open(_T("F:\\DLP\\12345.h"), _T("wb"));
	clsFile.Write((PBYTE)clsShareMemory.GetBasePoint(), dwFileSize);
	clsFile.Close();

	clsEventExe.Set();

	FreeLib(deProcessID, _T("F:\\Visual Studio Projects\\test-code-backup\\trunk\\WTL\\DecryptionDLP\\bin\\CreakDLP.dll"));
	return true;
}