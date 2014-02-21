// SysHotKeyDiyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysHotKeyDiy.h"
#include "SysHotKeyDiyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSysHotKeyDiyDlg dialog




CSysHotKeyDiyDlg::CSysHotKeyDiyDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSysHotKeyDiyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	gIndex = 0;
	bStart = FALSE;
}

void CSysHotKeyDiyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_ListShow);
	DDX_Control(pDX, IDC_EDIT_PATH, m_EditPath);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strEditPath);
	DDX_Control(pDX, IDC_COMBO_ASC, m_ComboAsc);
	DDX_Control(pDX, IDC_CHECK_CTRL, m_CtrlCheck);
	DDX_Control(pDX, IDC_CHECK_SHIFT, m_ShiftCheck);
	DDX_Control(pDX, IDC_CHECK_ALT, m_AltCheck);
	DDX_Control(pDX, IDC_CHECK_AUTO_START, m_AStartCheck);
	DDX_Control(pDX, IDC_CHECK_INIT_TRAYICON, m_InitIconCheck);
	DDX_Control(pDX, IDC_CHECK_MINSIZE_TRAYICON, m_MinSizeIconCheck);
}

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
BEGIN_MESSAGE_MAP(CSysHotKeyDiyDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BROWSE, &CSysHotKeyDiyDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_SETHOTKEY, &CSysHotKeyDiyDlg::OnBnClickedSethotkey)
	ON_MESSAGE(WM_HOTKEY, &CSysHotKeyDiyDlg::OnHotKey)
	ON_BN_CLICKED(IDC_RESETHOTKEY, &CSysHotKeyDiyDlg::OnBnClickedResethotkey)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_AUTO_START, &CSysHotKeyDiyDlg::OnBnClickedCheckAutoStart)
	ON_BN_CLICKED(IDC_CHECK_INIT_TRAYICON, &CSysHotKeyDiyDlg::OnBnClickedCheckInitTrayicon)
	ON_MESSAGE(MY_TRAYICON_MSG, OnTrayCallBackMsg)
	ON_COMMAND(ID_MENU_SHOW, &CSysHotKeyDiyDlg::OnMenuShow)
	ON_COMMAND(ID_MENU_CLOSE, &CSysHotKeyDiyDlg::OnMenuClose)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_MINSIZE_TRAYICON, &CSysHotKeyDiyDlg::OnBnClickedCheckMinsizeTrayicon)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SHOW, &CSysHotKeyDiyDlg::OnNMClickListShow)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RESET_ALLKEY, &CSysHotKeyDiyDlg::OnBnClickedResetAllkey)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, &CSysHotKeyDiyDlg::OnTaskBarCreated)
	ON_BN_CLICKED(IDC_LIST_ITEM_UP, &CSysHotKeyDiyDlg::OnBnClickedListItemUp)
	ON_BN_CLICKED(IDC_LIST_ITEM_DOWN, &CSysHotKeyDiyDlg::OnBnClickedListItemDown)
END_MESSAGE_MAP()


// CSysHotKeyDiyDlg message handlers

