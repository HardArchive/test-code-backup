// CH364ISL_MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCH364ISL_MFCApp:
// �йش����ʵ�֣������ CH364ISL_MFC.cpp
//

class CCH364ISL_MFCApp : public CWinApp
{
public:
	CCH364ISL_MFCApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCH364ISL_MFCApp theApp;