// SQLite Test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


class CSQLiteTestApp : public CWinApp
{
public:
    CSQLiteTestApp() { }

	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
    CAboutDlg() { }

	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
};

extern CSQLiteTestApp g_app;