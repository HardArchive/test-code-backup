// verinfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "verinf.h"
#include "verinfDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVerinfDlg dialog

typedef enum {
	LISTITEM_FFI_FILEVERSION	= 0,
	LISTITEM_FFI_PRODUCTVERSION	= 1,
	LISTITEM_FFI_FILEFLAGSMASK	= 2,
	LISTITEM_FFI_FILEFLAGS		= 3,
	LISTITEM_FFI_FILEOS			= 4,
	LISTITEM_FFI_FILETYPE		= 5,
	LISTITEM_FFI_FILESUBTYPE	= 6
} LISTITEM_FFI;

LPCTSTR CVerinfDlg::s_lpszFVI[] = { 
	_T( "FILEVERSION" ),	_T( "PRODUCTVERSION" ),
	_T( "FILEFLAGSMASK" ),	_T( "FILEFLAGS" ),
	_T( "FILEOS" ),			_T( "FILETYPE" ),
	_T( "FILESUBTYPE" )
};

MAP CVerinfDlg::s_lpVOS[] = {
	{ VOS_UNKNOWN,			_T( "VOS_UNKNOWN" )		},
	{ VOS_DOS,				_T( "VOS_DOS" ),		},
	{ VOS_OS216,			_T( "VOS_OS216" ),		},
	{ VOS_OS232,			_T( "VOS_OS232" ),		},
	{ VOS_NT,				_T( "VOS_NT" )			},
	{ VOS__WINDOWS16,		_T( "VOS__WINDOWS16" )	},	
	{ VOS__PM16,			_T( "VOS__PM16" )		},
	{ VOS__PM32,			_T( "VOS__PM32" )		},
	{ VOS__WINDOWS32,		_T( "VOS__WINDOWS32" )		},
	{ VOS_DOS_WINDOWS16,	_T( "VOS_DOS_WINDOWS16" )	},
	{ VOS_DOS_WINDOWS32,	_T( "VOS_DOS_WINDOWS32" )	},
	{ VOS_OS216_PM16,		_T( "VOS_OS216_PM16" )		},
	{ VOS_OS232_PM32,		_T( "VOS_OS232_PM32" )		},
	{ VOS_NT_WINDOWS32,		_T( "VOS_NT_WINDOWS32" )	}
};

MAP CVerinfDlg::s_lpFILT[] = {
	{ VFT_UNKNOWN,			_T( "VFT_UNKNOWN" )	},
	{ VFT_APP,				_T( "VFT_APP" )		},
	{ VFT_DLL,				_T( "VFT_DLL" )		},
	{ VFT_DRV,				_T( "VFT_DRV" )		},
	{ VFT_FONT,				_T( "VFT_FONT" )	},
	{ VFT_VXD,				_T( "VFT_VXD" )		},
	{ VFT_STATIC_LIB,		_T( "VFT_STATIC_LIB" )	}	
};

MAP CVerinfDlg::s_lpFNTT[] = {
	{ VFT2_UNKNOWN,			_T( "VFT2_UNKNOWN" )		},
	{ VFT2_FONT_RASTER,		_T( "VFT2_FONT_RASTER" )	},
	{ VFT2_FONT_VECTOR,		_T( "VFT2_FONT_VECTOR" )	},
	{ VFT2_FONT_TRUETYPE,	_T( "VFT2_FONT_TRUETYPE" )	}	
};

MAP CVerinfDlg::s_lpDRVT[] = {
	{ VFT2_UNKNOWN,			_T( "VFT2_UNKNOWN" )		},
	{ VFT2_DRV_PRINTER,		_T( "VFT2_DRV_PRINTER" )	},
	{ VFT2_DRV_KEYBOARD,	_T( "VFT2_DRV_KEYBOARD" )	},
	{ VFT2_DRV_LANGUAGE,	_T( "VFT2_DRV_LANGUAGE" )	},
	{ VFT2_DRV_DISPLAY,		_T( "VFT2_DRV_DISPLAY" )	},
	{ VFT2_DRV_MOUSE,		_T( "VFT2_DRV_MOUSE" )		},
	{ VFT2_DRV_NETWORK,		_T( "VFT2_DRV_NETWORK" )	},
	{ VFT2_DRV_SYSTEM,		_T( "VFT2_DRV_SYSTEM" )		},
	{ VFT2_DRV_INSTALLABLE,	_T( "VFT2_DRV_INSTALLABLE" )},
	{ VFT2_DRV_SOUND,		_T( "VFT2_DRV_SOUND" )		},
	{ VFT2_DRV_COMM,		_T( "VFT2_DRV_COMM" )		},
	{ VFT2_DRV_INPUTMETHOD, _T( "VFT2_DRV_INPUTMETHOD" )}	
};

