//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

enum ERootType
{
	rtNoRoot			= 0,
	rtDriveRoot			= 1,
	rtDriveCur			= 2,
	rtPathRoot			= 3,
	rtLongPath			= 4,
	rtServerOnly		= 5,
	rtServerShare		= 6,
	rtProtocol			= 7,
	rtPseudoProtocol	= 8,
	rtServer			= 9,
};

enum EPathCleanup
{
	epcTrim					= 1,
	epcUnquote				= 2,
	epcTrimInQuote			= 4,
	epcCanonicalize			= 8,
	epcRemoveXXL			= 16,
	epcSlashToBackslash		= 32,
	epcMakePretty			= 64,
	epcRemoveArgs			= 128,
	epcRemoveIconLocation	= 256,
	epcExpandEnvStrings		= 512,
	epc_Default = epcTrim | epcUnquote | epcTrimInQuote | epcCanonicalize | epcRemoveXXL | epcExpandEnvStrings,
};

enum EPathPacking
{
	eppAutoQuote			= 1,
	eppAutoXXL				= 2,
	eppBackslashToSlash		= 4,
	epp_Default				= eppAutoXXL,       
};

//////////////////////////////////////////////////////////////////////////

class CFilePath
{
public:
	class CStringLock
	{
	public:
		CString *mpString;
		LPTSTR mpBuffer;

		static LPTSTR psNullBuffer;

	public:
		CStringLock(CString &String) 
			: mpString(&String)
		{
			mpBuffer = mpString->GetBuffer(0);
			if (!String.GetLength())
				mpBuffer = psNullBuffer; 
		}

		CStringLock(CString &String, int nMinChars) 
			: mpString(&String)
		{
			mpBuffer = mpString->GetBuffer(nMinChars);
			if (!String.GetLength() && !nMinChars)
				mpBuffer = psNullBuffer; 

		}

		operator LPTSTR() 
		{ 
			return mpBuffer; 
		}

		void Release(int nNewLen = -1)
		{
			if (mpString)
			{
				mpString->ReleaseBuffer(nNewLen);
				mpString = NULL;
				mpBuffer = NULL;
			}
		}

		~CStringLock()  
		{
			Release();
		}
	};

public:
	class CAutoHKEY
	{
	private:
		CAutoHKEY const & operator = (CAutoHKEY const &);
		CAutoHKEY(CAutoHKEY const &);

	protected:
		HKEY mhKey;

	public:
		CAutoHKEY() 
			: mhKey(0) 
		{

		}

		CAutoHKEY(HKEY hKey) 
			: mhKey(hKey) 
		{

		}

		~CAutoHKEY()          
		{ 
			Close();
		}

		void Close()
		{
			if (mhKey) 
			{
				RegCloseKey(mhKey); 
				mhKey = 0;
			}
		}

		HKEY *OutArg()
		{
			Close();
			return &mhKey;
		}

		operator HKEY() const 
		{ 
			return mhKey; 
		}

	};

public:
	static void Trim(
		CString &String);

	static TCHAR GetDriveLetter(
		TCHAR cChar);

	static TCHAR GetDriveLetter(
		LPCTSTR psString);

	static CString QuoteSpaces(
		CString const &String);

	static ERootType GetRootType(
		LPCTSTR psPath, 
		int *pnLen, 
		BOOL bGreedy = TRUE); 

	static CFilePath SplitArgs(
		CString const &sPathArgs, 
		CString *psArgs = NULL, 
		DWORD dwCleanup = epc_Default);

	static CFilePath SplitIconLocation(
		CString const &sPathIcon, 
		int *pnIcon = NULL, 
		DWORD dwCleanup = epc_Default);

	static CFilePath BuildRoot(
		int nDriveNumber);

	static CFilePath GetTempPath();

	static CFilePath GetModuleFileName(
		HMODULE hModule = NULL);

	static CFilePath GetCurrentDirectory();

	static CFilePath GetSystemDirectory();

	static CFilePath FromRegistry(
		HKEY hBaseKey, 
		LPCTSTR psSubkey, 
		LPCTSTR psName);

	static CString ReplaceInvalid(
		CString const &sString, 
		TCHAR cReplaceChar = '_');

	static inline TCHAR GetFirstChar(
		CString const & sString);

	static inline TCHAR GetLastChar(
		CString const & sString);

	static CString GetEnvVar(
		LPCTSTR psEnvVar);

	static BOOL EnvUnsubstRoot(
		CString &sString, 
		LPCTSTR psEnvVar);

	static inline ERootType GRT_Return(
		ERootType RootType, 
		int nLen, 
		int *pnLen);

public:
    CFilePath() {}

    CFilePath(
		LPCWSTR psPath);

    CFilePath(
		CFilePath const &sPath);

    CFilePath(
		LPCWSTR psPath, 
		DWORD dwCleanup);

    CFilePath &operator = (
		LPCWSTR psPath);

    CFilePath &operator = (
		CString const &sPath);

    CFilePath &operator = (
		CFilePath const &Path);

    CFilePath &Assign(
		CString const &sString, 
		DWORD dwCleanup = epc_Default);

	void CAssign(
		CString const &psSrc);

