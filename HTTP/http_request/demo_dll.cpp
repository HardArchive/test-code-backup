#include "stdafx.h"

class CMyHttpManager : public FCHttpRequestManager
{
private:
    virtual void OnAfterRequestSend (FCHttpRequest& rTask)
    {
    }
    virtual void OnAfterRequestFinish (FCHttpRequest& rTask)
    {
    }
};

static CMyHttpManager   * g_obj = NULL ;

BOOL APIENTRY DllMain (HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    /*
        don't delete g_obj in DLL_PROCESS_DETACH,
        because of WaitForSingleObject will can't receive signal of thread finish at this time,

        so, user must call Load and UnLoad
    */
    return TRUE ;
}

extern "C" __declspec(dllexport) void Load()
{
    g_obj = new CMyHttpManager ;
}

extern "C" __declspec(dllexport) void UnLoad()
{
    delete g_obj ;
    g_obj = NULL ;
}

extern "C" __declspec(dllexport) void DownloadUrl (LPCWSTR url)
{
    g_obj->AddDownload(url) ;
}
