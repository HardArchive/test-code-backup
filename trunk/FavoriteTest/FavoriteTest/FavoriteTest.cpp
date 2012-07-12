// FavoriteTest.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
CQueueEx<INKINFO>* g_pInkInfoQue;   //��ַ��ݷ�ʽ����

//��ȡ�ղؼ�·��
BOOL GetFavoritesPath(WCHAR* pwszFavoritespath)
{
	if (!pwszFavoritespath)
	{
		return FALSE;
	}
	
//����1
	if (!SHGetSpecialFolderPath(NULL, pwszFavoritespath, CSIDL_FAVORITES, FALSE))
	{	//�õ�Ŀ¼��
		return FALSE;
	}
//����2
	//HKEY hKEY;
	//DWORD type=REG_SZ;
	//LPBYTE favpath = new BYTE[80];   //·������λ��
	//LPCTSTR path="Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders";
	//DWORD cbData=80;
	//::RegOpenKeyEx(HKEY_CURRENT_USER,path,0,KEY_READ,&hKEY);
	//::RegQueryValueEx(hKEY,"Favorites",NULL,&type,favpath,&cbData);
	//::RegCloseKey(hKEY);
	return TRUE;
}

/******************************************************************************* 
1�� �������ƣ� BOOL GetFileInfo(PWCHAR pwszPath)
2�� ���������� ��ȡ�����ҳ��ݷ�ʽ��Ϣ
3�� ��������� PWCHAR pwszPath ��ݷ�ʽ����·��
4�� �� �� ֵ�� �ɹ�����TRUE��ʧ�ܷ���FALSE
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-11-25 11:24:41
8�� ��    ע�� 
********************************************************************************/ 
BOOL GetFileInfo(PWCHAR pwszPath, PINKINFO pstuInkInfo)
{
	char *ptr = NULL;
	char szTem[MAX_PATH] = {0};
	

	if (!pwszPath && pstuInkInfo)
	{
		return FALSE;
	}

	//���ﻹӦ���ж��ļ���׺�Ƿ�Ϊurl

	GetPrivateProfileString("InternetShortcut", "URL", NULL, pstuInkInfo->szUrl, 512, pwszPath);//��ȡ��ַ
	wcscpy(pstuInkInfo->wszPath, pwszPath);      //��������·��
	

	return TRUE;
}

/******************************************************************************* 
1�� �������ƣ� int GetInkFileInfo(LPCTSTR pwszDirPath)
2�� ���������� �õ�ָ��Ŀ¼�еĿ�ݷ�ʽ��Ϣ 
3�� ��������� LPCTSTR pwszDirPath  ָ����Ŀ¼ָ�룬����ָ�ղؼ�ָ��
4�� �� �� ֵ�� -1 �������ָ��Ϊ��
			  -2 δ�ҵ��ļ�
5�� ��̬�ڴ棺 
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-11-25 11:00:27
8�� ��    ע�� 
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

	swprintf_s(wszFindFilter, L"%s\\*.*", pwszDirPath);    //�����ļ�����

	if ((hFind = FindFirstFile(wszFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
	{
		return -2;
	}

	do
	{
		//���˱���Ŀ¼�͸�Ŀ¼
		/*if (_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0 )
		continue;*/
		if(stuWFD.cFileName[0]=='.')
			continue;

		//ȥ��ֻ������
		dwAttributes = GetFileAttributes(wszFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(wszFullPath, dwAttributes);
		}

		//����ҵ�����Ŀ¼��������Ŀ¼���еݹ�
		if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			ZeroMemory(wszFullPath, sizeof(wszFullPath));
			swprintf_s(wszFullPath, L"%s\\%s", pwszDirPath, stuWFD.cFileName);  //�õ��ļ�������·��
			iRet = GetInkFileInfo(wszFullPath);
			if (1 != iRet)
			{
				return iRet;
			}
		}
		else 
		{ 
			//�ҵ������ļ�
			swprintf_s(wszFullPath, L"%s\\%s", pwszDirPath, stuWFD.cFileName); //�õ��ļ�����·��
			wcscpy(stuInkInfo.wszUrlName, stuWFD.cFileName);         //��ȡ�ļ���
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
////3.�޸�Link ͼ��
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