DWORD GetOFNSize( void )
{	
	DWORD dwSize = sizeof( OPENFILENAME );
		
	OSVERSIONINFO osvi = { sizeof( OSVERSIONINFO ) };	
	::GetVersionEx( &osvi );

	if( ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 5 ) ||
		( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && 
		( osvi.dwMinorVersion >= 90 || osvi.dwMajorVersion > 4 ) ) )
	{
		struct OPENFILENAMEEX : public OPENFILENAME {
			LPVOID		lpvReserved;
			DWORD		dwReserved;
			DWORD		dwFlagsEx;
		};
		dwSize = sizeof( OPENFILENAMEEX );
	};

	return dwSize;
}

CVerinfDlg::CVerinfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVerinfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVerinfDlg)	
	m_strFileName = _T("");
	m_strFileType = _T("");
	//}}AFX_DATA_INIT
	
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	::ZeroMemory( &m_szModule, MAX_PATH );
}

void CVerinfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVerinfDlg)
	DDX_Control(pDX, IDC_CBO_LANG, m_wndCboLang);
	DDX_Control(pDX, IDC_LST_FVB, m_wndLstFvb);
	DDX_Control(pDX, IDC_LST_FVI, m_wndLstFvi);	
	DDX_Text(pDX, IDC_LBL_FILENAME, m_strFileName);
	DDX_Text(pDX, IDC_LBL_FILETYPE, m_strFileType);
	DDX_Control(pDX, IDC_LNK_MAIL, m_wndLink);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVerinfDlg, CDialog)
	//{{AFX_MSG_MAP(CVerinfDlg)	
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CMD_OPEN, OnCmdOpen)
	ON_BN_CLICKED(IDC_CMD_RESET, OnCmdReset)
	ON_CBN_SELCHANGE(IDC_CBO_LANG, OnSelchangeCboLang)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVerinfDlg message handlers

BOOL CVerinfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_wndLink.SetURL( _T( "mailto:armen.h@web.am" ) );
	m_wndLstFvi.SetExtendedStyle( LVS_EX_FULLROWSELECT ); 
	m_wndLstFvb.SetExtendedStyle( LVS_EX_FULLROWSELECT );	

	OnCmdReset();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVerinfDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVerinfDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVerinfDlg::OnCmdOpen() 
{
	CFileDialog dlg( TRUE, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		_T( "Executable Files (*.exe;*.dll;*.ocx;*.cpl;*.vxd)|*.exe;*.dll;*.ocx;*.cpl;*.vxd|All Files (*.*)|*.*||" ),
		this );
	
	#if ( _MFC_VER < 0x0700 ) // 0x0421, 0x0600, 0x0700
		dlg.m_ofn.lStructSize = GetOFNSize();
	#endif

	if( dlg.DoModal() == IDOK )
		FillVersionData( dlg.GetPathName() );
}

void CVerinfDlg::OnCmdReset() 
{
	FillVersionData( );	
}

