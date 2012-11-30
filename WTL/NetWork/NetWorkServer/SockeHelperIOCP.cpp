#include "StdAfx.h"
#include "SockeHelperIOCP.h"
bool g_bExit = false;   //全局退出函数

CSockeHelperIOCP::CSockeHelperIOCP(void)
{
	g_bExit = true;
}

CSockeHelperIOCP::~CSockeHelperIOCP(void)
{
	g_bExit = false;
}
