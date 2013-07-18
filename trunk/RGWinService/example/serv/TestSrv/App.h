#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "ntserv.h"
#include "ntservServiceControlManager.h"

class CMyService : public CNTService
{
public:
	CMyService();
	virtual void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	virtual void OnStop();
  virtual void OnPause();
  virtual void OnContinue();
	virtual void ShowHelp();
  virtual void OnUserDefinedRequest(DWORD dwControl);
  //virtual void _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);

protected:
  volatile long m_lWantStop;
  volatile long m_lPaused;
  DWORD         m_dwBeepInternal;
};

