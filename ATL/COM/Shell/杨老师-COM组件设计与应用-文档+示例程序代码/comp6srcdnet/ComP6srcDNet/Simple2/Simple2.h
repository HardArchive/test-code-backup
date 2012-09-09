

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jun 15 15:30:29 2011
 */
/* Compiler settings for .\Simple2.idl:
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

#ifndef __Simple2_h__
#define __Simple2_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFun_FWD_DEFINED__
#define __IFun_FWD_DEFINED__
typedef interface IFun IFun;
#endif 	/* __IFun_FWD_DEFINED__ */


#ifndef __Fun_FWD_DEFINED__
#define __Fun_FWD_DEFINED__

#ifdef __cplusplus
typedef class Fun Fun;
#else
typedef struct Fun Fun;
#endif /* __cplusplus */

#endif 	/* __Fun_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IFun_INTERFACE_DEFINED__
#define __IFun_INTERFACE_DEFINED__

/* interface IFun */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IFun;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9C0330E2-D53F-43E3-B899-97B94BF76B64")
    IFun : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ LONG n1,
            /* [in] */ LONG n2,
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Cat( 
            /* [in] */ BSTR s1,
            /* [in] */ BSTR s2,
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFunVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFun * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFun * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFun * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IFun * This,
            /* [in] */ LONG n1,
            /* [in] */ LONG n2,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Cat )( 
            IFun * This,
            /* [in] */ BSTR s1,
            /* [in] */ BSTR s2,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IFunVtbl;

    interface IFun
    {
        CONST_VTBL struct IFunVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFun_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFun_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFun_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFun_Add(This,n1,n2,pVal)	\
    (This)->lpVtbl -> Add(This,n1,n2,pVal)

#define IFun_Cat(This,s1,s2,pVal)	\
    (This)->lpVtbl -> Cat(This,s1,s2,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFun_Add_Proxy( 
    IFun * This,
    /* [in] */ LONG n1,
    /* [in] */ LONG n2,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IFun_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFun_Cat_Proxy( 
    IFun * This,
    /* [in] */ BSTR s1,
    /* [in] */ BSTR s2,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFun_Cat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFun_INTERFACE_DEFINED__ */



#ifndef __Simple2Lib_LIBRARY_DEFINED__
#define __Simple2Lib_LIBRARY_DEFINED__

/* library Simple2Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Simple2Lib;

EXTERN_C const CLSID CLSID_Fun;

#ifdef __cplusplus

class DECLSPEC_UUID("CA61C492-9AD3-469F-B75A-C021E03F21FB")
Fun;
#endif
#endif /* __Simple2Lib_LIBRARY_DEFINED__ */

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


