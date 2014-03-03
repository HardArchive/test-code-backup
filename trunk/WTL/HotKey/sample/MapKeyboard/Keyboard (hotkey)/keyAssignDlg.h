#ifndef UPRIGHT_KEYASSIGNDLG_H
#define UPRIGHT_KEYASSIGNDLG_H

/////////////////////////////////////////////////////////////////////////////
// keyAssignDlg.h by N.A.Bozinis @ 27/04/2007 09:19:41
// Uses the menu system to populate the command list and allows changing keys
// Hence it doesn't require separate list of "commands" available
//
// This code is free to use in any context.
// If you use it in your application, a mention to the author and the website
// (http://zabkat.com) in the credits or About dialog will be appreciated.
/////////////////////////////////////////////////////////////////////////////


#pragma once

// combination of ACCEL structure with its equivalend HACCEL
// this is designed for long-term use e.g. within an application object
class CAccelCombo
{
public:
	CAccelCombo() {
		m_hSysAccel = 0;
		m_pAccelTable = 0;
		m_nStore = 0;
		m_nAccelCount = 0;
	}

	~CAccelCombo() {
#ifdef _DEBUG
		// are they really in sync?
		if(m_nAccelCount) {
			ATLASSERT(m_hSysAccel);
			int cnt_ = m_nAccelCount;
			ACCEL* tmp_ = (ACCEL*)_alloca(sizeof(ACCEL)*cnt_);
			memcpy(tmp_, m_pAccelTable, sizeof(ACCEL)*cnt_);
			SyncTable(m_hSysAccel);
			ATLASSERT(cnt_ == m_nAccelCount);
			ATLASSERT(memcmp(tmp_, m_pAccelTable, sizeof(ACCEL)*cnt_)==0);
		}
		else
			ATLASSERT(!m_hSysAccel);
#endif

		Reset();
		if(m_pAccelTable)
			free(m_pAccelTable);
	}

	// we return our internal state but it is only meant as const access, deal?
	operator HACCEL() const { return m_hSysAccel; }
	operator LPACCEL() const { return m_pAccelTable; }
	int GetCount() const { return m_nAccelCount; }

	// if you need Attach/Detach semantics use CAcceleratorT from atlgdix.h
	BOOL SetAccelTable(const LPACCEL pac/* we copy it*/, int size)
	{
		ATLASSERT(pac && size > 0);
		ATLASSERT(pac != m_pAccelTable || 0==pac);
		if(!AllocEntries(size))
			return FALSE;

		memcpy(m_pAccelTable, pac, sizeof(ACCEL)*size);
		m_nAccelCount = size;

		return SyncHandle();
	}

	BOOL SetHandle(const HACCEL hac/*we copy it*/)
	{
		ATLASSERT(hac);
		ATLASSERT(hac != m_hSysAccel || 0==hac);

		if(SyncTable(hac))
			return SyncHandle();

		return FALSE;
	}

	//	registry persistence: binary mode dump
#define ACCREGVALUENAME _T("binAccelTable")
	//从注册表读取虚拟键表
	void ReadProfile(LPCTSTR szSec) {
		ATLASSERT(szSec && *szSec);
		ATLASSERT(!GetCount()); // are you mental?

		// substitute here your app's registry persistence code
		CRegKey rkey;
		rkey.Open(HKEY_CURRENT_USER, szSec, KEY_READ);
		if(!rkey.m_hKey)
			return;

		// provide a large buffer; the normal keys are ~250, so this overdesign should be ok!
		ACCEL buf[1024];
		DWORD dwCount = sizeof(buf);
		DWORD dwType = NULL;
		LONG lRes = RegQueryValueEx(rkey.m_hKey, (LPTSTR)ACCREGVALUENAME, NULL, &dwType,
			(LPBYTE)buf, &dwCount);
		ATLASSERT( (lRes != ERROR_SUCCESS) || (dwType == REG_BINARY) );
		if(ERROR_SUCCESS==lRes) { // ERROR_MORE_DATA is also conceivable
			ATLASSERT(!(dwCount % sizeof(ACCEL)));
			dwCount /= sizeof(ACCEL); // counts accelerators
			ATLASSERT(dwCount);
			SetAccelTable(buf, dwCount);
		}
	}

