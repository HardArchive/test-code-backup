#pragma once

class DlgSetProxy : public CDialog
{
    enum
    {
        IDC_EDIT_IP = 1200,
        IDC_EDIT_PORT = 1201,
        IDC_EDIT_USERNAME = 1202,
        IDC_EDIT_PASSWORD = 1203,
    };
public:
    DlgSetProxy(CWnd* pParent = NULL) : CDialog(IDD_PROXY, pParent)
    {
    }

    HTTP_REQUEST_HEADER   m_info ;

private:
	virtual void DoDataExchange(CDataExchange* pDX)
    {
        DDX_Text(pDX, IDC_EDIT_IP, m_info.m_proxy_ip) ;
        DDX_Text(pDX, IDC_EDIT_PORT, m_info.m_proxy_port) ;
        DDX_Text(pDX, IDC_EDIT_USERNAME, m_info.m_proxy_username) ;
        DDX_Text(pDX, IDC_EDIT_PASSWORD, m_info.m_proxy_password) ;
    }
};
