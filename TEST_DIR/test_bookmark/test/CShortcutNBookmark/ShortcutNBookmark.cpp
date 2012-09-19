// ShortcutNBookmark.cpp: implementation of the CShortcutNBookmark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShortcutNBookmark.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShortcutNBookmark::CShortcutNBookmark()
{
	// 0. Vista or Not   ref: http://msdn2.microsoft.com/en-us/library/ms724429.aspx
	m_bNoVista = TRUE;
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	BOOL bOsVersionInfoEx;	
	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));	
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.	
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);	
	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) {
			//return FALSE;
		}
	}
	if ( VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
	{
		m_bNoVista = FALSE;
		if( osvi.wProductType == VER_NT_WORKSTATION )  {//printf ("Windows Vista ");
		}	else {//printf ("Windows Server \"Longhorn\" " );
		}
	}


}

CShortcutNBookmark::~CShortcutNBookmark()
{

}
//	csPath = ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Favorites");
//	csPath = ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Personal");
//	csPath = ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "My Pictures");
//	csPath = ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "My Music");
CString CShortcutNBookmark::ReadREG_SZValue(HKEY hkey, CString csKey, CString csValueName)
{
	HKEY  hKey;
	DWORD dwSize     = 0;
	DWORD dwDataType = REG_SZ;
	DWORD dwValue    = 0;
	LPBYTE lpValue   = NULL;
	LONG lRet;
	lRet = ::RegOpenKeyEx(hkey,
		csKey,
		0,
		KEY_QUERY_VALUE,
		&hKey) ;
	if ( lRet != ERROR_SUCCESS ) return "";

	// Call once RegQueryValueEx to retrieve the necessary buffer size
	::RegQueryValueEx(hKey, 
		csValueName,
		0,
		&dwDataType,
		lpValue,  // NULL
		&dwSize); // will contain the data size
	if ( dwSize == 0 ) return "";
	// Alloc the buffer
	lpValue = (LPBYTE)malloc(dwSize);
	// Call twice RegQueryValueEx to get the value
	lRet = ::RegQueryValueEx(hKey, 
		csValueName,
		0,
		&dwDataType,
		lpValue,
		&dwSize);
	::RegCloseKey(hKey);
	if(ERROR_SUCCESS != lRet) return "";
	// Enjoy of lpValue...
	CString csSZValue = lpValue;
	// free the buffer when no more necessary
	free(lpValue);

	return csSZValue;
}
CString CShortcutNBookmark::GetThisEXEFullPath()
{
	char    szAppPath[MAX_PATH] = "";
	::GetModuleFileName(_Module.GetModuleInstance(), szAppPath, sizeof(szAppPath) - 1);
	
	return szAppPath;
}
//http://wiki.tcl.tk/17492
#include <shlobj.h>
CString CShortcutNBookmark::GetSpecialFolderLocation(int nFolder)
{
	CString Result;
	
	LPITEMIDLIST pidl;
	HRESULT hr = SHGetSpecialFolderLocation(NULL, nFolder, &pidl);
	
	if (SUCCEEDED(hr))
	{
		// Convert the item ID list's binary
		// representation into a file system path
		char szPath[_MAX_PATH];
		
		if (SHGetPathFromIDList(pidl, szPath))
		{
			Result = szPath;
		}
		else
		{
			ATLASSERT(FALSE);
		}
	}
	else
	{
		ATLASSERT(FALSE);
	}
	
	return Result;
}

