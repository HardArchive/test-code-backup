// ShortcutNBookmark.h: interface for the CShortcutNBookmark class.

// If you need to create internet favorite(.url) or desktop shortcut (.lnk) with an attached icon that should be downloaded first
// Do as follow : 
/*
0) Add ShortcutNBookmark.h, ShortcutNBookmark.cpp file to your project
0) Add wininet.lib in [project setting]>[Link]
 
1) Adding Favorite ( ".url" file ) (would be triggered by a button maybe )
 CShortcutNBookmark csnb;
 csnb.AddFavoriteNAttachIcon("GrooveDive", "http://www.groovedive.com", "http://mfiles.naver.net/9b4fac7b613b57e4c9b1/data33/2008/6/28/55/tulip-jangtimjang.ico");
// csnb.AddFavorite("GrooveDive", "http://www.groovedive.com"); // is also available. this function will create a url file using the icon of your application

1) Creating Desktop Shortcut ( ".lnk" file )  (would be triggered by a button maybe )
 CShortcutNBookmark csnb;
 csnb.CreateDesktopShortcutNAttachIcon("GrooveDive", "http://www.groovedive.com", "http://mfiles.naver.net/9b4fac7b613b57e4c9b1/data33/2008/6/28/55/tulip-jangtimjang.ico");
// csnb.CreateDesktopShortcut("GrooveDive", "http://www.groovedive.com"); // is also available. this function will create a url file using the icon of your application

!!) In some case, your compilation would fail messaging below link error
		shell32.lib(shguid.obj) : fatal error LNK1103: debugging information corrupt; recompile module
	That is because of the new SDK library.
		Go to [tools]>[option]>[directories]>[library files]
		Place C:\Program Files\Microsoft Visual Studio\VC98\LIB (old library)
		higher than C:\PROGRAM FILES\MICROSOFT PLATFORM SDK\LIB (new library)
!) Note that in VISTA, downloading fails due to the enhanced security. 
   But there are some folders for low security and this class is downloading file under that path. 

jangtimjang@naver.com
2008.06.28
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHORTCUTNBOOKMARK_H__31D8DAF3_9764_49AE_B413_141F41AE966C__INCLUDED_)
#define AFX_SHORTCUTNBOOKMARK_H__31D8DAF3_9764_49AE_B413_141F41AE966C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShortcutNBookmark  
{
public:
	CShortcutNBookmark();
	virtual ~CShortcutNBookmark();

	// high level function for creating ".url" and ".lnk" file in Favorite and Desktop folder
	BOOL AddFavorite(const CString& LinkFilename, const CString& Argument);
	BOOL AddFavorite(const CString& LinkFilename, const CString& Argument, const CString& csIconUrl);
	BOOL CreateDesktopShortcut(const CString& ShortcutTitle, const CString& ShortcutURL);
	BOOL CreateDesktopShortcut(const CString& ShortcutTitle, const CString& ShortcutURL, const CString& csIconUrl);		

private:
	// low level function for creating ".url" and ".lnk" file at a specific location
	BOOL CreateInternetShortcut(const CString& LinkFilename, const CString& Argument, CString csTargetFolder, CString csIconLocalPath = "");
	BOOL CreateShortcut(const CString& ExePath, const CString& LinkFilename, const CString& Argument, CString csTargetFolder, CString csIconLocalPath = "");

	// untility function
	CString GetThisEXEFullPath();
	CString ReadREG_SZValue(HKEY hkey, CString csKey, CString csValueName);
	BOOL DownloadFile(CString csFullURL, CString csOutputFilePath);
	CString GetSpecialFolderLocation(int nFolder);

private:
	BOOL m_bNoVista;
};

#endif // !defined(AFX_SHORTCUTNBOOKMARK_H__31D8DAF3_9764_49AE_B413_141F41AE966C__INCLUDED_)
