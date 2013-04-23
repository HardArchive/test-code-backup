#include "StdAfx.h"
#include "RegistryOperate.h"

char CRegistryOperate::m_strError[1024] = {0};

CRegistryOperate::CRegistryOperate(void)
{

}

CRegistryOperate::~CRegistryOperate(void)
{

}

bool CRegistryOperate::CreateRegistry(HKEY OperateType,char *pStrKeyPath,HKEY *hKey)
{
	WCHAR pwRegPath[MAX_PATH] = {0};
	DWORD dwDisposition = 0;

	CChangeCode::Gb2312ToUnicodeCode(pwRegPath,pStrKeyPath);
	if(ERROR_SUCCESS != RegCreateKeyEx( OperateType,pwRegPath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,hKey,&dwDisposition))
	{
		sprintf_s(m_strError,"创建%s注册表错误!",pStrKeyPath);
		return false;
	}
	return true;
}

bool CRegistryOperate::SetRegistryValue(char *pStrKeyName,char *pStrKeyVale,HKEY *hKey,unsigned long ValeType)
{
	WCHAR pwKeyName[MAX_PATH] = {0};
	WCHAR pwStrKeyVale[MAX_PATH] = {0};

	CChangeCode::Gb2312ToUnicodeCode(pwKeyName,pStrKeyName);
	CChangeCode::Gb2312ToUnicodeCode(pwStrKeyVale,pStrKeyVale);

	if(ERROR_SUCCESS != ::RegSetValueEx((*hKey),pwKeyName,0,ValeType,(const BYTE *)pwStrKeyVale,(DWORD)wcslen(pwStrKeyVale)*2))
	{
		sprintf_s(m_strError,"写入%s项注册表错误!",pStrKeyName);
		return false;
	}
	return true;
}

bool CRegistryOperate::GetRegistryValue(char *pStrKeyName,char *pStrKeyVale,HKEY *hKey)
{
	unsigned long  KeyType = REG_SZ,
		KeyLen  = MAX_PATH;
	WCHAR pwKeyName[MAX_PATH] = {0};
	WCHAR pwKeyVale[MAX_PATH] = {0};
	CChangeCode::Gb2312ToUnicodeCode(pwKeyName,pStrKeyName);
	if(ERROR_SUCCESS != ::RegQueryValueEx((*hKey),pwKeyName,0,&KeyType,(LPBYTE)pwKeyVale,&KeyLen))
	{
		sprintf_s(m_strError,"读取%s项注册表错误!",pStrKeyName);
		return false;
	}
	CChangeCode::UnicodeToGB2312Code(pStrKeyVale,pwKeyVale);
	return true;
}

bool CRegistryOperate::OpenRegistry(HKEY OperateType,char *pStrKeyPath,HKEY *hKey)
{
	WCHAR pwKeyPath[MAX_PATH] = {0};
	CChangeCode::Gb2312ToUnicodeCode(pwKeyPath,pStrKeyPath);
	if(ERROR_SUCCESS != ::RegOpenKeyEx(OperateType,pwKeyPath,0,KEY_ALL_ACCESS,hKey))
	{
		sprintf_s(m_strError,sizeof(m_strError),"打开%s项注册表错误!",pStrKeyPath);
		return false;
	}
	return true;
}

bool CRegistryOperate::DeleteRegistry(HKEY *OperateType,char *pStrKeyPath)
{
	WCHAR pwKeyPath[MAX_PATH] = {0};
	CChangeCode::Gb2312ToUnicodeCode(pwKeyPath,pStrKeyPath);
	if(ERROR_SUCCESS != ::RegDeleteValue((*OperateType),pwKeyPath))
	{
		sprintf_s(m_strError,sizeof(m_strError),"删除%s项注册表错误!",pStrKeyPath);
		return false;
	}
	return true;
}