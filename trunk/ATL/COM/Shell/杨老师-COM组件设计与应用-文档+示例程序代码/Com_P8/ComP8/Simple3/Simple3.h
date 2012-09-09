/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jul 19 00:28:17 2005
 */
/* Compiler settings for E:\ComP8\Simple3\Simple3.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


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

#ifndef __Simple3_h__
#define __Simple3_h__

#ifdef __cplusplus
extern "C"{
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

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IMathe_INTERFACE_DEFINED__
#define __IMathe_INTERFACE_DEFINED__

/* interface IMathe */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMathe;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("072EA6CA-7D08-4E7E-B2B7-B2FB0B875595")
    IMathe : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ long n1,
            /* [in] */ long n2,
            /* [retval][out] */ long __RPC_FAR *pnVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMatheVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMathe __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMathe __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMathe __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IMathe __RPC_FAR * This,
            /* [in] */ long n1,
            /* [in] */ long n2,
            /* [retval][out] */ long __RPC_FAR *pnVal);
        
        END_INTERFACE
    } IMatheVtbl;

    interface IMathe
    {
        CONST_VTBL struct IMatheVtbl __RPC_FAR *lpVtbl;
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
    IMathe __RPC_FAR * This,
    /* [in] */ long n1,
    /* [in] */ long n2,
    /* [retval][out] */ long __RPC_FAR *pnVal);


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
    
    MIDL_INTERFACE("072EA6CB-7D08-4E7E-B2B7-B2FB0B875595")
    IStr : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Cat( 
            /* [in] */ BSTR s1,
            /* [in] */ BSTR s2,
            /* [retval][out] */ BSTR __RPC_FAR *psVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IStr __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IStr __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IStr __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cat )( 
            IStr __RPC_FAR * This,
            /* [in] */ BSTR s1,
            /* [in] */ BSTR s2,
            /* [retval][out] */ BSTR __RPC_FAR *psVal);
        
        END_INTERFACE
    } IStrVtbl;

    interface IStr
    {
        CONST_VTBL struct IStrVtbl __RPC_FAR *lpVtbl;
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
    IStr __RPC_FAR * This,
    /* [in] */ BSTR s1,
    /* [in] */ BSTR s2,
    /* [retval][out] */ BSTR __RPC_FAR *psVal);


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
    
    MIDL_INTERFACE("072EA6CC-7D08-4E7E-B2B7-B2FB0B875595")
    IMathe2 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ long n1,
            /* [in] */ long n2,
            /* [retval][out] */ long __RPC_FAR *pnVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Mul( 
            /* [in] */ long n1,
            /* [in] */ long n2,
            /* [retval][out] */ long __RPC_FAR *pnVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMathe2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMathe2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMathe2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMathe2 __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IMathe2 __RPC_FAR * This,
            /* [in] */ long n1,
            /* [in] */ long n2,
            /* [retval][out] */ long __RPC_FAR *pnVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Mul )( 
            IMathe2 __RPC_FAR * This,
            /* [in] */ long n1,
            /* [in] */ long n2,
            /* [retval][out] */ long __RPC_FAR *pnVal);
        
        END_INTERFACE
    } IMathe2Vtbl;

    interface IMathe2
    {
        CONST_VTBL struct IMathe2Vtbl __RPC_FAR *lpVtbl;
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
    IMathe2 __RPC_FAR * This,
    /* [in] */ long n1,
    /* [in] */ long n2,
    /* [retval][out] */ long __RPC_FAR *pnVal);


void __RPC_STUB IMathe2_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMathe2_Mul_Proxy( 
    IMathe2 __RPC_FAR * This,
    /* [in] */ long n1,
    /* [in] */ long n2,
    /* [retval][out] */ long __RPC_FAR *pnVal);


void __RPC_STUB IMathe2_Mul_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMathe2_INTERFACE_DEFINED__ */



#ifndef __SIMPLE3Lib_LIBRARY_DEFINED__
#define __SIMPLE3Lib_LIBRARY_DEFINED__

/* library SIMPLE3Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SIMPLE3Lib;

EXTERN_C const CLSID CLSID_Mathe;

#ifdef __cplusplus

class DECLSPEC_UUID("C6F241E2-43F6-4449-A024-B7340553221E")
Mathe;
#endif
#endif /* __SIMPLE3Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
