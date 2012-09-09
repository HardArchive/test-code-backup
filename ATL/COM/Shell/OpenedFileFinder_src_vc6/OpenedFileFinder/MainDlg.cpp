// MainDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "AboutDlg.h"
#include "utils.h"
#include <Tlhelp32.h>
#include <Psapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MainDlg dialog

#define VISTA_FILETYPE  25
#define XP_FILETYPE 28
PNtQuerySystemInformation NtQuerySystemInformation;

MainDlg::MainDlg(CString csPath, CWnd* pParent /*=NULL*/)
	: CDialog(MainDlg::IDD, pParent),
    m_bSortAscending(true),
    m_nCoumnclicked(-1)
{
    m_csPath = csPath;
	//{{AFX_DATA_INIT(MainDlg)
	//}}AFX_DATA_INIT
}


void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MainDlg)
	DDX_Control(pDX, IDC_COMBO2, m_combobox);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	//{{AFX_MSG_MAP(MainDlg)
	ON_COMMAND(ID_MAIN_TERMINATE, OnMainTerminate)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_COMMAND(ID_MAIN_TERMINATEALLPROCESS, OnMainTerminateallprocess)
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	ON_WM_SIZING()
	ON_COMMAND(ID_BUTTONREFRESH, OnButtonrefresh)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_COMMAND(ID_BUTTON32773, OnButtonGo )
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_MAIN_COPYFILENAME, OnMainCopyfilename)
	ON_COMMAND(ID_MAIN_FINDTARGET, OnMainFindtarget)
	ON_COMMAND(ID_MAIN_CLOSEHANDLE, OnMainClosehandle)
	ON_COMMAND(ID_MAIN_CLOSEALLHANDLES, OnMainCloseallhandles)
	ON_COMMAND(ID_MAIN_SHOWLOADEDMODULESONLY, OnMainShowloadedmodulesonly)
	ON_COMMAND(ID_MAIN_SHOWLOADEDFILESONLY, OnMainShowloadedfilesonly)
	//}}AFX_MSG_MAP
    ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipText )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainDlg message handlers

BOOL MainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    HICON hIcon =  AfxGetApp()->LoadIcon( IDI_ICON2 );
    SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu( "About Check Handles");		
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, 0x0010, strAboutMenu);
		}
	}
    ListView_SetExtendedListViewStyleEx( m_list.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT, 
                                                        LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT );
    m_list.InsertColumn( 0, _T("Process"), LVCFMT_LEFT, 100 );
    m_list.InsertColumn( 1, _T("Pid"), LVCFMT_LEFT, 100);
    m_list.InsertColumn( 2, _T("File Opened"), LVCFMT_LEFT, 350);
    

    // Create image list and add icons
    m_imgToolbar1.Create( 24,24, ILC_COLOR32|ILC_MASK, 1 , RGB(0,0,0));
    m_imgToolbar2.Create( 24,24, ILC_COLOR32|ILC_MASK, 1 , RGB(0,0,0));
    m_imgListCtrl.Create( 16,16, ILC_COLOR32|ILC_MASK, 1 , RGB(0,0,0));
    m_imgToolbar1.Add( AfxGetApp()->LoadIcon( IDI_ICON5 ));
    m_imgToolbar1.Add( AfxGetApp()->LoadIcon( IDI_ICON4 ));
    m_imgToolbar1.Add( AfxGetApp()->LoadIcon( IDI_ICON3 ));
    m_imgToolbar2.Add( AfxGetApp()->LoadIcon( IDI_ICON6 ));    
    

    // create tool bars
    m_toolBar.CreateEx( this, TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_TRANSPARENT );    
    m_toolBar.LoadToolBar( IDR_TOOLBAR1 );    
    m_toolBar.SetSizes( CSize( 31,30), CSize(24,24));      

    m_toolBarGo.CreateEx( this, TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_TRANSPARENT );
    m_toolBarGo.LoadToolBar( IDR_TOOLBAR2 );
    m_toolBarGo.SetSizes( CSize( 35,30),CSize(24,24));

    // set image list
    m_toolBar.SendMessage( TB_SETIMAGELIST, 0, (LPARAM)m_imgToolbar1.m_hImageList );    
    m_toolBarGo.SendMessage( TB_SETIMAGELIST, 0, (LPARAM)m_imgToolbar2.m_hImageList );
    m_list.SetImageList( &m_imgListCtrl, LVSIL_SMALL );
    
    RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,0, reposDefault,0, CRect(10,0, 300, 50 ));
    CRect m_comboRect;
    m_combobox.GetWindowRect( &m_comboRect );    
    ScreenToClient( &m_comboRect );

    m_toolBar.SetWindowPos( 0, 10,0,0,0, SWP_NOSIZE|SWP_NOZORDER );    
    m_toolBarGo.MoveWindow( m_comboRect.right, m_comboRect.top - 3, 35, 30 );

	// Add all drives to the combobox
    TCHAR tc[50];
    DWORD dwCount = GetLogicalDriveStrings( 50, tc );
    TCHAR *pDrive = tc;
    do
    {   CString csDrive = pDrive;
		csDrive.MakeLower();
        m_combobox.AddString( csDrive );
        pDrive += 4;
        dwCount -= 4;
        
    }
	while( dwCount > 0 );
    
	// Add current path is not already added
	int nPos = m_combobox.FindStringExact( -1, m_csPath );
	if( -1 == nPos )
	{
		nPos = m_combobox.AddString( m_csPath );
	}
    m_combobox.SetCurSel( nPos );

    m_combobox.AddString( _T("Browse...") );

	// Set auto complete in the combobox
	CWnd* pEdit =  m_combobox.GetDlgItem( 1001 );
	if( pEdit )
	{
		SHAutoComplete( pEdit->m_hWnd, SHACF_FILESYSTEM );
	}

    EnableTokenPrivilege( SE_DEBUG_NAME );
	// Now list all files
    Populate();
	return TRUE;
}


