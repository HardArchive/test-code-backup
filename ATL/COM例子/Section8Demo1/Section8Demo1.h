

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Dec 11 09:22:57 2012
 */
/* Compiler settings for .\Section8Demo1.idl:
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

#ifndef __Section8Demo1_h__
#define __Section8Demo1_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPasswordSpObj_FWD_DEFINED__
#define __IPasswordSpObj_FWD_DEFINED__
typedef interface IPasswordSpObj IPasswordSpObj;
#endif 	/* __IPasswordSpObj_FWD_DEFINED__ */


#ifndef ___IPasswordSpObjEvents_FWD_DEFINED__
#define ___IPasswordSpObjEvents_FWD_DEFINED__
typedef interface _IPasswordSpObjEvents _IPasswordSpObjEvents;
#endif 	/* ___IPasswordSpObjEvents_FWD_DEFINED__ */


#ifndef __PasswordSpObj_FWD_DEFINED__
#define __PasswordSpObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class PasswordSpObj PasswordSpObj;
#else
typedef struct PasswordSpObj PasswordSpObj;
#endif /* __cplusplus */

#endif 	/* __PasswordSpObj_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IPasswordSpObj_INTERFACE_DEFINED__
#define __IPasswordSpObj_INTERFACE_DEFINED__

/* interface IPasswordSpObj */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPasswordSpObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A8E69F07-3692-4DC9-875D-9DE46EC90DB9")
    IPasswordSpObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrorCountAllowed( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ErrorCountAllowed( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckPassword( 
            /* [in] */ BSTR bstrUser,
            /* [in] */ BSTR bstrPwd,
            /* [retval][out] */ VARIANT_BOOL *pvarbResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPasswordSpObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPasswordSpObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPasswordSpObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPasswordSpObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPasswordSpObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPasswordSpObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPasswordSpObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPasswordSpObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrorCountAllowed )( 
            IPasswordSpObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ErrorCountAllowed )( 
            IPasswordSpObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CheckPassword )( 
            IPasswordSpObj * This,
            /* [in] */ BSTR bstrUser,
            /* [in] */ BSTR bstrPwd,
            /* [retval][out] */ VARIANT_BOOL *pvarbResult);
        
        END_INTERFACE
    } IPasswordSpObjVtbl;

    interface IPasswordSpObj
    {
        CONST_VTBL struct IPasswordSpObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPasswordSpObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPasswordSpObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPasswordSpObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPasswordSpObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPasswordSpObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPasswordSpObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPasswordSpObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPasswordSpObj_get_ErrorCountAllowed(This,pVal)	\
    (This)->lpVtbl -> get_ErrorCountAllowed(This,pVal)

#define IPasswordSpObj_put_ErrorCountAllowed(This,newVal)	\
    (This)->lpVtbl -> put_ErrorCountAllowed(This,newVal)

#define IPasswordSpObj_CheckPassword(This,bstrUser,bstrPwd,pvarbResult)	\
    (This)->lpVtbl -> CheckPassword(This,bstrUser,bstrPwd,pvarbResult)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPasswordSpObj_get_ErrorCountAllowed_Proxy( 
    IPasswordSpObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IPasswordSpObj_get_ErrorCountAllowed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPasswordSpObj_put_ErrorCountAllowed_Proxy( 
    IPasswordSpObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IPasswordSpObj_put_ErrorCountAllowed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPasswordSpObj_CheckPassword_Proxy( 
    IPasswordSpObj * This,
    /* [in] */ BSTR bstrUser,
    /* [in] */ BSTR bstrPwd,
    /* [retval][out] */ VARIANT_BOOL *pvarbResult);


void __RPC_STUB IPasswordSpObj_CheckPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPasswordSpObj_INTERFACE_DEFINED__ */



#ifndef __Section8Demo1Lib_LIBRARY_DEFINED__
#define __Section8Demo1Lib_LIBRARY_DEFINED__

/* library Section8Demo1Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Section8Demo1Lib;

#ifndef ___IPasswordSpObjEvents_DISPINTERFACE_DEFINED__
#define ___IPasswordSpObjEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IPasswordSpObjEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IPasswordSpObjEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9ECC6F13-726F-4BE8-A961-D6146536733D")
    _IPasswordSpObjEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IPasswordSpObjEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IPasswordSpObjEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IPasswordSpObjEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IPasswordSpObjEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IPasswordSpObjEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IPasswordSpObjEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IPasswordSpObjEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IPasswordSpObjEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IPasswordSpObjEventsVtbl;

    interface _IPasswordSpObjEvents
    {
        CONST_VTBL struct _IPasswordSpObjEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IPasswordSpObjEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IPasswordSpObjEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IPasswordSpObjEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IPasswordSpObjEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IPasswordSpObjEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IPasswordSpObjEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IPasswordSpObjEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IPasswordSpObjEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PasswordSpObj;

#ifdef __cplusplus

class DECLSPEC_UUID("08D3A036-B259-4201-9B02-0A653DB1F4C0")
PasswordSpObj;
#endif
#endif /* __Section8Demo1Lib_LIBRARY_DEFINED__ */

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


