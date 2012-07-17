//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilePath.h"

//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

TCHAR gsNullBufferData[1] = { 0 };

LPTSTR CFilePath::CStringLock::psNullBuffer = gsNullBufferData;

const TCHAR gsBackslash = '\\';

const LPCTSTR gsWindowsInvalidChars =
L"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\\/:*?\"<>|";

//////////////////////////////////////////////////////////////////////////

inline TCHAR CFilePath::GetFirstChar(CString const &sString)
{
	if (sString.GetLength() == 0)
		return 0;
	else
		return sString[0];
}

//////////////////////////////////////////////////////////////////////////

inline TCHAR CFilePath::GetLastChar(CString const &sString)
{
	LPCTSTR psTemp = sString;
	LPCTSTR psLastChar = _tcsdec(psTemp, psTemp + sString.GetLength());
	if (psLastChar == NULL)
		return 0;
	else 
		return *psLastChar;
}

//////////////////////////////////////////////////////////////////////////

CString CFilePath::GetEnvVar(LPCTSTR psEnvVar)
{
    SetLastError(0);

    DWORD dwResult = GetEnvironmentVariable(psEnvVar, L"", 0);
    if (!dwResult)
        return CString();

    CString sResult;
    dwResult = GetEnvironmentVariable(psEnvVar, CStringLock(sResult, dwResult), dwResult);
    return sResult;
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::EnvUnsubstRoot(CString &sString, LPCTSTR psEnvVar)
{
    CString sEnvValue = GetEnvVar(psEnvVar);
    if (!sEnvValue.GetLength())
        return FALSE;

    if (sString.GetLength() >= sEnvValue.GetLength() && _tcsnicmp(sString, sEnvValue, sEnvValue.GetLength()) == 0)
    {
        CString sModified = CString('%');
        sModified += psEnvVar;
        sModified += '%';
        sModified += sString.Mid(sEnvValue.GetLength());
        sString = sModified;

        return TRUE;
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void CFilePath::Trim(CString &sString)
{
    if (_istspace(GetFirstChar(sString)))
        sString.TrimLeft();

    if (_istspace(GetLastChar(sString)))
        sString.TrimRight();
}

//////////////////////////////////////////////////////////////////////////

TCHAR CFilePath::GetDriveLetter(TCHAR cChar)
{
    if ( (cChar >= 'A' && cChar <= 'Z'))
        return cChar;

    if (cChar >= 'a' && cChar <= 'z')
		return (TCHAR) (cChar - 'a' + 'A');

    return 0;
}

//////////////////////////////////////////////////////////////////////////

TCHAR CFilePath::GetDriveLetter(LPCTSTR psString)
{
    if (psString == NULL || *psString == 0 || psString[1] != ':')
        return 0;

    return GetDriveLetter(psString[0]);
}

//////////////////////////////////////////////////////////////////////////

CString CFilePath::QuoteSpaces(CString const &sString)
{
    if (sString.Find(' ')>=0)
    {
        CString sCopy(sString);
        CStringLock Buffer(sCopy, sCopy.GetLength() + 2);
        PathQuoteSpaces(Buffer);
        Buffer.Release();
        return sCopy;
    }

    return sString;
}

//////////////////////////////////////////////////////////////////////////

inline ERootType CFilePath::GRT_Return(ERootType RootType, int nLen, int *pnLen)
{
   if (pnLen)
      *pnLen = nLen;

   return RootType;
}

//////////////////////////////////////////////////////////////////////////

ERootType CFilePath::GetRootType(
	LPCTSTR psPath, 
	int *pnLen, 
	BOOL bGreedy)
{
   const TCHAR *psInvalidChars = L"\\/:*/\"<>|";
   const TCHAR chBK = '\\';

   if (!psPath || !*psPath)
      return GRT_Return(rtNoRoot, 0, pnLen);

   if (_istalpha(*psPath) && psPath[1] == ':')
   {
      if (psPath[2] == chBK) 
	  { 
		  return GRT_Return(rtDriveRoot, 3, pnLen); 
	  }else               
	  { 
		  return GRT_Return(rtDriveCur, 2, pnLen); 
	  }
   }

   if (psPath[0] == chBK && psPath[1] == chBK)
   {
      if (psPath[2] == '?' && psPath[3] == chBK)
      {
         int nExtraLen = 0;
         GetRootType(psPath+4, &nExtraLen) ;
         return GRT_Return(rtLongPath, 4 + nExtraLen, pnLen);
      }

      int nLen = 2 + _tcscspn(psPath + 2, psInvalidChars);
      TCHAR const *psEnd = psPath+nLen;

      if (*psEnd == 0) 
         return GRT_Return(rtServerOnly, nLen, pnLen);

      if (*psEnd == chBK && psEnd[1] == 0) 
         return GRT_Return(rtServerOnly, nLen + 1, pnLen); 

      if (*psEnd == chBK)
      {
          if (!bGreedy)
              return GRT_Return(rtServer, nLen, pnLen);

         nLen += 1 + _tcscspn(psEnd + 1, psInvalidChars);
         psEnd = psPath + nLen;

         if (*psEnd == 0) 
            return GRT_Return(rtServerShare, nLen, pnLen); 

         if (*psEnd == '\\') 
            return GRT_Return(rtServerShare, nLen + 1, pnLen);
      }
   }

   int nLen = _tcscspn(psPath, psInvalidChars);
   TCHAR const *psEnd = psPath + nLen;

   if (nLen > 0 && *psEnd == ':')
   {
      if (psEnd[1] == '/' && psEnd[2] == '/') 
         return GRT_Return(rtProtocol, nLen + 3, pnLen);
      else 
         return GRT_Return(rtPseudoProtocol, nLen + 1, pnLen); 
   }

   return GRT_Return(rtNoRoot, 0, pnLen);
}

//////////////////////////////////////////////////////////////////////////

CFilePath & CFilePath::Trim()
{
    Trim(msPath);
    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::Unquote()
{
    if (GetFirstChar(msPath) == '"' && GetLastChar(msPath) == '"')
        msPath = msPath.Mid(1, msPath.GetLength() - 2);

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::Canonicalize()
{
    if (!msPath.GetLength())
        return *this;

    if (msPath.Find(L"\\.") >= 0)
    {
        CString sTarget = msPath;
        CStringLock Buffer(sTarget, msPath.GetLength() + 2);
        PathCanonicalize(Buffer, msPath);
        Buffer.Release();
        msPath = sTarget;
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::ShrinkXXLPath()
{
    if (msPath.GetLength() >= 6 && _tcsncmp(msPath, L"\\\\?\\", 4) == 0)
    {
        LPCTSTR psPath = msPath;

        if (GetDriveLetter(psPath[4]) != 0 && psPath[5] == ':')
		{
            msPath = msPath.Mid(4);
		}else if (msPath.GetLength() >= 8)
        {
            if (_tcsnicmp(psPath + 4, L"UNC\\", 4) == 0)
            {
                int nLen = msPath.GetLength() - 8;
                CStringLock Buffer(msPath);
                memmove(Buffer + 2, Buffer + 8, nLen * sizeof(TCHAR));
                Buffer.Release(nLen + 2);
            }
        }
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::Assign(
	CString const &sString, 
	DWORD dwCleanup)
{
    msPath = sString;
    Clean(dwCleanup);
    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::MakePretty()
{
    CStringLock Buffer(msPath);
    PathMakePretty(Buffer);
    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::Clean(DWORD dwCleanup)
{
    if (dwCleanup & epcRemoveArgs)
    {
       if (dwCleanup & epcTrim)
          msPath.TrimLeft();  
       
        PathRemoveArgs(CStringLock(msPath));
    }

    if (dwCleanup & epcRemoveIconLocation)
        PathParseIconLocation(CStringLock(msPath));

    if (dwCleanup & epcTrim)
        Trim();

    if (dwCleanup & epcUnquote)
    {
        Unquote();

        if (dwCleanup & epcTrimInQuote)
            Trim();
    }

    if (dwCleanup & epcExpandEnvStrings)
        ExpandEnvStrings();

    if (dwCleanup & epcCanonicalize)
        Canonicalize();

    if (dwCleanup & epcRemoveXXL)
        ShrinkXXLPath();

    if (dwCleanup & epcSlashToBackslash)
		msPath.Replace('/', '\\');

    if (dwCleanup & epcMakePretty)
        MakePretty();

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CString CFilePath::GetStr(DWORD dwPacking) const
{
    CString sTemp = msPath;

    if (dwPacking & eppAutoQuote)
        sTemp = QuoteSpaces(sTemp);

    if (dwPacking & eppBackslashToSlash)
        sTemp.Replace('\\', '/');

    return sTemp;
}

//////////////////////////////////////////////////////////////////////////

CFilePath::CFilePath(LPCWSTR psPath) 
:	msPath(psPath)
{
    Clean();
}

//////////////////////////////////////////////////////////////////////////

CFilePath::CFilePath(CFilePath const &Path) 
:	msPath(Path.msPath) 
{

}

//////////////////////////////////////////////////////////////////////////

CFilePath::CFilePath(LPCTSTR psPath,DWORD dwCleanup)
: msPath(psPath)
{
	Clean(dwCleanup);
}

/////////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::operator = (LPCWSTR psString)
{
    if (psString == msPath.operator LPCTSTR())
        return *this;

    msPath = psString;
    Clean();

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::operator = (CString const &sString)
{
    if (sString.operator LPCTSTR() == msPath.operator LPCTSTR())
        return *this;

    msPath = sString;
    Clean();

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::operator = (CFilePath const &Path)
{
    if (Path.msPath.operator LPCTSTR() == msPath.operator LPCTSTR())
        return *this;

    msPath = Path;

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::operator &=(LPCTSTR psString)
{
    return CFilePath::Append(psString);
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::AddBackslash()
{
    if (GetLastChar(msPath) != gsBackslash)
    {
        CStringLock Buffer(msPath, msPath.GetLength()+1);

        PathAddBackslash(Buffer);
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::RemoveBackslash()
{
    if (GetLastChar(msPath) == gsBackslash)
    {
        CStringLock Buffer(msPath, msPath.GetLength()+1);

        PathRemoveBackslash(Buffer);
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::Append(LPCTSTR psString)
{
    if (psString == NULL || *psString == '\0')
    {
        AddBackslash();
    }else
    {
        int nLen = psString ? _tcslen(psString) : 0;
        CStringLock Buffer(msPath, msPath.GetLength() + nLen + 1);
        PathAppend(Buffer, psString);
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////

void CFilePath::DirectAppend(LPCTSTR psAppend)
{
	msPath += psAppend;
}

/////////////////////////////////////////////////////////////////////////////

CString CFilePath::ShellGetRoot() const
{
    LPCTSTR psPath = msPath;
    LPCTSTR psRootEnd = PathSkipRoot(psPath);

    if (psRootEnd == NULL)
        return CString();

    return msPath.Left(psRootEnd - psPath);
}

//////////////////////////////////////////////////////////////////////////

ERootType CFilePath::GetRootType(
	int *pnLen, 
	BOOL bGreedy) const
{
   return GetRootType(msPath, pnLen, bGreedy);
}

//////////////////////////////////////////////////////////////////////////

CString CFilePath::GetRoot(
	ERootType *pRootType, 
	BOOL bGreedy) const
{
   int nLen = 0;

   ERootType RootType = GetRootType(msPath, &nLen, bGreedy);

   if (pRootType)
      *pRootType = RootType;

   return msPath.Left(nLen);
}

//////////////////////////////////////////////////////////////////////////

CString CFilePath::SplitRoot(ERootType *pRootType)
{
    CString sHead;
 
   if (!msPath.GetLength())
      return sHead;

   int nRootLen = 0;
   ERootType RootType = GetRootType(msPath, &nRootLen, FALSE);
   if (pRootType)
       *pRootType = RootType;

    if (RootType == rtNoRoot)
    {
        int nStart = 0;

        if (GetFirstChar(msPath) == '\\')
            ++ nStart;

        int nPos = msPath.Find('\\', nStart);
        if (nPos < 0)
        {
            sHead = nStart ? msPath.Mid(nStart) : msPath;
            msPath.Empty();
        }else
        {
            sHead = msPath.Mid(nStart, nPos-nStart);
            msPath = msPath.Mid(nPos+1);
        }
    }else
    {
        sHead = msPath.Left(nRootLen);

        if (nRootLen < msPath.GetLength() && msPath[nRootLen] == '\\')
            ++nRootLen;

        msPath = msPath.Mid(nRootLen);
    }

    return sHead;
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::GetPath(BOOL bIncludeRoot) const
{
    LPCTSTR psPath = msPath;
    LPCTSTR psFileName = PathFindFileName(psPath);

    if (psFileName == NULL)
        psFileName = psPath + msPath.GetLength();

    LPCTSTR psRootEnd = bIncludeRoot ? NULL : PathSkipRoot(psPath);

    CFilePath RetPath;

    if (psRootEnd == NULL)
        return (LPCTSTR)msPath.Left(psFileName - psPath);
    else
        return (LPCTSTR)msPath.Mid(psRootEnd - psPath, psFileName - psRootEnd);
}

//////////////////////////////////////////////////////////////////////////

CAtlStringW CFilePath::GetName() const
{
    if (GetLastChar(msPath) == '\\')
        return CAtlStringW();

    LPCTSTR psPath = msPath;
    LPCTSTR psFileName = PathFindFileName(psPath);
    if (psFileName == NULL)
        return CAtlStringW();

    return msPath.Mid(psFileName - psPath);
}

//////////////////////////////////////////////////////////////////////////

CAtlStringW CFilePath::GetTitle() const
{
    LPCTSTR psPath = msPath;
    LPCTSTR psFileName = PathFindFileName(psPath);
    LPCTSTR psExt = PathFindExtension(psPath);

    if (psFileName == NULL)
        return CString();

    if (psExt == NULL)
        return msPath.Mid(psFileName - psPath);

    return msPath.Mid(psFileName - psPath, psExt - psFileName);
}

//////////////////////////////////////////////////////////////////////////

CAtlStringW CFilePath::GetExtension() const
{
    LPCTSTR psPath = msPath;
    LPCTSTR psExt = PathFindExtension(psPath);

    if (psExt == NULL)
        return CString();

    if (*psExt == '.')
        ++psExt;

    return msPath.Mid(psExt - psPath);
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::AddExtension(
	LPCTSTR psExtension, 
	int nLen)
{
    if (!psExtension)
        return AddExtension(L"", 0);

    if (*psExtension != '.')
    {
        CString sTemp = CString('.') + psExtension;
        return AddExtension(sTemp, sTemp.GetLength());
    }

    if (nLen < 0)
        return AddExtension(psExtension, _tcslen(psExtension));

    int nTotalLen = msPath.GetLength() + nLen;

    PathAddExtension(CStringLock(msPath, nTotalLen), psExtension);

    return *this;
}


//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::RemoveExtension()
{
    PathRemoveExtension(CStringLock(msPath));

    return *this;
}


//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::RenameExtension(LPCTSTR psNewExt)
{
    if (psNewExt == NULL || *psNewExt != '.')
    {
        RemoveExtension();
        return AddExtension(psNewExt);
    }

    int nMaxLen = msPath.GetLength() + _tcslen(psNewExt) + 1;
    PathRenameExtension(CStringLock(msPath, nMaxLen), psNewExt);

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::RemoveFileSpec()
{
    PathRemoveFileSpec(CStringLock(msPath));

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::SplitArgs(
	CString const &sPathArgs, 
	CString *psArgs, 
	DWORD dwCleanup)
{
   CString sPathWithArgs = sPathArgs;

   if (dwCleanup & epcTrim)
      sPathWithArgs.TrimLeft();

    CFilePath Path(sPathWithArgs, epcRemoveArgs);

    if (psArgs)
    {
        *psArgs = sPathWithArgs.Mid(Path.GetLength());
        psArgs->TrimLeft();
    }

    Path.Clean(dwCleanup &~ epcRemoveArgs);

    return Path;
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::SplitIconLocation(
	CString const &sPathIcon, 
	int *pnIcon, 
	DWORD dwCleanup)
{
    CString sPath = sPathIcon;
    int icon = PathParseIconLocation(CStringLock(sPath));
    if (pnIcon)
        *pnIcon = icon;

    return CFilePath(sPath, dwCleanup & ~epcRemoveIconLocation);
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::BuildRoot(int nDriveNumber)
{
    CString sDriveRoot;
    ::PathBuildRoot(CStringLock(sDriveRoot, 3), nDriveNumber);

    return CFilePath(sDriveRoot, 0);
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::GetModuleFileName(HMODULE hModule)
{
    CString sPath;
    DWORD dwRetCode = ::GetModuleFileName(hModule, CStringLock(sPath, MAX_PATH), MAX_PATH + 1);
    if (dwRetCode == 0)
        return CFilePath();

    return CFilePath(sPath);
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::GetCurrentDirectory()
{
    CString sPath;
    CStringLock Buffer(sPath, MAX_PATH);
    DWORD dwRetCode = ::GetCurrentDirectory(MAX_PATH + 1, Buffer);
    Buffer.Release(dwRetCode);

    return CFilePath(sPath);
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::GetSystemDirectory()
{
	CString sPath;
	CStringLock Buffer(sPath, MAX_PATH);
	DWORD dwRetCode = ::GetSystemDirectory(Buffer, MAX_PATH + 1);
	Buffer.Release(dwRetCode);

	return CFilePath(sPath);
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::GetCommonPrefix(LPCTSTR psSecondPath)
{
    CString psPrefix;
    PathCommonPrefix(msPath, psSecondPath, CStringLock(psPrefix, msPath.GetLength()));

    return CFilePath(psPrefix, 0);
}

//////////////////////////////////////////////////////////////////////////

int CFilePath::GetDriveNumber()
{
    return PathGetDriveNumber(msPath);
}

//////////////////////////////////////////////////////////////////////////

TCHAR CFilePath::GetDriveLetter()
{
    int nDriveNum = GetDriveNumber();
    if (nDriveNum < 0)
        return 0;

    return (TCHAR)(nDriveNum + 'A');
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::RelativePathTo(
	LPCTSTR psPathTo,
	BOOL bSrcIsDir)
{
    CString sPath;

    if (!psPathTo)
        return CFilePath();

    int nMaxLen = 3 * msPath.GetLength() / 2 + 1  + _tcslen(psPathTo); 

    PathRelativePathTo(CStringLock(sPath, nMaxLen), msPath, bSrcIsDir ? FILE_ATTRIBUTE_DIRECTORY : 0, psPathTo, 0);

    return CFilePath(sPath, 0);
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::MakeRelative(CFilePath const &BasePath)
{
   CFilePath BasePathBS = BasePath;

   BasePathBS.AddBackslash();

   if (msPath.GetLength() > BasePathBS.GetLength())
   {
      if (0 == _tcsnicmp(BasePathBS, msPath, BasePathBS.GetLength()))
      {
         msPath = msPath.Mid(BasePathBS.GetLength());
         return TRUE;
      }
   }

   return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::MakeAbsolute(CFilePath const &BasePath)
{
   if (IsRelative())
   {
      msPath = BasePath & msPath;
      return TRUE;
   }

   return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::MatchSpec(LPCTSTR psSpec)
{
    return PathMatchSpec(msPath, psSpec) != 0;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::ExpandEnvStrings()
{
    CString sTarget;
    DWORD dwLen = msPath.GetLength();
    DWORD dwRequired = ExpandEnvironmentStrings(msPath, CStringLock(sTarget, dwLen), dwLen+1);

    if (dwRequired > dwLen + 1)
        ExpandEnvironmentStrings(msPath, CStringLock(sTarget, dwRequired), dwRequired + 1);

    msPath = CFilePath(sTarget, 0);

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CString CFilePath::GetCompactStr(
	HDC hDC,
	UINT unDx, 
	DWORD dwEPPFlags)
{
    CString sRet = GetStr(dwEPPFlags);
	PathCompactPath(hDC, CStringLock(sRet), unDx);

    return sRet;
}

//////////////////////////////////////////////////////////////////////////

CString CFilePath::GetCompactStr(
	UINT unMax,
	DWORD dwFlags, 
	DWORD dwEPPFlags)
{
    CString sCleanPath = GetStr(dwEPPFlags);
    CString sRet;

    PathCompactPathEx(CStringLock(sRet, sCleanPath.GetLength()), sCleanPath, unMax, dwFlags);

    return sRet;
}

//////////////////////////////////////////////////////////////////////////

void CFilePath::SetDlgItem(
	HWND hDlgWnd,
	UINT dwCtrlID, 
	DWORD dwEPPFlags)
{
    CString sCleanPath = GetStr(dwEPPFlags);

    PathSetDlgItemPath(hDlgWnd, dwCtrlID, sCleanPath);
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::SearchAndQualify()
{
    if (!msPath.GetLength())
        return *this;

    CString sQualified;
    DWORD dwLen = msPath.GetLength();
    while (sQualified.GetLength() == 0)
    {
        PathSearchAndQualify(msPath, CStringLock(sQualified, dwLen), dwLen + 1);
        dwLen *= 2;
    }

    msPath = sQualified;

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath & CFilePath::FindOnPath(LPCTSTR *psAdditionalDirs)
{
    DWORD dwLen = msPath.GetLength() + 1 + MAX_PATH;

    BOOL bRetCode = PathFindOnPath(CStringLock(msPath, dwLen), psAdditionalDirs) != 0;
    if (bRetCode)
       SetLastError(0);

    return *this;
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::Exists() const
{
    return PathFileExists(msPath) != 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::IsDirectory() const
{
    return PathIsDirectory(msPath) != 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::IsSystemFolder(DWORD dwAttrib) const
{
    return PathIsSystemFolder(msPath, dwAttrib) != 0;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::MakeSystemFolder(BOOL bMake)
{
   BOOL bRetCode = bMake ? PathMakeSystemFolder(msPath) != 0 : PathUnmakeSystemFolder(msPath) != 0;
   if (bRetCode) 
      SetLastError(0);

   return *this;
}

//////////////////////////////////////////////////////////////////////////

CFilePath &CFilePath::MakeFullPath()
{
   if (!IsRelative())
      return *this;

   LPTSTR psDummy = NULL;
   DWORD dwChars = GetFullPathName(msPath, 0, NULL, &psDummy);
   _ASSERTE(dwChars > 0);

   CString psFull;
   dwChars = GetFullPathName(msPath, dwChars, CStringLock(psFull, dwChars), &psDummy);
   msPath = psFull;

   return *this;
}

//////////////////////////////////////////////////////////////////////////

DWORD CFilePath::GetAttributes()
{
    return ::GetFileAttributes(msPath);
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::GetAttributes(WIN32_FILE_ATTRIBUTE_DATA &FileAttributes)
{
    ZeroMemory(&FileAttributes, sizeof(FileAttributes));

    return ::GetFileAttributesEx(msPath, GetFileExInfoStandard, &FileAttributes) != 0;
}


//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::EnvUnexpandRoot(LPCTSTR psEnvVar)
{
    return EnvUnsubstRoot(msPath, psEnvVar);
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::EnvUnexpandDefaultRoots()
{
    return EnvUnexpandRoot(L"APPDATA") ||
           EnvUnexpandRoot(L"USERPROFILE") ||
           EnvUnexpandRoot(L"ALLUSERSPROFILE") ||
           EnvUnexpandRoot(L"ProgramFiles") ||
           EnvUnexpandRoot(L"SystemRoot") ||
           EnvUnexpandRoot(L"WinDir") ||
           EnvUnexpandRoot(L"SystemDrive");
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::FromRegistry(
	HKEY hBaseKey, 
	LPCTSTR psSubKey, 
	LPCTSTR psName)
{
   SetLastError(0);

   CAutoHKEY Key;
   DWORD dwRetCode = RegOpenKeyEx(hBaseKey, psSubKey, 0, KEY_READ, Key.OutArg());
   if (dwRetCode != ERROR_SUCCESS)
   {
       SetLastError(dwRetCode);
       return CFilePath();
   }

   DWORD dwLen = 256;
   DWORD dwType = 0;

   CString sPath;

   do 
   {
       CStringLock Buffer(sPath, dwLen);
       if (!Buffer)
       {
           SetLastError(ERROR_OUTOFMEMORY);
           return CFilePath();
       }

       DWORD dwSize = (dwLen + 1) * sizeof(TCHAR);
       dwRetCode = RegQueryValueEx(Key, psName, NULL, &dwType, (LPBYTE) Buffer.operator LPTSTR(), &dwSize);

       if (dwRetCode == ERROR_SUCCESS)
       {
           if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
           {
               SetLastError(ERROR_INVALID_DATA);
               return CFilePath();
           }

           break;
       }

       if (dwRetCode == ERROR_MORE_DATA)
       {
           dwLen = (dwSize + sizeof(TCHAR) - 1) / sizeof(TCHAR);
           continue;
       }

       SetLastError(dwRetCode);
       return CFilePath();
   } while(1);

   DWORD dwCleanup = epc_Default;

   if (dwType == REG_SZ)
       dwCleanup &= ~epcExpandEnvStrings;
   else
       dwCleanup |= epcExpandEnvStrings;

   return CFilePath(sPath, dwCleanup);
}

//////////////////////////////////////////////////////////////////////////

LONG CFilePath::ToRegistry(
	HKEY hBaseKey,
	LPCTSTR psSubKey,
	LPCTSTR psName,
	BOOL bReplaceEnv)
{
    CAutoHKEY Key;
    DWORD dwRetCode = RegCreateKeyEx(hBaseKey, psSubKey, NULL, NULL, 0, KEY_WRITE, NULL, Key.OutArg(), NULL);
    if (dwRetCode != ERROR_SUCCESS)
        return dwRetCode;

    CString sPath;
    if (bReplaceEnv)
    { 
        CFilePath TempPath = sPath;
        TempPath.EnvUnexpandDefaultRoots();
        sPath = TempPath.GetStr();
    }else
	{
        sPath = GetStr();
	}

    dwRetCode = RegSetValueEx(
		Key, 
		psName, 
		0, 
		bReplaceEnv ? REG_EXPAND_SZ : REG_SZ,
		(BYTE *)sPath.operator LPCTSTR(), 
		(sPath.GetLength() + 1) * sizeof(TCHAR));

    return dwRetCode;
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::IsDot() const 
{ 
    return msPath.GetLength() == 1 && msPath[0] == '.';
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::IsDotDot() const
{
    return msPath.GetLength() == 2 && msPath[0] == '.' && msPath[1] == '.';
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::IsDotty() const
{
    return IsDot() || IsDotDot();
}

//////////////////////////////////////////////////////////////////////////

BOOL CFilePath::IsValid() const
{
    if (!msPath.GetLength()) 
		return FALSE;

    if (msPath.FindOneOf(gsWindowsInvalidChars) >= 0) 
		return FALSE;

    if (GetLastChar(msPath) == '.')
    {
        if (msPath.GetLength() > 2 || msPath[0] != '.')
            return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CString CFilePath::ReplaceInvalid(
	CString const &sString, 
	TCHAR cReplaceChar)
{
    if (!sString.GetLength() || CFilePath(sString).IsDotty())
        return sString;

    CString sTemp = sString;

    for (int nLoop = 0; nLoop < sTemp.GetLength(); ++ nLoop)
    {
        TCHAR cChar = sTemp.GetAt(nLoop);

        if (_tcschr(gsWindowsInvalidChars, cChar))
            sTemp.SetAt(nLoop, cReplaceChar);
    }

    int nLen = sTemp.GetLength();

    if (sTemp[nLen-1] == '.')
        sTemp.SetAt(nLen-1, cReplaceChar);

    return sTemp;
}

//////////////////////////////////////////////////////////////////////////

CFilePath CFilePath::GetTempPath()
{
	BOOL bRetCode = FALSE;
	WCHAR sTempPath[MAX_PATH];

	ExpandEnvironmentStrings(L"%USERPROFILE%\\Local Settings\\Temp\\", sTempPath, MAX_PATH);
	bRetCode = PathFileExists(sTempPath);
	if (bRetCode == TRUE)
		return CFilePath(sTempPath);

	ExpandEnvironmentStrings(L"%TMP%\\", sTempPath, MAX_PATH);
	bRetCode = PathFileExists(sTempPath);
	if (bRetCode == TRUE)
		return CFilePath(sTempPath);

	ExpandEnvironmentStrings(L"%TEMP%\\", sTempPath, MAX_PATH);
	bRetCode = PathFileExists(sTempPath);
	if (bRetCode == TRUE)
		return CFilePath(sTempPath);

	CFilePath TempPath = CFilePath::GetModuleFileName();
	TempPath = TempPath.GetPath();
	TempPath &= L"Temp";

	CreateDirectory(TempPath, NULL);

	return CFilePath(sTempPath);
}

//////////////////////////////////////////////////////////////////////////