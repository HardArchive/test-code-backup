#pragma once
#include <shobjidl.h>
#include <atlbase.h>
#include <atlconv.h>

class CShortcuts
{
public:

	CShortcuts(void)
	{
	}

	~CShortcuts(void)
	{
	}

	//VC操作Windows快捷方式
	//主要用到的是COM组件。IShellLink和IPersistFile
	//需要添加的头函数
	//shobjidl.h
	//IPersistFile主要用到两个成员函数：
	//1、Save。保存内容到文件中去
	//2、Load。读取
	//Load的函数原型
	HRESULT Load(
		LPCWSTR pszFileName, //快捷方式的文件名，应该是ANSI字符
		DWORD dwMode //读取方式
		);
	{
		//dwMode可取如下值：
		//STGM_READ：只读 
		//STGM_WRITE：只写 
		//STGM_READWRITE：读写
		//
		//1.新建
		/*注：在桌面上创建快捷方式，快捷方式的名称为"VC创建的快捷方式",快捷方式指向应用程序"c:\aa.exe"
		快捷方式的快捷键：无
		快捷方式的工作目录：c:\
		快捷方式的备注：VC写写捷方式
		快捷方式的运行方式：常规窗口*/
		HRESULT hRet;
		IShellLink *pLink;   //IShellLink对象指针
		IPersistFile * ppf; //IPersisFil对象指针
		WCHAR wsz[MAX_PATH];   //定义Unicode字符串
		//初始化COM库
		hRet = ::CoInitialize(NULL);
		if ( hRet != S_OK)   //初始化COM库失败，直接返回
		{
			AfxMessageBox("初始化COM库失败");
			return;
		}
		//创建IShellLink实例
		hRet = 
			::CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(void**)&pLink);
		if ( hRet != S_ok)
		{
			AfxMessageBox("创建IShellLink实例失败");
		}
		else
		{
			//创建IShellLink实例成功，继续操作

			//从IShellLink对象中获取IPersistFile接口
			hRet = pLink->QueryInterface(IID_IpersistFile, (void**)&ppf);
			if ( hRet != S_OK)
			{
				AfxMessageBox("获取IPersistFile接口失败");
			}
			else
			{
				//获取接口成功，继续操作
				//设置快捷方式中的程序路径
				pLink->SetPath("c:\\aa.exe");
				//设置快捷方式的工作目录
				pLink->SetWorkingDirectory("c:\\");
				//确保快捷方式路径由ANSI字符串组成
				MultiByteToWideChar(
					CP_ACP,
					0,
					"C:\\Documents and Settings\\hjs\\桌面[url=file://\\vc]\\vc[/url]创建的快捷方式.lnk",
					-1,
					wsz,
					MAX_PATH
					);
				//保存快捷方式
				ppf->Save(wsz,TRUE);
			}
		}
		//释放IPersistFile接口
		ppf->Release();
		//释放IShellLink对象
		pLink->Release();
		//释放COM接口
		::CoUninitialize();
	}

	//创建快捷方式       
	BOOL CreateLink (       
		LPSTR szPath,//快捷方式的目标应用程序名       
		LPSTR szLink)//快捷方式的数据文件名(*.lnk)       
	{       
		HRESULT hres ;       
		IShellLink * psl ;       
		IPersistFile* ppf ;       
		WORD wsz[ MAX_PATH]    ;
		//创建一个IShellLink实例       
		hres = CoCreateInstance( CLSID_ShellLink, NULL,       
			CLSCTX_INPROC_SERVER, IID_IShellLink,       
			(void **)&psl) ;       
		if( FAILED( hres))       
			return FALSE ;       
		//设置目标应用程序       
		psl -> SetPath( szPath) ;       
		//设置快捷键(此处设为Shift+Ctrl+'R')       
		psl -> SetHotkey( MAKEWORD( 'R',       
			HOTKEYF_SHIFT &brvbar;HOTKEYF_CONTROL)) ;       
		//从IShellLink获取其IPersistFile接口       
		//用于保存快捷方式的数据文件 (*.lnk)       
		hres = psl -> QueryInterface( IID_IPersistFile,       
			(void**)&ppf) ;       

		if( FAILED( hres))       
			return FALSE ; 

		// 确保数据文件名为ANSI格式       
		MultiByteToWideChar( CP_ACP, 0, szLink, -1,     wsz, MAX_PATH) ; 

		//调用IPersistFile::Save       
		//保存快捷方式的数据文件 (*.lnk)       
		hres = ppf -> Save( wsz, STGM_READWRITE) ;   

		//释放IPersistFile和IShellLink接口       
		ppf -> Release( ) ;       
		psl -> Release( ) ;       
		return TRUE;       
	}

	//2.解析快捷方式：
	HRESULT ResolveIt(HWND hwnd, LPCSTR lpszLinkFile, LPSTR lpszPath)    
	{    
		HRESULT hres;    
		IShellLink* psl;    
		char szGotPath[MAX_PATH];    
		char szDescription[MAX_PATH];    
		WIN32_FIND_DATA wfd;    

		*lpszPath = 0; // assume failure    

		// Get a pointer to the IShellLink interface.    
		hres = CoCreateInstance(&CLSID_ShellLink, NULL,    
			CLSCTX_INPROC_SERVER, &IID_IShellLink, &psl);    
		if (SUCCEEDED(hres)) {    
			IPersistFile* ppf;    

			// Get a pointer to the IPersistFile interface.    
			hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile,    
				&ppf);    
			if (SUCCEEDED(hres)) {    
				WORD wsz[MAX_PATH];    

				// Ensure that the string is Unicode.    
				MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz,   

					MAX_PATH);    

				// Load the shortcut.    
				hres = ppf->lpVtbl->Load(ppf, wsz, STGM_READ);    
				if (SUCCEEDED(hres)) {    

					// Resolve the link.    
					hres = psl->lpVtbl->Resolve(psl, hwnd, 
						SLR_ANY_MATCH);    
					if (SUCCEEDED(hres)) {    

						// Get the path to the link target.    
						hres = psl->lpVtbl->GetPath(psl, 
							szGotPath,    
							MAX_PATH, (WIN32_FIND_DATA 
							*)&wfd,    
							SLGP_SHORTPATH );    
						if (!SUCCEEDED(hres)    
							HandleErr(hres); // 
						application-defined function    

							// Get the description of the target.    
							hres = psl->lpVtbl->GetDescription(psl,   

							szDescription, MAX_PATH);    
						if (!SUCCEEDED(hres))    
							HandleErr(hres);    
						lstrcpy(lpszPath, szGotPath);    
					}    
				}    
				// Release the pointer to the IPersistFile interface.    
				ppf->lpVtbl->Release(ppf);    
			}    
			// Release the pointer to the IShellLink interface.    
			psl->lpVtbl->Release(psl);    
		}    
		return hres;    
	}

	//3.修改Link 图标
	UINT __stdcall ChangeDeskTopIconLink(MSIHANDLE hInstall)
	{
#ifdef RA2HP_Dubeg
		System::Windows::Forms::MessageBox::Show("ChangeDeskTopIconLink");
#endif
		HRESULT hres;
		IShellLink *psl = NULL;
		IPersistFile *pPf = NULL;
		WCHAR wsz[MAX_PATH];
		TCHAR buf[MAX_PATH];
		TCHAR szPath[MAX_PATH];
		TCHAR allusers[MAX_PATH];
		int id;
		LPITEMIDLIST pidl;
		::CoInitialize(NULL);
		hres = CoCreateInstance(CLSID_ShellLink,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IShellLink,
			(LPVOID*)&psl);
		if(FAILED(hres))
			goto cleanup;
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&pPf);
		if(FAILED(hres))
			goto cleanup;

		memcpy(allusers, OSVersion::getPath_UsersPublic(), MAX_PATH);
		//GetEnvironmentVariable(TEXT("USERPROFILE"),allusers,MAX_PATH);
		_stprintf_s(wsz,TEXT("%s%s"),allusers,TEXT("\\Desktop\\Remote Access to HP 
			Network 5.0.lnk"));
			TCHAR tmp[MAX_PATH];
		TCHAR filename[MAX_PATH]; 
		TCHAR* pBuffer = NULL;
		//memset(sProgramfiles, 0, MAX_PATH);
		size_t len;
		errno_t err = _wdupenv_s(&pBuffer,&len,L"programfiles"); // find the root
		if(err) return 1;
		_stprintf_s(filename,TEXT("%s%s"),pBuffer,TEXT("\\RA2HP\\RA2HP.ico"));
		hres = pPf->Load(wsz, STGM_READ);
		if(FAILED(hres))
			goto cleanup;
		hres = psl->Resolve(NULL, SLR_UPDATE);
		if(FAILED(hres))
			goto cleanup;
		hres = psl->SetIconLocation(filename, 0);
		if(FAILED(hres))
			goto cleanup;

		pPf->Save(wsz, TRUE);
		//change icon in starmenu
		ITEMIDLIST* pidlm = NULL;
		SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_PROGRAMS, &pidlm);
		TCHAR bufm[MAX_PATH];
		SHGetPathFromIDList(pidlm, bufm);
		lstrcat(bufm,TEXT("\\"));
		lstrcat(bufm,TEXT("Remote Access to HP Network 5.0.lnk"));
		WCHAR   wszm[MAX_PATH];    
		wmemcpy(wszm,bufm,MAX_PATH);
		hres = pPf->Load(wszm, STGM_READ);
		if(FAILED(hres))
			goto cleanup;
		hres = psl->Resolve(NULL, SLR_UPDATE);
		if(FAILED(hres))
			goto cleanup;
		hres = psl->SetIconLocation(filename, 0);
		if(FAILED(hres))
			goto cleanup;
		pPf->Save(wszm, TRUE);
cleanup:
		if(pPf)
			pPf->Release();
		if(psl)
			psl->Release();
		::CoUninitialize();
		return 0;
	}
};
