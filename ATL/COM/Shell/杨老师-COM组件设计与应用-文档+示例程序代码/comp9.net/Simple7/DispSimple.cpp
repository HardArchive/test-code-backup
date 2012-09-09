// DispSimple.cpp : 实现文件
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
	
	// 为了使应用程序在 OLE 自动化对象处于活动状态时保持
	//	运行，构造函数调用 AfxOleLockApp。
	
	AfxOleLockApp();
}

CDispSimple::~CDispSimple()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	
	AfxOleUnlockApp();
}


void CDispSimple::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CDispSimple, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CDispSimple, CCmdTarget)
	DISP_FUNCTION_ID(CDispSimple, "Add", dispidAdd, Add, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CDispSimple, "Upper", dispidUpper, Upper, VT_BSTR, VTS_BSTR)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IDispSimple 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {F9E6A98A-B849-475F-8D55-F951D86FCD37}
static const IID IID_IDispSimple =
{ 0xF9E6A98A, 0xB849, 0x475F, { 0x8D, 0x55, 0xF9, 0x51, 0xD8, 0x6F, 0xCD, 0x37 } };

BEGIN_INTERFACE_MAP(CDispSimple, CCmdTarget)
	INTERFACE_PART(CDispSimple, IID_IDispSimple, Dispatch)
END_INTERFACE_MAP()

// {C374DBA9-C292-42DA-B146-241748359875}
IMPLEMENT_OLECREATE_FLAGS(CDispSimple, "Simple7.DispSimple", afxRegApartmentThreading, 0xc374dba9, 0xc292, 0x42da, 0xb1, 0x46, 0x24, 0x17, 0x48, 0x35, 0x98, 0x75)


// CDispSimple 消息处理程序

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