BOOL CSysHotKeyDiyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CMenu *pMenu = GetSystemMenu(FALSE);
	if ( pMenu != NULL ){
		pMenu->DeleteMenu(0, MF_BYPOSITION);
		pMenu->DeleteMenu(1, MF_BYPOSITION);
		pMenu->DeleteMenu(2, MF_BYPOSITION);
		pMenu->DeleteMenu(2, MF_BYPOSITION);
	}

	SetProp(m_hWnd, _T("SysHotKeyDiy_Prop"), (HANDLE)9999);

	m_StatBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);
	int strPartDim[2]= {190, -1};
	m_StatBar.SetParts(2, strPartDim);
	m_StatBar.SetText(_T("作者：Syc  ★  < QQ：7657318 - WwW.CcTry.Com >  ★"), 1, 0);
	//SetTimer(1, 100, NULL);

	m_ListShow.SetExtendedStyle(m_ListShow.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ListShow.InsertColumn(0, _T("序 号"), LVCFMT_LEFT, 60);
	m_ListShow.InsertColumn(1, _T("热 键"), LVCFMT_LEFT, 120);
	m_ListShow.InsertColumn(2, _T("路 径"), LVCFMT_LEFT, 360);

	if ( ReadAutoStartState() == TRUE ){
		m_AStartCheck.SetCheck(BST_CHECKED);
	}
	if ( ReadMinSizeTrayIcon() == TRUE ){
		m_MinSizeIconCheck.SetCheck(BST_CHECKED);
	}
	if ( ReadInitTrayIcon() == TRUE ){
		m_InitIconCheck.SetCheck(BST_CHECKED);
		AddTrayIcon(m_hWnd);
	}

	CheckRadioButton(IDC_RADIO_FILE, IDC_RADIO_FOLDER, IDC_RADIO_FILE);
	int index = 0;
	for ( index = 0; index < 10; index++ ){
		CString str;
		str.Format(_T("%d"), index);
		m_ComboAsc.InsertString(index, str);
	}
	m_ComboAsc.InsertString(index++, _T("Space"));
	for ( int i = 1; i < 13; i++ ){
		CString str;
		str.Format(_T("%d"), i);
		str = _T("F") + str;
		m_ComboAsc.InsertString(index++, str);
	}
	TCHAR szCHAR = '@';
	for ( int i = 0; i < 26; i++ ){
		CString str;
		szCHAR += 1;
		str.Format(_T("%c"), szCHAR);
		m_ComboAsc.InsertString(index++, str);
	}
	m_ComboAsc.InsertString(index++, _T("-"));
	m_ComboAsc.InsertString(index++, _T("="));
	m_ComboAsc.InsertString(index++, _T("["));
	m_ComboAsc.InsertString(index++, _T("]"));
	m_ComboAsc.InsertString(index++, _T(";"));
	m_ComboAsc.InsertString(index++, _T("\'"));
	m_ComboAsc.InsertString(index++, _T("\\"));
	m_ComboAsc.InsertString(index++, _T(","));
	m_ComboAsc.InsertString(index++, _T("."));
	m_ComboAsc.InsertString(index++, _T("/"));
	m_ComboAsc.InsertString(index++, _T("`"));
	m_ComboAsc.InsertString(index++, _T("BackSpace"));
	m_ComboAsc.InsertString(index++, _T("Tab"));
	m_ComboAsc.InsertString(index++, _T("Enter"));
	m_ComboAsc.InsertString(index++, _T("Page Up"));
	m_ComboAsc.InsertString(index++, _T("Page Down"));
	m_ComboAsc.InsertString(index++, _T("End"));
	m_ComboAsc.InsertString(index++, _T("Home"));
	m_ComboAsc.InsertString(index++, _T("Left"));
	m_ComboAsc.InsertString(index++, _T("Right"));
	m_ComboAsc.InsertString(index++, _T("Up"));
	m_ComboAsc.InsertString(index++, _T("Down"));
	m_ComboAsc.InsertString(index++, _T("Insert"));
	m_ComboAsc.InsertString(index++, _T("Delete"));
	m_ComboAsc.InsertString(index++, _T("NumPad 0"));
	m_ComboAsc.InsertString(index++, _T("NumPad 1"));
	m_ComboAsc.InsertString(index++, _T("NumPad 2"));
	m_ComboAsc.InsertString(index++, _T("NumPad 3"));
	m_ComboAsc.InsertString(index++, _T("NumPad 4"));
	m_ComboAsc.InsertString(index++, _T("NumPad 5"));
	m_ComboAsc.InsertString(index++, _T("NumPad 6"));
	m_ComboAsc.InsertString(index++, _T("NumPad 7"));
	m_ComboAsc.InsertString(index++, _T("NumPad 8"));
	m_ComboAsc.InsertString(index++, _T("NumPad 9"));
	m_ComboAsc.InsertString(index++, _T("NumPad *"));
	m_ComboAsc.InsertString(index++, _T("NumPad +"));
	m_ComboAsc.InsertString(index++, _T("NumPad -"));
	m_ComboAsc.InsertString(index++, _T("NumPad ."));
	m_ComboAsc.InsertString(index++, _T("NumPad /"));

	arrHotKey.RemoveAll();
	if ( RegReadHotKey(&arrHotKey) == TRUE ) {
		for ( int index = 0; index < arrHotKey.GetCount(); index++ ){
			ParserStrHkToDword(&(arrHotKey.GetAt(index)));
			BOOL bRet = RegisterHotKey(m_hWnd, arrHotKey.GetAt(index).dwHkID, arrHotKey.GetAt(index).dwSys, arrHotKey.GetAt(index).dwAsc);
			if ( !bRet ){
				arrHotKey.RemoveAt(index--);
				MessageBox(_T("热键 [ ") + arrHotKey.GetAt(index).strHotKey + _T(" ] 冲突，注册热键失败！"), _T("提示"), MB_OK | MB_ICONERROR );
			}else{
				int nCount = m_ListShow.GetItemCount();
				m_ListShow.InsertItem(nCount, _T(""));
				CString strID;
				strID.Format(_T("%d"), arrHotKey.GetAt(index).dwHkID);
				m_ListShow.SetItemText(nCount, 0, strID);
				m_ListShow.SetItemText(nCount, 1, arrHotKey.GetAt(index).strHotKey);
				m_ListShow.SetItemText(nCount, 2, arrHotKey.GetAt(index).strPath);
			}
		}
	}
	bStart = TRUE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSysHotKeyDiyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSysHotKeyDiyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSysHotKeyDiyDlg::OnBnClickedBrowse()
{
	int nID = GetCheckedRadioButton(IDC_RADIO_FILE, IDC_RADIO_FOLDER);
	if ( nID == IDC_RADIO_FOLDER ){
		TCHAR FolderPath[MAX_PATH];
		TCHAR FullPath[MAX_PATH];
		memset(FolderPath, 0, MAX_PATH);
		memset(FullPath, 0, MAX_PATH);
		BROWSEINFO mBroInfo;
		mBroInfo.hwndOwner = m_hWnd;
		mBroInfo.iImage = 0;
		mBroInfo.lParam = 0;
		mBroInfo.lpfn = NULL;
		mBroInfo.lpszTitle = _T("Please Select A Folder...");
		mBroInfo.pidlRoot = NULL;
		mBroInfo.pszDisplayName = FolderPath;
		mBroInfo.ulFlags = BIF_RETURNONLYFSDIRS;
		ITEMIDLIST *pidl = ::SHBrowseForFolder(&mBroInfo);
		if (::SHGetPathFromIDList(pidl, FullPath) == TRUE){
			m_strEditPath.Format(_T("%s"), FullPath);
			UpdateData(FALSE);
		}
	}else{
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All Files (*.*)|*.*||"));
		if ( dlg.DoModal() == IDOK ){
			m_strEditPath = dlg.GetPathName();
			UpdateData(FALSE);
		}
	}
}

void CSysHotKeyDiyDlg::OnBnClickedSethotkey()
{
	UpdateData(TRUE);
	if ( CheckInfoFillRight() == FALSE ){
		MessageBox(_T("请把信息填写完整！"), _T("提示"), MB_OK | MB_ICONWARNING);
		return;
	}
	HKNODE mHkNode;
	GetCurHkInfo(&mHkNode);
	mHkNode.dwHkID = HOT_KEY_ID+gIndex++;
	BOOL bRet = RegisterHotKey(m_hWnd, mHkNode.dwHkID, mHkNode.dwSys, mHkNode.dwAsc);
	if ( !bRet && GetLastError() == 1409){
		int rID = 0;
		int iRow = FindStringInListCtrl(mHkNode.strHotKey, FIND_FLAG_HOTKEY);
		if ( iRow != -1 ){
			CString strPath = m_ListShow.GetItemText(iRow, 2);
			CString strHotKey = m_ListShow.GetItemText(iRow, 1);
			if ( strPath == mHkNode.strPath && strHotKey == mHkNode.strHotKey) {
				return;
			}
			rID = MessageBox(_T("热键 [ ") + mHkNode.strHotKey + _T(" ] 已注册为 [ ") + strPath + _T(" ] ！\r\n确定修改为 [ ") + mHkNode.strPath + _T(" ] 吗？"), _T("提示"), MB_OKCANCEL | MB_ICONERROR );
		}else{
			MessageBox(_T("热键 [ ") + mHkNode.strHotKey + _T(" ] 注册失败！"), _T("提示"), MB_OK | MB_ICONERROR );
			return;
		}
		if ( rID == IDOK ){
			DWORD dwIDed = 0;
			dwIDed = MyStrToInt(m_ListShow.GetItemText(iRow, 0));
			UnregisterHotKey(m_hWnd, dwIDed);
			bRet = RegisterHotKey(m_hWnd, dwIDed, mHkNode.dwSys, mHkNode.dwAsc);
			if ( !bRet ){
				MessageBox(_T("热键 [ ") + mHkNode.strHotKey + _T(" ] 注册失败！"), _T("提示"), MB_OK | MB_ICONERROR );
				return;
			}else{
				m_ListShow.SetItemText(iRow, 2, m_strEditPath);
				m_ListShow.SetFocus();
				m_ListShow.SetItemState(iRow, LVIS_SELECTED, LVIS_SELECTED);
				for ( int i = 0; i < arrHotKey.GetCount(); i++ ){
					if ( arrHotKey.GetAt(i).strHotKey == mHkNode.strHotKey ){
						arrHotKey.GetAt(i).strPath = m_strEditPath;
					}
				}
				RegSetHotKey(&mHkNode);
				m_CtrlCheck.SetCheck(BST_UNCHECKED);
				m_ShiftCheck.SetCheck(BST_UNCHECKED);
				m_AltCheck.SetCheck(BST_UNCHECKED);
				m_ComboAsc.SetCurSel(-1);
				CheckRadioButton(IDC_RADIO_FILE, IDC_RADIO_FOLDER, IDC_RADIO_FILE);
				SetDlgItemText(IDC_EDIT_PATH, _T(""));
				MessageBox(_T("修改成功！"), _T("提示"), MB_OK | MB_ICONWARNING);
			}
		}
	}else{
		int nCount = m_ListShow.GetItemCount();
		m_ListShow.InsertItem(nCount, _T(""));
		CString strID;
		strID.Format(_T("%d"), mHkNode.dwHkID);
		m_ListShow.SetItemText(nCount, 0, strID);
		m_ListShow.SetItemText(nCount, 1, mHkNode.strHotKey);
		m_ListShow.SetItemText(nCount, 2, mHkNode.strPath);
		arrHotKey.Add(mHkNode);
		RegSetHotKey(&mHkNode);
		m_CtrlCheck.SetCheck(BST_UNCHECKED);
		m_ShiftCheck.SetCheck(BST_UNCHECKED);
		m_AltCheck.SetCheck(BST_UNCHECKED);
		m_ComboAsc.SetCurSel(-1);
		CheckRadioButton(IDC_RADIO_FILE, IDC_RADIO_FOLDER, IDC_RADIO_FILE);
		SetDlgItemText(IDC_EDIT_PATH, _T(""));
		m_ListShow.EnsureVisible(nCount, TRUE);
	}
}

BOOL CSysHotKeyDiyDlg::CheckInfoFillRight()
{
	UpdateData(TRUE);
	if ( m_CtrlCheck.GetCheck() == BST_CHECKED || m_ShiftCheck.GetCheck() == BST_CHECKED || m_AltCheck.GetCheck() == BST_CHECKED ){
		CString strAsc;
		int index = m_ComboAsc.GetCurSel();
		if ( index >= 0 ) {
			m_ComboAsc.GetLBText(index, strAsc);
			if ( !strAsc.IsEmpty() && !m_strEditPath.IsEmpty() ){
				return TRUE;
			}
		}
	}
	return FALSE;
}

DWORD CSysHotKeyDiyDlg::StrAsc2Dword(CString in_StrAsc)
{
	if ( in_StrAsc.GetLength() == 1 ){
		switch(in_StrAsc[0])
		{
		case '-':
			return VK_OEM_MINUS;
		case '=':
			return VK_OEM_PLUS;
		case ';':
			return VK_OEM_1;
		case '\'':
			return VK_OEM_7;
		case '\\':
			return VK_OEM_5;
		case '[':
			return VK_OEM_4;
		case ']':
			return VK_OEM_6;
		case ',':
			return VK_OEM_COMMA;
		case '.':
			return VK_OEM_PERIOD;
		case '/':
			return VK_OEM_2;
		case '`':
			return VK_OEM_3;
		default:
			return in_StrAsc[0];
		}
	}else{
		if ( in_StrAsc == _T("Space") ){
			return VK_SPACE;
		}
		if ( in_StrAsc == _T("F1") ){
			return VK_F1;
		}
		if ( in_StrAsc == _T("F2") ){
			return VK_F2;
		}
		if ( in_StrAsc == _T("F3") ){
			return VK_F3;
		}
		if ( in_StrAsc == _T("F4") ){
			return VK_F4;
		}
		if ( in_StrAsc == _T("F5") ){
			return VK_F5;
		}
		if ( in_StrAsc == _T("F6") ){
			return VK_F6;
		}
		if ( in_StrAsc == _T("F7") ){
			return VK_F7;
		}
		if ( in_StrAsc == _T("F8") ){
			return VK_F8;
		}
		if ( in_StrAsc == _T("F9") ){
			return VK_F9;
		}
		if ( in_StrAsc == _T("F10") ){
			return VK_F10;
		}
		if ( in_StrAsc == _T("F11") ){
			return VK_F11;
		}
		if ( in_StrAsc == _T("F12") ){
			return VK_F12;
		}
		if ( in_StrAsc == _T("BackSpace") ){
			return VK_BACK;
		}
		if ( in_StrAsc == _T("Tab") ){
			return VK_TAB;
		}
		if ( in_StrAsc == _T("BackSpace") ){
			return VK_BACK;
		}
		if ( in_StrAsc == _T("Enter") ){
			return VK_RETURN;
		}
		if ( in_StrAsc == _T("Page Up") ){
			return VK_PRIOR;
		}
		if ( in_StrAsc == _T("Page Down") ){
			return VK_NEXT;
		}
		if ( in_StrAsc == _T("End") ){
			return VK_END;
		}
		if ( in_StrAsc == _T("Home") ){
			return VK_HOME;
		}
		if ( in_StrAsc == _T("Left") ){
			return VK_LEFT;
		}
		if ( in_StrAsc == _T("Right") ){
			return VK_RIGHT;
		}
		if ( in_StrAsc == _T("Up") ){
			return VK_UP;
		}
		if ( in_StrAsc == _T("Down") ){
			return VK_DOWN;
		}
		if ( in_StrAsc == _T("Insert") ){
			return VK_INSERT;
		}
		if ( in_StrAsc == _T("Delete") ){
			return VK_DELETE;
		}
		if ( in_StrAsc == _T("NumPad 0") ){
			return VK_NUMPAD0;
		}
		if ( in_StrAsc == _T("NumPad 1") ){
			return VK_NUMPAD1;
		}
		if ( in_StrAsc == _T("NumPad 2") ){
			return VK_NUMPAD2;
		}
		if ( in_StrAsc == _T("NumPad 3") ){
			return VK_NUMPAD3;
		}
		if ( in_StrAsc == _T("NumPad 4") ){
			return VK_NUMPAD4;
		}
		if ( in_StrAsc == _T("NumPad 5") ){
			return VK_NUMPAD5;
		}
		if ( in_StrAsc == _T("NumPad 6") ){
			return VK_NUMPAD6;
		}
		if ( in_StrAsc == _T("NumPad 7") ){
			return VK_NUMPAD7;
		}
		if ( in_StrAsc == _T("NumPad 8") ){
			return VK_NUMPAD8;
		}
		if ( in_StrAsc == _T("NumPad 9") ){
			return VK_NUMPAD9;
		}
		if ( in_StrAsc == _T("NumPad *") ){
			return VK_MULTIPLY;
		}
		if ( in_StrAsc == _T("NumPad +") ){
			return VK_ADD;
		}
		if ( in_StrAsc == _T("NumPad -") ){
			return VK_SUBTRACT;
		}
		if ( in_StrAsc == _T("NumPad .") ){
			return VK_DECIMAL;
		}
		if ( in_StrAsc == _T("NumPad /") ){
			return VK_DIVIDE;
		}
	}
	return FALSE;
}

BOOL CSysHotKeyDiyDlg::GetCurHkInfo(HKNODE *pHkNode)
{
	UpdateData(TRUE);
	pHkNode->dwSys = 0;
	pHkNode->strHotKey.Empty();
	if ( m_CtrlCheck.GetCheck() == BST_CHECKED ){
		pHkNode->strHotKey += _T("Ctrl+");
		pHkNode->dwSys |= MOD_CONTROL;
	}
	if ( m_ShiftCheck.GetCheck() == BST_CHECKED ){
		pHkNode->strHotKey += _T("Shift+");
		pHkNode->dwSys |= MOD_SHIFT;
	}
	if ( m_AltCheck.GetCheck() == BST_CHECKED ){
		pHkNode->strHotKey += _T("Alt+");
		pHkNode->dwSys |= MOD_ALT;
	}
	CString strAsc;
	m_ComboAsc.GetLBText(m_ComboAsc.GetCurSel(), strAsc);
	pHkNode->strHotKey += strAsc;
	pHkNode->dwAsc = StrAsc2Dword(strAsc);
	pHkNode->strPath = m_strEditPath;
	return TRUE;
}

LRESULT CSysHotKeyDiyDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	for ( int index = 0; index < arrHotKey.GetCount(); index++ ){
		if ( wParam == arrHotKey.GetAt(index).dwHkID ){
			TCHAR szPath[_MAX_PATH] = {0};
			DWORD dwRet = GetModuleFileName(NULL, szPath, _MAX_PATH);
			if ( dwRet > 0 ) {
				CString strPath(szPath);
				if ( strPath.CompareNoCase(arrHotKey.GetAt(index).strPath) == 0 ) {
					DelTrayIcon(m_hWnd);
					return TRUE;
				}
			}
			HINSTANCE hInstance = ShellExecute(NULL, _T("open"), arrHotKey.GetAt(index).strPath, NULL, NULL, SW_SHOWNORMAL);
			if ( (int)hInstance < 32 ){
				int iRet = MessageBox(_T("运行 [ ") + arrHotKey.GetAt(index).strPath + _T(" ] 错误！\r\n请确认文件或文件夹存在！\r\n删除热键吗？"), _T("提示"), MB_OKCANCEL | MB_ICONERROR);
				if ( iRet == IDOK ) {
					UnregisterHotKey(m_hWnd, arrHotKey.GetAt(index).dwHkID);
					RegSetHotKey(&(arrHotKey.GetAt(index)), TRUE);
					iRet = FindStringInListCtrl(arrHotKey.GetAt(index).strHotKey, FIND_FLAG_HOTKEY);
					m_ListShow.DeleteItem(iRet);
					arrHotKey.RemoveAt(index);
					index--;
				}
			}
		}
	}
	return TRUE;
}
void CSysHotKeyDiyDlg::OnBnClickedResethotkey()
{
	int ps =  (int)m_ListShow.GetFirstSelectedItemPosition();
	if ( ps > 0 ){
		int iRet = MessageBox(_T("确定撤销热键吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
		if ( iRet == IDOK ) {
			CString strID = m_ListShow.GetItemText(ps-1, 0);
			int iID = MyStrToInt(strID);
			for ( int i = 0; i < arrHotKey.GetCount(); i++ ){
				if ( arrHotKey.GetAt(i).dwHkID == iID ){
					RegSetHotKey( &(arrHotKey.GetAt(i)), TRUE);
					arrHotKey.RemoveAt(i--);
					break;
				}
			}
			m_ListShow.DeleteItem(ps-1);
			UnregisterHotKey(m_hWnd, iID);
			m_CtrlCheck.SetCheck(BST_UNCHECKED);
			m_ShiftCheck.SetCheck(BST_UNCHECKED);
			m_AltCheck.SetCheck(BST_UNCHECKED);
			m_ComboAsc.SetCurSel(-1);
			CheckRadioButton(IDC_RADIO_FILE, IDC_RADIO_FOLDER, IDC_RADIO_FILE);
			SetDlgItemText(IDC_EDIT_PATH, _T(""));
		}else{
			m_CtrlCheck.SetCheck(BST_UNCHECKED);
			m_ShiftCheck.SetCheck(BST_UNCHECKED);
			m_AltCheck.SetCheck(BST_UNCHECKED);
			m_ComboAsc.SetCurSel(-1);
			CheckRadioButton(IDC_RADIO_FILE, IDC_RADIO_FOLDER, IDC_RADIO_FILE);
			SetDlgItemText(IDC_EDIT_PATH, _T(""));
		}
	}else{
		MessageBox(_T("请选择一行！"), _T("提示"), MB_OK | MB_ICONWARNING);
	}
}

void CSysHotKeyDiyDlg::OnDestroy()
{
	CDialog::OnDestroy();
	for ( int index = 0; index < arrHotKey.GetCount(); index++ ){
		UnregisterHotKey(m_hWnd, arrHotKey.GetAt(index).dwHkID);
	}
	RemoveProp(m_hWnd, _T("SysHotKeyDiy_Prop"));
}

int CSysHotKeyDiyDlg::MyStrToInt(CString in_Str)
{
	int nLen = in_Str.GetLength();
	int len = nLen;
	int temp[10] = {0}, i = 0;
	UINT result = 0;
	while ( i < nLen ){
		switch(in_Str[i]){
			case '0' :temp[i] = 0; break;
			case '1' :temp[i] = 1; break;
			case '2' :temp[i] = 2; break;
			case '3' :temp[i] = 3; break;
			case '4' :temp[i] = 4; break;
			case '5' :temp[i] = 5; break;
			case '6' :temp[i] = 6; break;
			case '7' :temp[i] = 7; break;
			case '8' :temp[i] = 8; break;
			case '9' :temp[i] = 9; break;
			default :{
				return -1;
				break;
					 }
		}
		i++;
	}
	for (i = 0; i < nLen; i++){
		result += (UINT)(temp[i]*pow((double)10, --len));
	}
	return result;
}
BOOL CSysHotKeyDiyDlg::RegSetHotKey(HKNODE *pHkNode, BOOL bDelete)
{
	HKEY hKey;
	LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SysHotKeyDiy\\HotKey"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		if ( bDelete == FALSE ){
			LRet = RegSetValueEx(hKey, pHkNode->strHotKey, 0, REG_SZ, (BYTE *)pHkNode->strPath.GetBuffer(), pHkNode->strPath.GetLength()*2);
			if ( LRet == ERROR_SUCCESS ){
				return TRUE;
			}
		}else{
			LRet = RegDeleteValue(hKey, pHkNode->strHotKey);
			if ( LRet == ERROR_SUCCESS ){
				return TRUE;
			}
		}
	}
	RegCloseKey(hKey);
	return FALSE;
}

