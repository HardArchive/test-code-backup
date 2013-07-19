// LzfPropertySheet.cpp: implementation of the CLzfPropertySheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPropertySheetDlg.h"
#include "LzfPropertySheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CLzfPropertySheet, CMyPropertySheet)

CLzfPropertySheet::~CLzfPropertySheet()
{

}
CLzfPropertySheet::CLzfPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0)
:CMyPropertySheet( nIDCaption, pParentWnd,  iSelectPage)
{
}
CLzfPropertySheet::	CLzfPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0)
:CMyPropertySheet( nIDCaption, pParentWnd,  iSelectPage)
	{

	}