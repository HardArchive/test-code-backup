#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CLoginax 包装类

class CLoginax : public CWnd
{
protected:
	DECLARE_DYNCREATE(CLoginax)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x1BF0012C, 0x265F, 0x4ED2, { 0xA6, 0xD1, 0x25, 0x56, 0x53, 0xC9, 0xC1, 0x9 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:


// 操作
public:

// ILoginAx

// Functions
//

	BOOL get_JudgeNull()
	{
		BOOL result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_JudgeNull(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SetText(LPCTSTR bstrUserName, LPCTSTR bstrPassword, LPCTSTR bstrLogin)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bstrUserName, bstrPassword, bstrLogin);
	}

// Properties
//



};
