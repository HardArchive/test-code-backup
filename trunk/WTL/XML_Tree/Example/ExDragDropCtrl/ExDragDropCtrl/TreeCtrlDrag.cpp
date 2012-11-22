// TreeCtrlDrag.cpp : implementation file
//

#include "stdafx.h"
#include "TreeCtrlDrag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlDrag

CTreeCtrlDrag::CTreeCtrlDrag()
{
    m_bDragging = FALSE;
    m_pDragImgList = NULL;
    m_hDragItem = NULL;
    m_bTempEditingRequired = FALSE;
    m_nHoverTimerID = 0;
    m_nTimerID = 0;
    m_bNoDuplicatesOnSameLevel = FALSE;

    // Set up the drop target item
    m_bInsertAbove = FALSE;
    m_hDragTarget = NULL;
}

CTreeCtrlDrag::~CTreeCtrlDrag()
{
    // If we'd previously set a timer running, kill it now
	if (m_nHoverTimerID > 0)
	{
		KillTimer(m_nHoverTimerID);
		m_nHoverTimerID = 0;
	}
	if (m_nTimerID > 0)
	{
		KillTimer(m_nTimerID);
		m_nTimerID = 0;
	}
}

BEGIN_MESSAGE_MAP(CTreeCtrlDrag, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlDrag)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
    ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginLabelEdit)
    ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//================================================================================================
// Drawing Message Handlers and Functions
//================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////
// All drawing is done in OnPaint, so do nothing here
BOOL CTreeCtrlDrag::OnEraseBkgnd(CDC* pDC) 
{
    UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// All custom drawing of the tree items
void CTreeCtrlDrag::OnPaint()
{
    CRect rectItem, rectCaption;
    HTREEITEM hItem;
    CString strCaption;
    int n;

    CPaintDC dc(this); // device context for painting

    // Create a memory DC compatible with the paint DC
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);

    CRect rcClip, rcClient;
    dc.GetClipBox(&rcClip);
    GetClientRect(&rcClient);

    // Select a compatible bitmap into the memory DC
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
    memDC.SelectObject(&bitmap);

    // Set clip region to be same as that in paint DC
    CRgn rgn;
    rgn.CreateRectRgnIndirect(&rcClip);
    memDC.SelectClipRgn(&rgn);
    rgn.DeleteObject();

    // First let the control do its default drawing.
    CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

    // Only take control of the drawing if an item is being dragged.  In this case, clear all 
    // formatting except for the dragged item and the target location (line)
    if (m_bDragging)
    {
        // Start by erasing all backgrounds (we do this first as when we draw the target location
        // line it is infact a thin line that spans the edges of two items (is the mid point) so 
        // we don't want the erase for the latter item rubbing out the mid line we've just tried 
        // to draw.
        hItem = GetFirstVisibleItem();
        n = GetVisibleCount() + 1;
        while (hItem && n--)
        {
            GetItemRect(hItem, &rectItem, TRUE);
            rectItem.right = rcClient.right;
            CBrush brushBckgrnd(GetSysColor(COLOR_WINDOW));
            memDC.FillRect(rectItem, &brushBckgrnd);
            // Move on to the next item
            hItem = GetNextVisibleItem(hItem);
        }

        // Now replace all the items 
        DWORD  dwFormat = DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS | DT_NOCLIP;

        hItem = GetFirstVisibleItem();
        n = GetVisibleCount() + 1;
        while (hItem && n--)
        {
            GetItemRect(hItem, &rectItem, TRUE);
 
            // Get caption text
            strCaption = GetItemText(hItem);
            // Setup caption context
            CFont* pOldFont = memDC.SelectObject(GetFont());
            memDC.SetBkMode (TRANSPARENT);
            memDC.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

            // Now resize box according to how the text fits in
            rectCaption = rectItem;
            // Move caption down and to the right which is how it is drawn by default
            rectCaption.left += 2;
            rectCaption.top  += 1;
            rectCaption.right += 2;

            // If tree is narrower than items, resize item so end of caption is replaced with 
            // ellipsis (...)
            if (rectItem.right > (rcClient.right - MARGIN_PIXELS))
            {
                // Allow for the margin after the caption
                rectItem.right = rcClient.right - MARGIN_PIXELS;
            }

            // Fix caption rect to caption size
            memDC.DrawText(strCaption, &rectCaption, dwFormat | DT_CALCRECT);
            // Fix item rect right extents to fit caption + a margin
            rectItem.right = rectCaption.right + MARGIN_PIXELS;

            // Add the focus bar if item is selected
            if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
                memDC.DrawFocusRect(rectItem);

            if (m_bDragging && (hItem == m_hDragTarget))
            {
                // There is one tricky situation where the target item is a folder but with no
                // children and the insertion point is below it - i.e. it should be added as the
                // first child.  
                if (IsItemEmptyFolder(hItem) && !m_bInsertAbove)
                {
                    TRACE(_T("Target is inside an empty folder\n"));
                    // This is an empty folder, so we need to draw the line where a child item
                    // would go.  To do this we need to find a parent child combination and get 
                    // the difference in indents.
                    CRect rectDummy(rectItem);                    
                    int nExtraIndent = GetParentToChildIndent();
                    rectDummy.left += nExtraIndent;
                    DrawDropTargetLine(rectDummy, &memDC);
                }
                else
                {
                    // This is the target item, so draw the target location line either above or below
                    // it as appropriate.
                    DrawDropTargetLine(rectItem, &memDC);
                }
            }

            // Draw caption
            memDC.DrawText(strCaption, &rectCaption, dwFormat);

            // Free font
            if (pOldFont != NULL)
                memDC.SelectObject (pOldFont);

            // Move on to the next item
            hItem = GetNextVisibleItem(hItem);
        }
    }

    dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC,
              rcClip.left, rcClip.top, SRCCOPY);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Draws a line where the dragged item will be dropped
