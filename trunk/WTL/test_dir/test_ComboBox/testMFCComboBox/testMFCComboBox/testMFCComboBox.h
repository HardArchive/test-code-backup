
// testMFCComboBox.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestMFCComboBoxApp:
// �йش����ʵ�֣������ testMFCComboBox.cpp
//

class CtestMFCComboBoxApp : public CWinAppEx
{
public:
	CtestMFCComboBoxApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestMFCComboBoxApp theApp;