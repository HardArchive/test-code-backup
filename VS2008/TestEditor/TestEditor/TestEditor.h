
// TestEditor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestEditorApp:
// �йش����ʵ�֣������ TestEditor.cpp
//

class CTestEditorApp : public CWinAppEx
{
public:
	CTestEditorApp();

	HINSTANCE m_hDll;

// ��д
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestEditorApp theApp;
