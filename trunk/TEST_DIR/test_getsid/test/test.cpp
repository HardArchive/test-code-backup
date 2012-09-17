// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>  

using namespace std;

//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include "stdafx.h"
#include <windows.h>
#include <Sddl.h>

#include <shlobj.h>
#pragma comment(lib, "shell32.lib") 

//#define MAX_NAME 256

BOOL GetCurrentUserForSID (VOID) 
{
	DWORD dwSize = 0, dwResult = 0;
	HANDLE hToken;
	PTOKEN_USER pUserInfo;

	LPTSTR    StringSid;

	if (!OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken )) 
		return FALSE;

	if(!GetTokenInformation(hToken, TokenUser, NULL, dwSize, &dwSize)) 
	{
		dwResult = GetLastError();
		if( dwResult != ERROR_INSUFFICIENT_BUFFER )
			return FALSE;
	}

	pUserInfo = (PTOKEN_USER) GlobalAlloc( GPTR, dwSize );

	if(! GetTokenInformation(hToken, TokenUser, pUserInfo, 
		dwSize, &dwSize ) ) 
	{
		return FALSE;
	}


	ConvertSidToStringSid(pUserInfo->User.Sid, &StringSid);
		_tprintf(TEXT("Sid: %s\n"), StringSid);
	//MessageBox(NULL, StringSid, StringSid, MB_OK);
	//printf("Sid: %s\n", StringSid);


	if ( pUserInfo )
		GlobalFree( pUserInfo );
	return TRUE;
}

//通过用户名获取SID
PSID GetUserSid(LPCTSTR szUserName)
{
	SID		*sid = 0;
	TCHAR	*dom = 0;

	DWORD	sidlen = 0;
	DWORD	domlen = 0;

	SID_NAME_USE snu;

	//
	// with no machine specified, LookupAccountName looks up user's sid in
	// following locations:
	//
	// well-known, built-in, local-machine, primary-domain, trusted-domain
	//
	while(!LookupAccountName(NULL, szUserName, sid, &sidlen, dom, &domlen, &snu))
	{
		if(sid) free(sid);
		if(dom) free(dom);

		if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			return 0;

		sid = malloc(sidlen);
		dom = malloc(domlen);
	}

	printf("domain: %s\n", dom);

	free(dom);
	return sid;
}

int _tmain(int argc, _TCHAR* argv[])
{
	GetCurrentUserForSID();

	WCHAR wszRecentPath[MAX_PATH] = {0};
	if(!SHGetSpecialFolderPath(NULL, wszRecentPath, CSIDL_RECENT, false))
	{
		//TRACE(TEXT("获取Recent文件夹路径失败,错误码：%d"), GetLastError());
		return 0;
	}
	return 0;
}
