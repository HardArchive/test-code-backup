#pragma once
//http://pyhcx.blog.51cto.com/713166/192454
//让窗口支持拖放(文件，文本，URL...) 
class IDropTargetEvent
{
public:
	virtual void OnDropText(LPCTSTR ptszText){}
	virtual void OnDropFile(HDROP hDropFile){}
};

class CDropTargetEx : public IDropTarget
{
public:
	CDropTargetEx(IDropTargetEvent * pTarget):m_pTarget(pTarget){}
	~CDropTargetEx(void){}
	STDMETHOD(QueryInterface)(const struct _GUID &iid,void ** ppv){ *ppv = this; return S_OK ; }
	ULONG __stdcall AddRef(void){ return 1; }
	ULONG __stdcall Release(void){ return 0; }
	STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
	{
		*pdwEffect = DROPEFFECT_LINK|DROPEFFECT_COPY ;
		return S_OK ;
	}
	STDMETHOD(DragEnter)(IDataObject * pDataObject,DWORD grfKeyState, POINTL pt,DWORD * pdwEffect)
	{
		*pdwEffect = DROPEFFECT_LINK|DROPEFFECT_COPY ;
		return S_OK ;
	}
	STDMETHOD(DragLeave)(){ return S_OK ;}
	STDMETHOD(Drop)(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect)
	{
		FORMATETC cFmt = {(CLIPFORMAT) CF_TEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
		STGMEDIUM stgMedium;
		if(SUCCEEDED(pDataObj->GetData(&cFmt, &stgMedium)))
		{
			char *pBuff = NULL;
			pBuff=(LPSTR)GlobalLock(stgMedium.hGlobal);
			GlobalUnlock(stgMedium.hGlobal);
			USES_CONVERSION;
			if(m_pTarget) m_pTarget->OnDropText(A2T(pBuff));
			::ReleaseStgMedium(&stgMedium);
		}
		FORMATETC cFmt2 = {(CLIPFORMAT) CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
		if(SUCCEEDED(pDataObj->GetData(&cFmt2, &stgMedium)))
		{
			if(m_pTarget) m_pTarget->OnDropFile((HDROP)stgMedium.hGlobal);
			::ReleaseStgMedium(&stgMedium);
		}
		return S_OK ;
	}
private:
	IDropTargetEvent *	m_pTarget ;
};

template<class T>
class IDropTargetImpl : IDropTargetEvent
{
public:
	IDropTargetImpl():m_Target(this){}
	~IDropTargetImpl(){}

	HRESULT RegisterDragDrop()
	{
		T * pThis = (T*)this ;
		return ::RegisterDragDrop(pThis->m_hWnd,&m_Target); 
	}

private:
	CDropTargetEx	m_Target ;
};