
// DNS_Demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDNS_DemoApp:
// �йش����ʵ�֣������ DNS_Demo.cpp
//

class CDNS_DemoApp : public CWinAppEx
{
public:
	CDNS_DemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDNS_DemoApp theApp;