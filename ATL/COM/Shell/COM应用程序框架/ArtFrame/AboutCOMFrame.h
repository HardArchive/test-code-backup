/********************************************************************
 *	created:	2006/06/18
 *	created:	18:6:2006   12:23
 *	filename: 	f:\Test\ComFrame\ArtFrame\ArtFrame\AboutCOMFrame.h
 *	file path:	f:\Test\ComFrame\ArtFrame\ArtFrame
 *	file base:	AboutCOMFrame
 *	file ext:	h
 *	author:		许宗森
 *	
 *	purpose:	关于 COM MDI应用程序框架类，显示 COM MDI应用程序框架
 *
 *				介绍信息。
 *
*********************************************************************/

#pragma once

#include "resource.h"       // main symbols
#include <atlddx.h>
// CAboutCOMFrame

class CAboutCOMFrame : 
	public CDialogImpl<CAboutCOMFrame>,
	public CWinDataExchange<CAboutCOMFrame>	
{
protected:
	CEdit m_wndEditInfo;
public:
	CAboutCOMFrame();
	~CAboutCOMFrame();
	enum { IDD = IDD_ABOUTCOMFRAME };

    BEGIN_MSG_MAP(CAboutCOMFrame)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
        COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
    END_MSG_MAP()

	BEGIN_DDX_MAP(CAboutCOMFrame)
		DDX_CONTROL_HANDLE(IDC_EDIT1, m_wndEditInfo )
	END_DDX_MAP()
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};


