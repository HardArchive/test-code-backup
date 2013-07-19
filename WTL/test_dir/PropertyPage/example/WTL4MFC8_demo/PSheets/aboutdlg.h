// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__38047412_1217_4B03_937D_81F0A1F470EB__INCLUDED_)
#define AFX_ABOUTDLG_H__38047412_1217_4B03_937D_81F0A1F470EB__INCLUDED_

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
    enum { IDD = IDD_ABOUTBOX };

    BEGIN_MSG_MAP(CAboutDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

#endif // !defined(AFX_ABOUTDLG_H__38047412_1217_4B03_937D_81F0A1F470EB__INCLUDED_)
