/********************************************************************
	created:	2008/09/24
	filename: 	FileSupport.h
	file base:	FileSupport
	file ext:	h
	author:		huchao
	
	purpose:	Windows 文件函数的封装，使处理文件、路径更方便
*********************************************************************/
#pragma once
#include <Windows.h>
#include <atlstr.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Version.lib")

#ifndef UNICODE
#define UNICODE
#endif

#pragma warning (disable: 4996)
#pragma warning (disable: 4102)

class CFileSupport
{
private:
	CFileSupport(void);
	~CFileSupport(void);

public:	// 更改文件操作

	// 功能：删除指定目录的文件或文件夹（只读文件也可删除）
	// 参数说明
	// lpszFullPath：指定要删除的文件或文件夹全路径
	// 返回值：FALSE--失败，否则成功（文件不存在也返回成功）
	static BOOL DeleteFileEx(
		IN LPCWSTR lpszFullPath);

	// 功能：对文件加入标识，等到下次重启时再删除
	// 参数说明
	// lpszFullPath：指定要删除文件的全路径（不能是文件夹）
	// 返回值：FALSE--失败，否则成功（文件不存在也返回成功）
	static BOOL DeleteFileUntilReboot(
		IN LPCWSTR lpszFullPath);

	// 功能：对文件加入标识，等到下次重启时再替换
	// 参数说明
	// lpszExistingFileName：需要替换的文件
	// lpNewFileName：新文件
	// 返回值：FALSE--失败，TRUE--成功（文件不存在也返回TRUE）
	static BOOL MoveFileUntilReboot(
		IN LPCWSTR lpszExistingFileName, 
		IN LPCWSTR lpNewFileName);


public:	// 获取文件信息操作

	// 功能：获得指定目录的文件或文件夹的大小（单位：byte）
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// lpFileSizeHigh：文件高双字的大小
	// 返回值：文件大小（byte）
	static DWORD GetFileSize(
		IN LPCWSTR lpszFullPath, 
		OUT LPDWORD lpFileSizeHigh = NULL);

	// 功能：获得指定文件的版本号
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// pwVer1：版本号的第 1 个值
	// pwVer2：版本号的第 2 个值
	// pwVer3：版本号的第 3 个值
	// pwVer4：版本号的第 4 个值
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL GetFileVersion(
		IN LPCWSTR lpszFullPath, 
		OUT WORD* pwVer1, 
		OUT WORD* pwVer2, 
		OUT WORD* pwVer3, 
		OUT WORD* pwVer4);

	// 功能：获得指定文件的版本号字符串
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// lpszVersion：文件的版本号（类似于：2008.9.24.625）
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL GetFileVersionString(
		IN LPCWSTR lpszFullPath, 
		OUT LPWSTR lpszVersion);

	// 功能：获得指定文件的版本号字符串
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// 返回值：文件的版本号（类似于：2008.9.24.625）
	static CString GetFileVersionString(
		IN LPCWSTR lpszFullPath);

	// 功能：获得指定文件的创建时间
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// lpTime：文件的创建时间
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL GetFileCreationTime(
		IN LPCWSTR lpszFullPath, 
		OUT LPSYSTEMTIME lpTime);

	// 功能：获得指定文件访问时间
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// lpTime：文件的访问时间
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL GetFileLastAccessTime(
		IN LPCWSTR lpszFullPath, 
		OUT LPSYSTEMTIME lpTime);

	// 功能：获得指定文件修改时间
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// lpTime：文件的修改时间
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL GetFileLastWrite(
		IN LPCWSTR lpszFullPath, 
		OUT LPSYSTEMTIME lpTime);


public:	// 文件校验操作

	// 功能：获得指定文件的 CRC 值
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// 返回值：文件的CRC，0--失败
	static UINT GetFileCRC(
		IN LPCWSTR lpszFullPath);


public:	// 目录操作

	// 功能：创建多级目录，直至最深层（存在了与文件夹同名的文件，先删除同名的文件）
	// 参数说明
	// lpszFullPath：指定的文件的全路径
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL CreateDirectoryEx(
		IN LPCWSTR lpszFullPath, 
		IN BOOL bDelSameNameFile = FALSE);


public:	// 文件路径操作

	// 功能：获得当前文件的运行路径
	// 参数说明
	// lpszFullPath：返回文件的路径
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL GetCurFilePath(
		OUT LPWSTR lpszFullPath);

