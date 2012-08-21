// Censor.h
#pragma once

#include "resource.h"       // 主符号

#include "../SkinMgr/SkinMgr.h"
#ifdef _DEBUG
#pragma comment(lib, "../Debug/SkinMgr.lib")
#else
#pragma comment(lib, "../Release/SkinMgr.lib")
#endif
using namespace SkinMgr;

// CMainApp:
// 有关此类的实现，请参阅 Censor.cpp
//
class CMainApp : public CAppModule
{
public:
	CMainApp();
public:
	virtual BOOL InitInstance(void);
	virtual int ExitInstance(void);
};

extern CMainApp theApp;