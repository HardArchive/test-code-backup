// DataRecv.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDataRecvApp:
// �йش����ʵ�֣������ DataRecv.cpp
//

class CDataRecvApp : public CWinApp
{
public:
	CDataRecvApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDataRecvApp theApp;
