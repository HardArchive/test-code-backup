#pragma once
#include "stdafx.h"
#include "PaintBkgnd.h"

typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,WS_EX_APPWINDOW> CMyWindowTraits;
class CMyWindow : public CWindowImpl<CMyWindow, CWindow, CMyWindowTraits>,
                  public CPaintBkgnd<CMyWindow, RGB(0,255,0)>
{
public:
	CMyWindow()
	{

	}
	virtual ~CMyWindow()
	{

	}
	
	DECLARE_WND_CLASS(_T("My Window Class"))
	
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	typedef CPaintBkgnd<CMyWindow, RGB(0,255,0)> CPaintBkgndBase;
	BEGIN_MSG_MAP(CMyWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(IDC_ABOUT, OnAbout)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		CHAIN_MSG_MAP(CPaintBkgndBase);
	END_MSG_MAP() 
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};