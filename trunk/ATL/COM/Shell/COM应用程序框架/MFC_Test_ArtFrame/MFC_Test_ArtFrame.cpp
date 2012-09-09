// MFC_Test_ArtFrame.cpp : 定义应用程序的类行为。
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


// CFrameApp 构造

CFrameApp::CFrameApp()	:
	m_lpMdiFrame(NULL)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFrameApp 对象

CFrameApp theApp;


// CFrameApp 初始化

BOOL CFrameApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();
	CoInitialize(NULL);
	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	INT_PTR nResponse = 0;
	
	WinMain();

	//CMFC_Test_ArtFrameDlg dlg;
	//m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
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
//以下是

void CFrameApp::FileNew(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
{
	IMDIChildFrames *lpFrames = NULL;
	IMDIChildFrame *lpChildFrame = NULL;
	HRESULT hr = 0;

	//从主窗口IMDIFrame接口引出IMDIChildFrames 集合．
	hr = m_lpMdiFrame->get_MDIChildFrames((IDispatch**)&lpFrames);

	if (FAILED(hr))
	{
		ATLASSERT(0);
	}

	//用IMDIChildFrames 集合建立一个mdi子窗口．lpChildFrame，反回子窗口接口类．
	CTextView *lpTexView = NULL;
	lpTexView = new CTextView();
	lpFrames->CreateChildFrame((IDataView*)lpTexView, &lpChildFrame);

	//设置子窗口标题．
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