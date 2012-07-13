// CH364ISL_MFC.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCH364ISL_MFCApp:
// 有关此类的实现，请参阅 CH364ISL_MFC.cpp
//

class CCH364ISL_MFCApp : public CWinApp
{
public:
	CCH364ISL_MFCApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCH364ISL_MFCApp theApp;