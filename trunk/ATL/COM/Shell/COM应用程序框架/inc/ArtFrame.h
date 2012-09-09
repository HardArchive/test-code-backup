

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Jun 13 16:59:16 2011
 */
/* Compiler settings for .\ArtFrame.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ArtFrame_h__
#define __ArtFrame_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDataView_FWD_DEFINED__
#define __IDataView_FWD_DEFINED__
typedef interface IDataView IDataView;
#endif 	/* __IDataView_FWD_DEFINED__ */


#ifndef ___COMFrameDispObj_FWD_DEFINED__
#define ___COMFrameDispObj_FWD_DEFINED__
typedef interface _COMFrameDispObj _COMFrameDispObj;
#endif 	/* ___COMFrameDispObj_FWD_DEFINED__ */


#ifndef __IMDIFrame_FWD_DEFINED__
#define __IMDIFrame_FWD_DEFINED__
typedef interface IMDIFrame IMDIFrame;
#endif 	/* __IMDIFrame_FWD_DEFINED__ */


#ifndef __IMDIChildFrame_FWD_DEFINED__
#define __IMDIChildFrame_FWD_DEFINED__
typedef interface IMDIChildFrame IMDIChildFrame;
#endif 	/* __IMDIChildFrame_FWD_DEFINED__ */


#ifndef __IMDIChildFrames_FWD_DEFINED__
#define __IMDIChildFrames_FWD_DEFINED__
typedef interface IMDIChildFrames IMDIChildFrames;
#endif 	/* __IMDIChildFrames_FWD_DEFINED__ */


#ifndef ___CommandButtonEvents_FWD_DEFINED__
#define ___CommandButtonEvents_FWD_DEFINED__
typedef interface _CommandButtonEvents _CommandButtonEvents;
#endif 	/* ___CommandButtonEvents_FWD_DEFINED__ */


#ifndef ___IMDIFrameEvents_FWD_DEFINED__
#define ___IMDIFrameEvents_FWD_DEFINED__
typedef interface _IMDIFrameEvents _IMDIFrameEvents;
#endif 	/* ___IMDIFrameEvents_FWD_DEFINED__ */


#ifndef __MDIFrame_FWD_DEFINED__
#define __MDIFrame_FWD_DEFINED__

#ifdef __cplusplus
typedef class MDIFrame MDIFrame;
#else
typedef struct MDIFrame MDIFrame;
#endif /* __cplusplus */

#endif 	/* __MDIFrame_FWD_DEFINED__ */


#ifndef ___IMDIChildFrameEvents_FWD_DEFINED__
#define ___IMDIChildFrameEvents_FWD_DEFINED__
typedef interface _IMDIChildFrameEvents _IMDIChildFrameEvents;
#endif 	/* ___IMDIChildFrameEvents_FWD_DEFINED__ */


#ifndef __MDIChildFrame_FWD_DEFINED__
#define __MDIChildFrame_FWD_DEFINED__

#ifdef __cplusplus
typedef class MDIChildFrame MDIChildFrame;
#else
typedef struct MDIChildFrame MDIChildFrame;
#endif /* __cplusplus */

#endif 	/* __MDIChildFrame_FWD_DEFINED__ */


#ifndef __MDIChildFrames_FWD_DEFINED__
#define __MDIChildFrames_FWD_DEFINED__

#ifdef __cplusplus
typedef class MDIChildFrames MDIChildFrames;
#else
typedef struct MDIChildFrames MDIChildFrames;
#endif /* __cplusplus */

#endif 	/* __MDIChildFrames_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_ArtFrame_0000 */
/* [local] */ 

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("41F5D07A-04F6-11DB-B3CC-005056C00008") 
enum artCommandBarType
    {	artCommandBarTypePopup	= 10,
	artCommandBarTypeToolbar	= 23,
	artCommandBarTypeMenu	= 24
    } 	artCommandBarType;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1ED1D01A-E88C-4963-AD4F-C4775D068865") 
