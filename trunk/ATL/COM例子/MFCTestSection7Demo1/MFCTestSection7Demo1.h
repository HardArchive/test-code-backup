// MFCTestSection7Demo1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCTestSection7Demo1App:
// �йش����ʵ�֣������ MFCTestSection7Demo1.cpp
//

class CMFCTestSection7Demo1App : public CWinApp
{
public:
	CMFCTestSection7Demo1App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCTestSection7Demo1App theApp;