void MainDlg::Populate( bool bShowModule ) 
{
    try
    {
        // Clear the listcontrol and icon map
        m_list.DeleteAllItems();
        int nCount = m_imgListCtrl.GetImageCount();
        for( int nIdx = 0; nIdx < nCount; nIdx++ )
        {
            m_imgListCtrl.Remove( 0 );
        }
        m_stProcessInfo.RemoveAll();
        HICON hIcon = AfxGetApp()->LoadIcon( IDI_ICON2 );
        m_imgListCtrl.Add( hIcon );
        DestroyIcon( hIcon );
        // Remove any sort marks in header
		if( -1 != m_nCoumnclicked )
		{
			HDITEM stHItem = {0};
			stHItem.mask = HDI_FORMAT;
			stHItem.fmt = HDF_LEFT|HDF_STRING;
			m_list.GetHeaderCtrl()->SetItem( m_nCoumnclicked, &stHItem );
		}

        if( bShowModule )
        {
		    // First list all the loaded dll's in the specified path
            EnumerateLoadedModules();
        }
        else
        {
            // Load the icons corresponding to the processes.
            DWORD dwsize = 300;
            PDWORD pDwId = (PDWORD)new BYTE[dwsize];
            DWORD dwRetruned = dwsize;
	        // Enum all the process first
            while( 1 )
            {
                EnumProcesses( pDwId, dwsize, &dwRetruned );
                if( dwsize > dwRetruned  )
                {
                    break;
                }
                delete pDwId;
                dwsize += 50;
                pDwId = (PDWORD)new BYTE[dwsize];
            }
            int nCount = dwRetruned / sizeof(DWORD);
            for( int nIdx = 0; nIdx < nCount;nIdx++ )
            {    
                TCHAR tcFileName[MAX_PATH];
                CString csModule;
                HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, TRUE, pDwId[nIdx] );
                if( !hProcess )
                {
                    continue;
                }
                if( !GetProcessImageFileName( hProcess, tcFileName, MAX_PATH ))
                {
                    CloseHandle( hProcess );
                    continue;
                }
                GetDrive( tcFileName, csModule, false );
                CloseHandle( hProcess );
                PROCESS_INFO_t stInfo;
        
                stInfo.dwImageListIndex = 0;
                PathStripPath( tcFileName );
                stInfo.csProcess = tcFileName;
                SHFILEINFO stIcon = {0};
                if( SHGetFileInfo( csModule, 0, &stIcon, sizeof( stIcon), SHGFI_ICON ))
                {
                    stInfo.dwImageListIndex = m_imgListCtrl.Add( stIcon.hIcon );
                    DestroyIcon( stIcon.hIcon );
                }
                csModule.MakeLower();
                m_stProcessInfo[pDwId[nIdx]] = stInfo;
            }
        }

		// Extract the driver from the resource and install it.
		HANDLE hDriver = ExtractAndInstallDrv();
        if( hDriver )
        {
			HMODULE hModule = LoadLibrary( _T("ntdll.dll"));
			if( !hModule )
			{
				OutputDebugString( L"Loading ntdll failed" );
				return;
			}
			NtQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress( hModule, "NtQuerySystemInformation" );
			if( 0 == NtQuerySystemInformation )
			{
				OutputDebugString( L"Getting proc of NtQuerySystemInformation failed" );
			}

			// Now walk all handles
            EnumerateOpenedFiles( hDriver );
			
			// Time to wind up
			FreeLibrary( hModule );
            StopAndUninstallDrv( hDriver );
        }
    }
    catch(...)
    {
        OutputDebugString( _T("Exception occured in MainDlg::Populate()"));
    }
}

