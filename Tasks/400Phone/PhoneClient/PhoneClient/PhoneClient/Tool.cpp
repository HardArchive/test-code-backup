#include "stdafx.h"
#include "Tool.h"

//��ȡע�����Ϣ
void CTool::GetRegConfig(LPCTSTR pKey,char *pValue)
{
	HKEY	hkey;
	DWORD	dwType = REG_SZ, dwLenth = 128;
	char	sTemp[128] = {0};
	
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\RainSoft\\CloudCenter"), 0, KEY_QUERY_VALUE|KEY_WOW64_64KEY, &hkey) != ERROR_SUCCESS )
	{
		RegCloseKey(hkey);
		return;
	}
	if (RegQueryValueEx(hkey,pKey, 0, &dwType, (LPBYTE)&sTemp, (LPDWORD)&dwLenth) != 0 )
	{
		RegCloseKey(hkey);
		return;
	}
	RegCloseKey(hkey);
	strcpy(pValue, sTemp);
}
//���ַ���filename(num).*�л�ȡnum
int CTool::GetNum(const char *pszFileName)
{
	//char szFind[MAX_PATH]={0};
	//strncpy(szFind,pszFileName,MAX_PATH-1);
	//char *pInt=NULL;
	//for(int i=0;i<MAX_PATH-1;i++)
	//{
	//	if(*(szFind+i)=='(')
	//	{
	//		pInt=szFind+i+1;
	//	}
	//	else if(*(szFind+i)==')')
	//	{
	//		*(szFind+i)=0;
	//	}
	//}
	//if(pInt !=NULL)
	//{
	//    return atoi(pInt);
	//}
	//REQ1.TXT
	if (pszFileName && (8==strlen(pszFileName)))
	{
		return pszFileName[3]-48;
	}
	return -1;
}

void CTool::Get400RegConfig(LPCTSTR pKey,char *pValue)
{
	HKEY	hkey;
	DWORD	dwType = REG_SZ, dwLenth = MAX_PATH;
	char	sTemp[MAX_PATH] = {0};

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\RainSoft\\400phoneClient"), 0, KEY_QUERY_VALUE|KEY_WOW64_64KEY, &hkey) != ERROR_SUCCESS )
	{
		RegCloseKey(hkey);
		return;
	}
	if (RegQueryValueEx(hkey,pKey, 0, &dwType, (LPBYTE)&sTemp, (LPDWORD)&dwLenth) != 0 )
	{
		RegCloseKey(hkey);
		return;
	}
	RegCloseKey(hkey);
	strcpy(pValue, sTemp);
}
//����ע���ֵ
void CTool::SetRegConfig(LPCTSTR pKey,char *pValue)
{
	HKEY	hkey;
	DWORD	dwType = REG_SZ, dwLenth = 128;
	//char	sTemp[128] = {0};

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\RainSoft\\400phoneClient"), 0, KEY_ALL_ACCESS|KEY_WOW64_64KEY, &hkey) != ERROR_SUCCESS )
	{
		RegCloseKey(hkey);
		return;
	}
	//RegCreateKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\RainSoft\\400phoneClient"), &hKey); 
	//Regcode = RegSetValueEx(hKey, L"path", 0, REG_SZ, (CONST BYTE*)&wszPath, MAX_PATH); 
	//::RegSetValueEx(hKey,"MinAnimate",0,REG_SZ,(CONST BYTE*)&szValue,4);
	if (RegSetValueEx(hkey, pKey, 0, REG_SZ, (CONST BYTE*)pValue, MAX_PATH/*strlen(pValue)+1*/) != 0 )
	//if (RegQueryValueEx(hkey,pKey, 0, &dwType, (LPBYTE)&sTemp, (LPDWORD)&dwLenth) != 0 )
	{
		RegCloseKey(hkey);
		return;
	}
	RegCloseKey(hkey);
	//strcpy(pValue, sTemp);
}

//������ϵͳ���洦������ int iFileNum �ļ����, const char *pFileContent ������ 3000�ɹ� 4000ʧ��
bool CTool::SaveResult(int iFileNum, const char *pFileContent)
{
	FILE *pFile = NULL;
	char szFileFullPath[MAX_PATH] = {0};
	//���ɽ����Ľ���ļ� ��д������GOTOBOX(n).TXT	
	sprintf_s(szFileFullPath, MAX_PATH, "%sGOTOBOX%d.TXT", g_szMinitorFolderPah, iFileNum);
	pFile = fopen(szFileFullPath,"w");
	if(NULL == pFile)
	{
		return FALSE;
	}
	fputs(pFileContent, pFile);
	fclose(pFile);
	ZeroMemory(szFileFullPath, sizeof(szFileFullPath));

	//���ɽ�������ļ�EXEACK(n).TXT
	sprintf_s(szFileFullPath,MAX_PATH,"%sEXEACK%d.TXT", g_szMinitorFolderPah, iFileNum);
	pFile = fopen(szFileFullPath,"w");
	if(NULL ==pFile)
	{
		return FALSE;
	}
	fputs("", pFile);
	fclose(pFile);
	return true;
}