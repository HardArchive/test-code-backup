
// MFCtestCom_Lean4Demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCtestCom_Lean4DemoApp:
// �йش����ʵ�֣������ MFCtestCom_Lean4Demo.cpp
//

class CMFCtestCom_Lean4DemoApp : public CWinAppEx
{
public:
	CMFCtestCom_Lean4DemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCtestCom_Lean4DemoApp theApp;