	// 功能：获得当前文件的运行路径
	// 返回值：文件的路径
	static CString GetCurFilePath(void);

	// 功能：获得当前运行exe文件的名称
	// 参数说明
	// lpszFullPath：返回文件的名称
	// FALSE--失败，TRUE--成功
	static BOOL GetCurFileName(
		OUT LPWSTR lpszFullPath);

	// 功能：获得当前运行exe文件的名称
	// 返回值：文件的名称
	static CString GetCurFileName(void);

	// 功能：在路径最后加入 '\\'
	// 参数说明
	// lpszFullPath：需要添加的路径
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL PathAddBackslash(
		IN OUT LPWSTR lpszFullPath);

	// 功能：在路径最后加入 '\\'
	// 参数说明
	// strFullPath：需要添加的路径
	// 返回值：添加 '\\' 后的路径
	static CString PathAddBackslashStr(
		IN const CString& strFullPath);

	// 功能：去除路径最后的'\\'
	// 参数说明
	// lpszFullPath：需要去除的路径
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL PathRemoveBackslash(
		IN OUT LPWSTR lpszFullPath);

	// 功能：去除路径最后的'\\'
	// 参数说明
	// strFullPath：需要去除的路径
	// 返回值：去除后的路径
	static CString PathRemoveBackslashStr(
		IN const CString& strFullPath);

	// 功能：在路径最后加入'/'
	// 参数说明
	// lpszFullPath：需要添加的路径
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL PathAddSlash(
		IN OUT LPWSTR lpszFullPath);

	// 功能：在路径最后加入'/'
	// 参数说明
	// strFullPath：需要添加的路径
	// 返回值：添加 '/' 后的路径
	static CString PathAddSlashStr(
		IN const CString& strFullPath);

	// 功能：去除路径最后的'/'
	// 参数说明
	// lpszFullPath：需要去除的路径
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL PathRemoveSlash(
		IN OUT LPWSTR lpszFullPath);

	// 功能：去除路径最后的'/'
	// 参数说明
	// strFullPath：需要去除的路径
	// 返回值：去除后的路径
	static CString PathRemoveSlashStr(
		IN const CString& strFullPath);

	// 功能：将字符串 lpszFullPath 中 cOrg 字符更换为 cDest 字符
	// 参数说明
	// lpszFullPath：需要转换的字符串
	// cDest：目标字符
	// cOrg：原字符
	// 返回值：FALSE--失败，TRUE--成功
	static BOOL RelpacePathChar(
		IN OUT LPWSTR lpszFullPath, 
		IN const WCHAR cDest, 
		IN const WCHAR cOrg);

	// 功能：将字符串 strFullPtah 中 cOrg 字符更换为 cDest 字符
	// 参数说明
	// strFullPtah：需要转换的字符串
	// cDest：目标字符
	// cOrg：原字符
	// 返回值：转换后的字符串
	static CString RelpacePathCharStr(
		IN const CString& strFullPtah, 
		IN const WCHAR cDest, 
		IN const WCHAR cOrg);

	// 功能：获得 Windows Temp 目录下唯一的文件名
	// 参数说明
	// lpszTempFile：Windows Temp 路径下唯一的文件名字符串
	static BOOL GetTempFileName(
		OUT LPWSTR lpszTempFile);

	// 功能：获得 Windows Temp 目录下唯一的文件名
	// 返回值：Windows Temp 路径下唯一的文件名字符串
	static CString GetTempFileName();

private:
	static BOOL _GetFileVersion(LPCWSTR lpszFullPath, WORD* pwVer1, WORD* pwVer2, WORD* pwVer3, WORD* pwVer4);

	static BOOL _GetFileTime(LPCWSTR lpszFullPath,
		LPFILETIME lpCreationTime, 
		LPFILETIME lpLastAccessTime,
		LPFILETIME lpLastWriteTime);

	static BOOL _CreateDirectory(LPCWSTR lpszDir, BOOL bDelSameNameFile);

	static CString _ConfirmPathString(const CString& strPath);

	static BOOL _DeleteFileEx(LPCWSTR lpszFullPath);

	static BOOL _GetCurFilePath(LPWSTR lpszFullPath);

	static BOOL _GetCurFileName(LPWSTR lpszFullPath);
};
