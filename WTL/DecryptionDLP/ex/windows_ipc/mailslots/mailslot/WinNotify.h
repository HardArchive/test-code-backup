#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CWinNotifyApp : public CWinApp
{
public:
	CWinNotifyApp();
  HICON m_hMessageIcon;
  HICON m_hNoMessageIcon;

	virtual BOOL InitInstance();

	afx_msg void OnAppAbout();
	afx_msg void OnHelpIndex();

	DECLARE_MESSAGE_MAP()
};

CWinNotifyApp* GetApp();