	BOOL WriteProfile(LPCTSTR szSec) {
		ATLASSERT(szSec && *szSec);
		if(!GetCount())
			return TRUE;

		CRegKey rkey;
		rkey.Create(HKEY_CURRENT_USER, szSec, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE);
		if(!rkey.m_hKey)
			return FALSE;

		return RegSetValueEx(rkey.m_hKey, ACCREGVALUENAME, NULL, REG_BINARY,
			(CONST BYTE *)m_pAccelTable, sizeof(ACCEL)*m_nAccelCount) == ERROR_SUCCESS;
	}

	int FindCommand(WORD cmdid) // find the (first) accelerator associated with command, if any
	{
		for(int i=0; i<m_nAccelCount; i++)
			if(m_pAccelTable[i].cmd == cmdid)
				return i;

		return -1;
	}

	void UpdateMenuKeys(HMENU hMenu); // definition in CPP to avoid dependency on CKeyAssignDlg

	HACCEL LoadAccelerators(_U_STRINGorID accel)
   {
      ATLASSERT(!m_hSysAccel);
#if (_ATL_VER >= 0x0700)
      m_hSysAccel = ::LoadAccelerators(ATL::_AtlBaseModule.GetResourceInstance(), accel.m_lpstr);
#else
      m_hSysAccel = ::LoadAccelerators(_Module.GetResourceInstance(), accel.m_lpstr);
#endif

		SyncTable(m_hSysAccel);
      return m_hSysAccel;
   }

protected:
	BOOL AllocEntries(int newSize)
	{
		if(newSize <= m_nStore)
			return TRUE;
		ATLASSERT(newSize < 1024); // we'll have problems persisting those!

		ACCEL* tmp = (ACCEL*)realloc(m_pAccelTable, sizeof(ACCEL)*newSize);
		if(tmp) {
			m_nStore = newSize;
			m_pAccelTable = tmp;
			return TRUE;
		}
		else
			return FALSE;
	}

	BOOL SyncHandle() // from table to HACCEL
	{
		if(m_nAccelCount) {
			if(m_hSysAccel)
				DestroyAcceleratorTable(m_hSysAccel);

			m_hSysAccel = CreateAcceleratorTable(m_pAccelTable, m_nAccelCount);
			if(!m_hSysAccel) {
				m_nAccelCount = 0;
				return FALSE;
			}
		}
		else
			Reset();

		return TRUE;
	}

	// unless argument hac==m_hSysAccel it is not in risc of destruction
	BOOL SyncTable(HACCEL hac) // from HACCEL to table
	{
		if(hac) {
			int size = CopyAcceleratorTable(hac, NULL, 0);
			if(AllocEntries(size) && 
				CopyAcceleratorTable(hac, m_pAccelTable, size) == size) 
			{
				m_nAccelCount = size;
			}
			else {
				Reset();
				return FALSE;
			}
		}
		else
			Reset();

		return TRUE;
	}

	void Reset() {
		if(m_hSysAccel) {
			DestroyAcceleratorTable(m_hSysAccel);
			m_hSysAccel = 0;
		}
		m_nAccelCount = 0; // table storage intact
	}

protected:
	HACCEL m_hSysAccel;
	ACCEL* m_pAccelTable; // in sync with "real" accelerator table
	int m_nAccelCount; // items used in table
	int m_nStore; // current allocation size
};

// calculate byte-wise checksum using newcomer's algorithm
DWORD UGHChecksumBuf(LPCBYTE buf, DWORD dwLen)
{
	ATLASSERT(buf);
	if(!buf)
		return 0;

	// this can also be used for checking large amounts of input for changes
	// (whereas my simple addition is kak since it can't detect 0-1 -> 1-0)
	WORD r = 55665;
	DWORD sum = 0;

	LPCBYTE last = buf + dwLen;
	while(buf < last) {
		BYTE value = *buf++;
		BYTE cipher = (value ^ (r >> 8));
		r = (cipher + r) * 52845 + 22719; // magic constants
		sum += cipher;
	}

	return sum;
}

// the dialog class itself
// as you can tell CAccelCombo was an afterthought so it ain't used in the dialog
class CKeyAssignDlg : public CDialogImpl<CKeyAssignDlg> //, public CSubclassHook
{
public:
	enum { IDD = IDD_ACCELEDIT_DLG };

