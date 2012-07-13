#pragma once

//注意：这俩文件来自Scintilla的include目录 
#include "scintilla/Scintilla.h"
#include "scintilla/SciLexer.h"

// CScintillaWnd

class CScintillaWnd : public CWnd
{
	DECLARE_DYNAMIC(CScintillaWnd)

public:
	CScintillaWnd();
	virtual ~CScintillaWnd();

public:   
	virtual BOOL Create(   
		DWORD dwExStyle, DWORD dwStyle,const RECT& rect,    
		CWnd* pParentWnd, UINT nID);   


protected:
	DECLARE_MESSAGE_MAP()
};
