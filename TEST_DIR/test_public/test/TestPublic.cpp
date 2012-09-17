#include "stdafx.h"
#include "TestPublic.h"


//#include "public/RegUtil.h"
//#include "public/IniUtil.h"
//#include "public/PathUtil.h"
#include <iostream>
using namespace std;
//////
//////
////////#define TRACE_MAX_LEN 0x400   //最大TRACE长度为1024
////////#define TRACE_MAX_LEN 0xa   //最大TRACE长度为1024
////////#define RUNINDEBUGMODE
//////#define USES_RELEASE_TRACE
//////#include <TraceUtil.h>
//////int TraceUtil()
//////{
//////	char szTem[32] = {"Test^^^^^^char"};
//////	WCHAR wszTem[32] = {L"Test^^^^^^WCHAR"};
//////	TCHAR tszTem[32] = {_T("Test^^^^^^TCHAR")};
//////	//TRACE("char……\r\n");
//////	//for (int i=1; i<=15;i++)
//////	//{
//////	//	TRACE("%d、%s\r\n", i, szTem);
//////	//}
//////
//////	//TRACE(L"WCHAR……\r\n");
//////	//for (int i=1; i<=15;i++)
//////	//{
//////	//	TRACE(L"%d、%s\r\n", i, wszTem);
//////	//}
//////
//////	QNA::TRACE(_T("TCHAR……\r\n"));
//////	for (int i=1; i<=15;i++)
//////	{
//////		QNA::TRACE(_T("%d、%s\r\n"), i, tszTem);
//////	}
//////
//////}
//////int IniUtil()
//////{
//////	QNA::CIniUtil clsIniUtil;
//////
//////
//////	return 1;
//////}
//////
//////int FileUtil()
//////{
//////	TCHAR tszPath[MAX_PATH] = {0};
//////	if (!QNA::GetExePath(tszPath))
//////	{
//////		return -1;
//////	}
//////
//////	_tcscat_s(tszPath, _T("\\123"));
//////	
//////	if (!QNA::CreateMultipleDirectory(tszPath))
//////	{
//////		return -2;
//////	}
//////
//////	_tcscpy_s(tszPath, _T("D:\\123"));
//////
//////	if (!QNA::CreateMultipleDirectory(tszPath))
//////	{
//////		return -2;
//////	}
//////	return 1;
//////}


#include "public/FileVersionInfo.h"


//VI_STR_COMMENTS - Comments
//VI_STR_COMPANYNAME - CompanyName
//VI_STR_FILEDESCRIPTION - FileDescription
//VI_STR_FILEVERSION - FileVersion
//VI_STR_INTERNALNAME - InternalName
//VI_STR_LEGALCOPYRIGHT - LegalCopyright
//VI_STR_LEGALTRADEMARKS - LegalTrademarks
//VI_STR_ORIGINALFILENAME - OriginalFilename
//VI_STR_PRIVATEBUILD - PrivateBuild
//VI_STR_PRODUCTNAME - ProductName
//VI_STR_PRODUCTVERSION - ProductVersion
//VI_STR_SPECIALBUILD - SpecialBuild
//VI_STR_OLESELFREGISTER - OLESelfRegister 
int TestFileVersionInfo()
{
	CFileVersionInfo	cls_fvi;
	cls_fvi.Open(_T("C:\\windows\\explorer.exe"));
	cls_fvi.GetVersionInfo();
	cls_fvi.GetLIDName()

	return 1;
}