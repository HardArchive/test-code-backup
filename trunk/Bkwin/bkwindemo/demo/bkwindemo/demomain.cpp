#include "stdafx.h"
#include <bkwin/bklistview.h>
#include <bkres/bkres.h>

class CDemoMainDlg
    : public CBkDialogImpl<CDemoMainDlg>
{
public:
    CDemoMainDlg()
        : CBkDialogImpl<CDemoMainDlg>(IDR_BK_MAIN_DIALOG)
    {
    }
protected:

    CComboBox m_wndCboxxx;

    BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
    {
		EnableItem( 102,0 );
        m_wndCboxxx.Create(GetViewHWND(), 0, 0, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST, 0, 543);
        m_wndCboxxx.SetFont(BkFontPool::GetFont(BKF_DEFAULTFONT));
        m_wndCboxxx.AddString(L"第一项");
        m_wndCboxxx.AddString(L"第二项");

        return TRUE;
    }

    void OnSysClose()
    {
        EndDialog(IDCLOSE);
    }

    void OnXXTabPage()
    {
        SetTabPageVisible(136, 0, !IsTabPageVisible(136, 0));
    }

    void OnShowMessage()
    {
        CBkSimpleDialog dlg(IDR_BK_SIMPLE_DIALOG);

        int ret = dlg.DoModal();

        CString strRet;
        strRet.Format(L"DoModal returns %d", ret);
        MessageBox(strRet);
    }

    void OnxxxSelChange(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/)
    {
        CString strText;

        m_wndCboxxx.GetLBText(m_wndCboxxx.GetCurSel(), strText);

        SetItemText(101, strText);

        return;
    }

    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
        BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_CLOSE, OnSysClose)
        BK_NOTIFY_ID_COMMAND(100, OnXXTabPage)
        BK_NOTIFY_ID_COMMAND(102, OnShowMessage)
    BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX(CBkDialogImpl<CDemoMainDlg>)
        MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
        CHAIN_MSG_MAP(CBkDialogImpl<CDemoMainDlg>)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_HANDLER_EX(543, CBN_SELCHANGE, OnxxxSelChange)
        REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
#ifdef _DEBUG
//     bkconsole console;
#endif

    BkWinManager bkwinmgr;

    BkFontPool::SetDefaultFont(_T("宋体"), -12);

    CString strPath;

    GetModuleFileName((HMODULE)&__ImageBase, strPath.GetBuffer(MAX_PATH + 10), MAX_PATH);
    strPath.ReleaseBuffer();
    strPath.Truncate(strPath.ReverseFind(L'\\') + 1);
    strPath += L"res";

    BkResManager::SetResourcePath(strPath);

    BkSkin::LoadSkins(IDR_BK_SKIN_DEF);
    BkStyle::LoadStyles(IDR_BK_STYLE_DEF);
    BkString::Load(IDR_BK_STRING_DEF);

    CDemoMainDlg dlg;

    dlg.DoModal(NULL);

    return 0;
}