BOOL CShortcutNBookmark::CreateDesktopShortcut(const CString& ShortcutTitle, const CString& ShortcutURL)
{
	BOOL bRet = CreateShortcut( "%programfiles%\\Internet Explorer\\iexplore.exe", 
		ShortcutTitle + ".lnk", 
		ShortcutURL,  
		ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Desktop") 
		);
	return bRet;
}
// the icon will be downloaded to "my picture" folder in other OS or low security folder in VISTA
BOOL CShortcutNBookmark::CreateDesktopShortcut(const CString& ShortcutTitle, const CString& ShortcutURL, const CString& csIconUrl)
{
	BOOL bRet = FALSE;
	CString csDownloadedFilePath;
	CString csFileName = csIconUrl.Right( csIconUrl.GetLength() - csIconUrl.ReverseFind('/') -1 );

	if ( m_bNoVista ) {
		// create file under window\\web\\wallpaper\\  
		CString csLocalFolderPath = ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "My Pictures");  
		//csLocalFolderPath = csLocalFolderPath.Left( csLocalFolderPath.ReverseFind('\\') );
		if ( csLocalFolderPath.GetLength() == 0 ) {
			csLocalFolderPath = "C:";
		}
		csLocalFolderPath.Replace( "%SystemRoot%", GetSpecialFolderLocation(CSIDL_WINDOWS));
		csDownloadedFilePath = csLocalFolderPath + "\\" + csFileName;
	}
	else {
		// if it's VISTA create file under c:\\users\\~~\\AppData\\LocalLow\\		 
		CString csLocalFolderPath = GetSpecialFolderLocation(CSIDL_PROFILE ) + "\\AppData\\localLow";
		csDownloadedFilePath = csLocalFolderPath + "\\" + csFileName;
	}

	if ( DownloadFile(csIconUrl, csDownloadedFilePath) ) 
	{	
		bRet = CreateShortcut( "%programfiles%\\Internet Explorer\\iexplore.exe", 
			ShortcutTitle + ".lnk", 
			ShortcutURL,  
			ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Desktop") ,
			csDownloadedFilePath
			);
	}
	return bRet;
}


#include <objbase.h>
#include <shlguid.h>
#include <shobjidl.h>
// Thanks : http://www.personalmicrocosms.com/html/tips.html#tip_create_shortcuts
// Ex)  CreateShortcut( "%programfiles%\\Internet Explorer\\iexplore.exe", "TestShorcut.lnk", "http://www.groovedive.com", "c:");
BOOL CShortcutNBookmark::CreateShortcut(const CString& ExePath, const CString& LinkFilename, const CString& Argument, CString csTargetFolder, CString csIconLocalPath)
{
	// Must have called CoInitalize before this function is called!
	CoInitialize(NULL);
	
	IShellLink* psl; 
	//const CString PathLink = GetSpecialFolderLocation(nFolder) + "\\" + LinkFilename;
	const CString PathLink = csTargetFolder + "\\" + LinkFilename;
	
	// Get a pointer to the IShellLink interface. 
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
		IID_IShellLink, (PVOID *) &psl); 
	if (FAILED(hr)) return FALSE;
	
	IPersistFile* ppf; 
	
	// Set the path to the shortcut target and add the description. 
	psl->SetPath((LPCSTR) ExePath); 
	//	psl->SetWorkingDirectory((LPCSTR) WorkingDirectory);
	//	psl->SetDescription((LPCSTR) Description); 
	psl->SetArguments((LPCSTR) Argument); 
	//HICON hMyIcon = ::ExtractIcon( GetModuleHandle(NULL), "D:\\CProjects\\GFtp\Debug\\GFtp.exe", 0);
	//
	//psl->SetIconLocation ( "D:\\CProjects\\GShortcut\\shortcutjp.ico", 0); // works
	//psl->SetIconLocation ( "http://mfiles.naver.net/9b4fac7b613b57e4c9b1/data33/2008/6/28/55/tulip-jangtimjang.ico", 0); // doesn't work. compare with CreateInternetShortcut() 
	if ( csIconLocalPath.GetLength() > 0 ) psl->SetIconLocation ( csIconLocalPath, 0); 
	else psl->SetIconLocation ( GetThisEXEFullPath(), 0); 
	//psl->SetIconLocation ( "C:\\Windows\\System32\\SHELL32.dll", 12); // works
	//psl->SetIconLocation ( "D:\\CProjects\\GFtp\\Debug\\GFtp.exe", 0); // works
	
	
	// Query IShellLink for the IPersistFile interface for saving the 
	// shortcut in persistent storage. 
	hr = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf); 
	if (FAILED(hr)) {
		psl->Release();
		::CoUninitialize();
		return FALSE;
	}		
	
	WORD wsz[MAX_PATH]; 
	
	// Ensure that the string is ANSI. 
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR) PathLink, -1, wsz, MAX_PATH); 
	
	// Save the link by calling IPersistFile::Save. 
	hr = ppf->Save(wsz, TRUE); 
	ppf->Release(); 
	
	psl->Release(); 

	
	::CoUninitialize();

	return TRUE;
}

