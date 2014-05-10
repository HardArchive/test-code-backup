

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Dec 11 12:53:54 2012
 */
/* Compiler settings for .\Section7Demo1.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
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

#ifndef __Section7Demo1_h__
#define __Section7Demo1_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IHelloSOE_FWD_DEFINED__
#define __IHelloSOE_FWD_DEFINED__
typedef interface IHelloSOE IHelloSOE;
#endif 	/* __IHelloSOE_FWD_DEFINED__ */


#ifndef ___IHelloSOEEvents_FWD_DEFINED__
#define ___IHelloSOEEvents_FWD_DEFINED__
typedef interface _IHelloSOEEvents _IHelloSOEEvents;
#endif 	/* ___IHelloSOEEvents_FWD_DEFINED__ */


#ifndef __HelloSOE_FWD_DEFINED__
#define __HelloSOE_FWD_DEFINED__

#ifdef __cplusplus
typedef class HelloSOE HelloSOE;
#else
typedef struct HelloSOE HelloSOE;
#endif /* __cplusplus */

#endif 	/* __HelloSOE_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IHelloSOE_INTERFACE_DEFINED__
#define __IHelloSOE_INTERFACE_DEFINED__

/* interface IHelloSOE */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHelloSOE;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0155C7BF-721F-4107-A526-406A7F1D32BC")
    IHelloSOE : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ LONG lA,
            /* [in] */ LONG lB) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHelloSOEVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHelloSOE * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHelloSOE * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHelloSOE * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHelloSOE * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHelloSOE * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHelloSOE * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHelloSOE * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IHelloSOE * This,
            /* [in] */ LONG lA,
            /* [in] */ LONG lB);
        
        END_INTERFACE
    } IHelloSOEVtbl;

    interface IHelloSOE
    {
        CONST_VTBL struct IHelloSOEVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHelloSOE_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHelloSOE_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHelloSOE_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHelloSOE_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHelloSOE_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHelloSOE_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHelloSOE_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHelloSOE_Add(This,lA,lB)	\
    (This)->lpVtbl -> Add(This,lA,lB)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHelloSOE_Add_Proxy( 
    IHelloSOE * This,
    /* [in] */ LONG lA,
    /* [in] */ LONG lB);


void __RPC_STUB IHelloSOE_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHelloSOE_INTERFACE_DEFINED__ */



#ifndef __Section7Demo1Lib_LIBRARY_DEFINED__
#define __Section7Demo1Lib_LIBRARY_DEFINED__

/* library Section7Demo1Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Section7Demo1Lib;

#ifndef ___IHelloSOEEvents_DISPINTERFACE_DEFINED__
#define ___IHelloSOEEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IHelloSOEEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IHelloSOEEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A3F2D07E-4F99-4B17-B68F-761F2C203AF0")
    _IHelloSOEEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IHelloSOEEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IHelloSOEEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IHelloSOEEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IHelloSOEEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IHelloSOEEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IHelloSOEEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IHelloSOEEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IHelloSOEEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IHelloSOEEventsVtbl;

    interface _IHelloSOEEvents
    {
        CONST_VTBL struct _IHelloSOEEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IHelloSOEEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IHelloSOEEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IHelloSOEEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IHelloSOEEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IHelloSOEEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IHelloSOEEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IHelloSOEEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IHelloSOEEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_HelloSOE;

#ifdef __cplusplus

class DECLSPEC_UUID("EC5897D9-4F28-414F-9A5A-CFFD49F680E4")
HelloSOE;
#endif
#endif /* __Section7Demo1Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


