
// PassTCP.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPassTCPApp:
// �йش����ʵ�֣������ PassTCP.cpp
//

class CPassTCPApp : public CWinAppEx
{
public:
	CPassTCPApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPassTCPApp theApp;