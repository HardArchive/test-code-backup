

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Dec 11 10:01:44 2012
 */
/* Compiler settings for .\Section8Demo2.idl:
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

#ifndef __Section8Demo2_h__
#define __Section8Demo2_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILoginAx_FWD_DEFINED__
#define __ILoginAx_FWD_DEFINED__
typedef interface ILoginAx ILoginAx;
#endif 	/* __ILoginAx_FWD_DEFINED__ */


#ifndef ___ILoginAxEvents_FWD_DEFINED__
#define ___ILoginAxEvents_FWD_DEFINED__
typedef interface _ILoginAxEvents _ILoginAxEvents;
#endif 	/* ___ILoginAxEvents_FWD_DEFINED__ */


#ifndef __LoginAx_FWD_DEFINED__
#define __LoginAx_FWD_DEFINED__

#ifdef __cplusplus
typedef class LoginAx LoginAx;
#else
typedef struct LoginAx LoginAx;
#endif /* __cplusplus */

#endif 	/* __LoginAx_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ILoginAx_INTERFACE_DEFINED__
#define __ILoginAx_INTERFACE_DEFINED__

/* interface ILoginAx */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILoginAx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("79A3FF78-2A01-401F-93E6-658B74C988E4")
    ILoginAx : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_JudgeNull( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_JudgeNull( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetText( 
            /* [in] */ BSTR bstrUserName,
            /* [in] */ BSTR bstrPassword,
            /* [in] */ BSTR bstrLogin) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILoginAxVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILoginAx * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILoginAx * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILoginAx * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILoginAx * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILoginAx * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILoginAx * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILoginAx * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_JudgeNull )( 
            ILoginAx * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_JudgeNull )( 
            ILoginAx * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetText )( 
            ILoginAx * This,
            /* [in] */ BSTR bstrUserName,
            /* [in] */ BSTR bstrPassword,
            /* [in] */ BSTR bstrLogin);
        
        END_INTERFACE
    } ILoginAxVtbl;

    interface ILoginAx
    {
        CONST_VTBL struct ILoginAxVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILoginAx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILoginAx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILoginAx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILoginAx_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILoginAx_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILoginAx_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILoginAx_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ILoginAx_get_JudgeNull(This,pVal)	\
    (This)->lpVtbl -> get_JudgeNull(This,pVal)

#define ILoginAx_put_JudgeNull(This,newVal)	\
    (This)->lpVtbl -> put_JudgeNull(This,newVal)

#define ILoginAx_SetText(This,bstrUserName,bstrPassword,bstrLogin)	\
    (This)->lpVtbl -> SetText(This,bstrUserName,bstrPassword,bstrLogin)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILoginAx_get_JudgeNull_Proxy( 
    ILoginAx * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB ILoginAx_get_JudgeNull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ILoginAx_put_JudgeNull_Proxy( 
    ILoginAx * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ILoginAx_put_JudgeNull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILoginAx_SetText_Proxy( 
    ILoginAx * This,
    /* [in] */ BSTR bstrUserName,
    /* [in] */ BSTR bstrPassword,
    /* [in] */ BSTR bstrLogin);


void __RPC_STUB ILoginAx_SetText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILoginAx_INTERFACE_DEFINED__ */



#ifndef __Section8Demo2Lib_LIBRARY_DEFINED__
#define __Section8Demo2Lib_LIBRARY_DEFINED__

/* library Section8Demo2Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Section8Demo2Lib;

#ifndef ___ILoginAxEvents_DISPINTERFACE_DEFINED__
#define ___ILoginAxEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ILoginAxEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ILoginAxEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CB61249D-BF62-4101-B5BF-D9FB594F2AC8")
    _ILoginAxEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ILoginAxEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ILoginAxEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ILoginAxEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ILoginAxEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ILoginAxEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ILoginAxEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ILoginAxEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ILoginAxEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ILoginAxEventsVtbl;

    interface _ILoginAxEvents
    {
        CONST_VTBL struct _ILoginAxEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ILoginAxEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ILoginAxEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ILoginAxEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ILoginAxEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ILoginAxEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ILoginAxEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ILoginAxEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ILoginAxEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_LoginAx;

#ifdef __cplusplus

class DECLSPEC_UUID("9B90E487-AD06-48EF-9064-73DE5752B398")
LoginAx;
#endif
#endif /* __Section8Demo2Lib_LIBRARY_DEFINED__ */

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


