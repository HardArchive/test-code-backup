
// StandbyDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStandbyDemoApp:
// �йش����ʵ�֣������ StandbyDemo.cpp
//

class CStandbyDemoApp : public CWinAppEx
{
public:
	CStandbyDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStandbyDemoApp theApp;