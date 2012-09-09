// MFC_Test_ArtFrame.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MFC_Test_ArtFrame.h"
#include "../inc/ArtFrame_i.c"
#include "TextView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// CFrameApp

BEGIN_MESSAGE_MAP(CFrameApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFrameApp ����

CFrameApp::CFrameApp()	:
	m_lpMdiFrame(NULL)
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFrameApp ����

CFrameApp theApp;


// CFrameApp ��ʼ��

BOOL CFrameApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();
	CoInitialize(NULL);
	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	INT_PTR nResponse = 0;
	
	WinMain();

	//CMFC_Test_ArtFrameDlg dlg;
	//m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////

HRESULT CFrameApp::CreateMain()
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



	hr = this->DispEventAdvise((IUnknown*)m_lpMdiFrame);
	if (FAILED(hr))
	{
		ATLASSERT(0);
	}

	return hr;
}



//////////////////////////////////////////////////////////////////////////
//������

void CFrameApp::FileNew(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	IMDIChildFrames *lpFrames = NULL;
	IMDIChildFrame *lpChildFrame = NULL;
	HRESULT hr = 0;

	//��������IMDIFrame�ӿ�����IMDIChildFrames ���ϣ�
	hr = m_lpMdiFrame->get_MDIChildFrames((IDispatch**)&lpFrames);

	if (FAILED(hr))
	{
		ATLASSERT(0);
	}

	//��IMDIChildFrames ���Ͻ���һ��mdi�Ӵ��ڣ�lpChildFrame�������Ӵ��ڽӿ��࣮
	CTextView *lpTexView = NULL;
	lpTexView = new CTextView();
	lpFrames->CreateChildFrame((IDataView*)lpTexView, &lpChildFrame);

	//�����Ӵ��ڱ��⣮
	lpChildFrame->put_Title(L"MFC TextView");
}



void CFrameApp::WinMain()
{
	//CoInitialize(NULL);
	HRESULT hr = CreateMain();
	if (SUCCEEDED(hr))
	{
		m_lpMdiFrame->Run();
	}

}


void CFrameApp::OnFileExit(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_lpMdiFrame->Quit();
}


void CFrameApp::OnAbout(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_lpMdiFrame->AboutCOMFrame();

}

void CFrameApp::OnWindowWindows(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	m_lpMdiFrame->MDIWindows();
}