// ??? URL? ?? ???? ????? ??? ??? ???.
// THANKS TO: http://www.winapi.co.kr/ApiBoard/content.php?table=tblqa&pk=4
#include <intshcut.h>
// usage :
//AddFavorite("Favorite Title", "http://www.groovedive.com");
BOOL CShortcutNBookmark::AddFavorite(const CString& FavoriteTitle, const CString& FavoriteUrl)
{
	///*
	BOOL bRet = CreateInternetShortcut(FavoriteTitle +".url" , 
		FavoriteUrl, 
		ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Favorites") 
		);
	// */

	/*
	BOOL bRet = CreateShortcut( "%programfiles%\\Internet Explorer\\iexplore.exe", 
		FavoriteTitle + ".lnk", 
		FavoriteUrl,  
		ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Favorites") 
		);
	//*/

	return bRet;
}
// the icon will be downloaded to "my picture" folder in other OS or low security folder in VISTA
//	AddFavorite("Favorite Title", "http://www.groovedive.com", "http://mfiles.naver.net/9b4fac7b613b57e4c9b1/data33/2008/6/28/55/tulip-jangtimjang.ico");
BOOL CShortcutNBookmark::AddFavorite(const CString& FavoriteTitle, const CString& FavoriteUrl, const CString& csIconUrl)
{
	BOOL bRet = FALSE;
	CString csDownloadedFilePath;
	CString csFileName = csIconUrl.Right( csIconUrl.GetLength() - csIconUrl.ReverseFind('/') -1 );

	if ( m_bNoVista ) {
		// create file under window\\web\\wallpaper\\  
		CString csLocalFolderPath = ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "My Pictures");  
		//csLocalFolderPath = csLocalFolderPath.Left( csLocalFolderPath.ReverseFind('\\') );
		if ( csLocalFolderPath.GetLength() == 0 ) {
			csLocalFolderPath = "C:";
		}
		csLocalFolderPath.Replace( "%SystemRoot%", GetSpecialFolderLocation(CSIDL_WINDOWS));
		csDownloadedFilePath = csLocalFolderPath + "\\" + csFileName;
	}
	else {
		// if it's VISTA create file under c:\\users\\~~\\AppData\\LocalLow\\		 
		CString csLocalFolderPath = GetSpecialFolderLocation(CSIDL_PROFILE ) + "\\AppData\\localLow";
		csDownloadedFilePath = csLocalFolderPath + "\\" + csFileName;
	}

	if ( DownloadFile(csIconUrl, csDownloadedFilePath) ) 
	{
		bRet = CreateInternetShortcut(FavoriteTitle +".url" , 
			FavoriteUrl, 
			ReadREG_SZValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Favorites") ,
			csDownloadedFilePath
			);		
	}

	return bRet;
}
// reference : http://mser.net/microsoft-developer-network/17/visual-c++-programming-170920.shtm
// icon assignment : http://msdn.microsoft.com/en-us/library/bb776784(VS.85).aspx
// reference : http://support.microsoft.com/kb/229092
#define PID_IS_URL  2  //from    http://listes.admisource.gouv.fr/pipermail/atelier-dev-commits/2005-August/000347.html
#define PID_IS_NAME  4
#define PID_IS_WORKINGDIR  5
#define PID_IS_HOTKEY  6
#define PID_IS_SHOWCMD  7
#define PID_IS_ICONINDEX  8
#define PID_IS_ICONFILE  9
#define PID_IS_WHATSNEW  10
#define PID_IS_AUTHOR  11
#define PID_IS_DESCRIPTION  12
#define PID_IS_COMMENT  13
#define PID_INTSITE_WHATSNEW  2
#define PID_INTSITE_AUTHOR  3
#define PID_INTSITE_LASTVISIT  4
#define PID_INTSITE_LASTMOD  5
#define PID_INTSITE_VISITCOUNT  6
#define PID_INTSITE_DESCRIPTION  7
#define PID_INTSITE_COMMENT  8
#define PID_INTSITE_FLAGS  9
#define PID_INTSITE_CONTENTLEN  10
#define PID_INTSITE_CONTENTCODE  11
#define PID_INTSITE_RECURSE  12
#define PID_INTSITE_WATCH  13
#define PID_INTSITE_SUBSCRIPTION  14
#define PID_INTSITE_URL  15
#define PID_INTSITE_TITLE  16
#define PID_INTSITE_CODEPAGE  18
#define PID_INTSITE_TRACKING  19

