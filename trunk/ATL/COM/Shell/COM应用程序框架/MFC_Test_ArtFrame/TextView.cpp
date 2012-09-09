// CTextView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Test_ArtFrame.h"
#include "TextView.h"
#include ".\textview.h"


// CTextView

IMPLEMENT_DYNCREATE(CTextView, CEditView)

CTextView::CTextView()
{
	
}

CTextView::~CTextView()
{
}

BEGIN_MESSAGE_MAP(CTextView, CEditView)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_FORMAT_FONT, OnFormatFont)
END_MESSAGE_MAP()


// CTextView diagnostics

#ifdef _DEBUG
void CTextView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG


HRESULT STDMETHODCALLTYPE CTextView::CreateWnd(HWND hWndParent)
{
	CWnd *pWnd = (CWnd*)this;
	CWnd *wndParent = CWnd::FromHandle(hWndParent);
	CRect _Rect(0,0, 100, 100);

	if (pWnd->Create(NULL,
					("MFC CEditView "),
					WS_HSCROLL|WS_VSCROLL|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE|ES_MULTILINE|ES_AUTOHSCROLL,
					_Rect, wndParent, 0))
	{
		m_pEdit = &GetEditCtrl();
		return S_OK;
	}

	return S_FALSE;
}



void CTextView::OnCopy(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_pEdit->Copy();
}


void CTextView::OnPaste(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_pEdit->Paste();
}

void CTextView::OnSelectAll(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_pEdit->SetSel(0, GetWindowTextLength());
}

void CTextView::OnClear(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	int x;
	int y;

	m_pEdit->GetSel(x, y);
	m_pEdit->ReplaceSel(_T(""));
}


void CTextView::OnUndo(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_pEdit->Undo();
}


//void CTextView::OnFormatFont(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
//{
//	CFontDialog fd;
//	if (IDOK == fd.DoModal())
//	{
//		if (NULL != font.m_hObject)
//		{
//			font.DeleteObject();
//		}
//		font.CreateFontIndirect(&fd.m_lf);
//		SetFont(&font, TRUE);
//	}
//
//}
//void CTextView::OnCut(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
//{
//	m_pEdit->Cut();
//}
//



// CTextView message handlers
void CTextView::OnEditCut()
{
	m_pEdit->Cut();
}

void CTextView::OnFormatFont()
{
	CFontDialog fd;
	if (IDOK == fd.DoModal())
	{
		if (NULL != font.m_hObject)
		{
			font.DeleteObject();
		}
		font.CreateFontIndirect(&fd.m_lf);
		SetFont(&font, TRUE);
	}
}
