// BackgroundOptsPage.cpp: implementation of the CBackgroundOptsPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "BackgroundOptsPage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CBackgroundOptsPage::CBackgroundOptsPage() : m_nColor(0), m_nPicture(0)
{
    m_psp.dwFlags |= PSP_USEICONID;
    m_psp.pszIcon = MAKEINTRESOURCE(IDI_TABICON);
    m_psp.hInstance = _Module.GetResourceInstance();
}

CBackgroundOptsPage::~CBackgroundOptsPage()
{
}


//////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CBackgroundOptsPage::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
    GetPropertySheet().SendMessage ( UWM_CENTER_SHEET );

    DoDataExchange(false);
    return TRUE;
}

int CBackgroundOptsPage::OnApply()
{
    return DoDataExchange(true) ? PSNRET_NOERROR : PSNRET_INVALID;
}
