

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Jun 14 14:22:45 2011
 */
/* Compiler settings for .\Simple4.idl:
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

#ifndef __Simple4_h__
#define __Simple4_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMathe_FWD_DEFINED__
#define __IMathe_FWD_DEFINED__
typedef interface IMathe IMathe;
#endif 	/* __IMathe_FWD_DEFINED__ */


#ifndef __IStr_FWD_DEFINED__
#define __IStr_FWD_DEFINED__
typedef interface IStr IStr;
#endif 	/* __IStr_FWD_DEFINED__ */


#ifndef __IMathe2_FWD_DEFINED__
#define __IMathe2_FWD_DEFINED__
typedef interface IMathe2 IMathe2;
#endif 	/* __IMathe2_FWD_DEFINED__ */


#ifndef __Mathe_FWD_DEFINED__
#define __Mathe_FWD_DEFINED__

#ifdef __cplusplus
typedef class Mathe Mathe;
#else
typedef struct Mathe Mathe;
#endif /* __cplusplus */

#endif 	/* __Mathe_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IMathe_INTERFACE_DEFINED__
#define __IMathe_INTERFACE_DEFINED__

/* interface IMathe */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMathe;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A413CA85-AF88-40A8-9C29-5A5AD467014A")
    IMathe : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ LONG n1,
            /* [in] */ LONG n2,
            /* [retval][out] */ LONG *pnVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMatheVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMathe * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMathe * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMathe * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IMathe * This,
            /* [in] */ LONG n1,
            /* [in] */ LONG n2,
            /* [retval][out] */ LONG *pnVal);
        
        END_INTERFACE
    } IMatheVtbl;

    interface IMathe
    {
        CONST_VTBL struct IMatheVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMathe_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMathe_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMathe_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMathe_Add(This,n1,n2,pnVal)	\
    (This)->lpVtbl -> Add(This,n1,n2,pnVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMathe_Add_Proxy( 
    IMathe * This,
    /* [in] */ LONG n1,
    /* [in] */ LONG n2,
    /* [retval][out] */ LONG *pnVal);


void __RPC_STUB IMathe_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMathe_INTERFACE_DEFINED__ */


#ifndef __IStr_INTERFACE_DEFINED__
#define __IStr_INTERFACE_DEFINED__

/* interface IStr */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IStr;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A413CA86-AF88-40A8-9C29-5A5AD467014A")
    IStr : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Cat( 
            /* [in] */ BSTR s1,
            /* [in] */ BSTR s2,
            /* [retval][out] */ BSTR *psVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStr * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStr * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStr * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Cat )( 
            IStr * This,
            /* [in] */ BSTR s1,
            /* [in] */ BSTR s2,
            /* [retval][out] */ BSTR *psVal);
        
        END_INTERFACE
    } IStrVtbl;

    interface IStr
    {
        CONST_VTBL struct IStrVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStr_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStr_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStr_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStr_Cat(This,s1,s2,psVal)	\
    (This)->lpVtbl -> Cat(This,s1,s2,psVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStr_Cat_Proxy( 
    IStr * This,
    /* [in] */ BSTR s1,
    /* [in] */ BSTR s2,
    /* [retval][out] */ BSTR *psVal);


void __RPC_STUB IStr_Cat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStr_INTERFACE_DEFINED__ */


#ifndef __IMathe2_INTERFACE_DEFINED__
#define __IMathe2_INTERFACE_DEFINED__

/* interface IMathe2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMathe2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A413CA87-AF88-40A8-9C29-5A5AD467014A")
    IMathe2 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ LONG n1,
            /* [in] */ LONG n2,
            /* [retval][out] */ LONG *pnVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Mul( 
            /* [in] */ LONG n1,
            /* [in] */ LONG n2,
            /* [retval][out] */ LONG *pnVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMathe2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMathe2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMathe2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMathe2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IMathe2 * This,
            /* [in] */ LONG n1,
            /* [in] */ LONG n2,
            /* [retval][out] */ LONG *pnVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Mul )( 
            IMathe2 * This,
            /* [in] */ LONG n1,
            /* [in] */ LONG n2,
            /* [retval][out] */ LONG *pnVal);
        
        END_INTERFACE
    } IMathe2Vtbl;

    interface IMathe2
    {
        CONST_VTBL struct IMathe2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMathe2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMathe2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMathe2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMathe2_Add(This,n1,n2,pnVal)	\
    (This)->lpVtbl -> Add(This,n1,n2,pnVal)

#define IMathe2_Mul(This,n1,n2,pnVal)	\
    (This)->lpVtbl -> Mul(This,n1,n2,pnVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMathe2_Add_Proxy( 
    IMathe2 * This,
    /* [in] */ LONG n1,
    /* [in] */ LONG n2,
    /* [retval][out] */ LONG *pnVal);


void __RPC_STUB IMathe2_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMathe2_Mul_Proxy( 
    IMathe2 * This,
    /* [in] */ LONG n1,
    /* [in] */ LONG n2,
    /* [retval][out] */ LONG *pnVal);


void __RPC_STUB IMathe2_Mul_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMathe2_INTERFACE_DEFINED__ */



#ifndef __Simple4Lib_LIBRARY_DEFINED__
#define __Simple4Lib_LIBRARY_DEFINED__

/* library Simple4Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Simple4Lib;

EXTERN_C const CLSID CLSID_Mathe;

#ifdef __cplusplus

class DECLSPEC_UUID("0D8C856B-00CF-4F84-BCC3-B855562D8772")
Mathe;
#endif
#endif /* __Simple4Lib_LIBRARY_DEFINED__ */

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


