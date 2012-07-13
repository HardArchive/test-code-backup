// FavoriteTest.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
CQueueEx<INKINFO>* g_pInkInfoQue;   //网址快捷方式队列

//获取收藏夹路径
BOOL GetFavoritesPath(WCHAR* pwszFavoritespath)
{
	if (!pwszFavoritespath)
	{
		return FALSE;
	}
	
//方法1
	if (!SHGetSpecialFolderPath(NULL, pwszFavoritespath, CSIDL_FAVORITES, FALSE))
	{	//得到目录，
		return FALSE;
	}
//方法2
	//HKEY hKEY;
	//DWORD type=REG_SZ;
	//LPBYTE favpath = new BYTE[80];   //路径保存位置
	//LPCTSTR path="Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders";
	//DWORD cbData=80;
	//::RegOpenKeyEx(HKEY_CURRENT_USER,path,0,KEY_READ,&hKEY);
	//::RegQueryValueEx(hKEY,"Favorites",NULL,&type,favpath,&cbData);
	//::RegCloseKey(hKEY);
	return TRUE;
}

/******************************************************************************* 
1、 函数名称： BOOL GetFileInfo(PWCHAR pwszPath)
2、 功能描述： 获取快捷网页快捷方式信息
3、 输入参数： PWCHAR pwszPath 快捷方式完整路径
4、 返 回 值： 成功返回TRUE，失败返回FALSE
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-11-25 11:24:41
8、 备    注： 
********************************************************************************/ 
BOOL GetFileInfo(PWCHAR pwszPath, PINKINFO pstuInkInfo)
{
	char *ptr = NULL;
	char szTem[MAX_PATH] = {0};
	

	if (!pwszPath && pstuInkInfo)
	{
		return FALSE;
	}

	//这里还应该判断文件后缀是否为url

	GetPrivateProfileString("InternetShortcut", "URL", NULL, pstuInkInfo->szUrl, 512, pwszPath);//获取网址
	wcscpy(pstuInkInfo->wszPath, pwszPath);      //复制完整路径
	

	return TRUE;
}