    operator LPCTSTR () const   
	{ 
		return msPath.operator LPCTSTR(); 
	} 

    int GetLength() const  
	{ 
		return msPath.GetLength(); 
	}

    CFilePath &operator &=(
		LPCTSTR psPath);

    CFilePath &Append(
		LPCTSTR psAppendix);

	void DirectAppend(
		LPCTSTR psAppend);

    CFilePath &AddBackslash();

    CFilePath &RemoveBackslash();

    CString ShellGetRoot() const;

    CFilePath GetPath(
		BOOL bIncludeRoot = TRUE) const;

    CAtlStringW GetName() const;

    CAtlStringW GetTitle() const;

    CAtlStringW GetExtension() const;

    ERootType GetRootType(
		int *pnLen = 0, BOOL bGeedy = TRUE) const;

    CString GetRoot(
		ERootType *pRootType = NULL, 
		BOOL bGreedy = TRUE) const;

    CString SplitRoot(
		ERootType *pRootType = NULL);

    int GetDriveNumber();

    TCHAR GetDriveLetter();

    CFilePath &AddExtension(
		LPCTSTR psExt, 
		int nLen = -1);

    CFilePath &RemoveExtension();

    CFilePath &RenameExtension(
		LPCTSTR psNewExt);

    CFilePath &RemoveFileSpec();

    CFilePath &Trim();

    CFilePath &Unquote();

    CFilePath &Canonicalize();

    CFilePath &ShrinkXXLPath();

    CFilePath &MakePretty();

    CFilePath &Clean(
		DWORD dwCleanup = epc_Default);

    CString GetStr(
		DWORD dwPacking = epp_Default) const;
    
    BOOL IsValid() const;

    BOOL IsDot() const;

    BOOL IsDotDot() const;

    BOOL IsDotty() const;

    BOOL MatchSpec(
		LPCTSTR psSpec);

    BOOL IsContentType(
		LPCTSTR psContentType)  
	{ 
		return 0 != ::PathIsContentType(msPath, psContentType); 
	}

    BOOL IsFileSpec()
	{ 
		return 0 != ::PathIsFileSpec(msPath);         
	}

    BOOL IsPrefix(
		LPCTSTR psPrefix)    
	{ 
		return 0 != ::PathIsPrefix(msPath, psPrefix);
	}

    BOOL IsRelative()                
	{ 
		return 0 != ::PathIsRelative(msPath);         
	}

    BOOL IsRoot()                    
	{ 
		return 0 != ::PathIsRoot(msPath);             
	}

    BOOL IsSameRoot(
		LPCTSTR psOther)   
	{ 
		return 0 != ::PathIsSameRoot(msPath, psOther);  
	}

    BOOL IsUNC()                     
	{ 
		return 0 != ::PathIsUNC(msPath);              
	}

    BOOL IsUNCServer()               
	{ 
		return 0 != ::PathIsUNCServer(msPath);        
	}

    BOOL IsUNCServerShare()          
	{ 
		return 0 != ::PathIsUNCServerShare(msPath);   
	}

    BOOL IsURL()                     
	{ 
		return 0 != ::PathIsURL(msPath);              
	}

    CFilePath GetCommonPrefix(
		LPCTSTR psSecondPath);

    CFilePath RelativePathTo(
		LPCTSTR psPathTo, 
		BOOL bSrcIsDir = TRUE);

    BOOL MakeRelative(
		CFilePath const &BasePath);

    BOOL MakeAbsolute(
		CFilePath const &BasePath);

    CString GetCompactStr(
		HDC hDC, 
		UINT unDx, 
		DWORD dwEPPFlags = 0);

    CString GetCompactStr(
		UINT unMax, 
		DWORD dwEPPFlags = 0, 
		DWORD dwFlags = 0);

    void SetDlgItem(
		HWND hDlgWnd, 
		UINT unCtrlID, 
		DWORD dwEPPFlags = 0);

    CFilePath &SearchAndQualify();

    CFilePath &FindOnPath(
		LPCTSTR *ppsAdditionalDirs = 0);

    BOOL Exists() const;

    BOOL IsDirectory() const;

    BOOL IsSystemFolder(
		DWORD dwAttrib = FILE_ATTRIBUTE_SYSTEM) const;

    CFilePath &MakeSystemFolder(
		BOOL bMake = TRUE);

    DWORD GetAttributes();

    BOOL GetAttributes(
		WIN32_FILE_ATTRIBUTE_DATA &pAttributes);

    CFilePath &MakeFullPath(); 

    CFilePath &ExpandEnvStrings();

    BOOL EnvUnexpandRoot(
		LPCTSTR psEnvVar);

    BOOL EnvUnexpandDefaultRoots();

    LONG ToRegistry(
		HKEY hBaseKey, 
		LPCTSTR psSubKey, 
		LPCTSTR psName, 
		BOOL bReplaceEnv = TRUE);

protected:
	CString msPath;
};

//////////////////////////////////////////////////////////////////////////

inline CFilePath operator &(CFilePath const &sPath, LPCTSTR psString) 
{ 
	CFilePath RetPath = sPath; 
	RetPath &= psString; 
	return RetPath; 
}

//////////////////////////////////////////////////////////////////////////