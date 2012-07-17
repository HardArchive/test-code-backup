#pragma once
#include <string>
#include <vector>


class CKavData
{
public:
	CKavData();
	~CKavData();
public:
	static CKavData* Instance();
	void Initialize();
	CRITICAL_SECTION	m_sesion;

public:	
	CString GetUUIDEx( );
	CString GetAppVer();
	CString GetPid();
	CString Getiid();
	CString Gettid1();
	CString Gettid2();
	CString Gettod1();
	CString Gettod2();
	CString   GetTryOut();
	BOOL	LoadStringW(OUT std::wstring& sString, u_int uId);

private:
	BOOL _IsNeedUpdataAgain( IN const CString& strGUID, IN const CString& strFilePath );
	BOOL _GetValue( IN const CString strFindValue, OUT CString& strValue );
	BOOL _SetValue( IN const CString& strFindValue, OUT CString& strSetValue );
	BOOL _IsNeedUpdata( IN int nIsForceUpData );
	BOOL _SetGUID( OUT CString& strUUID );
	BOOL _GetGUID( OUT CString& strUUID );

	CString _GetUuid();
	BOOL _GetTempPath( OUT CString& strPath );
	void _UpdataValue( IN const CString& strFindValue, IN CString& strValue );
	void _GenerateFullPathName( IN TCHAR* szNameAfterSystem, OUT TCHAR* szFullPath, IN size_t uOutBufSzie );
	BOOL GetInfo();

private:
	CString m_strIID;
	CString m_strTOD1;
	CString m_strTOD2;
	CString m_strTID1;
	CString m_strTID2;
	CString m_strVersionTypes;
	CString m_strProductID;
};
