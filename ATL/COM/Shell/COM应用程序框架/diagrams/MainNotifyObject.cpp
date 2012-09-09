
#include "StdAfx.h"
#include "MainNotifyObject.h"
#include "TextView.h"
#include "resource.h"


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
	
	lpMDI.hMenu = LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	hr = m_lpMdiFrame->CreateWnd(&lpMDI);
	m_lpMdiFrame->ShowMe(VARIANT_TRUE);
	m_lpMdiFrame->put_NotifyObject((INotifyObject*)this);

	FileNew(NULL);

	return hr;
}



//////////////////////////////////////////////////////////////////////////
//以下是

void CMainNotifyObject::FileNew(LPNotifyCmdStruct lpnc)
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
}



void CMainNotifyObject::WinMain()
{
	//CoInitialize(NULL);
	HRESULT hr = CreateMain();
	if (SUCCEEDED(hr))
	{
		m_lpMdiFrame->Run();
	}

}


void CMainNotifyObject::OnFileExit(LPNotifyCmdStruct lpnc)
{
	m_lpMdiFrame->Quit();
}


void CMainNotifyObject::OnAbout(LPNotifyCmdStruct lpnc)
{
	m_lpMdiFrame->AboutCOMFrame();

}