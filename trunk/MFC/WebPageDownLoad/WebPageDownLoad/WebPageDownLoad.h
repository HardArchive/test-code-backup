
// WebPageDownLoad.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWebPageDownLoadApp:
// �йش����ʵ�֣������ WebPageDownLoad.cpp
//

class CWebPageDownLoadApp : public CWinAppEx
{
public:
	CWebPageDownLoadApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWebPageDownLoadApp theApp;