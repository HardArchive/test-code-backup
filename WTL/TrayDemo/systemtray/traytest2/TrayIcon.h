////////////////////////////////////////////////////////////////
// CTrayIcon
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.

#ifndef _TRAYICON_H
#define _TRAYICON_H

#include "Subclass.h"

////////////////
// CTrayIcon manages an icon in the Windows 95 system tray. 
// The sample program TRAYTEST shows how to use it.
// 
class CTrayIcon : public CCmdTarget {
public:
	CTrayIcon(UINT uID);
	~CTrayIcon();

	// Call this to receive tray notifications
	void SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg);

	// SetIcon functions. To remove icon, call SetIcon(0)
	//
	BOOL SetIcon(UINT uID); // main variant you want to use
	BOOL SetIcon(HICON hicon, LPCTSTR lpTip);
	BOOL SetIcon(LPCTSTR lpResName, LPCTSTR lpTip)
		{ return SetIcon(lpResName ? 
			AfxGetApp()->LoadIcon(lpResName) : NULL, lpTip); }
	BOOL SetStandardIcon(LPCTSTR lpszIconName, LPCTSTR lpTip)
		{ return SetIcon(::LoadIcon(NULL, lpszIconName), lpTip); }

// Following is obsolete. CTrayIcon does default handling auotmatically.
//	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
	virtual LRESULT OnTrayNotify(WPARAM uID, LPARAM lEvent);
	virtual LRESULT OnTaskBarCreate(WPARAM wp, LPARAM lp);

protected:
	NOTIFYICONDATA m_nid;		  // struct for Shell_NotifyIcon args

	// private class used to hook tray notification and taskbarcreated
	class CTrayHook : public CSubclassWnd {
	private:
		CTrayIcon* m_pTrayIcon;
		virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
		friend CTrayIcon;
	};
	friend CTrayHook;
	CTrayHook m_notifyHook; // trap tray notifications
	CTrayHook m_parentHook; // trap taskbarcreated message
	DECLARE_DYNAMIC(CTrayIcon)
};

#endif
