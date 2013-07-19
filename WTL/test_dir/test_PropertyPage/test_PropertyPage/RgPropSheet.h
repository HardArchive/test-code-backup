// RgPropSheet.h


class CRgPropSheet : public CPropertySheet,
	public CMessageFilter, public CIdleHandler
{
private:
	bool m_bCentered;
public:	
	CRgPropSheet(_U_STRINGorID title = (LPCTSTR)NULL, UINT uStartPage = 0, HWND hWndParent = NULL)
		: CPropertySheet(title, uStartPage, hWndParent), m_bCentered(false)
	{
	}

	~CRgPropSheet(void)
	{
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_MSG_MAP(CRgPropSheet)
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
