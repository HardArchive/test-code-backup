
// ArpAttacher.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CArpAttacherApp:
// �йش����ʵ�֣������ ArpAttacher.cpp
//

class CArpAttacherApp : public CWinAppEx
{
public:
	CArpAttacherApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CArpAttacherApp theApp;