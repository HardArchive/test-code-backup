// DispSimple.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Simple7.h"
#include "DispSimple.h"
#include ".\dispsimple.h"


// CDispSimple

IMPLEMENT_DYNCREATE(CDispSimple, CCmdTarget)
CDispSimple::CDispSimple()
{
	EnableAutomation();
	
	// Ϊ��ʹӦ�ó����� OLE �Զ��������ڻ״̬ʱ����
	//	���У����캯������ AfxOleLockApp��
	
	AfxOleLockApp();
}

CDispSimple::~CDispSimple()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	// 	������������ AfxOleUnlockApp��
	
	AfxOleUnlockApp();
}


void CDispSimple::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CDispSimple, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CDispSimple, CCmdTarget)
	DISP_FUNCTION_ID(CDispSimple, "Add", dispidAdd, Add, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CDispSimple, "Upper", dispidUpper, Upper, VT_BSTR, VTS_BSTR)
END_DISPATCH_MAP()

// ע��: ������� IID_IDispSimple ֧��
//��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//���Ƚӿڵ� GUID ƥ�䡣

// {F9E6A98A-B849-475F-8D55-F951D86FCD37}
static const IID IID_IDispSimple =
{ 0xF9E6A98A, 0xB849, 0x475F, { 0x8D, 0x55, 0xF9, 0x51, 0xD8, 0x6F, 0xCD, 0x37 } };

BEGIN_INTERFACE_MAP(CDispSimple, CCmdTarget)
	INTERFACE_PART(CDispSimple, IID_IDispSimple, Dispatch)
END_INTERFACE_MAP()

// {C374DBA9-C292-42DA-B146-241748359875}
IMPLEMENT_OLECREATE_FLAGS(CDispSimple, "Simple7.DispSimple", afxRegApartmentThreading, 0xc374dba9, 0xc292, 0x42da, 0xb1, 0x46, 0x24, 0x17, 0x48, 0x35, 0x98, 0x75)


// CDispSimple ��Ϣ�������

LONG CDispSimple::Add(LONG n1, LONG n2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return n1 + n2;
}

BSTR CDispSimple::Upper(LPCTSTR str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult(str);
	strResult.MakeUpper();

	return strResult.AllocSysString();
}
