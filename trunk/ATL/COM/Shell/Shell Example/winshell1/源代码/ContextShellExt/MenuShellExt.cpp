// MenuShellExt.cpp : Implementation of CMenuShellExt
#include "stdafx.h"
#include "ContextShellExt.h"
#include "MenuShellExt.h"


/////////////////////////////////////////////////////////////////////////////
// CMenuShellExt

HRESULT CMenuShellExt::Initialize(LPCITEMIDLIST pidlFolder,
								  LPDATAOBJECT lpdobj, 
								  HKEY hkeyProgID)
{		
	if(NULL==lpdobj){
		return E_INVALIDARG;
	}
	FORMATETC feFmtEtc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM mdmSTG = { TYMED_HGLOBAL };
				
	if(FAILED(lpdobj->GetData(&feFmtEtc, &mdmSTG)))
	{
		return E_INVALIDARG;
	}
	HDROP hDrop = (HDROP)GlobalLock(mdmSTG.hGlobal);
    if(NULL==hDrop)
	{
        return E_INVALIDARG;
	}
	//�õ��û��������ļ���
	int nDropCount = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	TCHAR szFile[MAX_PATH];
	//���λ�ò������ļ���
	for ( int i = 0; i < nDropCount; i++ )
	{
		if ( DragQueryFile ( hDrop, i, szFile, MAX_PATH )==0)
		{			
			continue;
		}
		else
		{	
			m_FileName.push_back(szFile);
		}
	}
	GlobalUnlock(hDrop);
	::ReleaseStgMedium(&mdmSTG);
	return S_OK;
}

HRESULT CMenuShellExt::QueryContextMenu(HMENU hmenu, 
										UINT indexMenu,
										UINT idCmdFirst, 
										UINT idCmdLast, 
										UINT uFlags)
{
	if(!(uFlags & CMF_DEFAULTONLY))
	{		
		HBITMAP hbitmap=::LoadBitmap(_Module.m_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
		HBITMAP hbitmap1=::LoadBitmap(_Module.m_hInst,MAKEINTRESOURCE(IDB_BITMAP2));
		HBITMAP hbitmap2=::LoadBitmap(_Module.m_hInst,MAKEINTRESOURCE(IDB_BITMAP3));
		HBITMAP hbitmap3=::LoadBitmap(_Module.m_hInst,MAKEINTRESOURCE(IDB_BITMAP4));
		HBITMAP hbitmap4=::LoadBitmap(_Module.m_hInst,MAKEINTRESOURCE(IDB_BITMAP5));
		///////////
		//���ɲ˵�
		HMENU hmenu1=::CreateMenu();
		::InsertMenu(hmenu1, 0, MF_BYPOSITION, 
			idCmdFirst, _T("���͵���������"));
		::InsertMenu(hmenu1, 1, MF_BYPOSITION, 
			idCmdFirst+1, _T("������������ɾ���ó����ݷ�ʽ"));
		::InsertMenu(hmenu1, 2, MF_BYPOSITION, 
			idCmdFirst+2, _T("�ļ�����"));
		::InsertMenu(hmenu1, 3, MF_BYPOSITION, 
			idCmdFirst+3, _T("����"));
		//Ϊ�˵�����ͼ��
		::SetMenuItemBitmaps(hmenu1,0,MF_BYPOSITION,hbitmap1,hbitmap1);
		::SetMenuItemBitmaps(hmenu1,1,MF_BYPOSITION,hbitmap2,hbitmap2);
		::SetMenuItemBitmaps(hmenu1,2,MF_BYPOSITION,hbitmap3,hbitmap3);
		::SetMenuItemBitmaps(hmenu1,3,MF_BYPOSITION,hbitmap4,hbitmap4);
		
		//////////////////
		//�����ɵĲ˵����뵽shell�����Ĳ˵���
		::InsertMenu(hmenu, indexMenu, MF_POPUP|MF_BYPOSITION, 
			(UINT_PTR)hmenu1, _T("��������"));
		::SetMenuItemBitmaps(hmenu,indexMenu,MF_BYPOSITION,hbitmap,hbitmap);
		//���ز���Ĳ˵�����һ��Ҫ������ȷ���������ɵĸ��˵�ҲҪ��������
		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(4));
	}
	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
}

