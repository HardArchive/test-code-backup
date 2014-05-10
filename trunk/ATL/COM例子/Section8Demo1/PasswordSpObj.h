// PasswordSpObj.h : CPasswordSpObj ������

#pragma once
#include "resource.h"       // ������

#include "Section8Demo1.h"
#include "_IPasswordSpObjEvents_CP.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CPasswordSpObj

class ATL_NO_VTABLE CPasswordSpObj :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPasswordSpObj, &CLSID_PasswordSpObj>,
	public IConnectionPointContainerImpl<CPasswordSpObj>,
	public CProxy_IPasswordSpObjEvents<CPasswordSpObj>,
	public IDispatchImpl<IPasswordSpObj, &IID_IPasswordSpObj, &LIBID_Section8Demo1Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPasswordSpObj()
	{
		m_lErrorCountAllowed = 3;
		m_lErrorCount = 0;
		m_bstrUser = L"admin";
		m_bstrPwd = L"123456";
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PASSWORDSPOBJ)


BEGIN_COM_MAP(CPasswordSpObj)
	COM_INTERFACE_ENTRY(IPasswordSpObj)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPasswordSpObj)
	CONNECTION_POINT_ENTRY(__uuidof(_IPasswordSpObjEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:
	//�������Ĵ���
	LONG m_lErrorCountAllowed;

	//�������
	LONG m_lErrorCount;

	//�û���
	CComBSTR m_bstrUser;

	//����
	CComBSTR m_bstrPwd;


	STDMETHOD(get_ErrorCountAllowed)(LONG* pVal);
	STDMETHOD(put_ErrorCountAllowed)(LONG newVal);
	STDMETHOD(CheckPassword)(BSTR bstrUser, BSTR bstrPwd, VARIANT_BOOL* pvarbResult);
};

OBJECT_ENTRY_AUTO(__uuidof(PasswordSpObj), CPasswordSpObj)
