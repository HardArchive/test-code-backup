// ScintillaWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestEditor.h"
#include "ScintillaWnd.h"
//#include "ColorTextBox.h"   


// CScintillaWnd

IMPLEMENT_DYNAMIC(CScintillaWnd, CWnd)

CScintillaWnd::CScintillaWnd()
{

}

CScintillaWnd::~CScintillaWnd()
{
}


BEGIN_MESSAGE_MAP(CScintillaWnd, CWnd)
END_MESSAGE_MAP()



// CScintillaWnd ��Ϣ�������
BOOL CScintillaWnd::Create(DWORD dwExStyle, DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, UINT nID)   

{   
	// TODO: �ڴ����ר�ô����/����û���   
	return CWnd::CreateEx(dwExStyle, L"Scintilla", L"", dwStyle, rect, pParentWnd, nID);   
}   