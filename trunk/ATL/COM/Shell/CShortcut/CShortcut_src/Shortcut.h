//*******************************************************************
//              
//  FILE:       Shortcut.h
//              
//  AUTHOR:     Thomas Latuske <CobUser@GMX.de>
//              
//  COMPONENT:  CShortcut
//              
//  DATE:       04.05.2004
//              
//  COMMENTS:   Update 11.05.2004:
//					- added ResolveLink
//					- added GetSpecialFolder (split off)
//              
//*******************************************************************
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHORTCUT_H__2599590A_2D2B_49B1_9E63_BF123E5B67B8__INCLUDED_)
#define AFX_SHORTCUT_H__2599590A_2D2B_49B1_9E63_BF123E5B67B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
******************************************************************
*!!!! Don't forget to start OLE in your app's InitInstance: !!!!!*
******************************************************************
AfxEnableControlContainer();

	// initialize OLE:
	if (!AfxOleInit())
	{
		AfxMessageBox("Unable to initialize OLE.\nTerminating application!");
		return FALSE;
	}

*/

class CShortcut  
{
public:
	/*  This routine resolves the lnk destination: */	
	HRESULT ResolveLink(CString LnkName, UINT SpecialFolder, HWND hwnd, CString &LnkPath, CString &LnkDescription);
	/* looks if the link, with the given name in the special folder, already exists: */
	BOOL isLinkAvailable(CString LnkName, UINT SpecialFolder);
	/* set argument(s) that will be used when a file is send (SendTo): */
	void SetCmdArguments(CString sArg);
	CShortcut();
	virtual ~CShortcut();
	/*! Use this routine to create a ShortCut (ShellLink) for this Application */
	BOOL CreateShortCut(CString LnkTarget, CString LnkName, UINT SpecialFolder = CSIDL_DESKTOP, CString LnkDescription = "", CString IconLocation = "", UINT IconIndex = 1);
	/*! Use this routine to delete any Shortcut */
	BOOL DeleteShortCut(CString LnkName, UINT SpecialFolder);



private:
	/*  This routine is a helper that finds the path to the special folder: */
	BOOL GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString);
	/*  This routine is a helper that builds a long path from 8+3 one */
	int ShortToLongPathName(CString sShortPath, CString &sLongPath);
	CString m_sCmdArg;
};

#endif // !defined(AFX_SHORTCUT_H__2599590A_2D2B_49B1_9E63_BF123E5B67B8__INCLUDED_)