BOOL CSysHotKeyDiyDlg::RegSetHotKey(CArray <HKNODE, HKNODE> *pArrHotKey, BOOL bRemoveAllFirst)
{
	if ( pArrHotKey != NULL ) {
		HKEY hKey;
		LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SysHotKeyDiy"), &hKey);
		if ( LRet == ERROR_SUCCESS ){
			if ( bRemoveAllFirst ) {
				LRet = SHDeleteKey(hKey, _T("HotKey"));
				if ( LRet != ERROR_SUCCESS ) {
					RegCloseKey(hKey);
					return FALSE;
				}
			}
			BOOL bRet = FALSE;
			for( int index = 0; index < pArrHotKey->GetCount (); index++ ) {
				bRet = RegSetHotKey(&(pArrHotKey->GetAt (index)));
				if ( !bRet ) {
					RegCloseKey(hKey);
					return FALSE;
				}
			}
		}
		RegCloseKey(hKey);
	}
	return FALSE;
}

BOOL CSysHotKeyDiyDlg::RegReadHotKey(CArray <HKNODE, HKNODE> *pArrHotKey)
{
	pArrHotKey->RemoveAll();
	HKEY hKey;
	LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SysHotKeyDiy\\HotKey"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		DWORD dwValues = 0, dwMaxValueNameLen = 0, dwMaxValueLen = 0;
		LRet = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValues, &dwMaxValueNameLen, &dwMaxValueLen, NULL, NULL);
		dwMaxValueNameLen = dwMaxValueNameLen*2+1;
		dwMaxValueLen = dwMaxValueLen*2+1;
		TCHAR *pszValueName = new TCHAR[dwMaxValueNameLen];
		TCHAR *pszValueData = new TCHAR[dwMaxValueLen];
		ZeroMemory(pszValueName, dwMaxValueNameLen);
		ZeroMemory(pszValueData, dwMaxValueLen);
		for ( DWORD i = 0; i < dwValues; i++ ){
			DWORD dwValueNameLen = dwMaxValueNameLen;
			DWORD dwValueLen = dwMaxValueLen;
			LRet = RegEnumValue(hKey, i, pszValueName, &dwValueNameLen, NULL, NULL, (LPBYTE)pszValueData, &dwValueLen);
			if ( LRet == ERROR_SUCCESS ){
				HKNODE hkNode;
				hkNode.strHotKey.Format(_T("%s"), pszValueName);
				hkNode.strPath.Format(_T("%s"), pszValueData);
				hkNode.dwHkID = HOT_KEY_ID+gIndex++;
				pArrHotKey->Add(hkNode);
			}
		}
		delete [] pszValueName;
		delete [] pszValueData;
		RegCloseKey(hKey);
		return TRUE;
	}
	RegCloseKey(hKey);
	return FALSE;
}
BOOL CSysHotKeyDiyDlg::ParserStrHkToDword(HKNODE *pOut_Node)
{
	int nLen = pOut_Node->strHotKey.GetLength();
	int index = 0, iLast = 0, iLastAdd = 0;
	CString strSys;
	pOut_Node->dwSys = 0;
	for ( int i = 0; i < nLen; i++ ){
		if ( pOut_Node->strHotKey[i] == '+' ){
			iLastAdd = i;
		}
	}
	for ( index = 0; index < nLen; index++ ){
		if ( pOut_Node->strHotKey[index] == '+' ){
			strSys = pOut_Node->strHotKey.Mid(iLast, index-iLast);
			pOut_Node->dwSys |= Sys2Dword(strSys);
			iLast = index+1;
		}
	}
	CString strAsc = pOut_Node->strHotKey.Right(nLen-iLastAdd-1);
	pOut_Node->dwAsc = StrAsc2Dword(strAsc);
	return TRUE;
}
DWORD CSysHotKeyDiyDlg::Sys2Dword(CString in_Str)
{
	DWORD dwSys = 0;
	if ( in_Str == _T("Ctrl") ){
		dwSys = MOD_CONTROL;
		return dwSys;
	}
	if ( in_Str == _T("Shift") ){
		dwSys = MOD_SHIFT;
		return dwSys;
	}
	if ( in_Str == _T("Alt") ){
		dwSys = MOD_ALT;
		return dwSys;
	}
	return dwSys;
}

