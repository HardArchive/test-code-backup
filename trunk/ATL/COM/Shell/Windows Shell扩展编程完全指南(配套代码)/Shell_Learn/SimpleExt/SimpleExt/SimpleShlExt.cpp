// SimpleShlExt.cpp : CSimpleShlExt ��ʵ��

#include "stdafx.h"
#include "SimpleShlExt.h"

#include "atlconv.h"
// Ϊʹ�� ATL �ַ���ת�����������ͷ�ļ� 

// CSimpleShlExt
//����Ҫ������ȡ�ñ��һ�ѡ����ļ���,�ٰѸ��ļ�����ʾ�ڵ�����Ϣ���С�
//���ܻ��ж���ļ�ͬʱ��ѡ���һ�, �������pDataObj �ӿ�ָ���ȡ���е��ļ���, ������Ϊ�����, ����ֻ��ȡ��һ���ļ���. 
//add by ray 2014-05-15 17:45
HRESULT CSimpleShlExt::Initialize ( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID )
{ 
	//�ļ����Ĵ�Ÿ�ʽ�����Ϸ��ļ�����WS_EX_ACCEPTFILES���Ĵ���ʱʹ�õ��ļ�����ʽ��һ���ġ� 
	// �����˵���ǿ���ʹ��ͬ����API����ȡ�ļ���: DragQueryFile(). 
	// ���������Ȼ�ȡ������IdataObject�е����ݾ��: 

	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL }; 
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP hDrop; 

	// �����ݶ����ڲ��� CF_HDROP ������. 
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ))) 
	{ 
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG; 
	}

	// ���ָ��ʵ�����ݵ�ָ��
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	// ����NULL. 
	if ( NULL == hDrop ) 
	{ 
		return E_INVALIDARG;
	} 

	//��ע������飬�ر���ָ��ļ�顣 
	// �������ǵ���չ������ Explorer ������, Ҫ�����ǵĴ��������, ExplorerҲ����֮����. ��Win 9x��, ������һ���������ܵ�����Ҫ����ϵͳ. 
	// ����, ������������һ�� HDROP ���, ���ǾͿ��Ի�ȡ������Ҫ���ļ����ˣ� 


	// ��Ч�Լ�� �C ��֤������һ���ļ���. 
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 ); 
	if ( 0 == uNumFiles )
	{ 
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg ); 
		return E_INVALIDARG; 
	} 
	HRESULT hr = S_OK; 

	// ȡ�õ�һ���ļ������������������Աm_szFile ��.
	if ( 0 == DragQueryFile ( hDrop, 0, m_szFile, MAX_PATH ))
	{ 
		hr = E_INVALIDARG; 
	} 
	GlobalUnlock ( stg.hGlobal ); 
	ReleaseStgMedium ( &stg ); 
	return hr; 
} 

//add by ray 2014-05-15 17:48
HRESULT CSimpleShlExt::QueryContextMenu ( HMENU hmenu,UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags )
{          
	// �����־���� CMF_DEFAULTONLY ���ǲ����κ�����. 
	if ( uFlags & CMF_DEFAULTONLY ) 
	{ 
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 ); 
	} 

	InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION, uidFirstCmd, _T("SimpleShlExt Test Item") ); 
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}



HRESULT CSimpleShlExt::GetCommandString( UINT idCmd, UINT uFlags,UINT* pwReserved, LPSTR pszName, UINT cchMax ) 
{
	USES_CONVERSION; 
	//��� idCmd, ��������0����Ϊ���ǽ���һ����ӵĲ˵���. 
	if ( 0 != idCmd ) 
		return E_INVALIDARG; 

	// ��� Explorer Ҫ������ַ������ͽ����������ṩ�Ļ�������. 
	if ( uFlags & GCS_HELPTEXT ) 
	{
		LPCTSTR szText = _T("This is the simple shell extension's help");              
		if ( uFlags & GCS_UNICODE )
		{ 
			// ������Ҫ�� pszName ת��Ϊһ�� Unicode �ַ���, ����ʹ��Unicode�ַ������� API. 
			lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax ); 
		} 
		else
		{ 
			// ʹ�� ANSI �ַ�������API �����ذ����ַ���. 
			lstrcpynA ( pszName, T2CA(szText), cchMax ); 
		} 
		return S_OK;
	} 
	return E_INVALIDARG; 
}


HRESULT CSimpleShlExt::InvokeCommand ( LPCMINVOKECOMMANDINFO pCmdInfo )
{
	// ���lpVerb ʵ��ָ��һ���ַ���, ���Դ˴ε��ò��˳�.
	if ( 0 != HIWORD( pCmdInfo->lpVerb ))
		return E_INVALIDARG;
	// ������������� �C �����Ψһ�Ϸ�������Ϊ0.
	switch ( LOWORD( pCmdInfo->lpVerb ))
	{
	case 0:
		{
			TCHAR szMsg [MAX_PATH + 32];
			wsprintf ( szMsg, _T("The selected file was:\n\n%s"), m_szFile );
			MessageBox ( pCmdInfo->hwnd, szMsg, _T("SimpleShlExt"),
				MB_ICONINFORMATION );
			return S_OK;
		}
		break;
	default:
		return E_INVALIDARG;
		break;
	}
}
