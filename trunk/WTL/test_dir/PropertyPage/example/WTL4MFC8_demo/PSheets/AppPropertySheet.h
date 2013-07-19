// AppPropertySheet.h: interface for the CAppPropertySheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPPROPERTYSHEET_H__17A39559_1271_4A86_8A9C_4EA43A47ED2E__INCLUDED_)
#define AFX_APPPROPERTYSHEET_H__17A39559_1271_4A86_8A9C_4EA43A47ED2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BackgroundOptsPage.h"

class CAppPropertySheet : public CPropertySheetImpl<CAppPropertySheet>
{
public:
    // Construction
    CAppPropertySheet ( _U_STRINGorID title = (LPCTSTR) NULL, 
                        UINT uStartPage = 0, HWND hWndParent = NULL );

    // Maps
    BEGIN_MSG_MAP(CAppPropertySheet)
        MESSAGE_HANDLER_EX(UWM_CENTER_SHEET, OnPageInit)
        CHAIN_MSG_MAP(CPropertySheetImpl<CAppPropertySheet>)
    END_MSG_MAP()

    // Message handlers
    LRESULT OnPageInit ( UINT uMsg, WPARAM wParam ,LPARAM lParam );
    
    // Property pages
    CBackgroundOptsPage         m_pgBackground;
    CPropertyPage<IDD_ABOUTBOX> m_pgAbout;

    // Implementation
protected:
    bool m_bCentered;
};

#endif // !defined(AFX_APPPROPERTYSHEET_H__17A39559_1271_4A86_8A9C_4EA43A47ED2E__INCLUDED_)
