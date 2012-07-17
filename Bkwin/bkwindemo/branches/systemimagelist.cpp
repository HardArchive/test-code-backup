/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Mark Otway
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes
// (don't get me blamed for your possible bugs), don't alter or remove
// this notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//////////////////////////////////////////////////////////////////////
// SystemImageList.cpp: implementation of the CSystemImageList class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemImageList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CImageList *CSystemImageList::m_gpLargeImageList = NULL;
CImageList *CSystemImageList::m_gpSmallImageList = NULL;
int CSystemImageList::m_gRefCount = 0;

void CSystemImageList::DestroyGlobalList( CImageList *& rpImageList )
{
    if (rpImageList)
    {
		// Detach the image list, otherwise when we delete the 
		// lists we'll deallocate the system-wide image list
		rpImageList->Detach();

		delete rpImageList;
		rpImageList = NULL;
    }
}

BOOL CSystemImageList::InitGlobalList( CImageList *& rpImageList, BOOL bSmall /* = TRUE */ )
{
    BOOL bRet = FALSE;

    if (!rpImageList)
    {
		// Create the singleton CImageList object, which will be
		// passed around to anyone who needs it
		rpImageList = new CImageList;
    
		if (rpImageList)
		{
			SHFILEINFO sfi;
			DWORD dwFlags = SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX;
	    
			if (bSmall)
				dwFlags |= SHGFI_SMALLICON;

			size_t size = sizeof(SHFILEINFO);

			//////////////////////////////////////////////////////////////////////////
			//Modify by QuiRuifeng
			// Load the image list - use an arbitrary file extension for the
			// call to SHGetFileInfo (we don't want to touch the disk, so use
			// FILE_ATTRIBUTE_NORMAL && SHGFI_USEFILEATTRIBUTES).
			rpImageList->Attach((HIMAGELIST)SHGetFileInfo(TEXT(".txt"), FILE_ATTRIBUTE_NORMAL, &sfi, size, dwFlags));
	    
			// Make the background colour transparent, works better for lists etc.
			rpImageList->SetBkColor(CLR_NONE);

			bRet = TRUE;
		}
    }

    return (bRet);
}

CSystemImageList::CSystemImageList()
{
    // Initialise both lists when the first instance is created
    if (m_gRefCount == 0)
    {
		ASSERT( !m_gpSmallImageList && !m_gpLargeImageList );

		InitGlobalList(m_gpSmallImageList, TRUE);
		InitGlobalList(m_gpLargeImageList, FALSE);
    }

    // Keep a reference count
    m_gRefCount++;
}

CSystemImageList::~CSystemImageList()
{
    // One less instance using the lists
    m_gRefCount--;

    // Nobody's using the list now, so throw 'em away
    if (m_gRefCount == 0)
    {
		DestroyGlobalList(m_gpLargeImageList);
		DestroyGlobalList(m_gpSmallImageList);
    }
}

// Helper function to get the icon index of a particular file/object
int CSystemImageList::GetIcon( const CString& sName, BOOL bOpen /* = FALSE */,  BOOL bSmall /* = TRUE */  ) const
{
    SHFILEINFO sfi;

    DWORD dwFlags = SHGFI_USEFILEATTRIBUTES|SHGFI_SYSICONINDEX;
    
    size_t size = sizeof( SHFILEINFO );

    if (bSmall)
		dwFlags |= SHGFI_SMALLICON;
    
    if (bOpen)
		dwFlags |= SHGFI_OPENICON;
    
    SHGetFileInfo(sName, FILE_ATTRIBUTE_NORMAL, &sfi, size, dwFlags);
    
    return (sfi.iIcon);
}

CImageList * CSystemImageList::GetImageList( BOOL bSmall /* = TRUE */ ) const 
{
    CImageList *pList = NULL;
    
    if (bSmall)
		pList = m_gpSmallImageList;
    else
		pList = m_gpLargeImageList;

    // Ought to return a const pointer here (so noone tries to delete
    // the lists). However, SetImageList in the common controls requires
    // a non-const pointer, so it wouldn't be much use to anyone.
    return (pList); 
}
