#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

//-------------------------------------------------------------------------------------
class CHttpApp : public CWinApp
{
    virtual BOOL InitInstance();
};

extern CHttpApp theApp;
