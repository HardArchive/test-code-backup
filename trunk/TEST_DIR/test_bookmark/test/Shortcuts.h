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

	//VC����Windows��ݷ�ʽ
	//��Ҫ�õ�����COM�����IShellLink��IPersistFile
	//��Ҫ��ӵ�ͷ����
	//shobjidl.h
	//IPersistFile��Ҫ�õ�������Ա������
	//1��Save���������ݵ��ļ���ȥ
	//2��Load����ȡ
	//Load�ĺ���ԭ��
	HRESULT Load(
		LPCWSTR pszFileName, //��ݷ�ʽ���ļ�����Ӧ����ANSI�ַ�
		DWORD dwMode //��ȡ��ʽ
		);
	{
		//dwMode��ȡ����ֵ��
		//STGM_READ��ֻ�� 
		//STGM_WRITE��ֻд 
		//STGM_READWRITE����д
		//
		//1.�½�
		/*ע���������ϴ�����ݷ�ʽ����ݷ�ʽ������Ϊ"VC�����Ŀ�ݷ�ʽ",��ݷ�ʽָ��Ӧ�ó���"c:\aa.exe"
		��ݷ�ʽ�Ŀ�ݼ�����
		��ݷ�ʽ�Ĺ���Ŀ¼��c:\
		��ݷ�ʽ�ı�ע��VCдд�ݷ�ʽ
		��ݷ�ʽ�����з�ʽ�����洰��*/
		HRESULT hRet;
		IShellLink *pLink;   //IShellLink����ָ��
		IPersistFile * ppf; //IPersisFil����ָ��
		WCHAR wsz[MAX_PATH];   //����Unicode�ַ���
		//��ʼ��COM��
		hRet = ::CoInitialize(NULL);
		if ( hRet != S_OK)   //��ʼ��COM��ʧ�ܣ�ֱ�ӷ���
		{
			AfxMessageBox("��ʼ��COM��ʧ��");
			return;
		}
		//����IShellLinkʵ��
		hRet = 
			::CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(void**)&pLink);
		if ( hRet != S_ok)
		{
			AfxMessageBox("����IShellLinkʵ��ʧ��");
		}
		else
		{
			//����IShellLinkʵ���ɹ�����������

			//��IShellLink�����л�ȡIPersistFile�ӿ�
			hRet = pLink->QueryInterface(IID_IpersistFile, (void**)&ppf);
			if ( hRet != S_OK)
			{
				AfxMessageBox("��ȡIPersistFile�ӿ�ʧ��");
			}
			else
			{
				//��ȡ�ӿڳɹ�����������
				//���ÿ�ݷ�ʽ�еĳ���·��
				pLink->SetPath("c:\\aa.exe");
				//���ÿ�ݷ�ʽ�Ĺ���Ŀ¼
				pLink->SetWorkingDirectory("c:\\");
				//ȷ����ݷ�ʽ·����ANSI�ַ������
				MultiByteToWideChar(
					CP_ACP,
					0,
					"C:\\Documents and Settings\\hjs\\����[url=file://\\vc]\\vc[/url]�����Ŀ�ݷ�ʽ.lnk",
					-1,
					wsz,
					MAX_PATH
					);
				//�����ݷ�ʽ
				ppf->Save(wsz,TRUE);
			}
		}
		//�ͷ�IPersistFile�ӿ�
		ppf->Release();
		//�ͷ�IShellLink����
		pLink->Release();
		//�ͷ�COM�ӿ�
		::CoUninitialize();
	}

	//������ݷ�ʽ       
	BOOL CreateLink (       
		LPSTR szPath,//��ݷ�ʽ��Ŀ��Ӧ�ó�����       
		LPSTR szLink)//��ݷ�ʽ�������ļ���(*.lnk)       
	{       
		HRESULT hres ;       
		IShellLink * psl ;       
		IPersistFile* ppf ;       
		WORD wsz[ MAX_PATH]    ;
		//����һ��IShellLinkʵ��       
		hres = CoCreateInstance( CLSID_ShellLink, NULL,       
			CLSCTX_INPROC_SERVER, IID_IShellLink,       
			(void **)&psl) ;       
		if( FAILED( hres))       
			return FALSE ;       
		//����Ŀ��Ӧ�ó���       
		psl -> SetPath( szPath) ;       
		//���ÿ�ݼ�(�˴���ΪShift+Ctrl+'R')       
		psl -> SetHotkey( MAKEWORD( 'R',       
			HOTKEYF_SHIFT &brvbar;HOTKEYF_CONTROL)) ;       
		//��IShellLink��ȡ��IPersistFile�ӿ�       
		//���ڱ����ݷ�ʽ�������ļ� (*.lnk)       
		hres = psl -> QueryInterface( IID_IPersistFile,       
			(void**)&ppf) ;       

		if( FAILED( hres))       
			return FALSE ; 

		// ȷ�������ļ���ΪANSI��ʽ       
		MultiByteToWideChar( CP_ACP, 0, szLink, -1,     wsz, MAX_PATH) ; 

		//����IPersistFile::Save       
		//�����ݷ�ʽ�������ļ� (*.lnk)       
		hres = ppf -> Save( wsz, STGM_READWRITE) ;   

		//�ͷ�IPersistFile��IShellLink�ӿ�       
		ppf -> Release( ) ;       
		psl -> Release( ) ;       
		return TRUE;       
	}

	//2.������ݷ�ʽ��
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

	//3.�޸�Link ͼ��
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
