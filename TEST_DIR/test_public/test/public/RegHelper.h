/***************************************************************************************************
* 1¡¢ File       £º RegHelper.h
* 2¡¢ Version    £º *.*
* 3¡¢ Description£º 
* 4¡¢ Author     £º RG (http://www.9cpp.com/)
* 5¡¢ Created    £º 2013-1-8 10:04:16
* 6¡¢ History    £º 
* 7¡¢ Remark     £º 
* //½Ó¿Ú
	LONG GetSettingInt(HKEY hkey, TCHAR szKeyName[], LONG nDefault);
    BOOL GetSettingBool(HKEY hkey, TCHAR szKeyName[], BOOL nDefault);
	LONG GetSettingStr(HKEY hkey, TCHAR szKeyName[], TCHAR szDefault[], TCHAR szReturnStr[], DWORD nSize);
	LONG GetSettingBinary(HKEY hkey, TCHAR szKeyName[], void *buf, ULONG nNumBytes);
	
	LONG WriteSettingInt(HKEY hkey, TCHAR szKeyName[], LONG nValue);
	LONG WriteSettingBool(HKEY hkey, TCHAR szKeyName[], BOOL nValue);
	LONG WriteSettingStr(HKEY hkey, TCHAR szKeyName[], TCHAR szString[]);
	LONG WriteSettingBinary(HKEY hkey, TCHAR szKeyName[], void *buf, UINT nNumBytes);
****************************************************************************************************/


#ifndef _REGHELPER_INCLUDED
#define _REGHELPER_INCLUDED

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	namespace RG
	{

		inline LONG GetSettingBinary(HKEY hkey, TCHAR szKeyName[], void *buf, ULONG nNumBytes)
		{
			DWORD type = REG_BINARY;
			ULONG len = nNumBytes;

			if(ERROR_SUCCESS == RegQueryValueEx(hkey, szKeyName, 0, &type, (BYTE *)buf, &nNumBytes))
			{
				if(type != REG_BINARY) return 0;
				else return nNumBytes;
			}
			else
			{
				return 0;
			}
		}

		inline LONG GetSettingInt(HKEY hkey, TCHAR szKeyName[], LONG nDefault)
		{
			DWORD type;
			LONG value;
			ULONG len = sizeof(value);

			if(ERROR_SUCCESS == RegQueryValueEx(hkey, szKeyName, 0, &type, (BYTE *)&value, &len))
			{
				if(type != REG_DWORD) return nDefault;
				return value;
			}
			else
			{
				return nDefault;
			}
		}

		inline BOOL GetSettingBool(HKEY hkey, TCHAR szKeyName[], BOOL nDefault)
		{
			DWORD type;
			BOOL  value;
			ULONG len = sizeof(value);

			if(ERROR_SUCCESS == RegQueryValueEx(hkey, szKeyName, 0, &type, (BYTE *)&value, &len))
			{
				if(type != REG_DWORD) return nDefault;
				return value != 0;
			}
			else
			{
				return nDefault;
			}
		}

		inline LONG GetSettingStr(HKEY hkey, TCHAR szKeyName[], TCHAR szDefault[], TCHAR szReturnStr[], DWORD nSize)
		{
			DWORD type = REG_SZ;
			TCHAR bigbuf[256];
			ULONG len = sizeof(bigbuf);

			if(ERROR_SUCCESS == RegQueryValueEx(hkey, szKeyName, 0, &type, (BYTE *)bigbuf, &len))
			{
				if(type != REG_SZ || type != REG_MULTI_SZ) return 0;
				memcpy(szReturnStr, bigbuf, len+sizeof(TCHAR));
				return len;
			}
			else
			{
				len = min(nSize, (DWORD)lstrlen(szDefault) * sizeof(TCHAR));
				memcpy(szReturnStr, szDefault, len+sizeof(TCHAR));
				return len;
			}
		}

		inline LONG WriteSettingInt(HKEY hkey, TCHAR szKeyName[], LONG nValue)
		{
			return RegSetValueEx(hkey, szKeyName, 0, REG_DWORD, (BYTE *)&nValue, sizeof(nValue));
		}

		inline LONG WriteSettingBool(HKEY hkey, TCHAR szKeyName[], BOOL nValue)
		{
			return RegSetValueEx(hkey, szKeyName, 0, REG_DWORD, (BYTE *)&nValue, sizeof(nValue));
		}

		inline LONG WriteSettingStr(HKEY hkey, TCHAR szKeyName[], TCHAR szString[])
		{
			return RegSetValueEx(hkey, szKeyName, 0, REG_SZ, (BYTE *)szString, (lstrlen(szString) + 1) * sizeof(TCHAR));
		}

		inline LONG WriteSettingBinary(HKEY hkey, TCHAR szKeyName[], void *buf, UINT nNumBytes)
		{
			return RegSetValueEx(hkey, szKeyName, 0, REG_BINARY, (BYTE *)buf, nNumBytes);
		}
	}


#ifdef __cplusplus
}
#endif

#endif