// FileTimeShlExt.cpp : Implementation of CFileTimeShlExt
#include "stdafx.h"
#include "FileTime.h"
#include "FileTimeShlExt.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "comctl32.lib")


// Functions to handle messages from the property page.
BOOL CALLBACK PropPageDlgProc ( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
UINT CALLBACK PropPageCallbackProc ( HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp );
BOOL OnInitDialog ( HWND hwnd, LPARAM lParam );
BOOL OnApply ( HWND hwnd, PSHNOTIFY* phdr );

// Misc utility functins.
void GetCombinedDateTime ( HWND hwnd, UINT idcDatePicker, UINT idcTimePicker,
                           FILETIME* pFiletime );
void SetCombinedDateTime ( HWND hwnd, UINT idcDatePicker, UINT idcTimePicker,
                           const FILETIME* pFiletime );


/////////////////////////////////////////////////////////////////////////////
// CFileTimeShlExt IShellExtInit methods

HRESULT CFileTimeShlExt::Initialize ( LPCITEMIDLIST pidlFolder,
                                      LPDATAOBJECT  pDataObj,
                                      HKEY          hProgID )
{
TCHAR     szFile [MAX_PATH];
UINT      uNumFiles;
HDROP     hdrop;
FORMATETC etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
STGMEDIUM stg;
INITCOMMONCONTROLSEX iccex = { sizeof(INITCOMMONCONTROLSEX), ICC_DATE_CLASSES };

    // Init the common controls.
    InitCommonControlsEx ( &iccex );

    // Read the list of folders from the data object.  They're stored in HDROP
    // form, so just get the HDROP handle and then use the drag 'n' drop APIs
    // on it.

    if ( FAILED( pDataObj->GetData ( &etc, &stg )))
        return E_INVALIDARG;

    // Get an HDROP handle.

    hdrop = (HDROP) GlobalLock ( stg.hGlobal );

    if ( NULL == hdrop )
        {
        ReleaseStgMedium ( &stg );
        return E_INVALIDARG;
        }

    // Determine how many files are involved in this operation.

    uNumFiles = DragQueryFile ( hdrop, 0xFFFFFFFF, NULL, 0 );

    for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
        {
        // Get the next filename.

        if ( 0 == DragQueryFile ( hdrop, uFile, szFile, MAX_PATH ))
            continue;

        // Skip over directories.  We *could* handle directories, since they
        // keep the creation time/date, but I'm just choosing not to do so
        // in this example project.

        if ( PathIsDirectory ( szFile ))
            continue;

        // Add the filename to our list o' files to act on.

        m_lsFiles.push_back ( szFile );
        }   // end for

    // Release resources.

    GlobalUnlock ( stg.hGlobal );
    ReleaseStgMedium ( &stg );

    // Check how many files were selected.  If the number is greater than the
    // maximum number of property pages, truncate our list.

    if ( m_lsFiles.size() > MAXPROPPAGES )
        {
        m_lsFiles.resize ( MAXPROPPAGES );
        }

    // If we found any files we can work with, return S_OK.  Otherwise,
    // return E_FAIL so we don't get called again for this right-click
    // operation.

    return ( m_lsFiles.size() > 0 ) ? S_OK : E_FAIL;
}


/////////////////////////////////////////////////////////////////////////////
// CFileTimeShlExt IShellPropSheetExt methods

HRESULT CFileTimeShlExt::AddPages ( LPFNADDPROPSHEETPAGE lpfnAddPageProc,
                                    LPARAM lParam )
{
PROPSHEETPAGE  psp;
HPROPSHEETPAGE hPage;
TCHAR          szPageTitle [MAX_PATH];
string_list::const_iterator it, itEnd;
                                  
    for ( it = m_lsFiles.begin(), itEnd = m_lsFiles.end();
          it != itEnd;
          it++ )
        {
        // 'it' points at the next filename.  Allocate a new copy of the string
        // that the page will own.
        LPCTSTR szFile = _tcsdup ( it->c_str() );

        if ( NULL == szFile )
            return E_OUTOFMEMORY;

        // Strip the path and extension from the filename - this will be the
        // page title.  The name is truncated at 24 chars so it fits on the tab.

        lstrcpy ( szPageTitle, it->c_str() );
        PathStripPath ( szPageTitle );
        PathRemoveExtension ( szPageTitle );
        szPageTitle[24] = '\0';

        // Set up the PROPSHEETPAGE struct.

        ZeroMemory ( &psp, sizeof(PROPSHEETPAGE) );

        psp.dwSize      = sizeof(PROPSHEETPAGE);
        psp.dwFlags     = PSP_USEREFPARENT | PSP_USETITLE | PSP_DEFAULT |
                            PSP_USEICONID | PSP_USECALLBACK;
        psp.hInstance   = _Module.GetModuleInstance();
        psp.pszTemplate = MAKEINTRESOURCE(IDD_FILETIME_PROPPAGE);
        psp.pszIcon     = MAKEINTRESOURCE(IDI_ICON);
        psp.pszTitle    = szPageTitle;
        psp.pfnDlgProc  = PropPageDlgProc;
        psp.lParam      = (LPARAM) szFile;
        psp.pfnCallback = PropPageCallbackProc;
        psp.pcRefParent = (UINT*) &_Module.m_nLockCnt;

        // Create the page & get a handle.

        hPage = CreatePropertySheetPage ( &psp );

        if ( NULL != hPage )
            {
            // Call the shell's callback function, so it adds the page to
            // the property sheet.

            if ( !lpfnAddPageProc ( hPage, lParam ))
                {
                DestroyPropertySheetPage ( hPage );
                }
            }
        }   // end for

    return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// CFileTimeShlExt dialog/callback procs

BOOL CALLBACK PropPageDlgProc ( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
BOOL bRet = FALSE;

    switch ( uMsg )
        {
        case WM_INITDIALOG:
            bRet = OnInitDialog ( hwnd, lParam );
        break;

        case WM_NOTIFY:
            {
            NMHDR* phdr = (NMHDR*) lParam;

            switch ( phdr->code )
                {
                case PSN_APPLY:
                    bRet = OnApply ( hwnd, (PSHNOTIFY*) phdr );
                break;

                case DTN_DATETIMECHANGE:
                    // If the user changes any of the DTP controls, enable
                    // the Apply button.

                    SendMessage ( GetParent(hwnd), PSM_CHANGED, (WPARAM) hwnd, 0 );
                break;
                }
            }
        break;
        }

    return bRet;
}

UINT CALLBACK PropPageCallbackProc ( HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp )
{
    if ( PSPCB_RELEASE == uMsg )
        {
        free ( (void*) ppsp->lParam );
        }

    return 1;                           // used for PSPCB_CREATE - let the page be created
}


/////////////////////////////////////////////////////////////////////////////
// CFileTimeShlExt message handlers

BOOL OnInitDialog ( HWND hwnd, LPARAM lParam )
{        
PROPSHEETPAGE*  ppsp = (PROPSHEETPAGE*) lParam;
LPCTSTR         szFile = (LPCTSTR) ppsp->lParam;
HANDLE          hFind;
WIN32_FIND_DATA rFind;

    // Store the filename in this window's user data area, for later use.

    SetWindowLong ( hwnd, GWL_USERDATA, (LONG) szFile );

    // Let FindFirstFile() do the work of retrieving the created, modified,
    // and last accessed times.

    hFind = FindFirstFile ( szFile, &rFind );

    if ( INVALID_HANDLE_VALUE != hFind )
        {
        // Initialize the DTP controls.

        SetCombinedDateTime ( hwnd, IDC_MODIFIED_DATE, IDC_MODIFIED_TIME,
                              &rFind.ftLastWriteTime );

        SetCombinedDateTime ( hwnd, IDC_ACCESSED_DATE, 0,
                              &rFind.ftLastAccessTime );

        SetCombinedDateTime ( hwnd, IDC_CREATED_DATE, IDC_CREATED_TIME,
                              &rFind.ftCreationTime );

        FindClose ( hFind );
        }

    // Display the full path in the top static control.

    PathSetDlgItemPath ( hwnd, IDC_FILENAME, szFile );
    
    return FALSE;                       // Take the default focus handling.
}

BOOL OnApply ( HWND hwnd, PSHNOTIFY* phdr )
{
LPCTSTR  szFile = (LPCTSTR) GetWindowLong ( hwnd, GWL_USERDATA );
HANDLE   hFile;
FILETIME ftModified, ftAccessed, ftCreated;

    // Open the file.

    hFile = CreateFile ( szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if ( INVALID_HANDLE_VALUE != hFile )
        {
        // Retrieve the dates/times from the DTP controls.

        GetCombinedDateTime ( hwnd, IDC_MODIFIED_DATE, IDC_MODIFIED_TIME,
                              &ftModified );

        GetCombinedDateTime ( hwnd, IDC_ACCESSED_DATE, 0,
                              &ftAccessed );

        GetCombinedDateTime ( hwnd, IDC_CREATED_DATE, IDC_CREATED_TIME,
                              &ftCreated );

        // Change the file's created, accessed, and last modified times.

        SetFileTime ( hFile, &ftCreated, &ftAccessed, &ftModified );
        CloseHandle ( hFile );
        }
    else
        {
        // Couldn't open the file!  Show an error message.

        std::strstream strMsg;

        strMsg << _T("Unable to open file \"") << szFile << _T("\" for writing.\n\n")
               << _T("Changes will not be saved.");

        MessageBox ( hwnd, strMsg.str(), _T("FileTime Extension"), MB_ICONERROR );
        }

    // Return PSNRET_NOERROR to allow the sheet to close if the user clicked OK.

    SetWindowLong ( hwnd, DWL_MSGRESULT, PSNRET_NOERROR );
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CFileTimeShlExt other stuff

//////////////////////////////////////////////////////////////////////////
//
// Function:    GetCombinedDateTime
//
// Description:
//  Reads a date DTP control and a time DTP control and stores their combined
//  date/time into a FILETIME struct.
//
// Input:
//  hwnd: [in] Handle of the property page.
//  idcDatePicker: [in] ID of a DTP control that displays a date.
//  idcTimePicker: [in] ID of a DTP control that displays a time.  This may
//                 be 0, in which case the returned time is 12 midnight.
//  pFiletime: [out] Pointer to a FILETIME struct that receives the date
//             and time in UTC.
//
// Returns:
//  Nothing.
//
//////////////////////////////////////////////////////////////////////////

void GetCombinedDateTime ( HWND hwnd, UINT idcDatePicker, UINT idcTimePicker,
                           FILETIME* pFiletime )
{
SYSTEMTIME st = {0}, stDate = {0}, stTime = {0};
FILETIME   ftLocal;

    SendDlgItemMessage ( hwnd, idcDatePicker, DTM_GETSYSTEMTIME,
                         0, (LPARAM) &stDate );

    if ( 0 != idcTimePicker )
        {
        SendDlgItemMessage ( hwnd, idcTimePicker, DTM_GETSYSTEMTIME,
                             0, (LPARAM) &stTime );
        }

    st.wMonth  = stDate.wMonth;
    st.wDay    = stDate.wDay;
    st.wYear   = stDate.wYear;
    st.wHour   = stTime.wHour;
    st.wMinute = stTime.wMinute;
    st.wSecond = stTime.wSecond;

    SystemTimeToFileTime ( &st, &ftLocal );
    LocalFileTimeToFileTime ( &ftLocal, pFiletime );
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    SetCombinedDateTime
//
// Description:
//  Stores a given date and time (in URC) in a date DTP control and a time
//  DTP control.
//
// Input:
//  hwnd: [in] Handle of the property page.
//  idcDatePicker: [in] ID of a DTP control that displays a date.
//  idcTimePicker: [in] ID of a DTP control that displays a time.  This may
//                 be 0, in which case only the date control is initialized.
//  pFiletime: [in] Pointer to a FILETIME struct that holds the date and time
//             in UTC.
//
// Returns:
//  Nothing.
//
//////////////////////////////////////////////////////////////////////////

void SetCombinedDateTime ( HWND hwnd, UINT idcDatePicker, UINT idcTimePicker,
                           const FILETIME* pFiletime )
{
SYSTEMTIME st;
FILETIME   ftLocal;

    FileTimeToLocalFileTime ( pFiletime, &ftLocal );
    FileTimeToSystemTime ( &ftLocal, &st );

    SendDlgItemMessage ( hwnd, idcDatePicker, DTM_SETSYSTEMTIME,
                         0, (LPARAM) &st );

    if ( 0 != idcTimePicker )
        {
        SendDlgItemMessage ( hwnd, idcTimePicker, DTM_SETSYSTEMTIME,
                             0, (LPARAM) &st );
        }
}
