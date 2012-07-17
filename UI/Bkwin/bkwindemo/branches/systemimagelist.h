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
// SystemImageList.h: interface for the CSystemImageList class.
//////////////////////////////////////////////////////////////////////

#ifndef SYSIMAGELIST_HDR_INCLUDED
#define SYSIMAGELIST_HDR_INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSystemImageList : public CObject
{
private:
    static CImageList * m_gpLargeImageList;
    static CImageList * m_gpSmallImageList;
    static int m_gRefCount;

private:
    BOOL InitGlobalList( CImageList *& rpImageList, BOOL bSmall = TRUE );
    void DestroyGlobalList( CImageList *& rpImageList );

public:
    CSystemImageList();
    virtual ~CSystemImageList();
    CImageList * GetImageList( BOOL bSmall = TRUE ) const;
    int GetIcon( const CString& sName, BOOL bOpen = FALSE, BOOL bSmall = TRUE ) const;
};

#endif // SYSIMAGELIST_HDR_INCLUDED
