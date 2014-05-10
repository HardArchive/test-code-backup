

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon Dec 10 18:17:04 2012
 */
/* Compiler settings for .\Section4Demo1.idl:
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

#ifndef __Section4Demo1_h__
#define __Section4Demo1_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IHelloSimpleObject_FWD_DEFINED__
#define __IHelloSimpleObject_FWD_DEFINED__
typedef interface IHelloSimpleObject IHelloSimpleObject;
#endif 	/* __IHelloSimpleObject_FWD_DEFINED__ */


#ifndef __HelloSimpleObject_FWD_DEFINED__
#define __HelloSimpleObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class HelloSimpleObject HelloSimpleObject;
#else
typedef struct HelloSimpleObject HelloSimpleObject;
#endif /* __cplusplus */

#endif 	/* __HelloSimpleObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IHelloSimpleObject_INTERFACE_DEFINED__
#define __IHelloSimpleObject_INTERFACE_DEFINED__

/* interface IHelloSimpleObject */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHelloSimpleObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AD3ADBBB-002E-4AB0-A23B-986A637585CB")
    IHelloSimpleObject : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SumLong( 
            /* [in] */ LONG lA,
            /* [in] */ LONG lB,
            /* [retval][out] */ LONG *plSum) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHelloSimpleObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHelloSimpleObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHelloSimpleObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHelloSimpleObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHelloSimpleObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHelloSimpleObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHelloSimpleObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHelloSimpleObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SumLong )( 
            IHelloSimpleObject * This,
            /* [in] */ LONG lA,
            /* [in] */ LONG lB,
            /* [retval][out] */ LONG *plSum);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IHelloSimpleObject * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IHelloSimpleObject * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IHelloSimpleObjectVtbl;

    interface IHelloSimpleObject
    {
        CONST_VTBL struct IHelloSimpleObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHelloSimpleObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHelloSimpleObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHelloSimpleObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHelloSimpleObject_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHelloSimpleObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHelloSimpleObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHelloSimpleObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHelloSimpleObject_SumLong(This,lA,lB,plSum)	\
    (This)->lpVtbl -> SumLong(This,lA,lB,plSum)

#define IHelloSimpleObject_get_Description(This,pVal)	\
    (This)->lpVtbl -> get_Description(This,pVal)

#define IHelloSimpleObject_put_Description(This,newVal)	\
    (This)->lpVtbl -> put_Description(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHelloSimpleObject_SumLong_Proxy( 
    IHelloSimpleObject * This,
    /* [in] */ LONG lA,
    /* [in] */ LONG lB,
    /* [retval][out] */ LONG *plSum);


void __RPC_STUB IHelloSimpleObject_SumLong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IHelloSimpleObject_get_Description_Proxy( 
    IHelloSimpleObject * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IHelloSimpleObject_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IHelloSimpleObject_put_Description_Proxy( 
    IHelloSimpleObject * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IHelloSimpleObject_put_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHelloSimpleObject_INTERFACE_DEFINED__ */



#ifndef __Section4Demo1Lib_LIBRARY_DEFINED__
#define __Section4Demo1Lib_LIBRARY_DEFINED__

/* library Section4Demo1Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Section4Demo1Lib;

EXTERN_C const CLSID CLSID_HelloSimpleObject;

#ifdef __cplusplus

class DECLSPEC_UUID("F65204D1-4E2E-494C-AB52-02D4D1D5438F")
HelloSimpleObject;
#endif
#endif /* __Section4Demo1Lib_LIBRARY_DEFINED__ */

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