int CSysHotKeyDiyDlg::FindStringInListCtrl(CString in_Str, DWORD dwFindFlag)
{
	int iResult = -1;
	int iFind = -1;
	if ( dwFindFlag == FIND_FLAG_ID ){
		iFind = 0;
	}else{
		if ( dwFindFlag == FIND_FLAG_HOTKEY ){
			iFind = 1;
		}
	}
	for ( int i = 0; i < m_ListShow.GetItemCount(); i++ ){
		CString str = m_ListShow.GetItemText(i, iFind);
		if ( in_Str == str ){
			iResult = i;
			break;
		}
	}
	return iResult;
}
void CSysHotKeyDiyDlg::OnBnClickedCheckAutoStart()
{
	UpdateData(TRUE);
	int iCheck = m_AStartCheck.GetCheck();
	HKEY hKey;
	LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		if ( iCheck == BST_CHECKED ){
			TCHAR szPath[_MAX_PATH] = {0};
			GetModuleFileName(NULL, szPath, _MAX_PATH);
			LRet = RegSetValueEx(hKey, _T("SysHotKeyDiy.exe"), 0, REG_SZ, (BYTE *)szPath, (DWORD)_tcslen(szPath)*2);
		}else{
			LRet = RegDeleteValue(hKey, _T("SysHotKeyDiy.exe"));
		}
	}
	RegCloseKey(hKey);
}