void CVerinfDlg::FillVersionData( LPCTSTR lpszFileName )
{
	TCHAR szFile[ MAX_PATH ] = { 0 };
	if( lpszFileName == NULL )
		::GetModuleFileName( NULL, szFile, MAX_PATH );
	else
		::lstrcpy( szFile, lpszFileName );
		
	if( m_fvi.Open( szFile ) == FALSE )
	{
		AfxMessageBox( IDP_NOFFI, MB_OK | MB_ICONEXCLAMATION );
		FillVersionData( m_szModule );
		return;
	}
	
	::lstrcpy( m_szModule, szFile );
	
	CStatic* lpLblFileName = (CStatic*)GetDlgItem( IDC_LBL_FILENAME );
	if( lpLblFileName != NULL )
	{
		CRect rc;
		lpLblFileName->GetClientRect( &rc );
		//使用ShLwApi中的PathCompactPathEx进行路径压缩显示 
		if( ::PathCompactPath( lpLblFileName->GetDC()->GetSafeHdc(), szFile, rc.Width() ) )
		{
			m_strFileName = szFile;
			UpdateData( FALSE );
		}
	}

	InitializeControls();
	SetProgramData( m_szModule );
	
	VS_FIXEDFILEINFO vsffi = m_fvi.GetVSFFI();
		
	TCHAR   szTmp[ 512 ] = { 0 };
	TCHAR	szLID[ 56 ]  = { 0 };
	LPCTSTR lpszTmp = NULL;
	UINT	nLangs  = m_fvi.GetTransCount();	

	for( UINT n = 0; n < nLangs; n++ )
	{
		m_fvi.GetLIDName( m_fvi.GetLIDByIndex( n ), szLID, 56 );
		m_fvi.GetCPName(  m_fvi.GetCPByIndex( n ), &lpszTmp );

		::wsprintf( szTmp, _T( "%s - %s" ), szLID, lpszTmp );
		m_wndCboLang.InsertString( n, szTmp );
	}	

	
	::wsprintf( szTmp, 
		_T( "%d.%d.%d.%d" ),
		m_fvi.GetFileVersionMajor(),
		m_fvi.GetFileVersionMinor(),
		m_fvi.GetFileVersionBuild(),
		m_fvi.GetFileVersionQFE() 
	);
	
	/*
	::wsprintf( szTmp, 
		_T( "%d.%d.%d.%d" ),
		HIWORD( vsffi.dwFileVersionMS ),
		LOWORD( vsffi.dwFileVersionMS ),
		HIWORD( vsffi.dwFileVersionLS ),
		LOWORD( vsffi.dwFileVersionLS ) 
	);
	*/

	m_wndLstFvi.SetItemText( LISTITEM_FFI_FILEVERSION, 1, szTmp );
	
	::wsprintf( szTmp, 
		_T( "%d.%d.%d.%d" ),
		m_fvi.GetProductVersionMajor(),
		m_fvi.GetProductVersionMinor(),
		m_fvi.GetProductVersionBuild(),
		m_fvi.GetProductVersionQFE() 
	);

	/*
	::wsprintf( szTmp, 
		_T( "%d.%d.%d.%d" ),
		HIWORD( vsffi.dwProductVersionMS ),
		LOWORD( vsffi.dwProductVersionMS ),
		HIWORD( vsffi.dwProductVersionLS ),
		LOWORD( vsffi.dwProductVersionLS ) 
	);
	*/

	m_wndLstFvi.SetItemText( LISTITEM_FFI_PRODUCTVERSION, 1, szTmp );

	::wsprintf( szTmp, _T( "0x%xL" ), vsffi.dwFileFlagsMask );
	m_wndLstFvi.SetItemText( LISTITEM_FFI_FILEFLAGSMASK, 1, szTmp );

	::wsprintf( szTmp, _T( "0x%xL" ), vsffi.dwFileFlags );
	m_wndLstFvi.SetItemText( LISTITEM_FFI_FILEFLAGS, 1, szTmp );

	lpszTmp = MapExtractData( s_lpVOS, _countof( s_lpVOS ), vsffi.dwFileOS );
	m_wndLstFvi.SetItemText( LISTITEM_FFI_FILEOS, 1, lpszTmp );

	lpszTmp = MapExtractData( s_lpFILT, _countof( s_lpFILT ), vsffi.dwFileType );
	m_wndLstFvi.SetItemText( LISTITEM_FFI_FILETYPE, 1, lpszTmp );

	if( vsffi.dwFileType == VFT_DRV )
		lpszTmp = MapExtractData( s_lpDRVT, _countof( s_lpDRVT ), vsffi.dwFileSubtype );
	else
		lpszTmp = MapExtractData( s_lpFNTT, _countof( s_lpFNTT ), vsffi.dwFileSubtype );			
	m_wndLstFvi.SetItemText( LISTITEM_FFI_FILESUBTYPE, 1, lpszTmp );

	m_wndCboLang.SetCurSel( m_fvi.GetCurTransIndex() );	
	OnSelchangeCboLang();
}

void CVerinfDlg::SetProgramData( LPCTSTR lpszFile )
{		
	
	HICON hLargeIcon = NULL;
	CStatic* lpIcon = (CStatic*)GetDlgItem( IDC_FILEICON );
	if( lpIcon && GetProgramIcons( lpszFile, NULL, &hLargeIcon ) )
	{		
		if( hLargeIcon != NULL )
			VERIFY( ::DestroyIcon( lpIcon->SetIcon( hLargeIcon ) ) );
	}

	SHFILEINFO shfi = { 0 };	
	::SHGetFileInfo( lpszFile, FILE_ATTRIBUTE_NORMAL, &shfi, 
				 sizeof( SHFILEINFO ), SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES );
	
	m_strFileType = shfi.szTypeName;
	UpdateData( FALSE );
}

