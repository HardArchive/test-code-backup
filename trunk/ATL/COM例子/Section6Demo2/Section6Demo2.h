// Section6Demo2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSection6Demo2App:
// �йش����ʵ�֣������ Section6Demo2.cpp
//

class CSection6Demo2App : public CWinApp
{
public:
	CSection6Demo2App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSection6Demo2App theApp;