// This Function will be called when the user click on the "Terminate process"
void MainDlg::OnMainTerminate() 
{
    try
    {
        int nItem =  m_list.GetNextItem( -1, LVNI_SELECTED );
        if( -1 == nItem )
        {
            return;
        }
    
        if( IDYES == MessageBox( _T("Terminate selected Process?"), _T("Terminate"), MB_YESNO))
        {
            CString csPid = m_list.GetItemText( nItem, 1 );
            DWORD dwPid = _ttoi( csPid );        
            HANDLE hProcess = OpenProcess( PROCESS_TERMINATE , FALSE, dwPid );
            if( !hProcess )
            {
                MessageBox( _T("Failed to Terminate process"), _T("Error"), MB_OK|MB_ICONERROR );
                return;
            }
            BOOL b = TerminateProcess( hProcess, 0 );
            CloseHandle( hProcess );
            if( !b )
            {
                MessageBox( _T("Failed to Terminate process"), _T("Error"), MB_OK|MB_ICONERROR );
                return;
            }
            m_list.DeleteAllItems();
			// Refresh
            Populate();            
        }
    }
    catch(...)
    {
    }
    
}

// Function to show the context menu
void MainDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    int nItem =  m_list.GetNextItem( -1, LVNI_SELECTED );
    //if( -1 == nItem )
    //{
    //    return;
    //}
    CMenu PopUpMenu;
    PopUpMenu.LoadMenu( IDR_MENU1 );
    CPoint pt;
    pt = GetCurrentMessage()->pt;
    PopUpMenu.GetSubMenu( 0 )->TrackPopupMenu( TPM_LEFTALIGN, pt.x, pt.y, this, 0 );
	*pResult = 0;
}

// This function will be called when the user clicks the "Terminate All" context menu
void MainDlg::OnMainTerminateallprocess() 
{
    try
    {
        if( IDYES == MessageBox( _T("Terminate All Process?"), _T("Terminate"), MB_YESNO))
        {
            int nCount = m_list.GetItemCount();
            bool bDeleted = true;
            for( int nIdx = 0; nIdx < nCount; nIdx++ )
            {        
                CString csPid = m_list.GetItemText( nIdx, 1 );
                DWORD dwPid = _ttoi( csPid );        
                HANDLE hProcess = OpenProcess(  PROCESS_TERMINATE , FALSE, dwPid );
                if( !hProcess )
                {
                    continue;
                }
                BOOL b = TerminateProcess( hProcess, 0 );
                CloseHandle( hProcess );
                if( !b )
                {
                    bDeleted = false;
                }
            }
            if( !bDeleted )
            {
                MessageBox(  _T("Failed to Terminate some process"),_T("Error"), MB_OK|MB_ICONERROR );
            }
            m_list.DeleteAllItems();
            Populate();
        }
    }
    catch(...)
    {
    }
}

void MainDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if ((nID & 0xFFF0) == 0x0010 )
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
	
}

// Function called when user clicks on the header
void MainDlg::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	try
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		HDITEM stHItem = {0};
		stHItem.mask = HDI_FORMAT;
		stHItem.fmt = HDF_LEFT|HDF_STRING;
		if( -1 != m_nCoumnclicked )
		{
			m_list.GetHeaderCtrl()->SetItem( m_nCoumnclicked, &stHItem );
		}
		if( m_nCoumnclicked != pNMListView->iSubItem )
		{
			m_bSortAscending = true;
		}
		m_nCoumnclicked = pNMListView->iSubItem;
		ListView_SortItemsEx( m_list.m_hWnd, CompareFunc, (LPARAM)this );
		stHItem.fmt |= (m_bSortAscending)? HDF_SORTUP:HDF_SORTDOWN;
		m_list.GetHeaderCtrl()->SetItem( m_nCoumnclicked, &stHItem );
		m_bSortAscending = !m_bSortAscending;
	}
	catch(...)
	{		
	}  	
	*pResult = 0;
}

// The comparison function for deciding the sort order
int MainDlg::CompareFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
    MainDlg* pThis = (MainDlg*)lParamSort;
    CString csText1 = pThis->m_list.GetItemText( lParam1, pThis->m_nCoumnclicked );
    CString csText2 = pThis->m_list.GetItemText( lParam2, pThis->m_nCoumnclicked );    
    int nReturn = 0;
    if( pThis->m_bSortAscending )
    {
        nReturn = csText1.Compare( csText2 );
    }
    else
    {
        nReturn = csText2.Compare( csText1 );
    }
    return nReturn;
}

// Function to size the List control when user sizes the dialog
void MainDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);
    m_list.SetWindowPos( 0,0,0, max( 1, pRect->right -  pRect->left - 30), max( 1, pRect->bottom - pRect->top -72), SWP_NOMOVE|SWP_NOZORDER );
}


