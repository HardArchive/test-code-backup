#include "stdafx.h"
#include "http.h"
#include "dlg_download.h"
#include "dlg_proxy.h"


BEGIN_MESSAGE_MAP(DlgDownload, CDialog)
    ON_COMMAND(IDC_SAVE_TO_FILE, OnSaveToFile)
    ON_COMMAND(IDC_SET_PROXY, OnSetProxy)
    ON_WM_TIMER()
END_MESSAGE_MAP()


//-------------------------------------------------------------------------------------
BOOL DlgDownload::OnInitDialog()
{
    ClearOutputText() ;

    m_header.m_header += L"Referer: http://www.9cpp.com/\r\n" ;
    m_header.m_header += L"Accept: */*\r\n" ;

    return __super::OnInitDialog() ;
}
//-------------------------------------------------------------------------------------
void DlgDownload::DoDataExchange(CDataExchange* pDX)
{
    DDX_Text(pDX, IDC_EDIT_URL, m_header.m_url) ;
    DDX_Text(pDX, IDC_EDIT_START, m_header.m_start) ;
    DDX_Text(pDX, IDC_EDIT_AGENT, m_header.m_user_agent) ;
    DDX_Text(pDX, IDC_EDIT_FIELD, m_header.m_header) ;
}
//-------------------------------------------------------------------------------------
void DlgDownload::ClearOutputText()
{
    SetDlgItemText(IDC_TEXT_STATUS, L"") ;
    SetDlgItemText(IDC_TEXT_TOTAL, L"") ;
    SetDlgItemText(IDC_TEXT_DOWNLOAD, L"") ;
    SetDlgItemText(IDC_TEXT_AVERAGE_SPEED, L"") ;
    SetDlgItemText(IDC_TEXT_ELAPSE_TIME, L"") ;
    SetDlgItemText(IDC_TEXT_MD5, L"") ;
    SetDlgItemText(IDC_TEXT_SEND_HEADER, L"") ;
    SetDlgItemText(IDC_TEXT_RESPONSE_HEADER, L"") ;
    SendDlgItemMessage(IDC_DOWN_PROGRESS, PBM_SETRANGE32, 0, 100) ;
    SendDlgItemMessage(IDC_DOWN_PROGRESS, PBM_SETPOS, 0) ;
    ::EnableWindow(::GetDlgItem(m_hWnd, IDC_SAVE_TO_FILE), FALSE) ;
}
//-------------------------------------------------------------------------------------
void DlgDownload::OnOK()
{
    UpdateData() ;

    // is stop
    CString   btn_txt ;
    GetDlgItemText(IDOK, btn_txt) ;
    if (btn_txt == L"Stop")
    {
        DeleteRequest(m_task_id) ;

        SetDlgItemText (IDC_TEXT_STATUS, L"User Stoped") ;
        SetDlgItemText (IDOK, L"Download") ;
        return ;
    }

    if (!UrlIs(m_header.m_url, URLIS_URL))
    {
        EDITBALLOONTIP   t ;
        t.cbStruct = sizeof(t) ;
        t.pszTitle = L"error" ;
        t.pszText = L"This url is not valid!" ;
        t.ttiIcon = TTI_ERROR ;
        SendDlgItemMessage (IDC_EDIT_URL, EM_SHOWBALLOONTIP, 0, (LPARAM)&t) ;
        return ;
    }

    ClearOutputText() ;
    SetDlgItemText(IDC_TEXT_STATUS, L"Connecting") ;

    m_task_id = AddRequest(m_header) ;
    m_receive.clear() ;

    KillTimer(ID_UPDATE_TIMER) ;

    SetDlgItemText(IDOK, L"Stop") ;
}
//-------------------------------------------------------------------------------------
void DlgDownload::OnSaveToFile()
{
    CFileDialog   dlg (FALSE) ;
    if (dlg.DoModal() == IDOK)
    {
        FCFileEx::Write (dlg.GetPathName(), m_receive.c_str(), (int)m_receive.size()) ;
    }
}
//-------------------------------------------------------------------------------------
void DlgDownload::OnSetProxy()
{
    DlgSetProxy   dlg ;
    dlg.m_info = m_header ;
    if (dlg.DoModal() == IDOK)
    {
        m_header.m_proxy_ip = dlg.m_info.m_proxy_ip ;
        m_header.m_proxy_port = dlg.m_info.m_proxy_port ;
        m_header.m_proxy_username = dlg.m_info.m_proxy_username ;
        m_header.m_proxy_password = dlg.m_info.m_proxy_password ;
    }
}
//-------------------------------------------------------------------------------------
CString DlgDownload::GetFormatKB (LONGLONG nByte)
{
    TCHAR   t[128] ;
    StrFormatKBSize (nByte, t, 128) ;
    return t ;
}
//-------------------------------------------------------------------------------------
void DlgDownload::OnTimer (UINT_PTR nIDEvent)
{
    FCHttpRequest   * pTask = FindRequest(m_task_id) ;
    if (!pTask)
    {
        KillTimer(ID_UPDATE_TIMER) ;
        return ;
    }

    int   living_ms = pTask->GetRunningTime() ;
    int   nDownload = pTask->GetTotalReceiveByte() ;

    if (!living_ms)
        living_ms = 100 ;

    CString   s ;

    // elapse time
    s.Format(L"%d Second", living_ms/1000) ;
    SetDlgItemText (IDC_TEXT_ELAPSE_TIME, s) ;

    // download
    s = GetFormatKB (nDownload) ;

    int   nTotal = pTask->GetResponseInfo().m_content_length ;
    if (nTotal)
    {
        int   nPercent = (int)(100.0 * nDownload / nTotal) ;
        s.AppendFormat (L" (%d%%)", nPercent) ;
        SendDlgItemMessage (IDC_DOWN_PROGRESS, PBM_SETPOS, nPercent) ;
    }
    SetDlgItemText (IDC_TEXT_DOWNLOAD, s) ;

    // average speed
    s = GetFormatKB ((int)(1000.0 * nDownload / living_ms)) ;
    SetDlgItemText (IDC_TEXT_AVERAGE_SPEED, s + L" / S") ;
}
//-------------------------------------------------------------------------------------
void DlgDownload::OnAfterRequestSend (FCHttpRequest& rTask)
{
    const HTTP_RESPONSE_INFO   & resp = rTask.GetResponseInfo() ;

    if (resp.m_status_code == 0)
    {
        SetDlgItemText (IDC_TEXT_STATUS, L"Can't Connect to server") ;
        SetDlgItemText (IDOK, L"Download") ;

        KillTimer(ID_UPDATE_TIMER) ;
        DeleteRequest (rTask.GetTaskID()) ;
        return ;
    }

    // show header
    SetDlgItemText(IDC_TEXT_SEND_HEADER, resp.QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF|HTTP_QUERY_FLAG_REQUEST_HEADERS)) ;
    SetDlgItemText(IDC_TEXT_RESPONSE_HEADER, resp.QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF)) ;

    // change status
    SetDlgItemText (IDC_TEXT_STATUS, L"Reading") ;

    // total length
    CString   s ;
    if (resp.m_content_length)
    {
        s = GetFormatKB (resp.m_content_length) ;
    }
    else
    {
        s = L"Unknow" ;
    }
    SetDlgItemText(IDC_TEXT_TOTAL, s) ;

    SetTimer(ID_UPDATE_TIMER, 500, NULL) ;
}
//-------------------------------------------------------------------------------------
CString DlgDownload::GetStatusText (FCHttpRequest& rTask)
{
    const HTTP_RESPONSE_INFO   & resp = rTask.GetResponseInfo() ;

    CString   s ;
    switch (resp.m_status_code)
    {
        case HTTP_STATUS_OK :
        case HTTP_STATUS_PARTIAL_CONTENT :
            if (resp.m_content_length)
            {
                if (resp.m_content_length == rTask.GetTotalReceiveByte())
                    s = L"Success" ;
                else
                    s = L"Read Error" ;
            }
            else
            {
                if (resp.m_final_read_result)
                    s = L"Success" ;
                else
                    s = L"Read Error" ;
            }

            // range request
            if (rTask.GetRequestHeader().m_start && (resp.m_status_code == HTTP_STATUS_OK))
            {
                s += L" ( not support range )" ;
            }
            break;

        case HTTP_STATUS_NOT_FOUND :
            s = L"404 error : Not Found" ;
            break;

        case HTTP_STATUS_FORBIDDEN :
            s = L"403 error : Forbidden" ;
            break;

        case HTTP_STATUS_PROXY_AUTH_REQ :
            s = L"407 error : Proxy Authentication Required" ;
            break;

        default :
            s.Format(L"Download Error, status code : %d", resp.m_status_code) ;
            break;
    }
    return s ;
}
//-------------------------------------------------------------------------------------
void DlgDownload::OnAfterRequestFinish (FCHttpRequest& rTask)
{
    OnTimer(ID_UPDATE_TIMER) ;

    SendDlgItemMessage (IDC_DOWN_PROGRESS, PBM_SETPOS, 100) ;

    rTask.PopReceived (m_receive) ;

    CString   s = FCCrypt::Get_MD5 (m_receive.c_str(), (int)m_receive.size()) ;
    SetDlgItemText (IDC_TEXT_MD5, s) ;

    SetDlgItemText (IDC_TEXT_STATUS, GetStatusText(rTask)) ;

    SetDlgItemText (IDOK, L"Download") ;
    ::EnableWindow(::GetDlgItem(m_hWnd, IDC_SAVE_TO_FILE), TRUE) ;
}
//-------------------------------------------------------------------------------------
