
// CheckProtocol.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCheckProtocolApp:
// �йش����ʵ�֣������ CheckProtocol.cpp
//

class CCheckProtocolApp : public CWinAppEx
{
public:
	CCheckProtocolApp();

// ��д
	public:
	virtual BOOL InitInstance();
    CGlobalData  m_globalData;
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCheckProtocolApp theApp;