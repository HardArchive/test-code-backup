#pragma once
#include "oaidl.h"

class CPropertyBag :
	public IPropertyBag
{
public:
	CPropertyBag(void);
	~CPropertyBag(void);
	// STDMETHOD(xx) 是宏，等价于 long __stdcall xx
	STDMETHOD(QueryInterface)(const struct _GUID &iid,void ** ppv);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);
	STDMETHOD(Read)(LPCOLESTR pszPropName,VARIANT *pVar,IErrorLog *pErrorLog);
	STDMETHOD(Write)(LPCOLESTR pszPropName,VARIANT *pVar);
	void SetEditWnd(CEdit * pEdit);
private:
	CEdit *m_pEdit;
};
