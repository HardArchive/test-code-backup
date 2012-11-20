#pragma once

//-------------------------------------------------------------------------------------
class DlgDownload : public CDialog,
                    public FCHttpRequestManager
{
    enum
    {
        IDC_EDIT_URL = 1200,
        IDC_EDIT_START = 1201,
        IDC_EDIT_AGENT = 1202,
        IDC_EDIT_FIELD = 1203,

        IDC_TEXT_STATUS = 1300,
        IDC_TEXT_TOTAL = 1301,
        IDC_TEXT_DOWNLOAD = 1302,
        IDC_TEXT_AVERAGE_SPEED = 1304,
        IDC_TEXT_ELAPSE_TIME = 1305,
        IDC_TEXT_MD5 = 1306,

        IDC_SAVE_TO_FILE = 1500,
        IDC_SET_PROXY = 1501,
        IDC_DOWN_PROGRESS = 1600,

        IDC_TEXT_SEND_HEADER = 1800,
        IDC_TEXT_RESPONSE_HEADER = 1801,
    };

    enum
    {
        ID_UPDATE_TIMER = 100,
    };

public:
    DlgDownload(CWnd* pParent = NULL) : CDialog(IDD_DOWNLOAD, pParent) {}

private:
    int   m_task_id ;
    std::string   m_receive ;

    HTTP_REQUEST_HEADER   m_header ;

private:
    void ClearOutputText() ;

    static CString GetStatusText (FCHttpRequest& rTask) ;
    static CString GetFormatKB (LONGLONG nByte) ;

    virtual void OnAfterRequestSend (FCHttpRequest& rTask) ;
    virtual void OnAfterRequestFinish (FCHttpRequest& rTask) ;

    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg void OnSaveToFile();
    afx_msg void OnSetProxy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};
