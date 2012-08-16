
// TestEditor.cpp : ����Ӧ�ó��������Ϊ��
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


// CTestEditorApp ����

CTestEditorApp::CTestEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTestEditorApp ����

CTestEditorApp theApp;


// CTestEditorApp ��ʼ��

BOOL CTestEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	m_hDll = LoadLibrary(_T("SciLexer.dll"));
	//Ҫ����Scintilla�����ȵ�Ȼ��Ҫ����֮ǰ�����DLL�ļ���~~
	//::LoadLibrary(_T("SciLexer.dll"));
	//SciLexer.dll���غ���Զ���"Scintilla"��Ϊ����ע��һ�������࣬����ֻҪֱ�������������������Ϳ����ˣ�
	//::CreateWindow(_T("Scintilla"),...);
	ASSERT( m_hDll != NULL);

	CTestEditorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
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