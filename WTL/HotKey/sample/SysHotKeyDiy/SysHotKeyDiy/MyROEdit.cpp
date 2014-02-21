// MyROEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MyROEdit.h"


// CMyROEdit

IMPLEMENT_DYNAMIC(CMyROEdit, CEdit)

CMyROEdit::CMyROEdit()
{
}

CMyROEdit::~CMyROEdit()
{
}


BEGIN_MESSAGE_MAP(CMyROEdit, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CMyROEdit::OnEnSetfocus)
END_MESSAGE_MAP()



// CMyROEdit message handlers



BOOL CMyROEdit::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN ){
		if ( ( ( pMsg->wParam == 'c' ) || ( pMsg->wParam == 'C' ) ) && (GetKeyState( VK_CONTROL) < 0) ){
			return CEdit::PreTranslateMessage(pMsg);
		}else{
			return TRUE;
		}
	}
	if ( pMsg->message == WM_RBUTTONDBLCLK || pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_RBUTTONUP){
		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CMyROEdit::OnEnSetfocus()
{
	HideCaret();
}
