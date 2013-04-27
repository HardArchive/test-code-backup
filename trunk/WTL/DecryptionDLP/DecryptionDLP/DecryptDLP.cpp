#include "StdAfx.h"
#include "DecryptDLP.h"


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

CDecryptDLP::CDecryptDLP(void)
{
}

CDecryptDLP::~CDecryptDLP(void)
{
}
