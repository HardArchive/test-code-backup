// MDIChildFrames.cpp : Implementation of CMDIChildFrames

#include "stdafx.h"
#include "MDIChildFrames.h"
#include ".\mdichildframes.h"
#include "MDIFrame.h"
#include "MDIChildFrame.h"

// CMDIChildFrames

STDMETHODIMP CMDIChildFrames::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMDIChildFrames
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


/************************************************************************
 *	
 *	功能 - 建立MDI子窗口框架，并把 lpcw视图加入到子窗口框架中。
 *	lpcw - 指定视图,可以设置为NULL
 *	lpcf - 返回 MDI子窗口框架对象
 *
 ************************************************************************/
STDMETHODIMP CMDIChildFrames::CreateChildFrame(IDataView* lpdv, IMDIChildFrame** lpcf)
{
	ATLASSERT(NULL != g_MDIFrame->m_hWnd);
	ATLASSERT(NULL != g_MDIFrame->m_hWnd);
	IMDIChildFrame *cf = 0;
	HRESULT hr = 0;

	hr = CoCreateInstance(CLSID_MDIChildFrame,
		NULL,
		CLSCTX_ALL,
		IID_IMDIChildFrame,
		(void**)&cf);

	if (FAILED(hr))
	{
		return S_FALSE;
	} 

	CMDIChildFrame *mdicf = (CMDIChildFrame*)cf;

	if (NULL == mdicf->CreateEx(g_MDIFrame->m_hWndMDIClient))
	{
		//写入错误信息
		//...

		ATLASSERT(0);
		return S_FALSE;
	}
	//	mdicf->m_hMenu = g_MDIFrame->m_hMenu;

	m_coll.push_back(cf);
	if (NULL != lpdv)
	{
		//lpdv->CreateWnd(NULL);
		cf->PushDataView(lpdv);
	}

	if (NULL != lpcf)
	{
		*lpcf = cf;
	}

	return S_OK;
}
