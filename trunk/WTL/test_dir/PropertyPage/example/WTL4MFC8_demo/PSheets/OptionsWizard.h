// OptionsWizard.h: interface for the COptionsWizard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONSWIZARD_H__36088272_239A_46F1_99D8_068FD7B1770F__INCLUDED_)
#define AFX_OPTIONSWIZARD_H__36088272_239A_46F1_99D8_068FD7B1770F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WizIntroPage.h"
#include "WizBkColorPage.h"
#include "WizBkPicturePage.h"
#include "WizFinishPage.h"

class COptionsWizard : public CPropertySheetImpl<COptionsWizard>
{
public:
    // Construction
    COptionsWizard ( HWND hWndParent = NULL );

    // Maps
    BEGIN_MSG_MAP(COptionsWizard)
        MESSAGE_HANDLER_EX(UWM_CENTER_SHEET, OnPageInit)
        CHAIN_MSG_MAP(CPropertySheetImpl<COptionsWizard>)
    END_MSG_MAP()

    // Message handlers
    LRESULT OnPageInit ( UINT uMsg, WPARAM wParam ,LPARAM lParam );

    // Property pages
    CWizIntroPage     m_pgIntro;
    CWizBkColorPage   m_pgBkcolor;
    CWizBkPicturePage m_pgBkpicture;
    CWizFinishPage    m_pgFinish;

    // Implementation
protected:
    bool m_bCentered;
};

#endif // !defined(AFX_OPTIONSWIZARD_H__36088272_239A_46F1_99D8_068FD7B1770F__INCLUDED_)
