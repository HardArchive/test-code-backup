// Simple7.h : Simple7 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号


// CSimple7App
// 有关此类实现的信息，请参阅 Simple7.cpp
//

class CSimple7App : public CWinApp
{
public:
	CSimple7App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