// This Function lists all the opened file in the specified folder
void MainDlg::EnumerateOpenedFiles( HANDLE hDriver ) 
{
    
	int nFileType = VISTA_FILETYPE;
	OSVERSIONINFO info = { 0 }; 
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&info); 
	if( info.dwMajorVersion == 5 )
	{
		nFileType = XP_FILETYPE;
	}
    CString csPath = m_csPath;
    csPath.MakeLower();
	CString csShortName;
    GetShortPathName( csPath, csShortName.GetBuffer( MAX_PATH), MAX_PATH );
    csShortName.ReleaseBuffer();
    csShortName.MakeLower();
    bool bShortPath = false;
    if(  csShortName != csPath && FALSE == csShortName.IsEmpty())
    {
        bShortPath = true;
    }

	// Get the list of all handles in the system
    PSYSTEM_HANDLE_INFORMATION pSysHandleInformation = new SYSTEM_HANDLE_INFORMATION;
    DWORD size = sizeof(SYSTEM_HANDLE_INFORMATION);
    DWORD needed = 0;
    NTSTATUS status = NtQuerySystemInformation( SystemHandleInformation, pSysHandleInformation, size, &needed );
    if( !NT_SUCCESS(status))
    {
        if( 0 == needed )
        {
            return;// some other error
        }
        // The previously supplied buffer wasn't enough.
        delete pSysHandleInformation;
        size = needed + 1024;
        pSysHandleInformation = (PSYSTEM_HANDLE_INFORMATION)new BYTE[size];
        status = NtQuerySystemInformation( SystemHandleInformation, pSysHandleInformation, size, &needed );
        if( !NT_SUCCESS(status))
        {
            // some other error so quit.
			delete pSysHandleInformation;
			return;
        }
    }
    int nCount = m_list.GetItemCount() - 1;

	// Walk through the handle list
    for ( DWORD i = 0; i < pSysHandleInformation->dwCount; i++ )
	{
        SYSTEM_HANDLE& sh = pSysHandleInformation->Handles[i];
        if( sh.bObjectType != nFileType )// Under windows XP file handle is of type 28 and on vista it is 25
        {
            continue;
        }
        HANDLE_INFO stHandle = {0};
		ADDRESS_INFO stAddress;
		stAddress.pAddress = sh.pAddress;
		DWORD dwReturn = 0;
        

		BOOL bSuccess = DeviceIoControl( hDriver, IOCTL_LISTDRV_BUFFERED_IO, &stAddress, sizeof(ADDRESS_INFO), 
										 &stHandle, sizeof(HANDLE_INFO), &dwReturn, NULL );

        
        if( bSuccess && stHandle.tcFileName[0] != 0 && 
            stHandle.uType != FILE_DEVICE_SOUND && 
            stHandle.uType != FILE_DEVICE_NAMED_PIPE )
        {
            CString csFileName;
            
            if( stHandle.uType != FILE_DEVICE_NETWORK_FILE_SYSTEM  )
            {
                // Get the drive name from the dos device name
                if( !GetDrive( stHandle.tcDeviceName, csFileName, true ))
			    {
				    OutputDebugString( L"GetDrive failed" );
			    }
                csFileName += stHandle.tcFileName;
            }
            else
            {
                csFileName = _T("\\");
                csFileName += stHandle.tcFileName;
            }
			
            
            csFileName.MakeLower();
			// Check whether the file belongs to the specified folder
            if( -1 == csFileName.Find( csPath ))
            {
				if( bShortPath )
				{
					// Some times the file name may be in short path form.
					if( -1 == csFileName.Find( csShortName ))
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}

            PROCESS_INFO_t stInfo;			
            stInfo.dwImageListIndex = 0;
			if( sh.dwProcessId == 4 )// check whether the process is system
			{			
				stInfo.csProcess = "System";
                
			}
			else
			{
                
                if( !m_stProcessInfo.Lookup( sh.dwProcessId, stInfo ))
                {
                    CString csProcessName = _T("<Un known>");
				    // Get the process file name
				    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, TRUE, sh.dwProcessId );
				    if( hProcess != 0 )
				    {                        
					    LPTSTR lpPath = csProcessName.GetBuffer( MAX_PATH);
					    BOOL b = GetProcessImageFileName( hProcess, lpPath, MAX_PATH );
					    if( b )                    
					    {
						    PathStripPath( lpPath );
						    csProcessName.ReleaseBuffer();
					    }                
					    else
					    {
						    csProcessName.ReleaseBuffer();
					    }                
					    CloseHandle( hProcess );
				    }
                    stInfo.csProcess = csProcessName;
                    stInfo.dwImageListIndex = -1;
                }

                
                
			}
			// Insert Process name, PID and file name
            m_list.InsertItem( ++nCount, stInfo.csProcess, stInfo.dwImageListIndex );                    
            CString csPid;
            csPid.Format( _T("%d ( 0x%x )"), sh.dwProcessId , sh.dwProcessId );			
            m_list.SetItemText( nCount, 1, csPid );
            m_list.SetItemText( nCount, 2, csFileName );
            m_list.SetItemData( nCount, sh.wValue );
        }        
    }
    delete pSysHandleInformation;
}

// Refresh the list
void MainDlg::OnButtonrefresh() 
{
    Populate();
}


