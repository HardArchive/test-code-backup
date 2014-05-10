#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。

/////////////////////////////////////////////////////////////////////////////
// CHellocompositecontrol1 包装类

class CHellocompositecontrol1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CHellocompositecontrol1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x56B23F6D, 0xF0D3, 0x401D, { 0xA1, 0xF8, 0xBF, 0x88, 0xB, 0xE8, 0xC2, 0x22 } };
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

// IHelloCompositeControl

// Functions
//

	CString get_Description()
	{
		CString result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Description(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void EnableButton(long lBtnIndex, BOOL varbEnable)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lBtnIndex, varbEnable);
	}

// Properties
//



};