BOOL CSysHotKeyDiyDlg::ReadAutoStartState()
{
	HKEY hKey;
	LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		TCHAR szByteData[_MAX_PATH*2] = {0};
		DWORD dwData = _MAX_PATH*2;
		LRet = RegQueryValueEx(hKey, _T("SysHotKeyDiy.exe"), NULL, NULL, (LPBYTE)szByteData, &dwData);
		if ( LRet == ERROR_SUCCESS ){
			TCHAR szPath[_MAX_PATH] = {0};
			GetModuleFileName(NULL, szPath, _MAX_PATH);
			CString strRegFile(szByteData), strFile(szPath);
			if ( strRegFile.CompareNoCase(strFile) == 0 ){
				RegCloseKey(hKey);
				return TRUE;
			}
		}
	}
	RegCloseKey(hKey);
	return FALSE;
}

void CSysHotKeyDiyDlg::OnBnClickedCheckInitTrayicon()
{
	UpdateData(TRUE);
	int iCheck = m_InitIconCheck.GetCheck();
	HKEY hKey;
	LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SysHotKeyDiy\\SetInfo"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		int iResult = -1;
		if ( iCheck == BST_CHECKED ){
			iResult = 1;
		}else{
			iResult = 0;
		}
		LRet = RegSetValueEx(hKey, _T("InitTrayIcon"), 0, REG_DWORD, (LPBYTE)&iResult, 4);
	}
	RegCloseKey(hKey);
}

