// Simple7.h : Simple7 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CSimple7App
// �йش���ʵ�ֵ���Ϣ������� Simple7.cpp
//

class CSimple7App : public CWinApp
{
public:
	CSimple7App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
