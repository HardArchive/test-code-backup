// CPlusPlusClass.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCPlusPlusClassApp:
// �йش����ʵ�֣������ CPlusPlusClass.cpp
//

class CCPlusPlusClassApp : public CWinApp
{
public:
	CCPlusPlusClassApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCPlusPlusClassApp theApp;
