#include "stdafx.h"
#include "BwWinINet.h"

BW_Network::BwWinINet::BwWinINet(void)
{
    hInternet = NULL;
}

BW_Network::BwWinINet::~BwWinINet(void)
{

}

UINT BW_Network::BwWinINet::OpenInternet()
{
    hInternet = ::InternetOpenA(
        "",
        INTERNET_OPEN_TYPE_DIRECT,
        NULL,
        NULL,
        0);
    return ::GetLastError();
}

UINT BW_Network::BwWinINet::ConnectInternet( LPCSTR host, UINT port )
{
    hInternet = ::InternetConnect(
        hInternet,
        host,
        port,
        _T(""),
        _T(""),
        INTERNET_SERVICE_HTTP,
        0,
        0);
    return ::GetLastError();
}

UINT BW_Network::BwWinINet::OpenRequest( LPCSTR reqMethod, LPCSTR reqPath )
{
    hInternet = ::HttpOpenRequest(
        hInternet,
        reqMethod,
        reqPath,
        _T("HTTP/1.1"),
        _T(""),
        NULL,
        0,
        0);
    return ::GetLastError();
}

BOOL BW_Network::BwWinINet::SendRequest( LPCTSTR heads, DWORD headLen, LPVOID optional, DWORD optLen )
{
    BOOL rs = ::HttpSendRequest(
        hInternet,
        heads,
        headLen,
        optional,
        optLen);
    int err = ::GetLastError();
    return rs;
}

UINT BW_Network::BwWinINet::GetHeadAllLength()
{
    DWORD len = 0;
    BOOL rs = ::HttpQueryInfo(
        hInternet,
        HTTP_QUERY_RAW_HEADERS_CRLF ,
        NULL,
        &len,
        NULL);
    return !rs && ERROR_INSUFFICIENT_BUFFER == GetLastError() ? len : -1;
}

UINT BW_Network::BwWinINet::GetHeadSingleLength( LPSTR pHead)
{
    DWORD len = 0;
    BOOL rs = ::HttpQueryInfo(
        hInternet,
        HTTP_QUERY_CUSTOM ,
        pHead,
        &len,
        NULL);
    return !rs && ERROR_INSUFFICIENT_BUFFER == GetLastError() ? len : -1;
}

BOOL BW_Network::BwWinINet::QueryHeadAll( LPSTR dest, LPDWORD length )
{
    return ::HttpQueryInfo(
        hInternet,
        HTTP_QUERY_RAW_HEADERS_CRLF ,
        dest,
        length,
        NULL);
}

BOOL BW_Network::BwWinINet::QueryHeadSingle( LPSTR dest, LPCSTR headname, size_t length )
{
    StringCchPrintf((LPSTR)dest, length, headname);
    return ::HttpQueryInfo(
        hInternet,
        HTTP_QUERY_CUSTOM ,
        dest,
        (LPDWORD)&length,
        NULL);
}

DWORD BW_Network::BwWinINet::GetDataSize()
{
    DWORD byteNums;
    ::InternetQueryDataAvailable(
        hInternet,
        &byteNums,
        0,
        0);
    return byteNums;
}

BOOL BW_Network::BwWinINet::AcquireData(LPVOID pData, DWORD size)
{
    DWORD toRead = 0;
    return ::InternetReadFile(
        hInternet,
        pData,
        size,
        &toRead);
}

BOOL BW_Network::BwWinINet::SaveFile( LPCSTR path, LPSTR buf, UINT length )
{
    FILE *pf = NULL;
    errno_t err = ::fopen_s(&pf, path,"wb");
    int wLen = fwrite(buf, sizeof(char), length, pf);
    ::fclose(pf);
    return wLen == length;
}

DWORD BW_Network::BwWinINet::GetLastError()
{
    return ::GetLastError();
}