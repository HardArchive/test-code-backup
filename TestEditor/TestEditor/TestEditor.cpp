
// TestEditor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "TestEditor.h"
#include "TestEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestEditorApp

BEGIN_MESSAGE_MAP(CTestEditorApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestEditorApp 构造

CTestEditorApp::CTestEditorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTestEditorApp 对象

CTestEditorApp theApp;


// CTestEditorApp 初始化

BOOL CTestEditorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	m_hDll = LoadLibrary(_T("SciLexer.dll"));
	//要启用Scintilla，首先当然是要加载之前编译的DLL文件啦~~
	//::LoadLibrary(_T("SciLexer.dll"));
	//SciLexer.dll加载后会自动以"Scintilla"作为类名注册一个窗体类，我们只要直接用这个类名建立窗体就可以了：
	//::CreateWindow(_T("Scintilla"),...);
	ASSERT( m_hDll != NULL);

	CTestEditorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


int CTestEditorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	// unload scintilla dll
	if (m_hDll != NULL)
		FreeLibrary(m_hDll);

	//ExitSkinMagicLib();
	return CWinApp::ExitInstance();
}