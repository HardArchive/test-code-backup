/********************************************************************
 *
 *	created:	2006/06/15
 *	created:	15:6:2006   13:24
 *	filename: 	F:\Test\ComFrame\ArtFrame\inc\ArtFrameImpl.h
 *	file path:	F:\Test\ComFrame\ArtFrame\inc
 *	file base:	ArtFrameImpl
 *	file ext:	h
 *	author:		许宗森
 *	Eamil:		ytf1978@163.com
 *	purpose:	实例化ArtFrame 库中的全部 纯虚接口类,命令处理的宏定义。
 *				MFC/WTL/ATL中都可以用。
 *
 *				2.IDatView
 *
 ********************************************************************/

#ifndef  __ArtFrameImpl__H
#define  __ArtFrameImpl__H
#include "..\inc\ArtFrame.h"


template<class _TInterface>
class IUnknownImpl : public _TInterface
{
private:
	LONG m_dwRef;
public:

	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_dwRef);
	}

	STDMETHODIMP_(ULONG) Release()
	{
		ULONG result = InterlockedDecrement(&m_dwRef);
		if (result == 0)
			delete this;
		return result;
	}

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
	{
		if (riid == IID_IUnknown)
		{
			*ppv = (_TInterface*)this;

			((IUnknown*)*ppv)->AddRef();
			return S_OK; 
		}

		*ppv = NULL;
		return S_FALSE;
	}
};


template<class _TWinClass>
class CDataViewImpl : public IDispatchImpl<IUnknownImpl<IDataView>, &IID_IDataView, &LIBID_ArtFrameLib>
{
public:
	STDMETHODIMP NotifyCommand(UINT codeNotify, UINT cmdID, BOOL *bHandle)
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE NotifyCommand(LONG codeNotify, LONG cmdID, VARIANT_BOOL *bHandle)
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE CreateWnd(HWND hWndParent)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetWnd(HWND *hWnd)
	{
		_TWinClass *pT = static_cast<_TWinClass*>(this);
		*hWnd = pT->m_hWnd;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnActiveWindow(UINT nType)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnCloseWindow(VARIANT_BOOL *vbClose)
	{
		return S_OK;
	}

protected:
private:
};



#define NC_BEGIN_MAP() \
	STDMETHODIMP NotifyCommand(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle) \
	{\
		ATLTRACE(_T("Notify command ID:%d\n"), cmdID);

#define NC_COMMAND_ID_HANDLER(cmd_id, func)  \
		if (cmd_id == cmdID) \
		{\
			ATLTRACE(_T("Notify command ID: %d - cmd_id:%d\n"), cmdID, cmd_id); \
			func(codeNotify, cmdID, bHandle);	\
			return S_OK;\
		}

#define  NC_END_MAP()	\
		*bHandle = VARIANT_TRUE;	\
		return S_OK; \
	} 

#define NC_COMMAND_TO_WND(hWndCtrl)		\
		ATLASSERT(::IsWindow(hWndCtrl));	\
		if (::SendMessage(hWndCtrl, WM_COMMAND, MAKEWPARAM(cmdID, codeNotify), 0))	\
		{	\
			return S_OK;\
		}


#endif	//end __ArtFrameImpl__H