void CVerinfDlg::InitializeControls( void )
{
	m_wndCboLang.ResetContent(); 
	m_wndLstFvb.DeleteAllItems(); 
	m_wndLstFvi.DeleteAllItems();

	while ( m_wndLstFvi.DeleteColumn( 0 ) );
	while ( m_wndLstFvb.DeleteColumn( 0 ) ); 

	LPCTSTR lpszKey = _T( "Key" );
	LPCTSTR lpszVal = _T( "Value" );
	m_wndLstFvi.InsertColumn( 0, lpszKey );
	m_wndLstFvi.InsertColumn( 1, lpszVal );
	m_wndLstFvb.InsertColumn( 0, lpszKey );
	m_wndLstFvb.InsertColumn( 1, lpszVal );

	int cx = 0;
	for( int i = 0; i < _countof( s_lpszFVI ); i++ )
	{
		cx = max( cx, m_wndLstFvi.GetStringWidth( s_lpszFVI[ i ] ) );
		m_wndLstFvi.InsertItem( i, s_lpszFVI[ i ] );
	}

	cx += 10;
	CRect rc;	
	m_wndLstFvi.GetClientRect( rc );
	
	m_wndLstFvi.SetColumnWidth( 0, cx ); 	
	m_wndLstFvi.SetColumnWidth( 1, rc.Width() - cx );
}

LPCTSTR CVerinfDlg::MapExtractData( MAP* lpMap, INT nSize, DWORD dwIndex )
{	
	for( INT i = 1; i < nSize; i++ )
	{
		if( lpMap[ i ].dwVal == dwIndex )
			return lpMap[ i ].lpszVal;
	}	
	return lpMap[ 0 ].lpszVal; // UNKNOWN
}

void CVerinfDlg::OnSelchangeCboLang( void ) 
{
	m_wndLstFvb.DeleteAllItems();
	m_fvi.SetTransIndex( m_wndCboLang.GetCurSel() );
		
	
	INT		cx = 0;
	UINT	uIndex	 = 0;
	LPCTSTR lpszItem = NULL;	
	TCHAR   szValue[ 256 ] = { 0 };	

	for( int i = 0; i <= VI_STR_OLESELFREGISTER; i++ )
	{
		lpszItem = m_fvi.GetVerStringName( i );
		if( m_fvi.QueryStringValue( lpszItem, szValue, 256 ) )
		{
			cx = max( cx, m_wndLstFvb.GetStringWidth( lpszItem ) );
			m_wndLstFvb.InsertItem(  uIndex, lpszItem );
			m_wndLstFvb.SetItemText( uIndex, 1, szValue );
			uIndex++;
		}
	};

	cx += 10;
	CRect rc;
	m_wndLstFvb.GetClientRect( rc );
	m_wndLstFvb.SetColumnWidth( 0, cx ); 
	m_wndLstFvb.SetColumnWidth( 1, rc.Width() - cx );
}

BOOL CVerinfDlg::GetProgramIcons( IN  LPCTSTR lpszFileName, 
					  OUT HICON*  lphIconSmall,
					  OUT HICON*  lphIconLarge )
{
	if( lphIconSmall == NULL && lphIconLarge == NULL )
		ASSERT_RETURN( FALSE );

	if( lphIconSmall != NULL ) *lphIconSmall = NULL;
	if( lphIconLarge != NULL ) *lphIconLarge = NULL;
	
	UINT uIcons = ::ExtractIconEx( lpszFileName, 0, lphIconLarge, lphIconSmall, 1 );
	if(  uIcons <= 0 )
	{
		SHFILEINFO shfi = { 0 };
		UINT uFlags = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES;

		if( lphIconSmall != NULL )
		{
			::SHGetFileInfo( lpszFileName, FILE_ATTRIBUTE_NORMAL, &shfi, 
							 sizeof( SHFILEINFO ), uFlags | SHGFI_SMALLICON );
			*lphIconSmall = shfi.hIcon;			
		}

		if( lphIconLarge != NULL )
		{
			::SHGetFileInfo( lpszFileName, FILE_ATTRIBUTE_NORMAL, &shfi, 
							 sizeof( SHFILEINFO ), uFlags | SHGFI_LARGEICON );
			*lphIconLarge = shfi.hIcon;
		}
	}
	return ( lphIconSmall != NULL && *lphIconSmall != NULL ) ||
		   ( lphIconLarge != NULL && *lphIconLarge != NULL );
}