	CKeyAssignDlg(HMENU menu, HACCEL accel, HACCEL defkey) {
		ATLASSERT(menu);
		ATLASSERT(accel);
		ATLASSERT(defkey);
		// this class uses CIconMenuImpl to draw listview icons

		m_hMenuCmd = menu;
		m_hInitialAccels = accel;
		m_hDefaultAccels = defkey;
		m_nAccel = ::CopyAcceleratorTable(accel, NULL, 0);

		m_nInitialLen = m_nAccel;
		m_pNewAccels = 0; // allocated dynamically later
#ifdef _DEBUG
		m_nUsed = 0;
#endif

		m_sCtrl = NameFromVKey(VK_CONTROL) + _T('+'); // key names used often
		m_sAlt = NameFromVKey(VK_MENU) + _T('+');
		m_sShift = NameFromVKey(VK_SHIFT) + _T('+');
	}

	~CKeyAssignDlg() 
	{
#ifdef _DEBUG
		// assert we don't have any duplicate keys
		int i,j;
		for(i=0; i<m_nUsed; i++) {
			ATLASSERT(m_pNewAccels[i].fVirt & FVIRTKEY); // at least for my acceleratores!
			// if next line blows, add FNOINVERT on line 623 (depends if your resource editor adds it by default)
			ATLASSERT(!(m_pNewAccels[i].fVirt & FNOINVERT)); // lack of that could mess my identity checks

			for(j=i+1; j<m_nUsed; j++)
				// @@@ this could be fooled by plain character shortcuts e.g. "a" vs VK_A?
				ATLASSERT(!(m_pNewAccels[i].fVirt==m_pNewAccels[j].fVirt && m_pNewAccels[i].key==m_pNewAccels[j].key));
		}
		// on the other hand it is conceivable that a command may have more than 1 keys
#endif

		delete m_pNewAccels;
	}

	// lame: here i have a few publics that do not depend too much on internal state
	// (if it wasn't for CTRL/ALT names they could be static!)

	// sadly there's no correspondence between FCONTROL & HOTKEYF_CONTROL so i need separate helpers
	CString NameFromAccel(ACCEL& key)
	{
		CString name;
		if(key.fVirt & FCONTROL)
			name = m_sCtrl;
		if(key.fVirt & FALT)
			name += m_sAlt;
		if(key.fVirt & FSHIFT)
			name += m_sShift;
		// FNOINVERT is useless, backward compatibility

		if(key.fVirt & FVIRTKEY)
			name += NameFromVKey(key.key);
		else {
			// key field is an ASCII key code. (i never saw one of these)
#ifdef _UNICODE
			char    ca = (char)key.key;
			wchar_t cu;

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, &ca, 1, &cu, 1);
			name += cu;
#else
			name += (char)key.key;
#endif
		}

		ATLASSERT(name.GetLength());
		return name;
	}

	BOOL IsValidCmdRange(WORD cmdid) {
		// return FALSE to exclude certain commands from customization
		return TRUE;
	}

	BEGIN_MSG_MAP(CKeyAssignDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_BUT_ASSIGN, OnAssign)
		COMMAND_ID_HANDLER(IDC_BUT_REMOVE, OnRemove)
		COMMAND_ID_HANDLER(IDC_BUT_RESET, OnReset)

		NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnLVItemChanged)
		COMMAND_CODE_HANDLER(LBN_SELCHANGE, OnLBSelChange)
		COMMAND_CODE_HANDLER(CBN_SELENDOK, OnCBSelectionEnd)
		COMMAND_CODE_HANDLER(EN_CHANGE, OnNewAccel)
	END_MSG_MAP()