BOOL CShortcutNBookmark::CreateInternetShortcut(const CString& LinkFilename, const CString& Argument, CString csTargetFolder, CString csIconLocalPath)
{
	CoInitialize(NULL);
//	const CString PathLink = GetSpecialFolderLocation(CSIDL_FAVORITES) + "\\" + LinkFilename;
	const CString PathLink = csTargetFolder + "\\" + LinkFilename;
	
	// #include <intshcut.h>?
	IUniformResourceLocator *pURL;
	IPersistFile *pPf;
	WCHAR wszSCPath[MAX_PATH]={0,};
	HRESULT hr;
	
	// 
	hr=CoCreateInstance(CLSID_InternetShortcut, NULL, CLSCTX_INPROC_SERVER,IID_IUniformResourceLocator, (void **)&pURL);
	if (FAILED(hr)) return FALSE;
	ATLASSERT(SUCCEEDED(hr));

	hr = pURL->SetURL(Argument,IURL_SETURL_FL_GUESS_PROTOCOL );
	ATLASSERT(SUCCEEDED(hr));

	hr=pURL->QueryInterface(IID_IPersistFile, (void **)&pPf);
	if (FAILED(hr)) {
		pURL->Release();
		::CoUninitialize();
		return FALSE;
	}
	ATLASSERT(SUCCEEDED(hr));

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR) PathLink, -1, wszSCPath, MAX_PATH);
	hr = pPf->Save(wszSCPath,TRUE);
	ATLASSERT(SUCCEEDED(hr));

	// set the property : icon
	CComPtr<IPropertySetStorage> spIPropertySetStorage;
	hr=pURL->QueryInterface(IID_IPropertySetStorage, (void **)&spIPropertySetStorage);
	if (FAILED(hr)) {
		pURL->Release();
		::CoUninitialize();
		return FALSE;
	}
	ATLASSERT(SUCCEEDED(hr));
//	CComPtr<IPropertyStorage> spIPropertyStorageNew;
//	hr=spIPropertySetStorage->Create(FMTID_Intshcut,&FMTID_Intshcut,PROPSETFLAG_DEFAULT,STGM_READWRITE,&spIPropertyStorageNew);
//  return value says E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80004001L)
	CComPtr<IPropertyStorage> spIPropertyStorage;
	hr = spIPropertySetStorage->Open(FMTID_Intshcut, STGM_READWRITE, &spIPropertyStorage);
	if (FAILED(hr)) {
		pURL->Release();
		::CoUninitialize();
		return FALSE;
	}
	ATLASSERT(SUCCEEDED(hr));
	// Writing Property 
	PROPSPEC propspecw[2];
	//PROPSPEC ppids[2] = { {PRSPEC_PROPID, PID_IS_ICONINDEX}, {PRSPEC_PROPID, PID_IS_ICONFILE} };
	propspecw[0].ulKind = PRSPEC_PROPID;
	propspecw[0].propid = PID_IS_ICONINDEX;  
	propspecw[1].ulKind = PRSPEC_PROPID;
	propspecw[1].propid = PID_IS_ICONFILE;  
	PROPVARIANT propvarw[2];
	PropVariantInit(&propvarw[0]);
	PropVariantInit(&propvarw[1]);
	propvarw[0].vt = VT_I4;
	propvarw[0].lVal = 0;
	propvarw[1].vt = VT_LPWSTR;
	//propvarw[1].pwszVal = L"D:\\CProjects\\GShortcut\\shortcutjp.ico"; //works
	//propvarw[1].pwszVal = L"http://mfiles.naver.net/9b4fac7b613b57e4c9b1/data33/2008/6/28/55/tulip-jangtimjang.ico"; // this also works	 !!!!!!
	if ( csIconLocalPath.GetLength() > 0 ) propvarw[1].pwszVal = CComBSTR(csIconLocalPath);
	else propvarw[1].pwszVal = CComBSTR(GetThisEXEFullPath());
	//::MessageBox(NULL, GetThisEXEFullPath(), "file path", MB_OK);
	hr = spIPropertyStorage->WriteMultiple(2, propspecw, propvarw, 0); 
	ATLASSERT(SUCCEEDED(hr));
	hr = spIPropertyStorage->Commit( STGC_OVERWRITE);	//STGC_OVERWRITE  STGC_DEFAULT 
	ATLASSERT(SUCCEEDED(hr));
