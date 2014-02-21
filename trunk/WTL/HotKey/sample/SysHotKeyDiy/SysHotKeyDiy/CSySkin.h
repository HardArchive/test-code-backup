
#pragma once
#include <atlenc.h>

typedef int ( CALLBACK * PFUNC ) ( PCHAR );

class CSySkin
{
public:
	CSySkin(void);
	~CSySkin(void);
private:
	PFUNC m_pFunc;
	HMODULE m_hModule;
	CString m_strPath;
	CString m_strDLLPath;
	CString m_strSkinPath;
private:
	CString GetModulePathName();
	CString GetSysPath();
	CString GetSrandPath();
	BOOL HideFolder(CString in_Path);
	CStringA W2CStringA(CString in_Str);
	CStringW A2CStringW(CString in_Str);
	BOOL ReleaseRes(UINT nRes_ID, CString strRes_Type, CString in_Path);
	BOOL ReleaseRes(CString in_strBase64, CString in_Path);
public:
	BOOL LoadDLLFromRes(UINT nRes_ID, CString strRes_Type);
	BOOL LoadDLLFromPath(CString in_Path);
	BOOL LoadSkinFromRes(UINT nRes_ID, CString strRes_Type);
	BOOL LoadSkinFromPath(CString in_Path);
	BOOL InstallSkin();
	BOOL UnInstallSkin(BOOL bDelete = TRUE);
};
