

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon Dec 10 18:17:16 2012
 */
/* Compiler settings for .\Section6Demo1.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __Section6Demo1_h__
#define __Section6Demo1_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IHelloCompositeControl_FWD_DEFINED__
#define __IHelloCompositeControl_FWD_DEFINED__
typedef interface IHelloCompositeControl IHelloCompositeControl;
#endif 	/* __IHelloCompositeControl_FWD_DEFINED__ */


#ifndef ___IHelloCompositeControlEvents_FWD_DEFINED__
#define ___IHelloCompositeControlEvents_FWD_DEFINED__
typedef interface _IHelloCompositeControlEvents _IHelloCompositeControlEvents;
#endif 	/* ___IHelloCompositeControlEvents_FWD_DEFINED__ */


#ifndef __HelloCompositeControl_FWD_DEFINED__
#define __HelloCompositeControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class HelloCompositeControl HelloCompositeControl;
#else
typedef struct HelloCompositeControl HelloCompositeControl;
#endif /* __cplusplus */

#endif 	/* __HelloCompositeControl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IHelloCompositeControl_INTERFACE_DEFINED__
#define __IHelloCompositeControl_INTERFACE_DEFINED__

/* interface IHelloCompositeControl */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHelloCompositeControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("185B107D-E152-4ABC-A45F-BDF0DA9E2C2F")
    IHelloCompositeControl : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableButton( 
            /* [in] */ LONG lBtnIndex,
            /* [in] */ VARIANT_BOOL varbEnable) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHelloCompositeControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHelloCompositeControl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHelloCompositeControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHelloCompositeControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHelloCompositeControl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHelloCompositeControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHelloCompositeControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHelloCompositeControl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IHelloCompositeControl * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IHelloCompositeControl * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableButton )( 
            IHelloCompositeControl * This,
            /* [in] */ LONG lBtnIndex,
            /* [in] */ VARIANT_BOOL varbEnable);
        
        END_INTERFACE
    } IHelloCompositeControlVtbl;

    interface IHelloCompositeControl
    {
        CONST_VTBL struct IHelloCompositeControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHelloCompositeControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHelloCompositeControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHelloCompositeControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHelloCompositeControl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHelloCompositeControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHelloCompositeControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHelloCompositeControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHelloCompositeControl_get_Description(This,pVal)	\
    (This)->lpVtbl -> get_Description(This,pVal)

#define IHelloCompositeControl_put_Description(This,newVal)	\
    (This)->lpVtbl -> put_Description(This,newVal)

#define IHelloCompositeControl_EnableButton(This,lBtnIndex,varbEnable)	\
    (This)->lpVtbl -> EnableButton(This,lBtnIndex,varbEnable)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IHelloCompositeControl_get_Description_Proxy( 
    IHelloCompositeControl * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IHelloCompositeControl_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IHelloCompositeControl_put_Description_Proxy( 
    IHelloCompositeControl * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IHelloCompositeControl_put_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHelloCompositeControl_EnableButton_Proxy( 
    IHelloCompositeControl * This,
    /* [in] */ LONG lBtnIndex,
    /* [in] */ VARIANT_BOOL varbEnable);


void __RPC_STUB IHelloCompositeControl_EnableButton_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHelloCompositeControl_INTERFACE_DEFINED__ */



#ifndef __Section6Demo1Lib_LIBRARY_DEFINED__
#define __Section6Demo1Lib_LIBRARY_DEFINED__

/* library Section6Demo1Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Section6Demo1Lib;

#ifndef ___IHelloCompositeControlEvents_DISPINTERFACE_DEFINED__
#define ___IHelloCompositeControlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IHelloCompositeControlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IHelloCompositeControlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2CD72493-CC54-451E-BBE0-5E5D25746644")
    _IHelloCompositeControlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IHelloCompositeControlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IHelloCompositeControlEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IHelloCompositeControlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IHelloCompositeControlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IHelloCompositeControlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IHelloCompositeControlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IHelloCompositeControlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IHelloCompositeControlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IHelloCompositeControlEventsVtbl;

    interface _IHelloCompositeControlEvents
    {
        CONST_VTBL struct _IHelloCompositeControlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IHelloCompositeControlEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IHelloCompositeControlEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IHelloCompositeControlEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IHelloCompositeControlEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IHelloCompositeControlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IHelloCompositeControlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IHelloCompositeControlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IHelloCompositeControlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_HelloCompositeControl;

#ifdef __cplusplus

class DECLSPEC_UUID("56B23F6D-F0D3-401D-A1F8-BF880BE8C222")
HelloCompositeControl;
#endif
#endif /* __Section6Demo1Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


