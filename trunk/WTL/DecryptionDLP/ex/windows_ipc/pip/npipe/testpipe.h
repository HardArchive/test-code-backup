#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CTestpipeApp : public CWinApp
{
public:
	CTestpipeApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


