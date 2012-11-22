/*================================================================================================
  File         : TreeCtrlDrag.h
  Description  : Header file for the draggable tree control
  Platform     : MSVC 6.00 windows 95/NT/XP/Vista/7
  Copyright    : Outram Research ltd
  Copyright Notice: 
                 This code is based substantially on an article and application written by 
                 Frederic My, available http://www.fairyengine.com/articles/dragdrop.htm for which
                 I'm extremely grateful.  Frederic's code if freeware.
                 You are free to use/modify this source code for whatever you like. If you do 
                 make any modifications or additions, I would appreciate it you let me know (or 
                 better, let everybody know by posting a comment). Of course, this control is 
                 provided as is, and no warranty is expressed or implied as to its fitness for any
                 particular purpose. 
  Author       : P.M.Outram
  Date         : 21 Jan 2010
================================================================================================*/

#ifndef __TREECTRLDRAG_H__
#define __TREECTRLDRAG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define FOLDER_IMAGE            0
#define DEFAULT_INDENT          19  // Default indent if we can't work it out from the visible
                                    // items in the tree
#define MARGIN_PIXELS           2
#define Y_SPRITE_OFFSET         6   // Not sure why this needs to be 6 and not 8 which would be 
                                    // have of the bitmap height (16)
#define OUT_OF_CONTROL_MARGIN   5   // Distance past edge of control before starting scrolling
#define FOLDER_BASE_NAME        _T("New Folder")

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlDrag window

class CTreeCtrlDrag : public CTreeCtrl
{
// Construction
public:
	CTreeCtrlDrag();

// Attributes
protected:
    UINT        m_nTimerID;
    UINT        m_nHoverTimerID;
    POINT       m_ptHoverPoint;
    BOOL        m_bDragging;
    CImageList* m_pDragImgList;
    HTREEITEM   m_hDragItem;
    
    // We keep track of the drag target and also whether the dragged item will be inserted above
    // or below it.
    BOOL        m_bInsertAbove;
    HTREEITEM   m_hDragTarget;

    // There are some instances when temporary label editing will be required (e.g. when adding a 
    // new folder in a directory that doesn't normally allow editing).  In these instances editing
    // needs to be turned on and then off again after editing.
    BOOL m_bTempEditingRequired;

    // Choose whether duplicate names are allowed in the same branch at the same level in the tree
    BOOL m_bNoDuplicatesOnSameLevel;

    // Items stored to reset the view after additions and deletions
    HTREEITEM m_hFirstVisibleItem;
    HTREEITEM m_hLastVisibleItem;

// Operations
public:
    void CompleteDrag(BOOL bLeftBtnUp);
    BOOL ShouldUpBtnBeEnabled();
    BOOL ShouldDownBtnBeEnabled();
    BOOL IsSelectedItemAFolder();
    BOOL IsDragging()                               { return m_bDragging; }
    void SetNoDuplicates(BOOL bNoDuplicates)        { m_bNoDuplicatesOnSameLevel = bNoDuplicates;}


    // Button input
	void MoveItemUp();
	void MoveItemDown();
	void RenameSelected();
	void DeleteSelected();
	HTREEITEM CreateNewFolder();

protected:
    // Drawing
    void DrawDropTargetLine(CRect rect, CDC* pDC);

    // Drag position calcs
    HTREEITEM GetDragTarget(HTREEITEM hItem, POINT point, BOOL& bInsertAbove);

    // Drag processing
    void SuccessfulDrag();
    void TidyUpEndOfDrag();
    HTREEITEM InsertItemAndSubtree(HTREEITEM hParent = NULL);
    void CopySubtree (HTREEITEM hDest, HTREEITEM hSrc);
    void CopyItem (HTREEITEM hDest, HTREEITEM hSrc);
    void ReportDragBackToParent(HTREEITEM hNew, HTREEITEM hOld);
    void ReportDeleteBackToParent(HTREEITEM hOld);

public:
    // Next and prev item utilties
    HTREEITEM GetPrevItem(HTREEITEM hItem);
    HTREEITEM GetNextItem(HTREEITEM hItem);
protected:
    HTREEITEM GetPrevVisibleItemToMoveTo(HTREEITEM hItem, BOOL& bInsertAbove, HTREEITEM& hParent);
    HTREEITEM GetNextNonChildItem(HTREEITEM hItem, BOOL& bInsertAbove);

    BOOL IsItemEmptyFolder(HTREEITEM hItem);
    int GetParentToChildIndent();
    void SaveFirstAndLastVisibleItems();
    void ResetVisibleItems(HTREEITEM hNew, HTREEITEM hMovedItem, HTREEITEM hSecondaryVisibleItem = NULL);
    BOOL IsUniqueNameOnThisLevel(const CString& strName,  HTREEITEM hOldItem, HTREEITEM hNewParent);
        
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlDrag)
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeCtrlDrag();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlDrag)
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // def __TREECTRLDRAG_H__
