// MFCTest2Section7Demo1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCTest2Section7Demo1App:
// �йش����ʵ�֣������ MFCTest2Section7Demo1.cpp
//

class CMFCTest2Section7Demo1App : public CWinApp
{
public:
	CMFCTest2Section7Demo1App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCTest2Section7Demo1App theApp;