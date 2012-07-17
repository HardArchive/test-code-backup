#include "StdAfx.h"
#include "CKavData.h"
#include <time.h>
#include "3rdparty/my_md5.h"
#include "../KRegister.h"

CKavData::CKavData()
{
	GetInfo();
}

CKavData::~CKavData()
{
	//::DeleteCriticalSection(&m_sesion);
}

CKavData* CKavData::Instance()
{
	static CKavData _Inst;

	return &_Inst;
}

void CKavData::Initialize()
{
	//_GetUuid();

	//::InitializeCriticalSection(&m_sesion);
}

#define		defUUID								_T("idex")
#define		defKIS_CFG_VAL_KAV32_KEY			_T("CLSID\\{9B7A98EC-7EF9-468c-ACC8-37C793DBD7E0}\\Implemented Categories\\{A5F7140E-4311-4ef9-AABC-F55941B5EBE5}")
#define		defKIS_NUUID_TERM					_T("id")
#define		defMAXMD5LEN						32	
#define		defKIS_SYSTEM_FULL_PATH				_T("\\All Users\\Application Data")
#define		defNEEDNOTTOUP						-1
#define		defPNO								_T("idno")
#define		defUUIDLIST							_T("ulst")
#define		defKAV_TEMPLATE_FILE				_T("hg.dat")

BOOL CKavData::_IsNeedUpdata( IN int nIsForceUpData )
{
	CString strCurPno = _T("0");
	BOOL bRet = _GetValue(defPNO, strCurPno);
	if (!bRet)
		return FALSE;

	if (nIsForceUpData > _wtol(strCurPno))
	{
		strCurPno.Format(_T("%ld"), nIsForceUpData);
		_SetValue(defPNO, strCurPno);
		return TRUE;
	}

	return FALSE;
}
BOOL CKavData::_SetGUID( OUT CString& strUUID )
{
	return _SetValue(defUUID, strUUID);
}
BOOL CKavData::_GetGUID( OUT CString& strUUID )
{
	return _GetValue(defUUID, strUUID);
}

BOOL CKavData::_IsNeedUpdataAgain( IN const CString& strGUID, IN const CString& strFilePath )
{
	if (strGUID.GetLength() <= 0)
		return TRUE;

	int nRet = ::GetPrivateProfileInt(defUUIDLIST, strGUID, defNEEDNOTTOUP, strFilePath);
	if (nRet != defNEEDNOTTOUP)
		return TRUE;
	else
		return FALSE;	
}

CString CKavData::GetUUIDEx()
{
	CString strUUIDTemp;
	if (_IsNeedUpdata(0))
		_SetGUID(strUUIDTemp);
	else
		_GetGUID(strUUIDTemp);

	if (_IsNeedUpdataAgain(strUUIDTemp, _T("c:\\1.ini")))
		_SetGUID(strUUIDTemp);

	return strUUIDTemp;

}

CString CKavData::GetAppVer()
{
	TCHAR   szVersion[MAX_PATH] = {0};
	TCHAR	szPath[MAX_PATH * 2]	= _T("");
	CString strTemp;

	if (::GetModuleFileName(NULL, szPath, MAX_PATH) != 0)
	{
		::PathRemoveFileSpec(szPath);
		::PathAppend(szPath, _T("ressrc\\chs\\kismain.ini"));
		if (GetPrivateProfileString(_T("infos"),  _T("version"), NULL, szVersion, sizeof(szVersion), szPath))
		{
			strTemp = szVersion;
			strTemp = strTemp.Right(6);
			ZeroMemory(szVersion, '\0');
			GetPrivateProfileString(_T("DisplayVersions"),  _T("version"), NULL, szVersion, sizeof(szVersion), szPath);
			strTemp = szVersion + strTemp;
			strTemp.Replace(_T(" "), _T(""));
		}
	}

	return strTemp;
}

