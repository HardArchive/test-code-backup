#include "StdAfx.h"
#include "SockeHelperIOCP.h"
bool g_bExit = false;   //ȫ���˳�����

CSockeHelperIOCP::CSockeHelperIOCP(void)
{
	g_bExit = true;
}

CSockeHelperIOCP::~CSockeHelperIOCP(void)
{
	g_bExit = false;
}