protected:
	// count all commands under this menu & submenus
	int GetSubitemCount(HMENU hMenu, BOOL everything=TRUE) 
	{
		ATLASSERT(hMenu);

		// scan all the immediate level items for command
		int nItems = ::GetMenuItemCount(hMenu);
		int nAll = 0;
		CMenuItemInfo mi;
		mi.fMask = MIIM_ID | MIIM_SUBMENU;
		for(int i = 0; i < nItems; i++)
		{
			::GetMenuItemInfo(hMenu, i, TRUE, &mi); // by position
			if(mi.hSubMenu)
				nAll += GetSubitemCount(mi.hSubMenu, everything);
			else if(mi.wID != 0) { // separators excluded
				if(everything || IsValidCmdRange(mi.wID))
					nAll++;
			}
		}

		return nAll;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent()); // note: ATL has a bug here if you are in multiple monitors!
		ATLASSERT(m_nAccel);

		// allocate room for the existing keys & read them
		m_nCommands = GetSubitemCount(m_hMenuCmd);
		ATLASSERT(m_nCommands);
		m_nStore = 2*m_nCommands + m_nAccel; // extreme: max 2 keys per command assigned
		m_pNewAccels = new ACCEL[m_nStore];
		if(0==m_pNewAccels) {
			PostMessage(WM_COMMAND, IDCANCEL);
			return 0;
		}

		// initialize listview
		m_wlcCommands = GetDlgItem(IDC_LIST_COMMANDS);
		m_wlcCommands.SetExtendedListViewStyle(LVS_EX_LABELTIP /*| LVS_EX_TWOCLICKACTIVATE*/);
		// in detailed view i MUST add a column else nothing will show
		CRect rc;
		m_wlcCommands.GetClientRect(&rc);
		int i = rc.Width();
		if(!(m_wlcCommands.GetStyle() & WS_VSCROLL))
			i -= GetSystemMetrics(SM_CXVSCROLL); // avoids appearence of horizontal one
		m_wlcCommands.InsertColumn(0, _T("cmd"), LVCFMT_LEFT, i, 0);

		// initialize hotkey
		m_wHotKey = GetDlgItem(IDC_HOTKEY);
		// invalid SHIFT could be accepted when we do +F1, only letters no goodna
		// $TSEK this allows plain F1 etc but sadly not plain "Home"/"PgUp"
		// the control by itself doesn't allow certain keys like TAB, ENTER, ESC, WINKEY...
		m_wHotKey.SetRules(HKCOMB_NONE | HKCOMB_S, HOTKEYF_ALT);
		//Hook(m_wHotKey); // for DLGC_WANTALLKEYS, same $#!+e

		// use window's passed menu to build categories
		CComboBox cbCat(GetDlgItem(IDC_COMBO_CATEGORY));
		int top = GetMenuItemCount(m_hMenuCmd); // no separators up here, surely
		CString str;
		for(i=0; i<top; i++) {
			str = GetFriendlyMenuText(m_hMenuCmd, i);
			cbCat.AddString(str);
		}
		cbCat.SetCurSel(0);

		m_wlbKeys = GetDlgItem(IDC_LIST_KEYS);

		m_nUsed = CopyAcceleratorTable(m_hInitialAccels, m_pNewAccels, m_nAccel);
		ATLASSERT(m_nUsed == m_nAccel);
		m_dwCheckSum = UGHChecksumBuf((LPCBYTE)m_pNewAccels, m_nUsed*sizeof(ACCEL));

		ChangeCategory(); // like reset

		return TRUE;
	}

	CString GetFriendlyMenuText(HMENU hMenu, int nItem, UINT uFlag=MF_BYPOSITION)
	{
		ATLASSERT(hMenu);
		TCHAR buf[256];
		ATLASSERT(!(buf[0]=0));

		GetMenuString(hMenu, nItem, buf, sizeof(buf)/sizeof(buf[0]), uFlag);
		ATLASSERT(buf[0]);
		// strip ampresands and tab characters
		CString str;
		LPTSTR prb = str.GetBuffer(lstrlen(buf)+1);
		int i=0;
		while(1) {
			TCHAR ch = buf[i++];
			if(_T('&') == ch)
				continue;
			else if(_T('\t') == ch) {
				*prb++ = 0;
				break;
			}
			else
				*prb++ = ch; // multibytes not an issue

			if(!ch)
				break;
		}

		// finally remove any trailing dots (usually "...")
		ATLASSERT(0== *(prb-1));
		prb -= 2;
		while(_T('.') == *prb)
			*prb-- = 0;
		str.ReleaseBuffer();
		ATLASSERT(str.GetLength());
		return str;
	}

	LRESULT OnCBSelectionEnd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ATLASSERT(IDC_COMBO_CATEGORY == wID);

		ChangeCategory();
		return 0;
	}

	void ChangeCategory() // react to category combo change
	{
		CComboBox cbCat(GetDlgItem(IDC_COMBO_CATEGORY));
		int cat = cbCat.GetCurSel();
		ATLASSERT(CB_ERR != cat);

		m_wlcCommands.DeleteAllItems();
		CString name;

		ATLASSERT(cat < GetMenuItemCount(m_hMenuCmd));
		// in case there are any submenus, their commands are flattened
		RecurseAddItems(GetSubMenu(m_hMenuCmd, cat), name);
		ATLASSERT( m_wlcCommands.GetItemCount() == GetSubitemCount(GetSubMenu(m_hMenuCmd, cat), FALSE) );
		ATLASSERT(m_wlcCommands.GetItemCount());

		m_wlcCommands.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		ChangeCommand();
	}

	void RecurseAddItems(HMENU hMenu, CString base)
	{
		ATLASSERT(hMenu);
		LV_ITEM lid; // lparam is command ID
		lid.mask = LVIF_TEXT | LVIF_PARAM;
		lid.iSubItem = 0;

		// browse this menu checking for submenus
		int nItems = ::GetMenuItemCount(hMenu);
		CMenuItemInfo mi;
		mi.fMask = MIIM_ID | MIIM_SUBMENU;
		CString name;
		for(int i = 0; i < nItems; i++)
		{
			::GetMenuItemInfo(hMenu, i, TRUE, &mi); // by position
			if(!mi.wID) // separators excluded
				continue;
			name = base + GetFriendlyMenuText(hMenu, i);

			if(mi.hSubMenu)
				RecurseAddItems(mi.hSubMenu, name + _T("> "));
			else {
				if(!IsValidCmdRange(mi.wID))
					continue;
				lid.pszText = (LPTSTR)(LPCTSTR)name;
				lid.iItem = m_wlcCommands.GetItemCount();
				lid.lParam = mi.wID;
				// is this mixed icon/nie mode going to make listview funny?
				m_wlcCommands.InsertItem(&lid);
			}
		}
	}

	// this helper from codeproject.com
	// Copyright 2004 J鰎gen Sigvardsson <jorgen@profitab.com>
	CString NameFromVKey(UINT nVK)
	{
		UINT nScanCode = MapVirtualKeyEx(nVK, 0, GetKeyboardLayout(0));
		switch(nVK) {
			// Keys which are "extended" (except for Return which is Numeric Enter as extended)
			case VK_INSERT:
			case VK_DELETE:
			case VK_HOME:
			case VK_END:
			case VK_NEXT:  // Page down
			case VK_PRIOR: // Page up
			case VK_LEFT:
			case VK_RIGHT:
			case VK_UP:
			case VK_DOWN:
				nScanCode |= 0x100; // Add extended bit
		}	

		// GetKeyNameText() expects the scan code to be on the same format as WM_KEYDOWN
		// Hence the left shift
		CString str;
		LPTSTR prb = str.GetBuffer(80);
		BOOL bResult = GetKeyNameText(nScanCode << 16, prb, 79);

		// these key names are capitalized and look a bit daft
		int len = lstrlen(prb);
		if(len > 1) {
			LPTSTR p2 = CharNext(prb);
			CharLowerBuff(p2, len - (p2 - prb) );
		}

		str.ReleaseBuffer();
		ATLASSERT(str.GetLength());
		return str; // internationalization ready, sweet!
	}

	LRESULT OnLVItemChanged(int idCtrl, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/) 
	{
		ATLASSERT(m_wlcCommands.GetDlgCtrlID() == idCtrl);

		ChangeCommand();
 		return 0;
	}

	LRESULT OnLBSelChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		if(IDC_LIST_KEYS == wID)
			::EnableWindow(GetDlgItem(IDC_BUT_REMOVE), m_wlbKeys.GetCurSel() >= 0);
		else
			bHandled = FALSE;
		return 0;
	}

	LRESULT OnNewAccel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		ATLASSERT(m_wHotKey == hWndCtl);
		::EnableWindow(GetDlgItem(IDC_BUT_ASSIGN), m_wHotKey.GetHotKey() );
		return 0;
	}

	void ChangeCommand() // new command selected
	{
		int active = m_wlcCommands.GetNextItem(-1, LVNI_SELECTED);
		CString descr, name;
		m_wlbKeys.ResetContent();

		if(active != -1) {
			UINT cmd = m_wlcCommands.GetItemData(active);
			descr.LoadString(cmd);
			int idx = descr.Find(_T('\n'));
			if(idx != -1)
				descr.SetAt(idx, 0); // we don't need the tooltip text

			// add all accelerators registered for this command
			for(int i=0; i<m_nUsed; i++) {
				if(m_pNewAccels[i].cmd == cmd) {
					name = NameFromAccel(m_pNewAccels[i]);
					ATLASSERT(name.GetLength());
					idx = m_wlbKeys.AddString(name);
					ATLASSERT(idx >= 0);
					m_wlbKeys.SetItemData(idx, MAKELPARAM(m_pNewAccels[i].fVirt, m_pNewAccels[i].key));
				}
			}

			m_wlbKeys.SetCurSel(LB_ERR);
		}

		SetDlgItemText(IDC_STATIC_DESCRIPTION, descr);
		m_wHotKey.SetHotKey(0,0);
		::EnableWindow(GetDlgItem(IDC_BUT_ASSIGN), FALSE);
		::EnableWindow(GetDlgItem(IDC_BUT_REMOVE), FALSE);
	}

	LRESULT OnAssign(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		WORD wVirtualKeyCode, wModifiers;
		// i thought WTL souce had a bug with this helper but it works so the MSDN DOCS have the problem!
		m_wHotKey.GetHotKey(wVirtualKeyCode, wModifiers);
		int active = m_wlcCommands.GetNextItem(-1, LVNI_SELECTED);
		ATLASSERT(wVirtualKeyCode);
		if(wVirtualKeyCode && active >= 0) {
			// i don't really understand that: is this ALWAYS a virt key? Looks like it!
			BYTE fVirt = (wModifiers & (HOTKEYF_SHIFT| HOTKEYF_CONTROL| HOTKEYF_ALT)) << 2;
			fVirt |= FVIRTKEY; // | FNOINVERT;

			ATLASSERT(FSHIFT/4 == HOTKEYF_SHIFT); // there is a resemblance
			ATLASSERT(FCONTROL/4 == HOTKEYF_CONTROL);
			ATLASSERT(FALT/4 == HOTKEYF_ALT);

			// various checks: valid range, not already assigned et cetera
			int newCmd = m_wlcCommands.GetItemData(active);
			int i=0;
			for(i=0; i<m_nUsed; i++)
				if(m_pNewAccels[i].fVirt == fVirt && m_pNewAccels[i].key == wVirtualKeyCode)
					break;
			if(i < m_nUsed) {
				// cheeky: could this be the same item being assigned a double key?
				if(newCmd==m_pNewAccels[i].cmd)
					return 0; // no actions required

				// ask for shortcut overwrite confirmation
				CString msg;
				msg.FormatMessage(IDS_MSG_KEYOVERWRITE, m_pNewAccels[i].cmd);
				
				if(::MessageBox(m_hWnd, msg, _T("problemo"), MB_ICONQUESTION | MB_YESNO) == IDNO)
					return 0;
				m_pNewAccels[i].cmd = newCmd;
			}
			else if(m_nUsed < m_nStore) { // i'm sure capacity will never be reached
				m_pNewAccels[m_nUsed].fVirt = fVirt;
				m_pNewAccels[m_nUsed].key = wVirtualKeyCode;
				m_pNewAccels[m_nUsed].cmd = newCmd;
				m_nUsed++;
			}

			ChangeCommand(); // reflect changes

			// this key can only be clicked, and if it is default & disabled, the key navigation gets screwy
			// so make something else default item
			ATLASSERT(CWindow(GetDlgItem(IDC_BUT_ASSIGN)).GetStyle() & WS_DISABLED);
			SendMessage(DM_SETDEFID, IDOK);
			m_wHotKey.SetFocus();
		}

		return 0;
	}

	LRESULT OnRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int idx = m_wlbKeys.GetCurSel();
		ATLASSERT(idx != LB_ERR);
		LRESULT lr = m_wlbKeys.GetItemData(idx);
		ATLASSERT(LB_ERR < 0);
		ATLASSERT(lr > 0);
		if(lr > 0) {
			BYTE fVirt = (BYTE)LOWORD(lr);
			WORD key = HIWORD(lr);
			for(int i=0; i<m_nUsed; i++)
				if(m_pNewAccels[i].fVirt == fVirt && m_pNewAccels[i].key == key) {
					m_wlbKeys.DeleteString(idx);
					// pack working accelerator table too
					int last = m_nUsed-1;
					if(i < last)
						m_pNewAccels[i] = m_pNewAccels[last];
					m_nUsed--;
					::EnableWindow(GetDlgItem(IDC_BUT_REMOVE), FALSE);
					break;
				}
		}

		return 0;
	}

	LRESULT OnReset(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ATLASSERT(m_pNewAccels);
		m_nAccel = ::CopyAcceleratorTable(m_hDefaultAccels, NULL, 0); // for good bookkeeping
		ATLASSERT(m_nAccel < m_nStore); // surely!
		m_nUsed = CopyAcceleratorTable(m_hDefaultAccels, m_pNewAccels, m_nAccel);
		ATLASSERT(m_nUsed == m_nAccel);

		ChangeCategory();

		return 0;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if(IDOK == wID) {
			m_nAccel = m_nUsed;
			// callers read new keys from m_pNewAccels
			ATLASSERT(m_nAccel); // there are hidden keys that are impossible to remove!

			// since these accelerators have large impact on GUI (menus etc) see if we really changed
			if(m_nAccel == m_nInitialLen) {
				DWORD chk = UGHChecksumBuf((LPCBYTE)m_pNewAccels, m_nUsed*sizeof(ACCEL));
				if(m_dwCheckSum == chk)
					m_nAccel = 0; // i trust newcomer's magic beans :)
			}
		}

		EndDialog(wID);
		return 0;
	}

	// handle some hotkey messages
