#ifndef __KFunction_h__
#define __KFunction_h__

class KFunction
{
public:
	static CString GetModulePath(HMODULE hModule);
	static CString GetPlatformLang(void);
	static CString GetPlatformRootPath(void);
	static CString GetProductRootPath(void);
	static CString GetPlatformResPath(void);
	static CString GetDriversPath(void);
	static CString GetKisAllUserPath(void);
	static void PathAddBackslash(CString& strPath);
	static BOOL IsFileInUse(LPCTSTR szPath);
	static BOOL CreateDeepDirectory(LPCTSTR szPath);
	static BOOL GetDebugPrivilege(void);
	static BOOL IsWow64(void);
	static BOOL RunApp(LPCTSTR szPath, LPCTSTR szFile, LPCTSTR szCmd, DWORD dwWaitTime = 0);
	static BOOL ShellRunApp(LPCTSTR szPath, LPCTSTR szFile, LPCTSTR szCmd);
	static BOOL CopyFolder(LPCTSTR szSrcPath, LPCTSTR szDstPath, BOOL& bReboot);
	static BOOL CopyFolder_Delay(LPCTSTR szSrcPath, LPCTSTR szDstPath);
	static BOOL CopyFolder_Force(LPCTSTR szSrcPath, LPCTSTR szDstPath);
	static BOOL IsEnableCopy(LPCTSTR szSrcPath, LPCTSTR szDstPath);
	static BOOL IsDirectoryExist(LPCTSTR szPath);
	static BOOL IsScomRegistered(LPCTSTR szGUID, LPCTSTR szPath);
	static int  KillProcessByPath(LPCTSTR szPath, int* pnKillCount = NULL, BOOL bDirectory = FALSE);
	static BOOL RebootSystem(void);
	static BOOL UninstallService(LPCTSTR szServiceName);
	static BOOL DeleteFolder(LPCTSTR szPath);
	static LONG DeleteSubKeyTree(HKEY hKey, LPCTSTR lpSubKey);
	static BOOL IsProcessRunning(LPCTSTR szPath);
	static BOOL IsDriverFile(LPCTSTR szPath);
	static BOOL EnableWow64FsRedirection(BOOL bEnable);

protected:
	static BOOL _CopyFolder(LPCTSTR szSrcPath, LPCTSTR szDstPath, BOOL& bReboot);
	static BOOL _CopyFolder_Delay(LPCTSTR szSrcPath, LPCTSTR szDstPath);
	static BOOL _CopyFolder_Force(LPCTSTR szSrcPath, LPCTSTR szDstPath);
	static BOOL _IsEnableCopy(LPCTSTR szSrcPath, LPCTSTR szDstPath);
	static BOOL _DeleteFolder(LPCTSTR szPath);
};

#endif