// this function will be called when the item in combobox is chaged
void MainDlg::OnSelchangeCombo2() 
{
    CString csText;
    m_combobox.GetLBText( m_combobox.GetCurSel(), csText );
    if( csText == _T("Browse...") )
    {
		// User have selected the browse item. so browse for folder
        BROWSEINFO bi = {0};
        bi.hwndOwner = m_hWnd;
        bi.ulFlags = BIF_RETURNONLYFSDIRS ;
        LPITEMIDLIST pIt= SHBrowseForFolder( &bi );
        if( pIt )
        {
			// Set the new path as the selected folder and refresh
            TCHAR tcPath[ MAX_PATH ];
            if( SHGetPathFromIDList( pIt, tcPath ))
            {
                m_csPath = tcPath;
                int nPos =m_combobox.FindStringExact( -1, m_csPath );
                if( -1 == nPos )
                {
                    nPos = m_combobox.AddString( m_csPath );                    
                }
                m_combobox.SetCurSel( nPos );
                Populate();
                return;
            }
        }
        m_combobox.SetCurSel( -1 );
        return;
    }
    m_csPath = csText;
    Populate();
}


BOOL MainDlg::PreTranslateMessage(MSG* pMsg) 
{
	//Handled the enter key press in the combobox
    if( WM_KEYDOWN == pMsg->message && ::GetParent( pMsg->hwnd ) == m_combobox.m_hWnd && VK_RETURN == pMsg->wParam )
    {		
		CString csText;
		m_combobox.GetWindowText( csText );
		m_csPath = csText;
		int nPos =m_combobox.FindStringExact( -1, csText );
		if( -1 == nPos )
		{
			nPos = m_combobox.AddString( m_csPath );                    
		}
		m_combobox.SetCurSel( nPos );
		Populate();
		return TRUE;
    }
	return CDialog::PreTranslateMessage(pMsg);
}

// Function called when the Go button is clicked
void MainDlg::OnButtonGo() 
{
    CString csText;
    m_combobox.GetWindowText( csText );
    m_csPath = csText;
    int nPos =m_combobox.FindStringExact( -1, csText );
    if( -1 == nPos )
    {
        nPos = m_combobox.AddString( m_csPath );                    
    }
    m_combobox.SetCurSel( nPos );
    Populate();
}

// Function for showing the tooltip
BOOL MainDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
    // Tooltip handler struct
    LPTOOLTIPTEXT pTTT = (LPTOOLTIPTEXT)pNMHDR;

    // If pointer invalid return false;
    if( !pTTT )
    {
        return FALSE;
    }

    // Get id of the sender
    UINT uCtrlID = pNMHDR->idFrom;

    // If idFrom is an HWND, then use GetDlgCtrlID to get the id of the control
    if ( pNMHDR->code == TTN_NEEDTEXT && ( pTTT->uFlags & TTF_IDISHWND ))
    {
        uCtrlID = ::GetDlgCtrlID( (HWND)uCtrlID );
    }

    if ( uCtrlID != 0 ) // will be zero on a separator
    {
        CString strTipText;
        strTipText.LoadString( uCtrlID );
        if ( pNMHDR->code == TTN_NEEDTEXT )
        {
            lstrcpyn( pTTT->szText, strTipText, sizeof( pTTT->szText ));
        }
        
        *pResult = 0;
        
        // Move tooltip window up
        ::SetWindowPos( pNMHDR->hwndFrom, 
                        HWND_TOP, 
                        0, 
                        0, 
                        0, 
                        0,
                        SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER );

        // Continue notification
        return TRUE;
    }

    // Stop notification
    return FALSE;
}

// Function to prevent the user from decreasing the size below a limit
void MainDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{	    
    lpMMI->ptMinTrackSize = CPoint( 597, 150 );
	CDialog::OnGetMinMaxInfo(lpMMI);
}


