/********************************************************************
 *
 *	created:	2006/06/13
 *	created:	13:6:2006   20:41
 *	filename: 	f:\Test\ComFrame\ArtFrame\ArtFrame\MDIChildFrames.h
 *	file path:	f:\Test\ComFrame\ArtFrame\ArtFrame
 *	file base:	MDIChildFrames
 *	file ext:	h
 *	author:		����ɭ
 *	Email:		ytf1978@163.com
 *	
 *	purpose:	
 *				��������COM MDIӦ�ó������е�����MDI�Ӵ��ڣ��������¶���:
 *				1.����MDI�Ӵ���
 *				2.ɾ��MDI�Ӵ���
 *				3.
 *
 ********************************************************************/

#pragma once
#include "resource.h"       // main symbols

#include "..\inc\ArtFrame.h"
#include <list>
#include "MDIFrame.h"
#include "_IMDIChildFrameEvents_CP.H"

// CMDIChildFrames

namespace ChildFramesCol
{
	typedef std::list<IMDIChildFrame*> CollType;
	typedef CCopyVariantLong<IMDIChildFrame> CopyVariantLong;
	typedef CComEnumOnSTL<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, CopyVariantLong, CollType> EnumType;
	typedef ICollectionOnSTLImpl<IMDIChildFrames, CollType, VARIANT, CopyVariantLong, EnumType> CollectionType;
};


class ATL_NO_VTABLE CMDIChildFrames : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMDIChildFrames, &CLSID_MDIChildFrames>,
	public ISupportErrorInfo,
	public IDispatchImpl<ChildFramesCol::CollectionType, &IID_IMDIChildFrames, &LIBID_ArtFrameLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IConnectionPointContainerImpl<CMDIChildFrames>,
	public CProxy_IMDIChildFrameEvents<CMDIChildFrames>
{
public:

	CMDIChildFrames()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MDICHILDFRAMES)


BEGIN_COM_MAP(CMDIChildFrames)
	COM_INTERFACE_ENTRY(IMDIChildFrames)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	//STDMETHOD(CreateChildFrame)(IDataView** lpcw, IMDIChildFrame** lpcf, INotifyObject **lpFrameNotifyCmd);
	STDMETHOD(CreateChildFrame)(IDataView* lpdv, IMDIChildFrame** lpcf);
	__COMMDIFrame;
	BEGIN_CONNECTION_POINT_MAP(CMDIChildFrames)
		CONNECTION_POINT_ENTRY(__uuidof(_IMDIChildFrameEvents))
	END_CONNECTION_POINT_MAP()
};

OBJECT_ENTRY_AUTO(__uuidof(MDIChildFrames), CMDIChildFrames)
