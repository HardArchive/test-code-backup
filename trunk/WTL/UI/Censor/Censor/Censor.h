// Censor.h
#pragma once

#include "resource.h"       // ������

#include "../SkinMgr/SkinMgr.h"
#ifdef _DEBUG
#pragma comment(lib, "../Debug/SkinMgr.lib")
#else
#pragma comment(lib, "../Release/SkinMgr.lib")
#endif
using namespace SkinMgr;

// CMainApp:
// �йش����ʵ�֣������ Censor.cpp
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