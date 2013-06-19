//注入程序代码
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

        HMODULE hMod	= lpInject->prcLoadLib(lpInject->szLibPath);  //加载DLL
        if (!hMod)
        {
            return 0xfffffffe;
        }

        //		#ifdef _UNICODE
        //			LPINIT_BTN	Init_Btn = (LPINIT_BTN)lpInject->prcGetProcAddr(hMod, TEXT("RC_InitBtnW"));
        //		#else
        LPINIT_BTN	Init_Btn = (LPINIT_BTN)lpInject->prcGetProcAddr(hMod, MAKEINTRESOURCE(1));//TEXT("RC_InitBtnA")); //获取函数地址
        //		#endif

        if (!Init_Btn)
        {
            lpInject->prcFreeLib(hMod); //释放DLL
            return 0xfffffffd;
        }
		
		//执行DLL函数
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
//初始化注入函数信息
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
        GetWindowThreadProcessId(hWnd, &dwProcessID);       //获取窗口句柄对应进程ID
		//打开进程
        HANDLE hProcess = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
                                       PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessID );
        if (!hProcess)
        {
            return FALSE;
        }

        INJECT_DLL InjectInfo;
        InjectDLL_Info(&InjectInfo, lpszLibFile, hWnd, dwID, pRtBtn, szCaptionBtn);

        //VirtualAllocEx在指定进程的虚拟空间保留或提交内存区域，除非指定MEM_RESET参数，否则将该内存区域置0
        //用于在目标进程内存空间中申请内存空间以写入 DLL 的文件名
        //分配(释放)一段虚拟地址空间,利用这段地址空间可以和指定的进程通讯
        //hProcess进程句柄 , NULL, MAXINJECTSIZE 欲分配的内存大小，字节单位,
        //MEM_COMMIT：为特定的页面区域分配内存中或磁盘的页面文件中的物理存储
        // PAGE_EXECUTE_READWRITE 区域包含可执行代码，应用程序可以读写该区域
        LPBYTE lpThreadAddr = (LPBYTE)::VirtualAllocEx(hProcess, NULL, MAXINJECTSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		//申请一块读写空间  用于写入LPINJECT_DLL
        LPINJECT_DLL param = (LPINJECT_DLL) VirtualAllocEx( hProcess, 0, sizeof(INJECT_DLL), MEM_COMMIT, PAGE_READWRITE );

        WriteProcessMemory(hProcess, lpThreadAddr, &RemoteControlThread, MAXINJECTSIZE, 0);   //写入远线程函数执行数据
        WriteProcessMemory( hProcess, param, &InjectInfo, sizeof(InjectInfo), 0 );            //写入LPINJECT_DLL结构信息

        DWORD dwThreadId;
		//创建远线程 传入参数param
        HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0,
                                              (unsigned long (__stdcall *)(void *))lpThreadAddr,
                                              param, 0, &dwThreadId);


        if (!hThread)
        {
            CloseHandle(hProcess);
			//释放申请空间
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

