// dll1.h : dll1 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号


// Cdll1App
// 有关此类实现的信息，请参阅 dll1.cpp
//

class Cdll1App : public CWinApp
{
public:
	Cdll1App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
