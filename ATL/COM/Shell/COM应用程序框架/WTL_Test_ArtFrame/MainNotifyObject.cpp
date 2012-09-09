
#include "StdAfx.h"
#include "MainNotifyObject.h"
#include "TextView.h"
#include "resource.h"

namespace 
{

	_ATL_FUNC_INFO OnShowInfo0	=
	{
		CC_STDCALL,	
			VT_EMPTY,
			0 ,
			NULL
	};

	_ATL_FUNC_INFO OnShowInfo1	=
	{
		CC_STDCALL,
			VT_EMPTY,
			1 ,
		{VT_BOOL|VT_BYREF}
	};

	_ATL_FUNC_INFO OnShowInfo3	=
	{
		CC_STDCALL,
			VT_EMPTY,
			3 ,
		{VT_I4, VT_I4, VT_BOOL|VT_BYREF}
	};

}

CMainNotifyObject::CMainNotifyObject(void):
	m_lpMdiFrame(NULL)
{
}

CMainNotifyObject::~CMainNotifyObject(void)
{
}


//建立窗口和视图。
HRESULT CMainNotifyObject::CreateMain()
{
	CreateMDIStruct lpMDI={0};
	HRESULT hr = 0;

	hr = CoCreateInstance(CLSID_MDIFrame, NULL, CLSCTX_ALL, IID_IMDIFrame, (VOID**)&m_lpMdiFrame);
	if (FAILED(hr))
	{
		ATLASSERT(0);
		return hr;
	}

	lpMDI.cbSize = sizeof(CreateMDIStruct);
	lpMDI.lParam = NULL;
	lpMDI.lpszWindowName = L"Test COM MDIFrame";
	lpMDI.hMenu = LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));

	hr = m_lpMdiFrame->CreateWnd(&lpMDI);
	m_lpMdiFrame->ShowMe(VARIANT_TRUE);


	AtlGetObjectSourceInterface(m_lpMdiFrame,&m_libid, &m_iid, &m_wMajorVerNum, &m_wMinorVerNum);
	hr = this->DispEventAdvise((IUnknown*)m_lpMdiFrame);

	if (FAILED(hr))
	{
		MessageBox(0, _T("建立 MDIFrameEvent 失败。"), _T("ArtFrame"), MB_OK);
	}

	return hr;
}



//////////////////////////////////////////////////////////////////////////
//以下是



void CMainNotifyObject::WinMain()
{
	HRESULT hr = CreateMain();
	if (SUCCEEDED(hr))
	{
		m_lpMdiFrame->Run();
	}

}


void CMainNotifyObject::OnFileExit(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_lpMdiFrame->Quit();
}


void CMainNotifyObject::OnAbout(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_lpMdiFrame->AboutCOMFrame();

}


//新建立一个文档
void CMainNotifyObject::FileNew(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	IMDIChildFrames  *lpFrames = NULL;
	IMDIChildFrame   *lpChildFrame = NULL;
	HRESULT hr = 0;
	hr = m_lpMdiFrame->get_MDIChildFrames((IDispatch**)&lpFrames);

	if (FAILED(hr))
	{
		ATLASSERT(0);
	}

	CTextView *lpTexView = NULL;
	lpTexView = new CTextView();
	lpFrames->CreateChildFrame((IDataView*)lpTexView, &lpChildFrame);

	lpChildFrame->put_Title(L"WTL TextView");
}


void CMainNotifyObject::OnWindows(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_lpMdiFrame->MDIWindows();
}