BOOL CSysHotKeyDiyDlg::ReadInitTrayIcon()
{
	HKEY hKey;
	LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SysHotKeyDiy\\SetInfo"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		int iResult = -1;
		DWORD dwLen = 4;
		LRet = RegQueryValueEx(hKey, _T("InitTrayIcon"), NULL, NULL, (LPBYTE)&iResult, &dwLen);
		if ( LRet == ERROR_SUCCESS ){
			if ( iResult == 1 ){
				RegCloseKey(hKey);
				return TRUE;
			}
		}
	}
	RegCloseKey(hKey);
	return FALSE;
}
LRESULT CSysHotKeyDiyDlg::OnTrayCallBackMsg(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case WM_RBUTTONUP:
		{
			CPoint pt;
			CMenu mMenu, *pMenu;
			mMenu.LoadMenu(IDR_TRAYICON_MENU);
			pMenu = mMenu.GetSubMenu(0);
			GetCursorPos(&pt);
			SetForegroundWindow();
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
			break;
		}
	case WM_LBUTTONDBLCLK:
		DelTrayIcon(m_hWnd);
	}
	return NULL;
}
void CSysHotKeyDiyDlg::OnMenuShow()
{
	DelTrayIcon(m_hWnd);
}

void CSysHotKeyDiyDlg::OnMenuClose()
{
	DelTrayIcon(m_hWnd, FALSE);
	DestroyWindow();
}

void CSysHotKeyDiyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if ( bStart == TRUE && nType == SIZE_MINIMIZED && m_MinSizeIconCheck.GetCheck() == BST_CHECKED){
		AddTrayIcon(m_hWnd);
	}
}

void CSysHotKeyDiyDlg::OnBnClickedCheckMinsizeTrayicon()
{
	UpdateData(TRUE);
	int iCheck = m_MinSizeIconCheck.GetCheck();
	HKEY hKey;
	LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SysHotKeyDiy\\SetInfo"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		int iResult = -1;
		if ( iCheck == BST_CHECKED ){
			iResult = 1;
		}else{
			iResult = 0;
		}
		LRet = RegSetValueEx(hKey, _T("MinSizeTrayIcon"), 0, REG_DWORD, (LPBYTE)&iResult, 4);
	}
	RegCloseKey(hKey);
}

BOOL CSysHotKeyDiyDlg::ReadMinSizeTrayIcon()
{
	HKEY hKey;
	LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SysHotKeyDiy\\SetInfo"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		int iResult = -1;
		DWORD dwLen = 4;
		LRet = RegQueryValueEx(hKey, _T("MinSizeTrayIcon"), NULL, NULL, (LPBYTE)&iResult, &dwLen);
		if ( LRet == ERROR_SUCCESS ){
			if ( iResult == 1 ){
				RegCloseKey(hKey);
				return TRUE;
			}
		}
	}
	RegCloseKey(hKey);
	return FALSE;
}

