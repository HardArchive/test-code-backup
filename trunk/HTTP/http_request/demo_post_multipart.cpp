#include "stdafx.h"

class CMyHttpManager : public FCHttpRequestManager
{
private:
    virtual void OnAfterRequestFinish (FCHttpRequest& rTask)
    {
        if (rTask.GetResponseInfo().m_status_code == 200)
        {
            ::MessageBox(NULL, L"Post OK", L"Demo", MB_OK) ;
        }
        else
        {
            ::MessageBox(NULL, L"Post Error", L"Demo", MB_OK) ;
        }

        PostQuitMessage(0) ;
    }
};

inline BOOL ReadFileToBuffer (LPCTSTR strFilename, std::vector<BYTE>& file_data)
{
    file_data.clear() ;

    BOOL     b = FALSE ;
    HANDLE   f = CreateFile (strFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) ;
    if (f != INVALID_HANDLE_VALUE)
    {
        DWORD   nLength = ::GetFileSize(f, NULL) ;
        if (nLength)
        {
            file_data.resize (nLength) ;

            DWORD   dwRead ;
            ::ReadFile (f, &file_data[0], nLength, &dwRead, NULL) ;
            b = (dwRead == nLength) ;
        }
        CloseHandle (f) ;
    }
    return b ;
}

int _tmain(int argc, _TCHAR* argv[])
{
    CMyHttpManager   http_mgr ;

    HTTP_REQUEST_HEADER   h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART) ;

    // your url
    h.m_url = L"http://www.you_url.com" ;

    // add string param
    h.AddMultipartFormData ("param1", "value of param1") ;
    h.AddMultipartFormData ("param2", "value of param2") ;

    // add post file, read file into buf
    std::vector<BYTE>   file_buf ;
    ReadFileToBuffer (L"c:\\1.jpg", file_buf) ;

    if (file_buf.size())
    {
        h.AddMultipartFormData ("param3", &file_buf[0], file_buf.size(), "123.jpg") ;
    }

    // add end flag
    h.EndMultipartFormData() ;

    http_mgr.AddRequest (h) ;

    // wait result
    MSG   msg ;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg) ;
        DispatchMessage(&msg) ;
    }
	return 0 ;
}