/******************************************************************************* 
1、 函数名称： int GetInkFileInfo(LPCTSTR pwszDirPath)
2、 功能描述： 得到指定目录中的快捷方式信息 
3、 输入参数： LPCTSTR pwszDirPath  指定的目录指针，这里指收藏夹指针
4、 返 回 值： -1 输入参数指针为空
			  -2 未找到文件
5、 动态内存： 
6、 代码设计：  阿毛
7、 开发日期： 2011-11-25 11:00:27
8、 备    注： 
********************************************************************************/ 
int GetInkFileInfo(LPCTSTR pwszDirPath)
{
	int iRet = 0;
	HANDLE hFind = NULL;
	WIN32_FIND_DATA stuWFD = {0};	
	WCHAR wszFullPath[MAX_PATH] = {0};
	WCHAR wszFindFilter[MAX_PATH] = {0};
	DWORD dwAttributes = 0;
	INKINFO stuInkInfo = {0};
	if (!pwszDirPath)
	{
		return -1;
	}

	swprintf_s(wszFindFilter, L"%s\\*.*", pwszDirPath);    //查找文件类型

	if ((hFind = FindFirstFile(wszFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
	{
		return -2;
	}

	do
	{
		//过滤本级目录和父目录
		/*if (_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0 )
		continue;*/
		if(stuWFD.cFileName[0]=='.')
			continue;

		//去掉只读属性
		dwAttributes = GetFileAttributes(wszFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(wszFullPath, dwAttributes);
		}

		//如果找到的是目录，则进入此目录进行递归
		if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			ZeroMemory(wszFullPath, sizeof(wszFullPath));
			swprintf_s(wszFullPath, L"%s\\%s", pwszDirPath, stuWFD.cFileName);  //得到文件夹完整路径
			iRet = GetInkFileInfo(wszFullPath);
			if (1 != iRet)
			{
				return iRet;
			}
		}
		else 
		{ 
			//找到的是文件
			swprintf_s(wszFullPath, L"%s\\%s", pwszDirPath, stuWFD.cFileName); //得到文件完整路径
			wcscpy(stuInkInfo.wszUrlName, stuWFD.cFileName);         //获取文件名
			if (!GetFileInfo(wszFullPath, &stuInkInfo))
			{
				return FALSE;
			}
		}	

	}while (FindNextFile(hFind, &stuWFD));

	return 1;
}



//BOOL ResolveIt(WCHAR lpszpath)
//{
//	HRESULT hres = 0;   
//	IShellLink* psl = NULL;    
//	hres = CoCreateInstance(&CLSID_ShellLink, NULL,  CLSCTX_INPROC_SERVER, &IID_IShellLink, &psl);   
//
//}
//
//
//HRESULT ResolveIt(HWND hwnd, LPCSTR lpszLinkFile, LPSTR lpszPath)    
//{    
//	HRESULT hres;    
//	IShellLink* psl;    
//	char szGotPath[MAX_PATH];    
//	char szDescription[MAX_PATH];    
//	WIN32_FIND_DATA stuWFD;    
//
//	*lpszPath = 0; // assume failure    
//
//	// Get a pointer to the IShellLink interface.    
//	hres = CoCreateInstance(&CLSID_ShellLink, NULL,    
//		CLSCTX_INPROC_SERVER, &IID_IShellLink, &psl);    
//	if (SUCCEEDED(hres))
//	{    
//		IPersistFile* ppf;    
//
//		// Get a pointer to the IPersistFile interface.    
//		hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile,    
//			&ppf);    
//		if (SUCCEEDED(hres))
//		{    
//			WORD wsz[MAX_PATH];    
//
//			// Ensure that the string is Unicode.    
//			MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz,   
//
//				MAX_PATH);    
//
//			// Load the shortcut.    
//			hres = ppf->lpVtbl->Load(ppf, wsz, STGM_READ);    
//			if (SUCCEEDED(hres)) 
//			{    
//
//				// Resolve the link.    
//				hres = psl->lpVtbl->Resolve(psl, hwnd, 
//					SLR_ANY_MATCH);    
//				if (SUCCEEDED(hres)) 
//				{    
//
//					// Get the path to the link target.    
//					hres = psl->lpVtbl->GetPath(psl, 
//						szGotPath,    
//						MAX_PATH, (WIN32_FIND_DATA 
//						*)&stuWFD,    
//						SLGP_SHORTPATH );  
//
//					if (!SUCCEEDED(hres))  
//						HandleErr(hres); //application-defined function    
//
//					// Get the description of the target.    
//					hres = psl->lpVtbl->GetDescription(psl,   
//
//						szDescription, MAX_PATH);    
//					if (!SUCCEEDED(hres))    
//						HandleErr(hres);    
//					lstrcpy(lpszPath, szGotPath);    
//				}    
//			}    
//			// Release the pointer to the IPersistFile interface.    
//			ppf->lpVtbl->Release(ppf);    
//		}    
//		// Release the pointer to the IShellLink interface.    
//		psl->lpVtbl->Release(psl);    
//	}    
//	return hres;    
//}
////3.修改Link 图标
//UINT __stdcall ChangeDeskTopIconLink(MSIHANDLE hInstall)
//{
//#ifdef RA2HP_Dubeg
//	System::Windows::Forms::MessageBox::Show("ChangeDeskTopIconLink");
//#endif
//	HRESULT hres;
//	IShellLink *psl = NULL;
//	IPersistFile *pPf = NULL;
//	WCHAR wsz[MAX_PATH];
//	TCHAR buf[MAX_PATH];
//	TCHAR szPath[MAX_PATH];
//	TCHAR allusers[MAX_PATH];
//	int id;
//	LPITEMIDLIST pidl;
//	::CoInitialize(NULL);
//	hres = CoCreateInstance(CLSID_ShellLink,
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		IID_IShellLink,
//		(LPVOID*)&psl);
//	if(FAILED(hres))
//		goto cleanup;
//	hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&pPf);
//	if(FAILED(hres))
//		goto cleanup;
//
//	memcpy(allusers, OSVersion::getPath_UsersPublic(), MAX_PATH);
//	//GetEnvironmentVariable(TEXT("USERPROFILE"),allusers,MAX_PATH);
//	_stprintf_s(wsz,TEXT("%s%s"),allusers,TEXT("\\Desktop\\Remote Access to HP 
//		Network 5.0.lnk"));
//		TCHAR tmp[MAX_PATH];
//	TCHAR filename[MAX_PATH]; 
//	TCHAR* pBuffer = NULL;
//	//memset(sProgramfiles, 0, MAX_PATH);
//	size_t len;
//	errno_t err = _wdupenv_s(&pBuffer,&len,L"programfiles"); // find the root
//	if(err) return 1;
//	_stprintf_s(filename,TEXT("%s%s"),pBuffer,TEXT("\\RA2HP\\RA2HP.ico"));
//	hres = pPf->Load(wsz, STGM_READ);
//	if(FAILED(hres))
//		goto cleanup;
//	hres = psl->Resolve(NULL, SLR_UPDATE);
//	if(FAILED(hres))
//		goto cleanup;
//	hres = psl->SetIconLocation(filename, 0);
//	if(FAILED(hres))
//		goto cleanup;
//
//	pPf->Save(wsz, TRUE);
//	//change icon in starmenu
//	ITEMIDLIST* pidlm = NULL;
//	SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_PROGRAMS, &pidlm);
//	TCHAR bufm[MAX_PATH];
//	SHGetPathFromIDList(pidlm, bufm);
//	lstrcat(bufm,TEXT("\\"));
//	lstrcat(bufm,TEXT("Remote Access to HP Network 5.0.lnk"));
//	WCHAR   wszm[MAX_PATH];    
//	wmemcpy(wszm,bufm,MAX_PATH);
//	hres = pPf->Load(wszm, STGM_READ);
//	if(FAILED(hres))
//		goto cleanup;
//	hres = psl->Resolve(NULL, SLR_UPDATE);
//	if(FAILED(hres))
//		goto cleanup;
//	hres = psl->SetIconLocation(filename, 0);
//	if(FAILED(hres))
//		goto cleanup;
//	pPf->Save(wszm, TRUE);
//cleanup:
//	if(pPf)
//		pPf->Release();
//	if(psl)
//		psl->Release();
//	::CoUninitialize();
//	return 0;
//}
