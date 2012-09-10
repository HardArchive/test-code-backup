#pragma once


template <class T, COLORREF t_crBrushColor>
class CPaintBkgnd : public CMessageMap  
{
public:
	CPaintBkgnd() 
	{
		m_hbrBkgnd = CreateSolidBrush(t_crBrushColor);
	}
	virtual ~CPaintBkgnd()
	{
		if (m_hbrBkgnd != NULL)
		{
			DeleteObject(m_hbrBkgnd);
			m_hbrBkgnd = NULL;
		}
	}
	
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T*   pT = static_cast<T*>(this);
		HDC  dc = (HDC) wParam;
		RECT rcClient;
		
		pT->GetClientRect ( &rcClient );
		FillRect ( dc, &rcClient, m_hbrBkgnd );
		return 1;    // we painted the background
		//    return 0;
	}
	
	BEGIN_MSG_MAP(CPaintBkgnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()
protected:
	HBRUSH m_hbrBkgnd;
};
