// MFCTestSection4Demo1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCTestSection4Demo1App:
// �йش����ʵ�֣������ MFCTestSection4Demo1.cpp
//

class CMFCTestSection4Demo1App : public CWinApp
{
public:
	CMFCTestSection4Demo1App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCTestSection4Demo1App theApp;