#pragma once

// Extended class used to automatically centre a property sheet
class CPropertySheetEx : public CPropertySheet
{
private:
	bool m_bCentered;
public:	
	CPropertySheetEx(_U_STRINGorID title = (LPCTSTR)NULL, UINT uStartPage = 0, HWND hWndParent = NULL)
		: CPropertySheet(title, uStartPage, hWndParent), m_bCentered(false)
	{
	}
	
	~CPropertySheetEx(void)
	{
	}

	BEGIN_MSG_MAP(CPropertySheetEx)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		CHAIN_MSG_MAP(CPropertySheet)
	END_MSG_MAP()

	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// Centre?
		if (wParam == TRUE)
			Center();
		// Done
		bHandled = FALSE;
		return 0;
	}

	void Center(void)
	{
		// Only do this once
		if (!m_bCentered)
		{
			CenterWindow();
			m_bCentered = true;
		}
	}
};
