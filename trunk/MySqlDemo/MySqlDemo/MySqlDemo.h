
// MySqlDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMySqlDemoApp:
// �йش����ʵ�֣������ MySqlDemo.cpp
//

class CMySqlDemoApp : public CWinAppEx
{
public:
	CMySqlDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMySqlDemoApp theApp;