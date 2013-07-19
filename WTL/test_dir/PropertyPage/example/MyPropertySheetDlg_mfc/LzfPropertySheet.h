// LzfPropertySheet.h: interface for the CLzfPropertySheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LZFPROPERTYSHEET_H__C76B9C91_3A58_45E0_8818_2AA0005609C0__INCLUDED_)
#define AFX_LZFPROPERTYSHEET_H__C76B9C91_3A58_45E0_8818_2AA0005609C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPropertySheet.h"

class CLzfPropertySheet : public CMyPropertySheet  
{
public:
	DECLARE_DYNAMIC(CLzfPropertySheet)
	CLzfPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CLzfPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CLzfPropertySheet();

};

#endif // !defined(AFX_LZFPROPERTYSHEET_H__C76B9C91_3A58_45E0_8818_2AA0005609C0__INCLUDED_)