//	PropVariantClear(&propvarw[0]);
//	PropVariantClear(&propvarw[1]);
	// */

	// save once again
	 hr = pPf->Save(NULL,TRUE);
	 ATLASSERT(SUCCEEDED(hr));

	// veryfy the value : Reading Property Sample
	//PROPSPEC propspec;
	//propspec.ulKind = PRSPEC_PROPID;
	//propspec.propid = PID_IS_ICONFILE;  
	//PROPVARIANT propvar;
	//propvar.vt = VT_LPWSTR;
	//hr = spIPropertyStorage->ReadMultiple(1, &propspec, &propvar);
	//CString csPropValue =	propvar.pwszVal;	

	pURL->Release();
	pPf->Release();
	
	::CoUninitialize();


/*  //??? ?? ?? ??

	LPMALLOC pMalloc;
	LPITEMIDLIST lpidl;
	//char szSCPath[MAX_PATH];
	char szShortcutInfo[MAX_PATH];
	DWORD dwWritten;

  // ???? ???? ??? ???. ?? ??? ?????? ???? ??? ? ??? ?????. 
  //const CString PathLink = GetSpecialFolderLocation(CSIDL_FAVORITES) + "\\" + LinkFilename;
  CString szSCPath = GetSpecialFolderLocation(CSIDL_FAVORITES);
  
	//  if (nFolder != -1) {
	//   SHGetSpecialFolderLocation(HWND_DESKTOP, nFolder, &lpidl);
	//   SHGetPathFromIDList(lpidl, szSCPath);
	//   strcat(szSCPath, "\\");
	//    strcat(szSCPath, szFileName);
	//   SHGetMalloc(&pMalloc);
	//   pMalloc->Free(lpidl);
	//   pMalloc->Release();
	//  }
	//  if (nFolder == -1) {
	//   strcpy(szSCPath, szFileName);
	//  }
	
	  // ???? ??? ??? ???.
	  wsprintf(szShortcutInfo, "[InternetShortcut]nURL=%snIconIndex=0nIconFile=%sn","Modified=000000000000000000n",URL,IconPath);
	  
		// ??? ??? ????.
		HANDLE hFile = CreateFile(szSCPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,  FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) {
		return FALSE;
		}
		WriteFile(hFile, szShortcutInfo, strlen (szShortcutInfo), &dwWritten, NULL);
		CloseHandle(hFile);
		
	//*/
	

	return TRUE;
}