enum artCommandItemStyle
    {	msoButtonAutomatic	= 0,
	msoButtonIcon	= 1,
	msoButtonCaption	= 2,
	msoButtonIconAndCaption	= 3,
	msoButtonIconAndCaptionBelow	= 11
    } 	artCommandItemStyle;



extern RPC_IF_HANDLE __MIDL_itf_ArtFrame_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ArtFrame_0000_v0_0_s_ifspec;

#ifndef __IDataView_INTERFACE_DEFINED__
#define __IDataView_INTERFACE_DEFINED__

/* interface IDataView */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDataView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E2E319A4-D764-4dc1-95CD-C92D3754D95B")
    IDataView : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NotifyCommand( 
            UINT codeNotify,
            UINT cmdID,
            VARIANT_BOOL *bHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWnd( 
            HWND *hWnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnActiveWindow( 
            UINT nType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnCloseWindow( 
            VARIANT_BOOL *vbClose) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateWnd( 
            HWND hWndParent) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDataViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDataView * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDataView * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDataView * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDataView * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDataView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDataView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDataView * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NotifyCommand )( 
            IDataView * This,
            UINT codeNotify,
            UINT cmdID,
            VARIANT_BOOL *bHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWnd )( 
            IDataView * This,
            HWND *hWnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnActiveWindow )( 
            IDataView * This,
            UINT nType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnCloseWindow )( 
            IDataView * This,
            VARIANT_BOOL *vbClose);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateWnd )( 
            IDataView * This,
            HWND hWndParent);
        
        END_INTERFACE
    } IDataViewVtbl;

    interface IDataView
    {
        CONST_VTBL struct IDataViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDataView_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDataView_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDataView_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDataView_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDataView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDataView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDataView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDataView_NotifyCommand(This,codeNotify,cmdID,bHandle)	\
    (This)->lpVtbl -> NotifyCommand(This,codeNotify,cmdID,bHandle)

#define IDataView_GetWnd(This,hWnd)	\
    (This)->lpVtbl -> GetWnd(This,hWnd)

#define IDataView_OnActiveWindow(This,nType)	\
    (This)->lpVtbl -> OnActiveWindow(This,nType)

#define IDataView_OnCloseWindow(This,vbClose)	\
    (This)->lpVtbl -> OnCloseWindow(This,vbClose)

#define IDataView_CreateWnd(This,hWndParent)	\
    (This)->lpVtbl -> CreateWnd(This,hWndParent)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDataView_NotifyCommand_Proxy( 
    IDataView * This,
    UINT codeNotify,
    UINT cmdID,
    VARIANT_BOOL *bHandle);


void __RPC_STUB IDataView_NotifyCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDataView_GetWnd_Proxy( 
    IDataView * This,
    HWND *hWnd);


void __RPC_STUB IDataView_GetWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDataView_OnActiveWindow_Proxy( 
    IDataView * This,
    UINT nType);


void __RPC_STUB IDataView_OnActiveWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDataView_OnCloseWindow_Proxy( 
    IDataView * This,
    VARIANT_BOOL *vbClose);


void __RPC_STUB IDataView_OnCloseWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDataView_CreateWnd_Proxy( 
    IDataView * This,
    HWND hWndParent);


void __RPC_STUB IDataView_CreateWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDataView_INTERFACE_DEFINED__ */


#ifndef ___COMFrameDispObj_INTERFACE_DEFINED__
#define ___COMFrameDispObj_INTERFACE_DEFINED__

/* interface _COMFrameDispObj */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID__COMFrameDispObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3B8C7933-4F37-4d83-9107-CF68586155E2")
    _COMFrameDispObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_COMMDIFrame( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _COMFrameDispObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _COMFrameDispObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _COMFrameDispObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _COMFrameDispObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _COMFrameDispObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _COMFrameDispObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _COMFrameDispObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _COMFrameDispObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_COMMDIFrame )( 
            _COMFrameDispObj * This,
            /* [retval][out] */ IDispatch **pVal);
        
        END_INTERFACE
    } _COMFrameDispObjVtbl;

    interface _COMFrameDispObj
    {
        CONST_VTBL struct _COMFrameDispObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _COMFrameDispObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _COMFrameDispObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _COMFrameDispObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _COMFrameDispObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _COMFrameDispObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _COMFrameDispObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _COMFrameDispObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define _COMFrameDispObj_get_COMMDIFrame(This,pVal)	\
    (This)->lpVtbl -> get_COMMDIFrame(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE _COMFrameDispObj_get_COMMDIFrame_Proxy( 
    _COMFrameDispObj * This,
    /* [retval][out] */ IDispatch **pVal);


void __RPC_STUB _COMFrameDispObj_get_COMMDIFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___COMFrameDispObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_ArtFrame_0258 */
/* [local] */ 

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("C90C84B4-FA10-11DA-AB3A-005056C00008") struct CreateMDIStruct
    {
    UINT cbSize;
    DWORD mask;
    LPWSTR lpszWindowName;
    HMENU hMenu;
    LPARAM lParam;
    } 	CreateMDIStruct;

typedef CreateMDIStruct *LPCreateMDIStruct;



extern RPC_IF_HANDLE __MIDL_itf_ArtFrame_0258_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ArtFrame_0258_v0_0_s_ifspec;

#ifndef __IMDIFrame_INTERFACE_DEFINED__
#define __IMDIFrame_INTERFACE_DEFINED__

/* interface IMDIFrame */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMDIFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D3137FF6-206B-4767-A905-5D252B3D9325")
    IMDIFrame : public _COMFrameDispObj
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Run( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateWnd( 
            CreateMDIStruct *lpMDIStruct) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Quit( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowMe( 
            VARIANT_BOOL vbShow) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MDIChildFrames( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AboutCOMFrame( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MDIWindows( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMDIFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMDIFrame * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMDIFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMDIFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMDIFrame * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMDIFrame * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMDIFrame * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMDIFrame * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_COMMDIFrame )( 
            IMDIFrame * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            IMDIFrame * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateWnd )( 
            IMDIFrame * This,
            CreateMDIStruct *lpMDIStruct);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Quit )( 
            IMDIFrame * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowMe )( 
            IMDIFrame * This,
            VARIANT_BOOL vbShow);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MDIChildFrames )( 
            IMDIFrame * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AboutCOMFrame )( 
            IMDIFrame * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MDIWindows )( 
            IMDIFrame * This);
        
        END_INTERFACE
    } IMDIFrameVtbl;

    interface IMDIFrame
    {
        CONST_VTBL struct IMDIFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMDIFrame_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMDIFrame_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMDIFrame_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMDIFrame_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMDIFrame_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMDIFrame_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMDIFrame_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMDIFrame_get_COMMDIFrame(This,pVal)	\
    (This)->lpVtbl -> get_COMMDIFrame(This,pVal)


#define IMDIFrame_Run(This)	\
    (This)->lpVtbl -> Run(This)

#define IMDIFrame_CreateWnd(This,lpMDIStruct)	\
    (This)->lpVtbl -> CreateWnd(This,lpMDIStruct)

#define IMDIFrame_Quit(This)	\
    (This)->lpVtbl -> Quit(This)

#define IMDIFrame_ShowMe(This,vbShow)	\
    (This)->lpVtbl -> ShowMe(This,vbShow)

#define IMDIFrame_get_MDIChildFrames(This,pVal)	\
    (This)->lpVtbl -> get_MDIChildFrames(This,pVal)

#define IMDIFrame_AboutCOMFrame(This)	\
    (This)->lpVtbl -> AboutCOMFrame(This)

#define IMDIFrame_MDIWindows(This)	\
    (This)->lpVtbl -> MDIWindows(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIFrame_Run_Proxy( 
    IMDIFrame * This);


void __RPC_STUB IMDIFrame_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIFrame_CreateWnd_Proxy( 
    IMDIFrame * This,
    CreateMDIStruct *lpMDIStruct);


void __RPC_STUB IMDIFrame_CreateWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIFrame_Quit_Proxy( 
    IMDIFrame * This);


void __RPC_STUB IMDIFrame_Quit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIFrame_ShowMe_Proxy( 
    IMDIFrame * This,
    VARIANT_BOOL vbShow);


void __RPC_STUB IMDIFrame_ShowMe_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMDIFrame_get_MDIChildFrames_Proxy( 
    IMDIFrame * This,
    /* [retval][out] */ IDispatch **pVal);


void __RPC_STUB IMDIFrame_get_MDIChildFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIFrame_AboutCOMFrame_Proxy( 
    IMDIFrame * This);


void __RPC_STUB IMDIFrame_AboutCOMFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIFrame_MDIWindows_Proxy( 
    IMDIFrame * This);


void __RPC_STUB IMDIFrame_MDIWindows_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMDIFrame_INTERFACE_DEFINED__ */


#ifndef __IMDIChildFrame_INTERFACE_DEFINED__
#define __IMDIChildFrame_INTERFACE_DEFINED__

/* interface IMDIChildFrame */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMDIChildFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A61BB8F9-6141-427C-AA31-CC4888335E51")
    IMDIChildFrame : public _COMFrameDispObj
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PushDataView( 
            IDataView *lpDataView) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PopDataView( 
            /* [in] */ IDataView *lpCW) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveChildWindow( 
            /* [retval][out] */ IDataView **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ActiveChildWindow( 
            /* [in] */ IDataView *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Title( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Title( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CommandBars( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMDIChildFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMDIChildFrame * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMDIChildFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMDIChildFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMDIChildFrame * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMDIChildFrame * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMDIChildFrame * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMDIChildFrame * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_COMMDIFrame )( 
            IMDIChildFrame * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PushDataView )( 
            IMDIChildFrame * This,
            IDataView *lpDataView);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PopDataView )( 
            IMDIChildFrame * This,
            /* [in] */ IDataView *lpCW);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveChildWindow )( 
            IMDIChildFrame * This,
            /* [retval][out] */ IDataView **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ActiveChildWindow )( 
            IMDIChildFrame * This,
            /* [in] */ IDataView *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Title )( 
            IMDIChildFrame * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Title )( 
            IMDIChildFrame * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CommandBars )( 
            IMDIChildFrame * This,
            /* [retval][out] */ IDispatch **pVal);
        
        END_INTERFACE
    } IMDIChildFrameVtbl;

    interface IMDIChildFrame
    {
        CONST_VTBL struct IMDIChildFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMDIChildFrame_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMDIChildFrame_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMDIChildFrame_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMDIChildFrame_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMDIChildFrame_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMDIChildFrame_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMDIChildFrame_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMDIChildFrame_get_COMMDIFrame(This,pVal)	\
    (This)->lpVtbl -> get_COMMDIFrame(This,pVal)


#define IMDIChildFrame_PushDataView(This,lpDataView)	\
    (This)->lpVtbl -> PushDataView(This,lpDataView)

#define IMDIChildFrame_PopDataView(This,lpCW)	\
    (This)->lpVtbl -> PopDataView(This,lpCW)

#define IMDIChildFrame_get_ActiveChildWindow(This,pVal)	\
    (This)->lpVtbl -> get_ActiveChildWindow(This,pVal)

#define IMDIChildFrame_put_ActiveChildWindow(This,newVal)	\
    (This)->lpVtbl -> put_ActiveChildWindow(This,newVal)

#define IMDIChildFrame_get_Title(This,pVal)	\
    (This)->lpVtbl -> get_Title(This,pVal)

#define IMDIChildFrame_put_Title(This,newVal)	\
    (This)->lpVtbl -> put_Title(This,newVal)

#define IMDIChildFrame_get_CommandBars(This,pVal)	\
    (This)->lpVtbl -> get_CommandBars(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIChildFrame_PushDataView_Proxy( 
    IMDIChildFrame * This,
    IDataView *lpDataView);


void __RPC_STUB IMDIChildFrame_PushDataView_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIChildFrame_PopDataView_Proxy( 
    IMDIChildFrame * This,
    /* [in] */ IDataView *lpCW);


void __RPC_STUB IMDIChildFrame_PopDataView_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMDIChildFrame_get_ActiveChildWindow_Proxy( 
    IMDIChildFrame * This,
    /* [retval][out] */ IDataView **pVal);


void __RPC_STUB IMDIChildFrame_get_ActiveChildWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMDIChildFrame_put_ActiveChildWindow_Proxy( 
    IMDIChildFrame * This,
    /* [in] */ IDataView *newVal);


void __RPC_STUB IMDIChildFrame_put_ActiveChildWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMDIChildFrame_get_Title_Proxy( 
    IMDIChildFrame * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMDIChildFrame_get_Title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMDIChildFrame_put_Title_Proxy( 
    IMDIChildFrame * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMDIChildFrame_put_Title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMDIChildFrame_get_CommandBars_Proxy( 
    IMDIChildFrame * This,
    /* [retval][out] */ IDispatch **pVal);


void __RPC_STUB IMDIChildFrame_get_CommandBars_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMDIChildFrame_INTERFACE_DEFINED__ */


#ifndef __IMDIChildFrames_INTERFACE_DEFINED__
#define __IMDIChildFrames_INTERFACE_DEFINED__

/* interface IMDIChildFrames */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMDIChildFrames;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DBDC4265-324B-4E01-9BBE-8AB76FBB8CF6")
    IMDIChildFrames : public _COMFrameDispObj
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [hidden][restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long Index,
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateChildFrame( 
            /* [in] */ IDataView *lpdv,
            /* [out] */ IMDIChildFrame **lpcf) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMDIChildFramesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMDIChildFrames * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMDIChildFrames * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMDIChildFrames * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMDIChildFrames * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMDIChildFrames * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMDIChildFrames * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMDIChildFrames * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_COMMDIFrame )( 
            IMDIChildFrames * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IMDIChildFrames * This,
            /* [retval][out] */ long *pVal);
        
        /* [hidden][restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IMDIChildFrames * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IMDIChildFrames * This,
            /* [in] */ long Index,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateChildFrame )( 
            IMDIChildFrames * This,
            /* [in] */ IDataView *lpdv,
            /* [out] */ IMDIChildFrame **lpcf);
        
        END_INTERFACE
    } IMDIChildFramesVtbl;

    interface IMDIChildFrames
    {
        CONST_VTBL struct IMDIChildFramesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMDIChildFrames_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMDIChildFrames_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMDIChildFrames_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMDIChildFrames_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMDIChildFrames_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMDIChildFrames_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMDIChildFrames_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMDIChildFrames_get_COMMDIFrame(This,pVal)	\
    (This)->lpVtbl -> get_COMMDIFrame(This,pVal)


#define IMDIChildFrames_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IMDIChildFrames_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define IMDIChildFrames_get_Item(This,Index,pVal)	\
    (This)->lpVtbl -> get_Item(This,Index,pVal)

#define IMDIChildFrames_CreateChildFrame(This,lpdv,lpcf)	\
    (This)->lpVtbl -> CreateChildFrame(This,lpdv,lpcf)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IMDIChildFrames_get_Count_Proxy( 
    IMDIChildFrames * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IMDIChildFrames_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IMDIChildFrames_get__NewEnum_Proxy( 
    IMDIChildFrames * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB IMDIChildFrames_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IMDIChildFrames_get_Item_Proxy( 
    IMDIChildFrames * This,
    /* [in] */ long Index,
    /* [retval][out] */ VARIANT *pVal);


void __RPC_STUB IMDIChildFrames_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDIChildFrames_CreateChildFrame_Proxy( 
    IMDIChildFrames * This,
    /* [in] */ IDataView *lpdv,
    /* [out] */ IMDIChildFrame **lpcf);


void __RPC_STUB IMDIChildFrames_CreateChildFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMDIChildFrames_INTERFACE_DEFINED__ */



#ifndef __ArtFrameLib_LIBRARY_DEFINED__
#define __ArtFrameLib_LIBRARY_DEFINED__

/* library ArtFrameLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ArtFrameLib;

#ifndef ___CommandButtonEvents_DISPINTERFACE_DEFINED__
#define ___CommandButtonEvents_DISPINTERFACE_DEFINED__

/* dispinterface _CommandButtonEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__CommandButtonEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("62F13529-A28A-4ECB-ACD6-14192AF1205D")
    _CommandButtonEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _CommandButtonEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _CommandButtonEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _CommandButtonEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _CommandButtonEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _CommandButtonEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _CommandButtonEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _CommandButtonEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _CommandButtonEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _CommandButtonEventsVtbl;

    interface _CommandButtonEvents
    {
        CONST_VTBL struct _CommandButtonEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _CommandButtonEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _CommandButtonEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _CommandButtonEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _CommandButtonEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _CommandButtonEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _CommandButtonEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _CommandButtonEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___CommandButtonEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___IMDIFrameEvents_DISPINTERFACE_DEFINED__
#define ___IMDIFrameEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IMDIFrameEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IMDIFrameEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("866BD9AA-8EC6-45D1-B1D2-55B1E8967E37")
    _IMDIFrameEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IMDIFrameEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IMDIFrameEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IMDIFrameEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IMDIFrameEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IMDIFrameEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IMDIFrameEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IMDIFrameEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IMDIFrameEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IMDIFrameEventsVtbl;

    interface _IMDIFrameEvents
    {
        CONST_VTBL struct _IMDIFrameEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IMDIFrameEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IMDIFrameEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IMDIFrameEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IMDIFrameEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IMDIFrameEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IMDIFrameEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IMDIFrameEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IMDIFrameEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_MDIFrame;

#ifdef __cplusplus

class DECLSPEC_UUID("B092D48B-B0B7-4AFE-9010-986274A78673")
MDIFrame;
#endif

#ifndef ___IMDIChildFrameEvents_DISPINTERFACE_DEFINED__
#define ___IMDIChildFrameEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IMDIChildFrameEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IMDIChildFrameEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5BE13374-8203-4A09-8A34-CAE92D2747A0")
    _IMDIChildFrameEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IMDIChildFrameEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IMDIChildFrameEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IMDIChildFrameEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IMDIChildFrameEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IMDIChildFrameEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IMDIChildFrameEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IMDIChildFrameEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IMDIChildFrameEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IMDIChildFrameEventsVtbl;

    interface _IMDIChildFrameEvents
    {
        CONST_VTBL struct _IMDIChildFrameEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IMDIChildFrameEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IMDIChildFrameEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IMDIChildFrameEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IMDIChildFrameEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IMDIChildFrameEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IMDIChildFrameEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IMDIChildFrameEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IMDIChildFrameEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_MDIChildFrame;

#ifdef __cplusplus

class DECLSPEC_UUID("47B924E4-4E16-4DB9-AE43-3F0144BE0DD2")
MDIChildFrame;
#endif

EXTERN_C const CLSID CLSID_MDIChildFrames;

#ifdef __cplusplus

class DECLSPEC_UUID("9462FE98-891A-463D-9B6D-CDEA80462BB2")
MDIChildFrames;
#endif
#endif /* __ArtFrameLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  HMENU_UserSize(     unsigned long *, unsigned long            , HMENU * ); 
unsigned char * __RPC_USER  HMENU_UserMarshal(  unsigned long *, unsigned char *, HMENU * ); 
unsigned char * __RPC_USER  HMENU_UserUnmarshal(unsigned long *, unsigned char *, HMENU * ); 
void                      __RPC_USER  HMENU_UserFree(     unsigned long *, HMENU * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long *, unsigned long            , HWND * ); 
unsigned char * __RPC_USER  HWND_UserMarshal(  unsigned long *, unsigned char *, HWND * ); 
unsigned char * __RPC_USER  HWND_UserUnmarshal(unsigned long *, unsigned char *, HWND * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long *, HWND * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


