
// MyUIDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyUIDemoApp:
// �йش����ʵ�֣������ MyUIDemo.cpp
//

class CMyUIDemoApp : public CWinAppEx
{
public:
	CMyUIDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyUIDemoApp theApp;