#if 0
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg) { 
		case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS; // NOP: tab etc keys still not returned

		default:
			break;
		}

		return CSubclassHook::WindowProc(msg, wp, lp);
	}
#endif

public:
	int m_nAccel; // length of initial and final accelerator table
	ACCEL* m_pNewAccels; // working accelerator copy (we own it)

protected:
	HMENU m_hMenuCmd; // each menu is a category, all submenus flattened
	HACCEL m_hInitialAccels, m_hDefaultAccels; // original tables not touched
	CListViewCtrl m_wlcCommands; // all submenus of current category
	CHotKeyCtrl	m_wHotKey;
	CListBox m_wlbKeys; // shortcuts for currently selected command
	int m_nCommands; // total commands listed (all categories)
	int m_nStore, m_nUsed; // how many ACCELs allocated and how many used already
	CString m_sCtrl, m_sAlt, m_sShift;
	int m_nInitialLen; DWORD m_dwCheckSum; // trivia for initial table

#ifdef _DEBUG
private:
	CKeyAssignDlg(); // force nonstandard constructor
#endif
};


// use custom accelerator table to change the keyboard shortcuts displayed on said hMenu
void CAccelCombo::UpdateMenuKeys(HMENU hMenu) 
{
	ATLASSERT(hMenu);
	ATLASSERT(GetCount()); // we need accelerator info

	// <lame code>
	CKeyAssignDlg dummy((HMENU)1,m_hSysAccel,m_hSysAccel);
	// </lame code>

	// i can't know what has changed so do all the menues recursively
	int nItems = ::GetMenuItemCount(hMenu);
	CMenuItemInfo mi;
	mi.fMask = MIIM_ID | MIIM_SUBMENU;
	TCHAR buf[512];
	CString name;
	for(int i = 0; i < nItems; i++)
	{
		// i hope we're not going to have GPFs like in RepairMenu!
		::GetMenuItemInfo(hMenu, i, TRUE, &mi); // by position
		if(mi.hSubMenu)
			UpdateMenuKeys(mi.hSubMenu);
		else if(mi.wID != 0 && dummy.IsValidCmdRange(mi.wID)) { // separators etc excluded
			// see if there's accelerator info in text
			ATLASSERT(!(buf[0] = 0));
			::GetMenuString(hMenu, i, buf, sizeof(buf)/sizeof(buf[0]), MF_BYPOSITION);
			ATLASSERT(buf[0]);

			int len = lstrlen(buf), k = len;
			while(k--)
				if(_T('\t') == buf[k])
					break;
			BOOL bTab = k > 0;
			BOOL bChanged = 1;

			// is there any accelerator for this command nowadays?
			int idx = FindCommand(mi.wID);
			if(-1 == idx) {
				if(bTab)
					buf[k] = 0; // remove old one
				else
					bChanged = 0;
			}
			else {
				if(!bTab) {
					k = len;
					buf[k] = _T('\t');
				}
				k++;

				name = dummy.NameFromAccel(m_pAccelTable[idx]);
				ATLASSERT(k+name.GetLength() < sizeof(buf)/sizeof(buf[0]));
				lstrcpy(buf+k, name);
			}

			if(bChanged) {
				ATLASSERT(lstrlen(buf));
				::ModifyMenu(hMenu, i, MF_BYPOSITION, mi.wID, buf);
				// $TSEK no need to update item enable/icon states? (see wtl's command bar atlctrlw.h line 2630)
			}
		}
	}
}


#endif // #ifndef UPRIGHT_KEYASSIGNDLG_H