// Function list all the modules that have been loaded from the specified path
void MainDlg::EnumerateLoadedModules()
{
    CString csPath = m_csPath;
    csPath.MakeLower();
    CString csShortName;
    GetShortPathName( csPath, csShortName.GetBuffer( MAX_PATH), MAX_PATH );
    csShortName.ReleaseBuffer();
    csShortName.MakeLower();
    bool bShortPath = false;
    if(  csShortName != csPath && FALSE == csShortName.IsEmpty())
    {
        bShortPath = true;
    }

    DWORD dwsize = 300;
    PDWORD pDwId = (PDWORD)new BYTE[dwsize];
    DWORD dwRetruned = dwsize;
	// Enum all the process first
    while( 1 )
    {
        EnumProcesses( pDwId, dwsize, &dwRetruned );
        if( dwsize > dwRetruned  )
        {
            break;
        }
        delete pDwId;
        dwsize += 50;
        pDwId = (PDWORD)new BYTE[dwsize];
    }
    int nCount = dwRetruned / sizeof(DWORD);
    int nItemCount = -1;
	// Enumerate modules of the above process
    for( int nIdx = 0; nIdx < nCount;nIdx++ )
    {    
        if( 0 != pDwId[nIdx] )
        {
            HANDLE hModuleSnap = INVALID_HANDLE_VALUE; 
            MODULEENTRY32 me32; 
            // Take a snapshot of all modules in the specified process. 
            hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pDwId[nIdx] ); 
            if( hModuleSnap == INVALID_HANDLE_VALUE ) 
            {     
                continue; 
            } 

            me32.dwSize = sizeof( MODULEENTRY32 ); 
            if( !Module32First( hModuleSnap, &me32 ) ) 
            {     
                CloseHandle( hModuleSnap );
                continue; 
            }
            bool bFirst = true;
            CString csPid;
            csPid.Format( _T("%d ( 0x%x )"), pDwId[nIdx] , pDwId[nIdx] );
            PROCESS_INFO_t stInfo;
            do 
            { 
                CString csModule;
                if( bFirst )
                {
                    // First module is always the exe name
                    bFirst = false;
                    stInfo.csProcess = me32.szModule;
                    if( !PathFileExists( me32.szExePath ))
                    {
                        TCHAR tcFileName[MAX_PATH];
                        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, TRUE, pDwId[nIdx] );                        
                        if( GetProcessImageFileName( hProcess, tcFileName, MAX_PATH ))
                        {
                            GetDrive( tcFileName, csModule, false );
                        }
                        CloseHandle( hProcess );
                    }
                    else
                    {
                        csModule = me32.szExePath;
                    }
                    stInfo.dwImageListIndex = 0;
                    SHFILEINFO stIcon = {0};
                    if( SHGetFileInfo( csModule, 0, &stIcon, sizeof( stIcon), SHGFI_ICON ))
                    {
                        stInfo.dwImageListIndex = m_imgListCtrl.Add( stIcon.hIcon );
                        DestroyIcon( stIcon.hIcon );
                    }
                    csModule.MakeLower();
                    m_stProcessInfo[pDwId[nIdx]] = stInfo;
                    
                }
                else
                {
                    csModule = me32.szExePath;
                    csModule.MakeLower();
                }
                
                
                if( -1 == csModule.Find( csPath ))
                {
                    if( bShortPath )
                    {
                        if( -1 == csModule.Find( csShortName ))
                        {
                            continue;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                int nInsertedPos = m_list.InsertItem( ++nItemCount, stInfo.csProcess, stInfo.dwImageListIndex );
                m_list.SetItemText( nInsertedPos , 1, csPid );
                m_list.SetItemText( nInsertedPos , 2, csModule  );
            }
            while( Module32Next( hModuleSnap, &me32 ) ); 
            CloseHandle( hModuleSnap ); 
        }
    }
    delete pDwId;
}

// Function to copy the filename to clipboard
void MainDlg::OnMainCopyfilename() 
{
	int nItem =  m_list.GetNextItem( -1, LVNI_SELECTED );
    if( -1 == nItem )
    {
        return;
    }
	CString csPath;
	csPath = m_list.GetItemText( nItem,2 );
	OpenClipboard();
	EmptyClipboard(); 
	int nSize = (csPath.GetLength() + 1) * sizeof(TCHAR);
    HGLOBAL  hglbCopy = GlobalAlloc(GMEM_MOVEABLE, nSize ); 
    if (hglbCopy == NULL) 
    { 
        CloseClipboard(); 
        return; 
    } 
    LPTSTR lpData = (LPTSTR)GlobalLock(hglbCopy); 
    memcpy(lpData, csPath, nSize ); 
    lpData[nSize-1] = (TCHAR) 0;
    GlobalUnlock(hglbCopy);
    SetClipboardData( CF_UNICODETEXT, hglbCopy ); 
	CloseClipboard();

}
void MainDlg::OnMainFindtarget()
{
	int nItem =  m_list.GetNextItem( -1, LVNI_SELECTED );
	if( -1 == nItem )
	{
		return;
	}
	CString csPath;
	csPath = m_list.GetItemText( nItem,2 );
	LPITEMIDLIST stId = 0;
	SFGAOF stSFGAOFIn = 0;	
	SFGAOF stSFGAOFOut = 0;
	if( !FAILED(SHParseDisplayName( csPath, 0, &stId,stSFGAOFIn, &stSFGAOFOut )))
	{
		SHOpenFolderAndSelectItems( stId, 0, 0, 0 );
	}
	
}

void MainDlg::OnMainClosehandle() 
{
    int nItem =  m_list.GetNextItem( -1, LVNI_SELECTED );
    if( -1 == nItem )
    {
        return;
    }
    HANDLE hFile = (HANDLE)m_list.GetItemData( nItem );
    if( !hFile )
    {
        return;
    }
    CString csFileName = m_list.GetItemText( nItem, 2 );
    CString csPid = m_list.GetItemText( nItem, 1 );
    DWORD dwPid = _ttoi( csPid );
    HANDLE hProcess = OpenProcess( PROCESS_DUP_HANDLE , FALSE, dwPid );
    if( !hProcess )
    {
        AfxMessageBox( L"Failed to open the process" );
        return;
    }
    HANDLE hDup = 0;
    BOOL b = DuplicateHandle( hProcess, hFile, GetCurrentProcess(), &hDup, DUPLICATE_SAME_ACCESS , FALSE, DUPLICATE_CLOSE_SOURCE );
    if( hDup )
    {
        CloseHandle( hDup );
    }    
    CloseHandle( hProcess );
    //b = DeleteFile( csFileName );
    Populate();
}

void MainDlg::OnMainCloseallhandles() 
{
    int nCount = m_list.GetItemCount();    
    for( int nIdx = 0; nIdx < nCount; nIdx++ )
    {        
        CString csPid = m_list.GetItemText( nIdx, 1 );
        DWORD dwPid = _ttoi( csPid );                
        HANDLE hFile = (HANDLE)m_list.GetItemData( nIdx );
        if( !hFile )
        {
            continue;
        }        
        HANDLE hProcess = OpenProcess( PROCESS_DUP_HANDLE , FALSE, dwPid );
        if( !hProcess )
        {
            //AfxMessageBox( L"Failed to open the process" );
            continue;
        }
        HANDLE hDup = 0;
        BOOL b = DuplicateHandle( hProcess, hFile, GetCurrentProcess(), &hDup, DUPLICATE_SAME_ACCESS , FALSE, DUPLICATE_CLOSE_SOURCE );
        if( hDup )
        {
            CloseHandle( hDup );
        }    
        CloseHandle( hProcess );
    }

    Populate();
}

void MainDlg::OnMainShowloadedmodulesonly() 
{
    // Clear the listcontrol and icon map
        m_list.DeleteAllItems();
        int nCount = m_imgListCtrl.GetImageCount();
        for( int nIdx = 0; nIdx < nCount; nIdx++ )
        {
            m_imgListCtrl.Remove( 0 );
        }
        m_stProcessInfo.RemoveAll();
        HICON hIcon = AfxGetApp()->LoadIcon( IDI_ICON2 );
        m_imgListCtrl.Add( hIcon );
        DestroyIcon( hIcon );
        // Remove any sort marks in header
		if( -1 != m_nCoumnclicked )
		{
			HDITEM stHItem = {0};
			stHItem.mask = HDI_FORMAT;
			stHItem.fmt = HDF_LEFT|HDF_STRING;
			m_list.GetHeaderCtrl()->SetItem( m_nCoumnclicked, &stHItem );
		}

        
		// First list all the loaded dll's in the specified path
        EnumerateLoadedModules();
}

void MainDlg::OnMainShowloadedfilesonly() 
{
    Populate( false );
}
