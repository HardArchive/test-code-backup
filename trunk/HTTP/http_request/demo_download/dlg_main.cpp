#include "stdafx.h"
#include "http.h"
#include "dlg_main.h"
#include "dlg_download.h"

BEGIN_MESSAGE_MAP(DlgMain, CDialog)
    ON_COMMAND(IDC_OPEN_SIMPLE_DOWNLOAD, OnOpenSimpleDownload)
    ON_NOTIFY(NM_CLICK, IDC_SHOW_LINK, OnClickLink)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------
BOOL DlgMain::OnInitDialog()
{
	__super::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);	// Set big icon
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);	// Set small icon

    ::SetProcessWorkingSetSize (GetCurrentProcess(), (SIZE_T)-1, (SIZE_T)-1) ;
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//-------------------------------------------------------------------------------------
void DlgMain::OnOpenSimpleDownload()
{
    DlgDownload   dlg ;
    dlg.DoModal() ;
}
//-------------------------------------------------------------------------------------
void DlgMain::OnClickLink(NMHDR* pNotifyStruct, LRESULT* pResult)
{
    ::ShellExecute (NULL, L"open", L"http://www.9cpp.com", NULL, NULL, SW_SHOW) ;
}
