// MFCTestSection6Demo1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCTestSection6Demo1App:
// �йش����ʵ�֣������ MFCTestSection6Demo1.cpp
//

class CMFCTestSection6Demo1App : public CWinApp
{
public:
	CMFCTestSection6Demo1App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCTestSection6Demo1App theApp;