CString CKavData::GetTryOut()
{
	KRegister reg;
	DWORD	  dwTryOut = 1328;
	CString   strTryOut;
	if (reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Kingsoft\\KISCommon\\Update")))
	{	
		reg.Read(_T("TryNo"), dwTryOut);
		reg.Close();
	}
	strTryOut.Format(_T("%d"), dwTryOut);
	return strTryOut;
}


void CKavData::_GenerateFullPathName( IN TCHAR* szNameAfterSystem, OUT TCHAR* szFullPath, IN size_t uOutBufSzie )
{
	int nRet = 0;
	TCHAR szTemp[MAX_PATH] = { 0 };

	if (!szFullPath)
	{
		return;
	}

	nRet = SHGetSpecialFolderPathW(
		NULL,
		(LPTSTR)szTemp,
		CSIDL_COMMON_APPDATA,
		true
		);
	if (!nRet)
	{
		nRet = GetWindowsDirectoryW(szTemp, MAX_PATH);
		if (!nRet)
		{
			return;
		}

		wcscat(szTemp, defKIS_SYSTEM_FULL_PATH);
	}

	wcscat(szTemp, szNameAfterSystem);

	if (wcslen(szTemp) < uOutBufSzie)
	{
		wcscat(szFullPath, szTemp);
	}

}
BOOL CKavData::_GetTempPath( OUT CString& strPath )
{
	WCHAR szModulePath[MAX_PATH] = { 0 };

	_GenerateFullPathName(L"\\Kingsoft\\KIS\\", szModulePath, MAX_PATH);

	strPath.Format(_T("%s"), szModulePath);

	if (strPath.IsEmpty())
		return FALSE;

	strPath += defKAV_TEMPLATE_FILE;

	return TRUE;

}
BOOL CKavData::_GetValue( IN const CString strFindValue, OUT CString& strValue )
{
	BOOL bReadFromData = FALSE;
	WCHAR szValue[MAX_PATH] = {0};
	CRegKey reg;
	ULONG lSize = 128;
	long lRet = reg.Open(HKEY_CLASSES_ROOT, defKIS_CFG_VAL_KAV32_KEY);
	if (lRet != ERROR_SUCCESS)
		bReadFromData = TRUE;
	else if (reg.QueryStringValue((LPTSTR)(LPCTSTR)strFindValue, szValue, &lSize) != ERROR_SUCCESS)
		bReadFromData = TRUE;

	strValue = szValue;
	BOOL bRet = FALSE;
	do 
	{
		DWORD dwLen = 0;
		CString strPath;
		bRet = _GetTempPath(strPath);
		if (bRet)
			dwLen = ::GetPrivateProfileString(defKIS_NUUID_TERM, strFindValue, NULL, szValue, MAX_PATH, strPath);

		if (bReadFromData)
		{
			if (dwLen <= 0)
			{
				bRet = _SetValue(strFindValue, strValue);
				break;
			}
			strValue = szValue;
			bRet = TRUE;

			_UpdataValue(strFindValue, strValue);
		}
		else
		{
			if (dwLen <= 0)
				_UpdataValue(strFindValue, strValue);
		}

	} while(0);

	reg.Close();

	return bRet;
}



BOOL CKavData::_SetValue( IN const CString& strFindValue, OUT CString& strSetValue )
{
	if (strFindValue == defUUID)
	{
		CString strUUID;
		srand(time(NULL));
		strUUID.AppendFormat(L"%05ld", rand());

		GUID	guidGen;
		::CoCreateGuid(&guidGen);
		char szUUID[MAX_PATH] = {0};
		if (GUID_NULL != guidGen)
		{
			// 4表示第四代的UUID
			sprintf(szUUID, "%08lX%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X", 
				guidGen.Data1, guidGen.Data2, guidGen.Data3,
				guidGen.Data4[0], guidGen.Data4[1], guidGen.Data4[2], guidGen.Data4[3],
				guidGen.Data4[4], guidGen.Data4[5], guidGen.Data4[6], guidGen.Data4[7]);
		}

		strUUID += CA2W(szUUID);

		SYSTEMTIME time;
		GetLocalTime(&time);
		CStringW strSysTime;
		strSysTime.Format(L"%03d%02d%02d%04d%02d%02d%02d", time.wMilliseconds, time.wMinute, time.wDay, time.wYear, time.wHour, time.wSecond, time.wMonth);

		strUUID += strSysTime;

		char szUserName[MAX_PATH] = {0};
		DWORD dwSize = MAX_PATH;
		GetComputerNameA(szUserName, &dwSize);
		unsigned char szMd5Temp[defMAXMD5LEN] = {0};
		Md5_calc(szMd5Temp, (unsigned char*)szUserName, strlen(szUserName));
		CStringW strMD5Value;
		for (int i = 0; i < 16; i++)
			strMD5Value.AppendFormat(_T("%02x"), szMd5Temp[i]);

		strUUID += strMD5Value;

		WCHAR szSystemDrive[MAX_PATH] = {0};
		::GetSystemDirectory(szSystemDrive, MAX_PATH);
		ULARGE_INTEGER i64FreeBytes = { 0 };
		GetDiskFreeSpaceEx(szSystemDrive, NULL, NULL, &i64FreeBytes);

		strUUID.AppendFormat(_T("%010lu"), i64FreeBytes.u.LowPart);

		char szGUIDTemp[MAX_PATH] = {0};
		strcpy_s(szGUIDTemp, MAX_PATH, CW2A(strUUID));
		unsigned char szRetMd5Temp[defMAXMD5LEN] = {0};
		Md5_calc(szMd5Temp, (unsigned char*)szGUIDTemp, strlen(szGUIDTemp));
		strMD5Value.Empty();
		for (int i = 0; i < 16; i++)
			strMD5Value.AppendFormat(_T("%02x"), szMd5Temp[i]);

		strSetValue = strMD5Value;
	}
	// 	else if (strFindValue == defPNO)
	// 		strSetValue = _T("0");

	_UpdataValue(strFindValue, strSetValue);

	return TRUE;
}


void CKavData::_UpdataValue( IN const CString& strFindValue, IN CString& strValue )
{
	CRegKey reg;
	long lRet = reg.Open(HKEY_CLASSES_ROOT, defKIS_CFG_VAL_KAV32_KEY);
	if (lRet == ERROR_SUCCESS)
		reg.SetStringValue(strFindValue, strValue);

	reg.Close();
}

u_long Simple_HashW(IN LPCWSTR lpStrW)
{
	register u_long ulHashValue = 0;
	register wchar_t * pStrPos = NULL;

	for (ulHashValue = 0, pStrPos = (wchar_t*)lpStrW; *pStrPos; pStrPos++)
	{
		//ulHashValue = 31 * ulHashValue + *pStrPos;
		ulHashValue -= ((ulHashValue << 5) + *pStrPos);
	}

	return ulHashValue;
}

CString CKavData::_GetUuid()
{
	SYSTEMTIME sTime = { 0 };
	::GetLocalTime(&sTime);
	// 初始化随机数。
	u_long uRondom = 0;
	{
		wchar_t szRandomGuid[39] = { 0 };
		GUID   gRandomGuid = { 0 };   

		CoCreateGuid(&gRandomGuid);   
		::StringFromGUID2(gRandomGuid, szRandomGuid, sizeof(szRandomGuid));

		uRondom = Simple_HashW(szRandomGuid);
	}

	CString strUuid;
	strUuid.Format(_T("%04d%02d%02d%02d%02d%02d%03d%08X"), 
		sTime.wYear, sTime.wMonth, sTime.wDay, 
		sTime.wHour, sTime.wMinute, sTime.wSecond, sTime.wMilliseconds,
		uRondom);
	
	return strUuid;
//	.swap(strUuid);
}

BOOL CKavData::LoadStringW(OUT std::wstring& sString, u_int uId)
{
	CString strString;
	u_int uBufferSize = 1;
	int nRetCode = false;

	do 
	{
		uBufferSize *= MAX_PATH;

		nRetCode = ::LoadStringW(_Module.GetResourceInstance(), uId, strString.GetBuffer(uBufferSize), uBufferSize);
		strString.ReleaseBuffer();
		if (0 == nRetCode || nRetCode < uBufferSize)
			break;

	} while(true);

	sString = strString;

	return !sString.empty();
}


BOOL CKavData::GetInfo()
{
	BOOL	  bRetCode = FALSE;
	KRegister reg;

	if (reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Kingsoft\\Antivirus")))
	{	
		reg.Read(_T("ProductID"), m_strProductID);
		reg.Read(_T("versiontypes"), m_strVersionTypes);
		reg.Close();
	}
	
	if (reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Kingsoft\\Antivirus\\Setup")) == TRUE)
	{
		reg.Read(_T("iid"), m_strIID);
		reg.Read(_T("tod1"), m_strTOD1);
		reg.Read(_T("tod2"), m_strTOD2);	
		reg.Read(_T("tid1"), m_strTID1);
		reg.Read(_T("tid2"), m_strTID2);	
		reg.Close();
		bRetCode = TRUE;
	}

	return bRetCode;
}

CString CKavData::GetPid()
{
	// 上报时ProductID为iid
	return m_strIID;
}

CString CKavData::Gettod1()
{
	return m_strTOD1;
}

CString CKavData::Gettid1()
{
	return m_strTID1;
}
CString CKavData::Gettid2()
{
	return m_strTID2;
}

CString CKavData::Getiid()
{
	return m_strIID;
}

CString CKavData::Gettod2()
{
	return m_strTOD2;

}