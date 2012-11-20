#pragma once

//-------------------------------------------------------------------------------------
class DlgMain : public CDialog
{
    enum
    {
        IDC_OPEN_SIMPLE_DOWNLOAD = 1200,
        IDC_SHOW_LINK = 2000,
    };

public:
    DlgMain(CWnd* pParent = NULL) : CDialog(IDD_MAIN, pParent) {}

private:
    virtual BOOL OnInitDialog();

    afx_msg void OnOpenSimpleDownload();
    afx_msg void OnClickLink(NMHDR* pNotifyStruct, LRESULT* pResult) ;
	DECLARE_MESSAGE_MAP()
};