HRESULT CMenuShellExt::GetCommandString(
							UINT        idCmd,
							UINT        uType,
							UINT        *pwReserved,
							LPSTR       pszName,
							UINT        cchMax)
{
	USES_CONVERSION;	
	HRESULT hr = E_INVALIDARG;

	LPCTSTR  strTipSend = _T("���ý��ó����͵��������������");
	LPCTSTR strTipDel = _T("������������ɾ���ó���Ŀ�ݷ�ʽ");
	LPCTSTR strTipCrpt = _T("���ܸó���");
	LPCTSTR strTipHlp = _T("��ȡ����");
	
	if(uType & GCS_HELPTEXT)
	{
		switch(idCmd){
	
		case 0:
			{				
				if(uType & GCS_UNICODE)
				{
					lstrcpynW((LPWSTR)pszName, T2CW(strTipSend), cchMax);
				}
				else
				{
					lstrcpynA (pszName, T2CA(strTipSend), cchMax );
				 }
				
				hr = S_OK;
			}
			break;
			
		case 1:
			{
				if(uType & GCS_UNICODE)
				{
					lstrcpynW((LPWSTR)pszName, T2CW(strTipDel), cchMax);
				}
				else
				{
					lstrcpynA (pszName, T2CA(strTipDel), cchMax );
				}
				hr = S_OK;
			}
			break;
		
		case 2:
			{
				if(uType & GCS_UNICODE)
				{
					lstrcpynW((LPWSTR)pszName, T2CW(strTipCrpt), cchMax);
				}
				else
				{
					lstrcpynA (pszName, T2CA(strTipCrpt), cchMax );
				}
				hr = S_OK;
			}
			break;
			
		case 3:
			{
				if(uType & GCS_UNICODE)
				{
					lstrcpynW((LPWSTR)pszName, T2CW(strTipHlp), cchMax);
				}
				else
				{
					lstrcpynA (pszName, T2CA(strTipHlp), cchMax );
				}
				hr = S_OK;
			}
			break;
		}
			
	}
	return hr;
}

HRESULT CMenuShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
{

	int nFlag = HIWORD(lpici->lpVerb);
	//�õ��˵���������
	int nMnuId = LOWORD(lpici->lpVerb);
	if(0 != nFlag){
		return E_INVALIDARG;
	}
	
	//��Ӧ�˵�����
	switch(nMnuId){
		
	case 0:
		{
			//������ļ���д�뵽ע�����
			CRegKey reg;
			LONG    lRet;			
			lRet = reg.Open ( HKEY_LOCAL_MACHINE,
				_T("Software"),
				KEY_SET_VALUE );			
			if ( ERROR_SUCCESS != lRet )
				return S_FALSE;
			lRet=reg.Create(HKEY_LOCAL_MACHINE,_T("Software\\VirtualDesk"));
			if ( ERROR_SUCCESS != lRet )
				return S_FALSE;
			for (int i=0;i<m_FileName.size();i++)
			{
				lRet = reg.SetValue ( m_FileName[i].c_str(), m_FileName[i].c_str());
			}
			return S_OK;
		}		
		break;
		
	case 1:
		{
			//��ע�����ɾ���û���ѡ���ļ�
			CRegKey reg;
			LONG    lRet;
			lRet = reg.Open ( HKEY_LOCAL_MACHINE,
				_T("Software\\VirtualDesk"),
				KEY_SET_VALUE );
			
			if ( ERROR_SUCCESS != lRet )
				return S_FALSE;
			for (int i=0;i<m_FileName.size();i++)
			{
				reg.DeleteValue(m_FileName[i].c_str());
			}
			return S_OK;
		}
		break;
	case 2:
		{
			//û��ʵ����Ӧ�Ĺ��ܣ��ڴ�ʡ��
			::MessageBox(lpici->hwnd,_T("����"),_T("��������"), MB_ICONINFORMATION);
			return S_OK;			
		}
		break;
	case 3:
		{
			//û��ʵ����Ӧ�Ĺ��ܣ��ڴ�ʡ��
			::MessageBox(lpici->hwnd,_T("����"),_T("��������"), MB_ICONINFORMATION);
			return S_OK;			
		}
		break;
	}
	return E_INVALIDARG;
}