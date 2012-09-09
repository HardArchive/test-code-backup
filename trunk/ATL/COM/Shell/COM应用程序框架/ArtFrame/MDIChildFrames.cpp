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
 *	���� - ����MDI�Ӵ��ڿ�ܣ����� lpcw��ͼ���뵽�Ӵ��ڿ���С�
 *	lpcw - ָ����ͼ,��������ΪNULL
 *	lpcf - ���� MDI�Ӵ��ڿ�ܶ���
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
		//д�������Ϣ
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
