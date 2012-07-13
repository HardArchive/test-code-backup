// ScintillaWnd.cpp : 实现文件
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



// CScintillaWnd 消息处理程序
BOOL CScintillaWnd::Create(DWORD dwExStyle, DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, UINT nID)   

{   
	// TODO: 在此添加专用代码和/或调用基类   
	return CWnd::CreateEx(dwExStyle, L"Scintilla", L"", dwStyle, rect, pParentWnd, nID);   
}   