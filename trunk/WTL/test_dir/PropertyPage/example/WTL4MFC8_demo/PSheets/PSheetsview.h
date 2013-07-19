// PSheetsView.h : interface of the CPSheetsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSHEETSVIEW_H__E842410D_8680_4C59_8C77_A2DC0DE35697__INCLUDED_)
#define AFX_PSHEETSVIEW_H__E842410D_8680_4C59_8C77_A2DC0DE35697__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CPSheetsView : public CWindowImpl<CPSheetsView>
{
public:
    DECLARE_WND_CLASS(NULL)

    CPSheetsView();
    ~CPSheetsView();

    BOOL PreTranslateMessage(MSG* pMsg);

    BEGIN_MSG_MAP(CPSheetsView)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
    END_MSG_MAP()

    BOOL OnEraseBkgnd(HDC hdc);

    void SetBackgroundOptions ( int nColor, int nPicture );

    // Background painting options
    int m_nColor, m_nPicture;

protected:
    CBitmap m_bmpBkgnd;
    CSize   m_szBkgndBitmap;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSHEETSVIEW_H__E842410D_8680_4C59_8C77_A2DC0DE35697__INCLUDED_)
