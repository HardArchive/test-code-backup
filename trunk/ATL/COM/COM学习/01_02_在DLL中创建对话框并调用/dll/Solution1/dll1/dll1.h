// dll1.h : dll1 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// Cdll1App
// �йش���ʵ�ֵ���Ϣ������� dll1.cpp
//

class Cdll1App : public CWinApp
{
public:
	Cdll1App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
