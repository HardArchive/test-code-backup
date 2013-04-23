#pragma once

class CRegistryOperate
{
public:
	CRegistryOperate(void);
	~CRegistryOperate(void);

	bool CreateRegistry(HKEY OperateType,char *pStrKeyPath,HKEY *hKey = NULL);
	bool SetRegistryValue(char *pStrKeyName,char *pStrKeyVale,HKEY *hKey = NULL,unsigned long ValeType = REG_SZ); 
	bool OpenRegistry(HKEY OperateType,char *pStrKeyPath,HKEY *hKey = NULL);
	bool GetRegistryValue(char *pStrKeyName,char *pStrKeyVale,HKEY *hKey = NULL);
	bool DeleteRegistry(HKEY *OperateType,char *pStrKeyPath);													//Xt:????????????
public:
	static char m_strError[1024];
};
