#pragma once
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "strsafe.lib")
#include <Windows.h>
#include <WinInet.h>
#include <stdio.h>
#include <strsafe.h>
/*********************************
    WinINet 封装类
*********************************/
namespace BW_Network{
    class BwWinINet
    {
    public:
        BwWinINet(void);
        virtual ~BwWinINet(void);
        UINT OpenInternet();
        UINT ConnectInternet(LPCSTR host, UINT port);
        UINT OpenRequest(LPCSTR reqMethod, LPCSTR reqPath);
        BOOL SendRequest(LPCTSTR heads, DWORD headLen, LPVOID optional, DWORD optLen);
        UINT GetHeadAllLength();
        UINT GetHeadSingleLength(LPSTR pHead);
        BOOL QueryHeadAll(LPSTR dest, LPDWORD length);
        BOOL QueryHeadSingle(LPSTR dest, LPCSTR headname, size_t length);
        DWORD GetDataSize();
        BOOL AcquireData(LPVOID pData, DWORD size);
        BOOL SaveFile(LPCSTR path, LPSTR buf, UINT length );
        DWORD GetLastError();
    private:
        //Internet对象指针
        HINTERNET hInternet;
    };
}