void CSysHotKeyDiyDlg::OnNMClickListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_CtrlCheck.SetCheck(BST_UNCHECKED);
	m_ShiftCheck.SetCheck(BST_UNCHECKED);
	m_AltCheck.SetCheck(BST_UNCHECKED);
	m_ComboAsc.SetCurSel(-1);
	CheckRadioButton(IDC_RADIO_FILE, IDC_RADIO_FOLDER, IDC_RADIO_FILE);
	SetDlgItemText(IDC_EDIT_PATH, _T(""));

	int iPos = (int)m_ListShow.GetFirstSelectedItemPosition();
	if ( iPos > 0 ) {
		CString strHotKey = m_ListShow.GetItemText(iPos-1, 1);
		CString strPath = m_ListShow.GetItemText(iPos-1, 2);
		SetDlgItemText(IDC_EDIT_PATH, strPath);
		int iLastAdd = strHotKey.ReverseFind('+');
		CString strAsc = strHotKey.Right(strHotKey.GetLength()-iLastAdd-1);
		for( int i = 0; i < m_ComboAsc.GetCount(); i++ ) {
			CString strTmp;
			m_ComboAsc.GetLBText(i, strTmp);
			if ( strTmp == strAsc ) {
				m_ComboAsc.SetCurSel(i);
			}
		}
		int index = strHotKey.Find(_T("Ctrl+"));
		if ( index != -1 ) {
			m_CtrlCheck.SetCheck(BST_CHECKED);
		}
		index = strHotKey.Find(_T("Shift+"));
		if ( index != -1 ) {
			m_ShiftCheck.SetCheck(BST_CHECKED);
		}
		index = strHotKey.Find(_T("Alt+"));
		if ( index != -1 ) {
			m_AltCheck.SetCheck(BST_CHECKED);
		}
	}
	*pResult = 0;
}

void CSysHotKeyDiyDlg::OnTimer(UINT_PTR nIDEvent)
{
	CPoint pt;
	GetCursorPos(&pt);
	CString strX, strY, strResult;
	strX.Format(_T("%d"), pt.x);
	strY.Format(_T("%d"), pt.y);
	strResult = _T("  鼠标位置：X = ") + strX + _T("，Y = ") + strY;
	m_StatBar.SetText(strResult, 0, 0);
	CDialog::OnTimer(nIDEvent);
}

void CSysHotKeyDiyDlg::OnBnClickedResetAllkey()
{
	int nCount = m_ListShow.GetItemCount();
	if ( nCount > 0 ) {
		int iRet = MessageBox(_T("确定撤销全部热键吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
		if ( iRet == IDOK ) {
			m_ListShow.DeleteAllItems();
			arrHotKey.RemoveAll();
			HKEY hKey;
			LONG LRet =  RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SysHotKeyDiy"), &hKey);
			if ( LRet == ERROR_SUCCESS ){
				LRet = SHDeleteKey(hKey, _T("HotKey"));
			}
		}
	}
}

void CSysHotKeyDiyDlg::OnCancel()
{
	CDialog::OnCancel();
}

LRESULT CSysHotKeyDiyDlg::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)
{
	AddTrayIcon(m_hWnd);
	return TRUE;
}

void CSysHotKeyDiyDlg::OnBnClickedListItemUp()
{
	int ps =  (int)m_ListShow.GetFirstSelectedItemPosition();
	if ( ps > 1 ){
		m_ListShow.MoveUp (ps-1);
		m_ListShow.SetFocus();
		m_ListShow.SetItemState(ps-2, LVIS_SELECTED, LVIS_SELECTED);
		CString strHotKey = m_ListShow.GetItemText(ps-2, 1);
		for( int index = 0; index < arrHotKey.GetCount(); index++ ) {
			if ( strHotKey == arrHotKey.GetAt(index).strHotKey ) {
				HKNODE hNode = arrHotKey.GetAt(index);
				arrHotKey.RemoveAt(index);
				arrHotKey.InsertAt(index-1, hNode);
				break;
			}
		}
		RegSetHotKey(&arrHotKey);
	}else{
		if ( ps == 1 ) {
			m_ListShow.SetFocus();
			m_ListShow.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void CSysHotKeyDiyDlg::OnBnClickedListItemDown()
{
	int ps =  (int)m_ListShow.GetFirstSelectedItemPosition();
	int nCount = m_ListShow.GetItemCount();
	if ( ps > 0 ){
		m_ListShow.MoveDown(ps-1);
		m_ListShow.SetFocus();
		m_ListShow.SetItemState(ps, LVIS_SELECTED, LVIS_SELECTED);
		CString strHotKey = m_ListShow.GetItemText(ps, 1);
		for( int index = 0; index < arrHotKey.GetCount(); index++ ) {
			if ( strHotKey == arrHotKey.GetAt(index).strHotKey ) {
				HKNODE hNode = arrHotKey.GetAt(index);
				arrHotKey.RemoveAt(index);
				arrHotKey.InsertAt(index+1, hNode);
				break;
			}
		}
		RegSetHotKey(&arrHotKey);
	}else{
		if ( ps == nCount ) {
			m_ListShow.SetFocus();
			m_ListShow.SetItemState(ps, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}
