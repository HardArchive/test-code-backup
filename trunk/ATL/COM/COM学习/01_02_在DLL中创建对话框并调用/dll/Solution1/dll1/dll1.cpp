// dll1.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "dll1.h"
#include "d1.h"
#include "dd.h"
#include "a.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
		//extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// Cdll1App

BEGIN_MESSAGE_MAP(Cdll1App, CWinApp)
END_MESSAGE_MAP()


// Cdll1App ����

Cdll1App::Cdll1App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cdll1App ����

Cdll1App theApp;


// Cdll1App ��ʼ��

BOOL Cdll1App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" __declspec(dllexport) void Show()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CD1 dl;
	dl.m_valueEdit1 ="VC++";
	dl.DoModal ();

}
