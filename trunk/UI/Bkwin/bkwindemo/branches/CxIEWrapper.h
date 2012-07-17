#pragma once
#include "wtl_helper.h"
// IE控件接口声明。
#include <exdisp.h>

class CxIEWrapper 
	: public CWindowImpl<CxIEWrapper , CAxWindow>
{
public:
	DECLARE_WND_SUPERCLASS(_T("CIEWrapper"), CAxWindow::GetWndClassName())

	BEGIN_MSG_MAP_EX(CIEWrapper)
	END_MSG_MAP()

public:
	HWND Create(HWND hWndParent, _U_RECT rect/* = NULL*/);
	BOOL CreateWebBrowser(HWND hWndParent, _U_RECT rect);
	HRESULT Navigate(LPCTSTR pszUrl);
protected:
	void SetIDocHostUIHandler();
protected:
	CComPtr<IWebBrowser2> m_spWeb;
};
