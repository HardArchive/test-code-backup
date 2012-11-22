/*
Module : WtlFileTreeCtrl.h
Purpose: Interface for an WTL class which provides a tree control similiar 
         to the left hand side of explorer

Copyright (c) 2003 by iLYA Solnyshkin. 
E-mail: isolnyshkin@yahoo.com 
All rights reserved.
*/
#if !defined( WTL_FILETREECTRL_H__)
#define WTL_FILETREECTRL_H__

//Class which encapsulates access to the System image list which contains
//all the icons used by the shell to represent the file system

#include <string>
#include <algorithm>
#include <vector>

#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlmisc.h>
#include <shlobj.h>
#include "resource.h"

const UINT WM_POPULATE_TREE = WM_APP + 1;
const UINT WM_ITEM_SELECTED = WM_APP + 2;

class CSystemImageList
{
public:
	CSystemImageList();
	~CSystemImageList();

	//Methods
	CImageList* GetImageList();

protected:
	CImageList m_ImageList;
	static int m_nRefCount;
};

class CWtlFileTreeCtrl : public CWindowImpl< CWtlFileTreeCtrl, CTreeViewCtrl>			                   
{
public:
	CWtlFileTreeCtrl::CWtlFileTreeCtrl()
	{
		m_bShowFiles		= TRUE;
		m_hSelItem			= NULL;
	}

	virtual ~CWtlFileTreeCtrl(){};
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL SubclassWindow( HWND hWnd );

// Public methods
	void		SetRootFolder( const std::string sPath );
	std::string GetRootFolder() const { return m_sRootFolder; };
	std::string	ItemToPath( HTREEITEM hItem );
	std::string	GetSelectedPath();
	HTREEITEM	SetSelectedPath( const std::string sPath, BOOL bExpanded = FALSE );
	void		SetShowFiles( BOOL bFiles );
	BOOL		GetShowFiles() const { return m_bShowFiles; };

	BEGIN_MSG_MAP(CWtlFileTreeCtrl)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClick)
		MESSAGE_HANDLER(WM_POPULATE_TREE, OnPopulateTree)
		REFLECTED_NOTIFY_CODE_HANDLER(TVN_ITEMEXPANDING, OnItemExpanding )
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	
	LRESULT OnLButtonDblClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPopulateTree(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnItemExpanding(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);

	void OnViewRefresh();

protected:

	void      DisplayPath( const std::string sPath, HTREEITEM hParent, BOOL bUseSetRedraw = TRUE );
	void      DisplayDrives( HTREEITEM hParent, BOOL bUseSetRedraw = TRUE );
	int       GetIconIndex( const std::string sFilename );
	int       GetIconIndex( HTREEITEM hItem );
	int       GetSelIconIndex( const std::string sFilename );
	int       GetSelIconIndex( HTREEITEM hItem );
	BOOL      HasGotSubEntries( const std::string sDirectory );
	HTREEITEM InsertFileItem( const std::string sFile, const std::string sPath, HTREEITEM hParent );
	HTREEITEM FindSibling( HTREEITEM hParent, const std::string sItem );
	static int CompareByFilenameNoCase( std::string element1, std::string element2 );

	CSystemImageList m_SysImageList;
	std::string      m_sRootFolder;
	BOOL             m_bShowFiles;
	HTREEITEM        m_hSelItem;
};

#endif //WTL_FILETREECTRL_H__