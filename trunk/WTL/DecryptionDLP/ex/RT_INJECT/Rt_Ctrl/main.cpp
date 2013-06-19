//ע��������
#include "main.h"

BOOL WINAPI DllMain(HANDLE hInstance, ULONG Command, LPVOID Reserved)
{
    return TRUE;
}

#pragma check_stack(off)
static DWORD WINAPI RemoteControlThread(LPVOID lpvoid)
{
    try
    {
        LPINJECT_DLL lpInject = (LPINJECT_DLL)lpvoid;

        if (!lpInject)
        {
            return 0xffffffff;
        }

        HMODULE hMod	= lpInject->prcLoadLib(lpInject->szLibPath);  //����DLL
        if (!hMod)
        {
            return 0xfffffffe;
        }

        //		#ifdef _UNICODE
        //			LPINIT_BTN	Init_Btn = (LPINIT_BTN)lpInject->prcGetProcAddr(hMod, TEXT("RC_InitBtnW"));
        //		#else
        LPINIT_BTN	Init_Btn = (LPINIT_BTN)lpInject->prcGetProcAddr(hMod, MAKEINTRESOURCE(1));//TEXT("RC_InitBtnA")); //��ȡ������ַ
        //		#endif

        if (!Init_Btn)
        {
            lpInject->prcFreeLib(hMod); //�ͷ�DLL
            return 0xfffffffd;
        }
		
		//ִ��DLL����
        if (!Init_Btn(lpInject->dwIDBtn, lpInject->szCaptionBtn, lpInject->rtBtn, lpInject->hWnd))
        {
            lpInject->prcFreeLib(hMod);
            return 0xfffffffc;
        }

        /*		while (TRUE){
        			TCHAR szName[128];

        			wsprintf(szName, TEXT("%08d_%s_zy"), lpInject->hWnd, lpInject->szCaptionBtn);

        			HANDLE hMutex = z_OpenMutex(MUTEX_ALL_ACCESS, FALSE, szName);
        			if (hMutex){
        				CloseHandle(hMutex);
        				lpInject->prcFreeLib(hMod);
        				lpInject->prcFreeLib(hKernel32);
        				return 0;
        			}
        			Sleep(1000);
        		}
        */
        lpInject->prcFreeLib(hMod);
        //				lpInject->prcFreeLib(hKernel32);
        //				return 0;
    }
    catch (...)
    {
        return 0xfffffffb;
    }
    return 0;
}
#pragma check_stack
//��ʼ��ע�뺯����Ϣ
BOOL InjectDLL_Info(LPINJECT_DLL lpInject, LPCSTR lpszLibFile, HWND hWnd, DWORD dwID, LPRECT pRtBtn, LPCTSTR szCaptionBtn)
{
    try
    {
        HMODULE hMod = ::GetModuleHandle(TEXT("kernel32"));
#ifdef _UNICODE
        lpInject->prcLoadLib = (LPLOADLIBRARY)::GetProcAddress(hMod, TEXT("LoadLibraryW"));
#else
        lpInject->prcLoadLib = (LPLOADLIBRARY)::GetProcAddress(hMod, TEXT("LoadLibraryA"));
#endif

        lpInject->prcFreeLib = (LPFREELIBRARY)::GetProcAddress(hMod, TEXT("FreeLibrary"));
        lpInject->prcGetProcAddr = (LPGETPROCADDRESS)::GetProcAddress(hMod, TEXT("GetProcAddress"));

        strcpy(lpInject->szLibPath, lpszLibFile);
        lpInject->dwIDBtn = dwID;
        lpInject->hWnd = hWnd;
        memcpy(&lpInject->rtBtn, pRtBtn, sizeof(RECT));
        strcpy(lpInject->szCaptionBtn, szCaptionBtn);
    }
    catch (...)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL WINAPI RT_CTRL_BTN(LPCSTR lpszLibFile, HWND hWnd, DWORD dwID, LPRECT pRtBtn, LPCTSTR szCaptionBtn)
{
    try
    {
        DWORD dwProcessID;
        GetWindowThreadProcessId(hWnd, &dwProcessID);       //��ȡ���ھ����Ӧ����ID
		//�򿪽���
        HANDLE hProcess = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
                                       PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessID );
        if (!hProcess)
        {
            return FALSE;
        }

        INJECT_DLL InjectInfo;
        InjectDLL_Info(&InjectInfo, lpszLibFile, hWnd, dwID, pRtBtn, szCaptionBtn);

        //VirtualAllocEx��ָ�����̵�����ռ䱣�����ύ�ڴ����򣬳���ָ��MEM_RESET���������򽫸��ڴ�������0
        //������Ŀ������ڴ�ռ��������ڴ�ռ���д�� DLL ���ļ���
        //����(�ͷ�)һ�������ַ�ռ�,������ε�ַ�ռ���Ժ�ָ���Ľ���ͨѶ
        //hProcess���̾�� , NULL, MAXINJECTSIZE ��������ڴ��С���ֽڵ�λ,
        //MEM_COMMIT��Ϊ�ض���ҳ����������ڴ��л���̵�ҳ���ļ��е�����洢
        // PAGE_EXECUTE_READWRITE ���������ִ�д��룬Ӧ�ó�����Զ�д������
        LPBYTE lpThreadAddr = (LPBYTE)::VirtualAllocEx(hProcess, NULL, MAXINJECTSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		//����һ���д�ռ�  ����д��LPINJECT_DLL
        LPINJECT_DLL param = (LPINJECT_DLL) VirtualAllocEx( hProcess, 0, sizeof(INJECT_DLL), MEM_COMMIT, PAGE_READWRITE );

        WriteProcessMemory(hProcess, lpThreadAddr, &RemoteControlThread, MAXINJECTSIZE, 0);   //д��Զ�̺߳���ִ������
        WriteProcessMemory( hProcess, param, &InjectInfo, sizeof(InjectInfo), 0 );            //д��LPINJECT_DLL�ṹ��Ϣ

        DWORD dwThreadId;
		//����Զ�߳� �������param
        HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0,
                                              (unsigned long (__stdcall *)(void *))lpThreadAddr,
                                              param, 0, &dwThreadId);


        if (!hThread)
        {
            CloseHandle(hProcess);
			//�ͷ�����ռ�
            VirtualFreeEx( hProcess, lpThreadAddr, 0, MEM_RELEASE );
            VirtualFreeEx( hProcess, param, 0, MEM_RELEASE );
            return FALSE;
        }
        else
        {
            CloseHandle(hThread);
            CloseHandle(hProcess);
            VirtualFreeEx( hProcess, lpThreadAddr, 0, MEM_RELEASE );
            VirtualFreeEx( hProcess, param, 0, MEM_RELEASE );
        }

    }
    catch (...)
    {
        return FALSE;
    }
    return TRUE;
}