void CTreeCtrlDrag::DrawDropTargetLine(CRect rect, CDC* pDC)
{
    CRect rectTree;
    GetClientRect(rectTree);

    // Extend the line from the start of this item to the edge of the tree control
    rect.right = rectTree.right - MARGIN_PIXELS;

    // Draw the line at the top or bottom of the rect depending on whether the dragged item is 
    // being inserted before or after this item
    ASSERT(rect.Height() >= 6);
    if (m_bInsertAbove)
    {
        rect.bottom = rect.top + 3;
        rect.top = rect.top - 3;
    }
    else
    {
        rect.top = rect.bottom - 3;
        rect.bottom += 3;
    }

    CPen  penBorder;
    CPen* pOldPen;
    VERIFY(penBorder.CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
    pOldPen = pDC->SelectObject(&penBorder);
    pDC->MoveTo(rect.left, rect.bottom);
    pDC->LineTo(rect.left, rect.top);
    pDC->MoveTo(rect.left + 1, rect.bottom - 1);
    pDC->LineTo(rect.left + 1, rect.top + 1);
    pDC->MoveTo(rect.left + 2, rect.bottom - 2);
    pDC->LineTo(rect.right - 1, rect.bottom - 2);
    pDC->MoveTo(rect.left + 2, rect.bottom - 3);
    pDC->LineTo(rect.right - 1, rect.bottom - 3);
    pDC->MoveTo(rect.right - 1, rect.bottom - 1);
    pDC->LineTo(rect.right - 1, rect.top + 1);
    pDC->MoveTo(rect.right, rect.bottom);
    pDC->LineTo(rect.right, rect.top);
    pDC->SelectObject(pOldPen);
}

//================================================================================================
// Other Mouse Message Handlers
//================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////
// Begin drag
void CTreeCtrlDrag::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    m_hDragItem = pNMTreeView->itemNew.hItem;
    if (m_hDragItem == NULL)
        return;

    SelectItem(m_hDragItem);
    m_pDragImgList = CreateDragImage(m_hDragItem);
    if (m_pDragImgList == NULL)
        return;
  
    // Draw the drag sprite approximately centred on the old cursor position (i.e. shift the top 
    // left cornercloser to the vertical middle of the line.
    m_pDragImgList->BeginDrag(0, CPoint(0, Y_SPRITE_OFFSET));
    m_pDragImgList->DragEnter(this, pNMTreeView->ptDrag);
    m_pDragImgList = m_pDragImgList;                   // for CTreeCtrlDrag::OnMouseWheel
    m_bDragging = true;

    ShowCursor(false);
    SetCapture();
    m_hDragTarget = NULL;

    m_nTimerID = SetTimer(1,25,NULL);
    *pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// As the mouse is moving around, check whether the user is hovering over a folder, in which case
// set a timer which may in due course expand/collapse it.
void CTreeCtrlDrag::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM hItem;
	UINT      flags;

    // If we'd previously set a timer running, kill hit with this move.
	if (m_nHoverTimerID > 0)
	{
		KillTimer(m_nHoverTimerID);
		m_nHoverTimerID = 0;
	}

    // If we are dragging a tree item, start the timer and set this drop item
	if (m_bDragging)
	{
		m_pDragImgList->DragMove(point);

		m_nHoverTimerID = SetTimer(2, 1000, NULL);
		m_ptHoverPoint = point;

        hItem = HitTest(point, &flags);
        HTREEITEM hNextDragTarget;
        BOOL bNextInsertAbove;
		if (hItem != NULL)
		{
			m_pDragImgList->DragLeave(this);
            hNextDragTarget = GetDragTarget(hItem, point, bNextInsertAbove);
            if ((hNextDragTarget != m_hDragTarget) || (m_bInsertAbove != bNextInsertAbove))
            {
                // If we've moved onto a new item, redraw the window
                SelectDropTarget(hNextDragTarget);
                m_hDragTarget = hNextDragTarget;
                m_bInsertAbove = bNextInsertAbove;
                EnsureVisible(m_hDragTarget);
                RedrawWindow();
            }
			m_pDragImgList->DragEnter(this, point);
        }

        // There is a bug in the drawing code which means when not all the text of an item fits in
        // the tree, a yellow box pops up and shows what it is when the mouse is over it, but when 
        // we're dragging, the drawing competes and the scroll bar isn't refreshed properly, so if
        // both scroll bars are present, refresh them whenever the mouse is moved.  This doesn't 
        // fix the problem completely but makes it much better - bit of a hack though.
        if ((GetScrollPos(SB_HORZ) > 0) && (GetScrollLimit(SB_VERT) > 0))
        {
            Invalidate();
        }

        m_pDragImgList->DragShowNolock(TRUE);
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Depending on where the mouse is over this item and what children it has, work out whether the 
// drag target item is this or the one below and where the drop should be relative to the drag 
// target (i.e. above or below).
HTREEITEM CTreeCtrlDrag::GetDragTarget(HTREEITEM hItem, POINT point, BOOL& bInsertAbove)
{
    ASSERT(hItem != NULL);
    HTREEITEM hDragTarget;

    // Is the mouse in the top or bottom half of this item?
    CRect rectItem;
    GetItemRect(hItem, &rectItem, FALSE);
    if (point.y < rectItem.CenterPoint().y)
    {
        // The mouse is in the top half of this item, so this will definitely be the target drop
        // item with the drop being above this item.
        hDragTarget = hItem;
        bInsertAbove = TRUE;
    }
    else
    {
        // The mouse is in the bottom half of this item, so if it has visible children, we 
        // actually pass the target to the item below as it has the correct indent for the target
        if (ItemHasChildren(hItem) && (GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED))
        {
            hDragTarget = GetChildItem(hItem);    
            bInsertAbove = TRUE;
        }
        else
        {
            hDragTarget = hItem;
            bInsertAbove = FALSE;
        }
    }
    ASSERT(hDragTarget != NULL);
    return hDragTarget;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// When the target changes, the corresponding parts of the tree are redrawn to unselect the old 
// target and highlight the new one. But a picture of the item being moved is displayed on top of 
// the tree as a sprite, that is to say the background it covers is saved in order to be restored 
// when it will move again. If the background changes without the picture knowing it, this can 
// lead to this kind of undesirable "trails".
// Every function call that modifies the display of the tree (like OnMouseWheel) must be preceded 
// by DragShowNolock(false) which removes the picture of the item being moved, and in general 
// followed by a DragShowNolock(true) unless you don't want to show the picture again.
BOOL CTreeCtrlDrag::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    BOOL bResult;
    if (m_pDragImgList != NULL)
        m_pDragImgList->DragShowNolock(false);

    bResult = CTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);

    if (m_pDragImgList != NULL)
        m_pDragImgList->DragShowNolock(true);
    return bResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// When the target changes, the corresponding parts of the tree are redrawn to unselect the old 
void CTreeCtrlDrag::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (m_bDragging)
        CompleteDrag(TRUE);
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Right mouse button down allows the user to abort the drag.
void CTreeCtrlDrag::OnRButtonDown(UINT nFlags, CPoint point) 
{
    if (m_bDragging)
        CompleteDrag(FALSE);
    // Select this item in case we want to add some right mouse processing.
	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);
    SelectItem(hItem);
	CTreeCtrl::OnRButtonDown(nFlags, point);
}


//================================================================================================
// Timer Message Handler
//================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////
// Timer is required to handle hover folder expansions and then highlighting & auto scrolling when 
// mouse is moved above control
void CTreeCtrlDrag::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nHoverTimerID)
	{
		KillTimer(m_nHoverTimerID);
		m_nHoverTimerID = 0;

        HTREEITEM hItem = 0;
		UINT uFlag = 0;
		hItem = HitTest(m_ptHoverPoint, &uFlag);
		if (hItem != NULL)
		{
            m_pDragImgList->DragShowNolock(FALSE);
			Expand(hItem, TVE_TOGGLE);
            // Once we've expanded/contracted, the position of the target item might have changed
            m_hDragTarget = GetDragTarget(hItem, m_ptHoverPoint, m_bInsertAbove);
    		SelectDropTarget(m_hDragTarget);
            // Finally redraw everything
            RedrawWindow();
            m_pDragImgList->DragShowNolock(TRUE);
        }
	}
    else if (m_bDragging)
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(&point);

        // Highlight target
        TVHITTESTINFO tvHit;
        tvHit.pt = point;
        HTREEITEM hTarget = HitTest(&tvHit);

        if (hTarget != NULL)
        {
            // The target item isn't necessarily the one the mouse is hovering over, so do the 
            // full check relative to the mouse position within the item.
            BOOL bInsertAbove;
            hTarget = GetDragTarget(hTarget, point, bInsertAbove);
            if (hTarget != m_hDragTarget)
            {                                                     // this test avoids flickering
                m_pDragImgList->DragShowNolock(false);
                EnsureVisible(hTarget);
                SelectDropTarget(hTarget);
                m_pDragImgList->DragShowNolock(true);
                m_hDragTarget = hTarget;
                m_bInsertAbove = bInsertAbove;
            }
        }
        else
        {
            // Scroll tree
            RECT rect;
            GetClientRect(&rect);

            int nMaxVScrollPos = GetScrollLimit(SB_VERT);
 	        int nCurrentVScrollPos = GetScrollPos(SB_VERT);

            // UP - Is the mouse above the tree control and the scroll bar not at zero?
            if ((point.y < (rect.top - OUT_OF_CONTROL_MARGIN)) && (nCurrentVScrollPos > 0))
            {
                HTREEITEM hPrev = GetPrevVisibleItem(GetFirstVisibleItem());
                m_pDragImgList->DragShowNolock(false);
                EnsureVisible(hPrev);
                // Update the target drop position
                m_hDragTarget = GetDragTarget(hPrev, point, m_bInsertAbove);
    		    SelectDropTarget(m_hDragTarget);
                RedrawWindow();
                m_pDragImgList->DragShowNolock(true);
            }
            // DOWN - Is the mouse below the control
            else if ((point.y > (rect.bottom + OUT_OF_CONTROL_MARGIN)) && 
                     (nCurrentVScrollPos != nMaxVScrollPos))
            {
                UINT nVisibleCount = GetVisibleCount();
                if (nVisibleCount != -1)
                {
                    HTREEITEM hNext = GetFirstVisibleItem();
                    for(UINT i = 0; i < nVisibleCount; i++) 
                        hNext = GetNextVisibleItem(hNext);
                    m_pDragImgList->DragShowNolock(false);
                    EnsureVisible(hNext);
                    // Update the target drop position
                    m_hDragTarget = GetDragTarget(hNext, point, m_bInsertAbove);
    		        SelectDropTarget(m_hDragTarget);
                    RedrawWindow();
                    m_pDragImgList->DragShowNolock(true);
                }
            }

            int nCurrentHScrollPos = GetScrollPos(SB_HORZ);
            int nMaxHScrollPos = GetScrollLimit(SB_HORZ);
            // LEFT
            if((point.x < rect.left) && (nCurrentHScrollPos > 0))
            {
                m_pDragImgList->DragShowNolock(false);
                SendMessage(WM_HSCROLL,SB_LINELEFT);
                RedrawWindow();
                m_pDragImgList->DragShowNolock(true);
            }
            // RIGHT
            else if((point.x > rect.right) && (nCurrentHScrollPos != nMaxHScrollPos))
            {
                m_pDragImgList->DragShowNolock(false);
                SendMessage(WM_HSCROLL,SB_LINERIGHT);
                RedrawWindow();
                m_pDragImgList->DragShowNolock(true);
            }
        }
        m_pDragImgList->DragMove(point);
    }
	    
    CTreeCtrl::OnTimer(nIDEvent);
}


