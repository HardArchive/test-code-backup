//*************************************************************************
//
// Copyright (C), 2009-2010, Handy Information Co.,Ltd,All Rights Reserved
//
// FileName:      ColorComboBox.h  
// Author:        yinxufeng
// Date:		  2012/2012/19
// Description:   自绘ComboBox，改变颜色和大小
// Function List:  
// History:              
//          <author>    	<time>		   <desc>        
//            yinxf          2010/07/13		Build
//
//*************************************************************************

#pragma once

class CColorComboBox
    : public CWindowImpl<CColorComboBox, CComboBox>
    , public COwnerDraw<CColorComboBox>
{
public:
    CColorComboBox(){}
    ~CColorComboBox(){}

protected:
    BEGIN_MSG_MAP(CColorComboBox)
        MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
    	CHAIN_MSG_MAP_ALT(COwnerDraw<CColorComboBox> ,1)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

// Overrideables

    HWND Create(HWND hWndParent, _U_RECT rect, _U_MENUorID MenuOrID = 0U)
    {
        HWND hWnd = __super::Create(hWndParent, rect, NULL, 
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN /*or CBS_DROPDOWNLIST*/
            | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED/* or CBS_OWNERDRAWVARIABLE*/, 
            0, MenuOrID, NULL);

        if ( hWnd )
            _Init();

        return hWnd;
    }

    BOOL SubclassWindow(HWND hWnd)
    {
        ATLASSERT(hWnd);
        ATLASSERT(::IsWindow(hWnd));

        BOOL bRet = __super::SubclassWindow(hWnd);
        if ( bRet )
            _Init();

        ATLASSERT(GetStyle() & CBS_OWNERDRAWVARIABLE);
        ATLASSERT(GetStyle() & CBS_HASSTRINGS);

        return bRet;
    }

    // 自绘下拉列表
    void DrawItem(LPDRAWITEMSTRUCT lpdis)
    {
        if ( (long)lpdis->itemID == -1 )
            return;

        CDCHandle dc(lpdis->hDC);
        CRect rcItem(lpdis->rcItem);
        CString strItem;
        GetLBText(lpdis->itemID, strItem);

        dc.SetBkMode(TRANSPARENT);

        BOOL bSelected = (lpdis->itemState & ODS_SELECTED) ? TRUE : FALSE;
        if ( bSelected )
        {
            dc.FillSolidRect(rcItem, RGB(238, 238, 238));
            dc.SetTextColor(RGB(0, 0, 0));
        }
        else
        {
            dc.FillSolidRect(rcItem, RGB(218, 218, 218));
            dc.SetTextColor(RGB(255, 255, 255));
        }

        dc.DrawText(strItem, strItem.GetLength(), rcItem, DT_LEFT | DT_SINGLELINE);
    }

    // 改变宽度高度
    void MeasureItem(LPMEASUREITEMSTRUCT lpmis)
    {
        if ( (long)lpmis->itemID == -1 )    
            lpmis->itemHeight = 25;  // 文本框(对子类化无效)
        else
            lpmis->itemHeight = 40;  // 列表
    }

// Message Handle
    HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit)
    {
        dc.SetBkMode(TRANSPARENT);
        dc.SetTextColor(RGB(255, 0, 0));
        
        HBRUSH hBrush = ::CreateSolidBrush(RGB(216, 216, 216));
        return hBrush;
    }

// Help
    void _Init()
    {
        // do somethin initialization
    }
};