#include <WinINet.h>
#include <objidl.h>
#define BUF_SIZE	512
//	BOOL bRe = DownloadFile("http://mfiles.naver.net/9b4fac7b613b57e4c9b1/data33/2008/6/28/55/tulip-jangtimjang.ico", "c:\\tulip.ico");
BOOL CShortcutNBookmark::DownloadFile(CString csFullURL, CString csOutputFilePath)
{
	HINTERNET hIOpen ;
	hIOpen = InternetOpen( "GWallCtrl", INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL,0);
//	catch (CException* pEx)
//	{
//		char szStatus[256];
//		pEx->GetErrorMessage(szStatus, sizeof(szStatus));
//		pEx->Delete();
//	}	
	if ( hIOpen == NULL ) return FALSE;
	
	// Connect to a web page
	CString csTempFullURL = csFullURL;
	csTempFullURL.Replace("http://", "");		
	CString csServer = csTempFullURL.Left( csTempFullURL.Find( '/' ) );
	CString csTempWithGetQuery =csTempFullURL.Right( csTempFullURL.GetLength() - csTempFullURL.Find( '/' ) );

	HINTERNET hIConnect = InternetConnect(hIOpen,
		csServer, //"www.groovedive.com"
		INTERNET_INVALID_PORT_NUMBER,NULL,NULL,
		INTERNET_SERVICE_HTTP, 0, 0); 
	if ( hIConnect == NULL ) {  InternetCloseHandle(hIOpen); return FALSE; }
	
	// Request the file from the server.
	CString csAType;	
	HINTERNET hIReq = HttpOpenRequest(
		hIConnect,
		NULL,// or "Get",
		csTempWithGetQuery,//(LPCSTR )csUrlCon,
		NULL, // or (LPCSTR)HTTP_VERSION,
		NULL,
		NULL, //(LPCSTR FAR *) &csAType,
		0, //INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE,
		0);
	if ( hIReq == NULL ) {  InternetCloseHandle(hIConnect); InternetCloseHandle(hIOpen); return FALSE; }
	
	// Send the request.
	if ( !HttpSendRequest(hIReq, NULL, 0, NULL, 0) ) {
		InternetCloseHandle(hIReq); 
		InternetCloseHandle(hIConnect) ;
		InternetCloseHandle(hIOpen) ;
		return FALSE;
	}
	
	//MessageBox( "csOutputFilePath finally = " + csOutputFilePath  ); // vista
	//*/

	//
	HANDLE  hOutputFile = CreateFile(csOutputFilePath,    // open or create
		GENERIC_WRITE,              // open for writing 
		0 ,           // share for reading  
		NULL,                      // no security 
		CREATE_ALWAYS,                // overwrite existing . OPEN_EXISTING¿Ã∏È existing file only 
		FILE_ATTRIBUTE_NORMAL,     // normal file 
		NULL);                     // no attr. template  
	if (hOutputFile == INVALID_HANDLE_VALUE) 	{    
		::MessageBox(NULL, "Could not open file.", "FAILED", MB_OK); // process error 
		InternetCloseHandle(hIReq); 
		InternetCloseHandle(hIConnect) ;
		InternetCloseHandle(hIOpen) ;
		return FALSE;
	}
	
	// Read the file into the buffer. 
	DWORD dwBytesRead;
	BYTE pBuffer[BUF_SIZE]; // Read from file in 1K chunks	
	DWORD lNumberOfBytesWritten;
	BOOL bRead;
	DWORD sum = 0;
	do
	{
		if (!(bRead = InternetReadFile (hIReq, pBuffer, sizeof(pBuffer), &dwBytesRead)))
		{
			//	DWORD dwGetLastError = GetLastError(); 
			//	CString cstr;
			//	cstr.Format("ReadFile failed on buffer ; The Error Code is %d ", dwGetLastError); 				
			//	::MessageBox(NULL, cstr, "error", MB_OK);
			
			InternetCloseHandle(hIReq); 
			InternetCloseHandle(hIConnect) ;
			InternetCloseHandle(hIOpen) ;
			CloseHandle(hOutputFile);	
			return FALSE;				
		}
		sum += dwBytesRead; 

		// write out data   
		BOOL bResult = WriteFile(hOutputFile, pBuffer, dwBytesRead , &lNumberOfBytesWritten, NULL);
		ATLASSERT(dwBytesRead == lNumberOfBytesWritten );
	}
	while (dwBytesRead == sizeof(pBuffer)) ;
	// Close all of the Internet handles.
	InternetCloseHandle(hIReq); 
	InternetCloseHandle(hIConnect) ;
	InternetCloseHandle(hIOpen) ;	

	// close file   
	CloseHandle(hOutputFile);	
	return TRUE;
	
}
