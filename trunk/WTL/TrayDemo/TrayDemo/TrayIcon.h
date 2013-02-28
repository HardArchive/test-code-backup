#pragma once

class CTrayIcon
{
public:

	CTrayIcon(void)
	{
	}

	~CTrayIcon(void)
	{
	}

	bool SetIcon(UINT uID); // main variant you want to use
	bool SetIcon(HICON hicon, LPCTSTR lpTip);
	bool SetIcon(LPCTSTR lpResName, LPCTSTR lpTip)
	{ return SetIcon(lpResName ? 
	AfxGetApp()->LoadIcon(lpResName) : NULL, lpTip); }

private:
	NOTIFYICONDATA m_stuNotifyIconData;
};