//================================================================================================
// Label Editing Messages
//================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////
// There may be certain tree items (such as the root) that the calling dlg doesn't want the user 
// to be able to edit, so give the calling dlg the chance to cancel.
void CTreeCtrlDrag::OnBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Send a message back up to the parent informing of a successful move.  If they want to abort
    // the edit, they need to set the *pResult value to 1.
    ASSERT((sizeof(NMHDR*) == sizeof(WPARAM)) && (sizeof(LRESULT*) == sizeof(LPARAM)));
    GetParent()->SendMessage(UWM_TREEBEGINEDIT, (WPARAM)pNMHDR, (LPARAM)pResult);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Save the new user-defined text value for the tree item if the user has left the control or 
// clicked RETURN.  Revert back to the previous text if the user has hit ESC.
void CTreeCtrlDrag::OnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    HTREEITEM hItem = pTVDispInfo->item.hItem;

    CString str = pTVDispInfo->item.pszText;
    // Trim unnecessary spaces from the start and end
    str.TrimLeft();
    str.TrimRight();
    // If the edit control string is not empty, the user has finished editing successfully
    if (!str.IsEmpty())
    {
        BOOL bGoingOK = TRUE;
        // If required, check there are no duplicates of this string
        if (m_bNoDuplicatesOnSameLevel)
        {
            if (!IsUniqueNameOnThisLevel(str, hItem, GetParentItem(hItem)))
            {
                // An item with the same name was found, so continue editing the label
                AfxMessageBox(_T("Invalid name - an item with the same name already exists in this location."), MB_OK | MB_ICONEXCLAMATION);
                bGoingOK = FALSE;
                EditLabel(hItem);
            }
        }
        if (bGoingOK)
        {
            // Label edited successfully so inform the calling app.
            SetItemText(hItem, str);
            GetParent()->SendMessage(UWM_TREEENDEDIT, (WPARAM)hItem, 0);
        }
    }
    *pResult = 0;

    // If we've had to enable editing specifically for this item, we need to disable it again now
    // the editing has finished.
    if (m_bTempEditingRequired)
    {
        DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
        dwStyle &= ~TVS_EDITLABELS;
        SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Various keyboard inputs need to be managed to get correct working of the edit label process
BOOL CTreeCtrlDrag::PreTranslateMessage(MSG* pMsg) 
{
    BOOL bProcessAsNormal = TRUE;
    BOOL bResult = TRUE;

    if (pMsg->message == WM_KEYDOWN)
    {
        CEdit* pEdit = GetEditControl();
        if (pEdit != NULL)
        {
            // If we are currently editing a label, the keyboard strokes need to go to that, 
            // unless they're RETURN or ESCAPE which end the control editing.
            if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
            {
                pEdit->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
                bProcessAsNormal = FALSE;
            }
            else if ((GetKeyState(VK_CONTROL) & 0x8000) && 
                     ((pMsg->wParam == TCHAR('X')) || (pMsg->wParam == TCHAR('x'))))
            {
#pragma message("Why are CTRL C and CTRL V (but not CTRL X) both disabled?  Don't know.")
                // This is a bit of a hack...CTRL-X should work automatically using the 
                // accelerators but for some reason it isn't in some situations, so we have to
                // trap it explicitly here.
                pEdit->Cut();
                bProcessAsNormal = FALSE;
            }
        }
        else if ((pMsg->wParam == VK_F2) && (GetWindowLong(m_hWnd, GWL_STYLE) & TVS_EDITLABELS))
        {
            HTREEITEM hItem = GetSelectedItem();
            // Try to edit this item and then at the end we still call the normal message
            EditLabel(hItem);
        }
    }
    
    if (bProcessAsNormal)
    {
        // Otherwise handle as normal
        bResult = CTreeCtrl::PreTranslateMessage(pMsg);
    }

    return bResult;
}


//================================================================================================
// External Interaction
//================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////
// The user has finished the drag
void CTreeCtrlDrag::CompleteDrag(BOOL bLeftBtnUp)
{
    TidyUpEndOfDrag();
    if (bLeftBtnUp)
    {
        // User has released the left button which signals the drag should be completed
        if (m_hDragTarget != NULL)
            SuccessfulDrag();
    }
    else
    {
        // The User chose to abort the drag
        EnsureVisible(m_hDragItem);
        // Refresh the screen to get rid of any remnants of the drag drawing
        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Is the selected item at the top of the list (i.e. the Up button should be disabled)
BOOL CTreeCtrlDrag::ShouldUpBtnBeEnabled()
{
    HTREEITEM hSelectedItem = GetSelectedItem();
    
    // If no item was selected, select the root - if there isn't one it doesn't matter.
    if (hSelectedItem == NULL)
    {
        SelectItem(GetRootItem());   
        return FALSE;
    }
    return (hSelectedItem != GetRootItem());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// The Down button should be enabled unless the selected item is the last in the list which is at
// the root level, otherwise part of the purpose of the down button is to move items out of 
// folders and up levels in the sub-folder hierachy.
BOOL CTreeCtrlDrag::ShouldDownBtnBeEnabled()
{
    HTREEITEM hSelectedItem = GetSelectedItem();
    // If no item was selected, select the root
    if (hSelectedItem == NULL)
    {
        SelectItem(GetRootItem());  
        // Now enable the button if we've more than jus the root item - if the tree is empty, we
        // obviously leave it disabled.
        return (GetCount() > 1);
    }

    // Does this item have siblings lower in the tree?
    if (GetNextSiblingItem(hSelectedItem) != NULL)
        return TRUE;
    // Is this at the highest level?
    if (GetParentItem(hSelectedItem) != NULL)
        return TRUE;

    // This is the last item.
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Move the selected item up.
void CTreeCtrlDrag::MoveItemUp()
{
    HTREEITEM hParent = NULL;
    SetRedraw(FALSE);
    // Remember the first and last visible items before we start adding and deleting items
    SaveFirstAndLastVisibleItems();
    // Set the selected item as the drag item and the item above as the target and call the normal
    // drag routine (the only exception is when we're moving up to an open folder in which case
    // our first upward move is actually into the folder, but on the end).
    m_hDragItem = GetSelectedItem();
    m_hDragTarget = GetPrevVisibleItemToMoveTo(m_hDragItem, m_bInsertAbove, hParent);

    BOOL bGoingOK = TRUE;
    if (m_bNoDuplicatesOnSameLevel && 
        ((hParent != TVI_ROOT) || (GetParentItem(m_hDragItem) != NULL)) &&
        (hParent != GetParentItem(m_hDragItem)))
    {
        // We're moving the item to a new folder, so check the destination location to see if 
        // there is already an item with this name
        if (!IsUniqueNameOnThisLevel(GetItemText(m_hDragItem), m_hDragItem, hParent))
        {
            // Found a duplicate item, so abort this move
            AfxMessageBox(_T("Invalid move - an item with the same name has been found in the destination location."), MB_OK | MB_ICONEXCLAMATION);
            bGoingOK = FALSE;
        }
    }

    if (bGoingOK)
    {
        // We're good to proceed
        HTREEITEM hNew = InsertItemAndSubtree(hParent);
        if (hNew != NULL) 
        {
            // Switch focus to the new item - start redrawing before EnsureVisible() to make it work
            SelectItem(hNew);
            DeleteItem(m_hDragItem);
            // Fix the visibility, making sure, if possible, we can see at least one place above the 
            // move visible, in case the user wants to keep moving upwards which is quite likely.
            ResetVisibleItems(hNew, m_hDragItem, GetPrevVisibleItem(hNew));
            // Send a message back up to the parent informing of a successful move
            GetParent()->SendMessage(UWM_TREEMOVECOMPLETE, (WPARAM)m_hDragItem,(LPARAM)hNew);
        }
    }
    SetRedraw(TRUE);
    Invalidate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Move the selected item up.  Send a message with the old and new tree item ids for the moved 
// item if successful.
void CTreeCtrlDrag::MoveItemDown()
{
    SetRedraw(FALSE);
    // Remember the first and last visible items before we start adding and deleting items
    SaveFirstAndLastVisibleItems();
    // Set the selected item as the drag item and the item below as the target and call the normal
    // drag routine (the only exception is when we're moving out of an open folder in which case
    // our first downward move is simply out of the folder, but not moving past any other items).
    m_hDragItem = GetSelectedItem();
    m_hDragTarget = GetNextNonChildItem(m_hDragItem, m_bInsertAbove);

    // If the new target is an empty folder, we actually want to put the item in the folder.
    HTREEITEM hParent = GetParentItem(m_hDragTarget);
    if (IsItemEmptyFolder(m_hDragTarget) && !m_bInsertAbove)
        hParent = m_hDragTarget;
    if (hParent == NULL)
        hParent = TVI_ROOT;

    BOOL bGoingOK = TRUE;
    if (m_bNoDuplicatesOnSameLevel && 
        ((hParent != TVI_ROOT) || (GetParentItem(m_hDragItem) != NULL)) &&
        (hParent != GetParentItem(m_hDragItem)))
    {
        // We're moving the item to a new folder, so check the destination location to see if 
        // there is already an item with this name
        if (!IsUniqueNameOnThisLevel(GetItemText(m_hDragItem), m_hDragItem, hParent))
        {
            // Found a duplicate item, so abort this move
            AfxMessageBox(_T("Invalid move - an item with the same name has been found in the destination location."), MB_OK | MB_ICONEXCLAMATION);
            bGoingOK = FALSE;
        }
    }

    if (bGoingOK)
    {
        // We're good to go
        HTREEITEM hNew = InsertItemAndSubtree(hParent);
        if (hNew != NULL) 
        {
            // Switch focus to the new item - start redrawing before EnsureVisible() to make it work
            SelectItem(hNew);
            DeleteItem(m_hDragItem);
            // Fix the visibility, making sure, if possible, we can see at least one place above the 
            // move visible, otherwise the ResetVisibleItems() function keeps the moving item at the 
            // top.
            ResetVisibleItems(hNew, m_hDragItem, GetPrevVisibleItem(hNew));
            // Send a message back up to the parent informing of a successful move
            GetParent()->SendMessage(UWM_TREEMOVECOMPLETE, (WPARAM)m_hDragItem,(LPARAM)hNew);
        }
    }
    SetRedraw(TRUE);
    Invalidate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Create a new folder item below the selected location, just inside this folder if the selected
// item is a folder.  To allow new folders to be created and their labels edited in a tree that
// would otherwise not allow label editing, we have to temporarily change the tree style and 
// change it back once the editing has finished in OnEndLabelEdit().  Returns the newly created 
// tree item.
HTREEITEM CTreeCtrlDrag::CreateNewFolder()
{
    HTREEITEM hPrevItem;
    HTREEITEM hParent;
    if (GetCount() == 0)
    {
        // The tree is empty, so add this as the first item
        hParent = TVI_ROOT;
        hPrevItem = TVI_FIRST;
    }
    else if (IsSelectedItemAFolder())
    {
        // We're adding a sub-folder - add it first.
        hParent = GetSelectedItem();
        hPrevItem = TVI_FIRST;
    }
    else
    {
        // We're simply adding a folder below the selected item at the same level.
        hPrevItem = GetSelectedItem();
        hParent = GetParentItem(hPrevItem);
        if (hParent == NULL)
            hParent = TVI_ROOT;
    }
    // Create and then start editing the label, ensuring the tree has Editing enabled.  
    // NOTE: if we have to force editing to be enabled, we must disable it again once the the 
    // label has been edited in OnEndLabelEdit().
    DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
    m_bTempEditingRequired = !(dwStyle & TVS_EDITLABELS);
    if (m_bTempEditingRequired)
        SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | TVS_EDITLABELS);

    // Set the folder name
    CString strNewFolder = FOLDER_BASE_NAME;
    if (m_bNoDuplicatesOnSameLevel)
    {
        // Give the folder a unique name
        int nIndex = 1;
        while (!IsUniqueNameOnThisLevel(strNewFolder, NULL, hParent))
            strNewFolder.Format("%s %d", FOLDER_BASE_NAME, nIndex++);
    }

    HTREEITEM hNew = InsertItem(strNewFolder, FOLDER_IMAGE, FOLDER_IMAGE, hParent, hPrevItem);
    SelectItem(hNew);
    EnsureVisible(hNew);
    EditLabel(hNew);
    return hNew;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Bring up the rename edit control for the selected item
void CTreeCtrlDrag::RenameSelected()
{
    EditLabel(GetSelectedItem());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Delete the selected item.  Send a message back to the parent to let them know tree item has
// been deleted successfully
void CTreeCtrlDrag::DeleteSelected()
{
    HTREEITEM hOld = GetSelectedItem();
    if (ItemHasChildren(hOld))
    {
        int nRet = AfxMessageBox(_T("You have chosen to delete a folder.  Please confirm you want to delete this folder and all items in it?"), MB_YESNO);
        if (nRet != IDYES)
        {
            return;     // User has changed their mind
        }
    }
    if (hOld != NULL)
    {
        // Use recursive function to callback to parents on all items being deleted.
        ReportDeleteBackToParent(hOld);
        DeleteItem(hOld);
    }
}


//================================================================================================
// Internal Utilities
//================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////
// Instructions to close down the drag procedure
void CTreeCtrlDrag::TidyUpEndOfDrag()
{
    // Kill all timers
    KillTimer(m_nTimerID);
    m_nTimerID = 0;
    KillTimer(m_nHoverTimerID);
    m_nHoverTimerID = 0;

    // Return to the normal mouse cursor
    ReleaseCapture();
    ShowCursor(true);
    m_bDragging = false;

    m_pDragImgList->DragLeave(this);
    m_pDragImgList->EndDrag();
    if (m_pDragImgList == NULL)
        delete m_pDragImgList;
    m_pDragImgList = NULL;
    SelectDropTarget(NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Check this is a valid drag then call into the tree control to process the move
void CTreeCtrlDrag::SuccessfulDrag()
{
    BOOL bGoingOK = TRUE;
    // Remember the first and last visible items before we start adding and deleting items
    SaveFirstAndLastVisibleItems();

    // Check this isn't a scenario where nothing needs to be done, e.g. we're dropping this item
    // after the preceding item, i.e. on the same spot.
    if ((m_hDragTarget == m_hDragItem) ||
        (m_bInsertAbove && (GetNextSiblingItem(m_hDragItem) == m_hDragTarget)) ||
        (!m_bInsertAbove && (GetPrevSiblingItem(m_hDragItem) == m_hDragTarget) && 
         !IsItemEmptyFolder(m_hDragTarget)))
    {
        TRACE("Drag aborted as item won't move anywhere\n");
        bGoingOK = FALSE;
    }
    else
    {
        // If m_hDragTarget is a child of m_hDragItem, we're going to have a stack overflow 
        // (infinite loop!)
        HTREEITEM hParent = m_hDragTarget;
        while (hParent != NULL)
        {
            if(hParent == m_hDragItem)
            {
                AfxMessageBox("Invalid drag & drop - the destination location can't be within the dragged item's sub-folder!", MB_ICONEXCLAMATION);
                bGoingOK = FALSE;
                break;
            }
            hParent = GetParentItem(hParent);
        }
    }

    HTREEITEM hParent;
    if (bGoingOK)
    {
        // We're good to proceed - to prevent flicker stop the drawing temporarily
        SetRedraw(FALSE);            
        // If the indended target is as a child of an empty folder, specify the folder as the 
        // parent when we call to create the new item.
        if (IsItemEmptyFolder(m_hDragTarget) && !m_bInsertAbove)
            hParent = m_hDragTarget;
        else
        {
            hParent = GetParentItem(m_hDragTarget);
            if (hParent == NULL)
                hParent = TVI_ROOT;
        }

        // Check we're not going to foul up on an item in the target folder with the same name
        if (m_bNoDuplicatesOnSameLevel && (hParent != GetParentItem(m_hDragItem)))
        {
            // We're moving the item to a new folder, so check the destination location to see if 
            // there is already an item with this name
            if (!IsUniqueNameOnThisLevel(GetItemText(m_hDragItem), m_hDragItem, hParent))
            {
                // Found a duplicate item, so abort this move
                AfxMessageBox(_T("Invalid drag & drop - an item with the same name has been found in the destination location."), MB_OK | MB_ICONEXCLAMATION);
                bGoingOK = FALSE;
            }
        }

        if (bGoingOK)
        {
            // We're finally ready to insert the actual item.
            HTREEITEM hNew = InsertItemAndSubtree(hParent);
            if (hNew != NULL) 
            {
                // Successful swap, so send a message to our parent window in case they care for all 
                // moved items (i.e. any children too)
                ReportDragBackToParent(hNew, m_hDragItem);
                // Switch focus to the new item
                SelectItem(hNew);
                DeleteItem(m_hDragItem);
                // Fix the visibility.
                ResetVisibleItems(hNew, m_hDragItem);
            }
        }
    }
    // Regardless of what happens, we need to reset the tree.
    SetRedraw(TRUE);            
    Invalidate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Create the dummy item under the passed in parent if applicable (otherwise if it's NULL, assume
// it is the same parent as the target).
HTREEITEM CTreeCtrlDrag::InsertItemAndSubtree(HTREEITEM hParent /* = NULL */)
{
    // Sanity check input tree items
    if ((m_hDragTarget == NULL) || (m_hDragItem == NULL)) 
    {
        ASSERT(FALSE);
        return NULL;
    }

    // Unless specified as an input parameter, we assume the target will be on the same level, so 
    // get the insert parent from the target.
    if (hParent == NULL)
        hParent = GetParentItem(m_hDragTarget);
    if (hParent == NULL)
        hParent = TVI_ROOT;

    HTREEITEM hInsertAfter;
    if (m_bInsertAbove)
        hInsertAfter = GetPrevSiblingItem(m_hDragTarget);
    else
        hInsertAfter = m_hDragTarget;

    // In the case where we're inserting the new item as the child of the target we need to reset
    // the 'insert after' item (of if there was no previous sibling)
    if ((hInsertAfter == hParent) || (hInsertAfter == NULL))
        hInsertAfter = TVI_FIRST;

    HTREEITEM hNew = InsertItem("dummy", hParent, hInsertAfter);
    if (hNew == NULL)
        return NULL;

    // Start with subtree so that item is correctly expanded
    CopySubtree(hNew, m_hDragItem);
    CopyItem(hNew, m_hDragItem);

    return hNew;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Recursively copy an entire branch of a tree
void CTreeCtrlDrag::CopySubtree(HTREEITEM hDest, HTREEITEM hSrc)
{
    ASSERT((hDest != NULL) && (hSrc != NULL));

    HTREEITEM hChildSrc = GetChildItem(hSrc);
    while(hChildSrc)
    {
        HTREEITEM hChildDest = InsertItem("dest child", hDest);
        CopySubtree(hChildDest,hChildSrc);
        CopyItem(hChildDest,hChildSrc);

        hChildSrc = GetNextSiblingItem(hChildSrc);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Copy all the attributes of a single item across
void CTreeCtrlDrag::CopyItem(HTREEITEM hDest, HTREEITEM hSrc)
{
    TV_ITEM tvSrc;
    tvSrc.mask  = TVIF_HANDLE | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE;
    tvSrc.hItem = hSrc;
    if (!GetItem(&tvSrc)) 
        return;

    tvSrc.hItem = hDest;
    SetItem(&tvSrc);
    SetItemText(hDest,GetItemText(hSrc));
    SetCheck(hDest,GetCheck   (hSrc));

    if (tvSrc.state & TVIS_EXPANDED) 
        Expand(hDest,TVE_EXPAND);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// If the calling dialog is keeping track of items by their HTREEITEM code, it will need to remap
// all dragged items that have been deleted and recreated.  So, for each child under the dragged
// item hNew, send a message up to the parent with this new, and the corresponding old, HTREEITEM.
void CTreeCtrlDrag::ReportDragBackToParent(HTREEITEM hNew, HTREEITEM hOld)
{
    // We'll use the basic functionality of the GetNextItem() code but with a few small changes. 
    // As the new vs old dragged item and it's children should be identical mirrors, assert that
    // the two trees have the same structure at every point and mirror all movement up/down the 
    // tree in the two branches.
    // Start with the top level dragged items
    HTREEITEM hNextNewItem = GetChildItem(hNew);
    HTREEITEM hNextOldItem = GetChildItem(hOld);
    BOOL bStillGoing = ItemHasChildren(hNew);
    // If one has no children, the other can't either
    ASSERT(bStillGoing || !ItemHasChildren(hOld));
    while (bStillGoing)
    {
        // For each set of old and new items found, send the drag complete message.
        GetParent()->SendMessage(UWM_TREEMOVECOMPLETE, (WPARAM)hNextOldItem,(LPARAM)hNextNewItem);
        if (ItemHasChildren(hNextNewItem))
        {
            // At each point, assert that the old tree items have the same structure as the new
            ASSERT(ItemHasChildren(hNextOldItem));
            hNextNewItem = GetChildItem(hNextNewItem);
            hNextOldItem = GetChildItem(hNextOldItem);
        }
        else
        {
            // Return next sibling item.  Go up the tree to find a parent's sibling if needed.
            HTREEITEM hTmpNew;
            while ((hTmpNew = GetNextSiblingItem(hNextNewItem)) == NULL)
            {
                ASSERT(GetNextSiblingItem(hNextOldItem) == NULL);
                if ((hNextNewItem = GetParentItem(hNextNewItem)) == hNew)
                {
                    // We're back to the top level dragged items, so we're done.
                    ASSERT(GetParentItem(hNextOldItem) == hOld);
                    bStillGoing = FALSE;
                    break;
                }
                hNextOldItem = GetParentItem(hNextOldItem);
            }
            if (bStillGoing)
            {
                // We've found a sibling
                hNextNewItem = hTmpNew;
                hNextOldItem = GetNextSiblingItem(hNextOldItem);
                ASSERT(hNextOldItem != NULL);
            }
        }
    }
    // Finally send the message for the top level dragged items.
    GetParent()->SendMessage(UWM_TREEMOVECOMPLETE, (WPARAM)hOld,(LPARAM)hNew);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// If the calling dialog is keeping track of items by their HTREEITEM code, it will need to know 
// of all items deleted under a branch.  So, use this function recursively to send the message up
// for the lowest level items first.
void CTreeCtrlDrag::ReportDeleteBackToParent(HTREEITEM hOld)
{
    // Call this function recursively for all children of this item
    ASSERT(hOld != NULL);
    HTREEITEM hNextChildItem = GetChildItem(hOld);
    while (hNextChildItem != NULL)
    {
        ReportDeleteBackToParent(hNextChildItem);
        hNextChildItem = GetNextSiblingItem(hNextChildItem);
    }
    GetParent()->SendMessage(UWM_TREEITEMDELETED, (WPARAM)hOld, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get prev item in outline
HTREEITEM CTreeCtrlDrag::GetPrevItem(HTREEITEM hItem)
{
    HTREEITEM hPrevItem;

    if (hItem == GetRootItem())
        hPrevItem = NULL;                    // This is the root
    else if ((hPrevItem = GetPrevSiblingItem(hItem)) == NULL)
        hPrevItem = GetParentItem(hItem);    // There were no prev. siblings, so must be a parent
    else
    {
        // Find the last child/grandchild of this sibling
        // Start down a level if possible
        hItem = GetChildItem(hPrevItem);
        while (hItem != NULL)
        {
            // Go along to the end of this iterated level
            while (hItem != NULL)
            {
                hPrevItem = hItem;
                hItem = GetNextSiblingItem(hPrevItem);
            }
            // Then go down another level
            hItem = GetChildItem(hPrevItem);
        }
    }
    return hPrevItem;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get next item in outline
HTREEITEM CTreeCtrlDrag::GetNextItem(HTREEITEM hItem)
{
    HTREEITEM hNextItem;

    if (ItemHasChildren(hItem))
        hNextItem = GetChildItem(hItem);           // return first child
    else
    {
        // Return next sibling item.  Go up the tree to find a parent's sibling if needed.
        while ((hNextItem = GetNextSiblingItem(hItem)) == NULL)
        {
            if ((hItem = GetParentItem(hItem)) == NULL)
                return NULL;
        }
    }
    return hNextItem;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get prev item in outline (used to move items up) - particular different with GetPrevVisible()
// is that we only want to explore down one level of the previous sibling if it has expanded 
// children.  Each upward move will then drop it nearer the last child/grandchild sibling.
HTREEITEM CTreeCtrlDrag::GetPrevVisibleItemToMoveTo(HTREEITEM hItem, BOOL& bInsertAbove, 
                                                                               HTREEITEM& hParent)
{
    HTREEITEM hPrevItem = GetPrevSiblingItem(hItem);
    hParent = NULL;

    if (hPrevItem == NULL)
    {
        // There were no prev. siblings, so must be a parent or the root.
        hPrevItem = GetParentItem(hItem);
        bInsertAbove = TRUE;
        // But we shouldn't have been offered the chance to move the item up if it's the root, so 
        // assert this condition is the case
        ASSERT(hPrevItem != NULL);
    }
    else if (ItemHasChildren(hPrevItem) && 
             (GetItemState(hPrevItem, TVIS_EXPANDED) & TVIS_EXPANDED))
    {
        // The previous sibling item has visible children, so find the last child of this sibling 
        // (only go one level down as we want each upward move to take us down one level to give 
        // the user all the placement options)
        hItem = GetChildItem(hPrevItem);
        while (hItem != NULL)
        {
            hPrevItem = hItem;
            hItem = GetNextSiblingItem(hPrevItem);
        }
        bInsertAbove = FALSE;
    }
    else if (IsItemEmptyFolder(hPrevItem))
    {
        // This is an empty folder, so we actually want to move this item into the folder as a 
        // child.
        hParent = hPrevItem;
        bInsertAbove = FALSE;
    }
    else
    {
        // Normal operation
        bInsertAbove = TRUE;
    }
    // If it's not already been set, set the parent now
    if (hParent == NULL)
    {
        hParent = GetParentItem(hPrevItem);
        if (hParent == NULL)
            hParent = TVI_ROOT;
    }

    return hPrevItem;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get next non-child item in outline (used to move items up and down - you can't move an item to
// above or below one of it's own children!)
HTREEITEM CTreeCtrlDrag::GetNextNonChildItem(HTREEITEM hItem, BOOL& bInsertAbove)
{
    HTREEITEM hNextItem;

    // By default, new items added to following items will be added below them
    bInsertAbove = FALSE;

    // Ignore children and return next sibling item.  Note: if this item has no siblings then the
    // next downward move is infact out of this folder and up a level, but not passed any other 
    // items.  So if there are no siblings, use the parent as the target to add below.
    if ((hNextItem = GetNextSiblingItem(hItem)) == NULL)
    {
        if ((hNextItem = GetParentItem(hItem)) == NULL)
            return NULL;
    }
    // Note: if next item has visible children, we actually set the first child item as the target
    // and insert this item above it (which gets the right indent on the drag line if we're using 
    // it).  Also, the InsertItemAndSubtree() function assumes we will not set the target to below 
    // an item if the children are visible.
    else if (ItemHasChildren(hNextItem) && (GetItemState(hNextItem, TVIS_EXPANDED) & TVIS_EXPANDED))
    {
        hNextItem = GetChildItem(hNextItem);
        bInsertAbove = TRUE;
    }

    return hNextItem;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Check whether the selected item is a folder, based simply on it's image index.  This covers
// folders with children as well as empty folders.  Returns TRUE if the tree is empty.  
// CAUTION: This function relies on the tree bitmaps starting with the active folder icon at 
// index FOLDER_IMAGE (= 0)
BOOL CTreeCtrlDrag::IsSelectedItemAFolder()
{
    HTREEITEM hSelectedItem = GetSelectedItem();
    if (hSelectedItem == NULL)
    {
        if (GetCount() == 0)
            return FALSE;   // Empty tree
        hSelectedItem = GetRootItem();
    }
    ASSERT(hSelectedItem != NULL);

    int nImage;
    int nSelectedImage;
    GetItemImage(hSelectedItem, nImage, nSelectedImage);
    return (nImage == FOLDER_IMAGE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// It's possible for folders to be empty in which case they have no children and can only be 
// identified by their image no.
BOOL CTreeCtrlDrag::IsItemEmptyFolder(HTREEITEM hItem)
{
    int nImage;
    int nSelectedImage;
    GetItemImage(hItem, nImage, nSelectedImage);
    // Is this a folder and is it empty?
    if ((nImage == FOLDER_IMAGE) && !ItemHasChildren(hItem))
        return TRUE;
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Find a parent child combination and get the difference in indents.  Otherwise return the 
// default indent
int CTreeCtrlDrag::GetParentToChildIndent()
{
    int nIndent = DEFAULT_INDENT;

    // Get an item with visible children
    HTREEITEM hParent = GetRootItem();
    while ((hParent != NULL) && 
           (!ItemHasChildren(hParent) || !(GetItemState(hParent, TVIS_EXPANDED) & TVIS_EXPANDED)))
    {
        hParent = GetNextVisibleItem(hParent);
    }
    if (hParent != NULL)
    {
        HTREEITEM hChild = GetChildItem(hParent);
        ASSERT(hChild != NULL);
        // Now get the position of the child
        CRect rect;
        GetItemRect(hChild, &rect, TRUE);
        nIndent = rect.left;
        // Take off the left position of the parent
        GetItemRect(hParent, &rect, TRUE);
        nIndent -= rect.left;
    }
    return nIndent;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Remember the first and last visible items before we start adding and deleting items
void CTreeCtrlDrag::SaveFirstAndLastVisibleItems()
{
    m_hFirstVisibleItem = GetFirstVisibleItem();
    HTREEITEM hTmp = m_hFirstVisibleItem;
    while (hTmp != NULL)
    {
        m_hLastVisibleItem = hTmp;
        hTmp = GetNextVisibleItem(hTmp);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Reset the visibility to as close as possible to the original view.  
// NOTE 1: This function turns tree drawing on.  
// NOTE 2: hMovedItem is more than likely to represent a deleted tree item.
void CTreeCtrlDrag::ResetVisibleItems(HTREEITEM hNew, HTREEITEM hMovedItem, 
                                                     HTREEITEM hSecondaryVisibleItem /* = NULL */)
{
    // Move the window to the last item then first then ensure the view is back as closely as 
    // possible to the original position (unless one of the items was the one that moved - i.e. 
    // has now been deleted)
    if ((m_hFirstVisibleItem != hMovedItem) && (m_hLastVisibleItem != hMovedItem))
    {
        EnsureVisible(m_hLastVisibleItem);
        EnsureVisible(m_hFirstVisibleItem);
    }
    // Sometimes there's a secondary item that it'd be good to be visible if possible.
    if (hSecondaryVisibleItem != NULL)
        EnsureVisible(hSecondaryVisibleItem);
    // But the new item is always the one that must be visible, so is done last (and always with
    // drawing turned back on) as it may not have been drawn yet!
    SetRedraw(TRUE);
    EnsureVisible(hNew);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Check all siblings of this item to see if this is a unique name or not.
BOOL CTreeCtrlDrag::IsUniqueNameOnThisLevel(const CString& strName, HTREEITEM hOldItem, 
                                                                             HTREEITEM hNewParent)
{
    // If this item hasn't been created yet, hOldItem may be NULL.
    HTREEITEM hNextItem = NULL;

    // Get the first item on the required level
    if (hNewParent == NULL)
        hNextItem = GetRootItem();
    else 
        hNextItem = GetChildItem(hNewParent);

    // If no item on this level (i.e. this new item is the first child of a parent), the answer 
    // is "Yes, it is unique".
    if (hNextItem == NULL)
        return TRUE;

    // Else run through all items on this level and compare strings to find a match or not.
    while (hNextItem != NULL)
    {
        if ((hNextItem != hOldItem) && (strName.CompareNoCase(GetItemText(hNextItem)) == 0))
            return FALSE;
        hNextItem = GetNextSiblingItem(hNextItem);
    }
    return TRUE;
}

