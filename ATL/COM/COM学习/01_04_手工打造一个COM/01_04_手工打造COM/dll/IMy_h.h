

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Aug 10 14:51:20 2005
 */
/* Compiler settings for .\IMy.idl:
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

#ifndef __IMy_h_h__
#define __IMy_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMy_FWD_DEFINED__
#define __IMy_FWD_DEFINED__
typedef interface IMy IMy;
#endif 	/* __IMy_FWD_DEFINED__ */


#ifndef __IMy2_FWD_DEFINED__
#define __IMy2_FWD_DEFINED__
typedef interface IMy2 IMy2;
#endif 	/* __IMy2_FWD_DEFINED__ */


#ifndef __SSZ_FWD_DEFINED__
#define __SSZ_FWD_DEFINED__

#ifdef __cplusplus
typedef class SSZ SSZ;
#else
typedef struct SSZ SSZ;
#endif /* __cplusplus */

#endif 	/* __SSZ_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IMy_INTERFACE_DEFINED__
#define __IMy_INTERFACE_DEFINED__

/* interface IMy */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FAEAE6B7-67BE-42a4-A318-3256781E945A")
    IMy : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ int x1,
            /* [in] */ int x2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Get( 
            /* [retval][out] */ int *pret) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMy * This);
        
        HRESULT ( STDMETHODCALLTYPE *Add )( 
            IMy * This,
            /* [in] */ int x1,
            /* [in] */ int x2);
        
        HRESULT ( STDMETHODCALLTYPE *Get )( 
            IMy * This,
            /* [retval][out] */ int *pret);
        
        END_INTERFACE
    } IMyVtbl;

    interface IMy
    {
        CONST_VTBL struct IMyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMy_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMy_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMy_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMy_Add(This,x1,x2)	\
    (This)->lpVtbl -> Add(This,x1,x2)

#define IMy_Get(This,pret)	\
    (This)->lpVtbl -> Get(This,pret)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMy_Add_Proxy( 
    IMy * This,
    /* [in] */ int x1,
    /* [in] */ int x2);


void __RPC_STUB IMy_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMy_Get_Proxy( 
    IMy * This,
    /* [retval][out] */ int *pret);


void __RPC_STUB IMy_Get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMy_INTERFACE_DEFINED__ */


#ifndef __IMy2_INTERFACE_DEFINED__
#define __IMy2_INTERFACE_DEFINED__

/* interface IMy2 */
/* [unique][object][helpstring][uuid] */ 


EXTERN_C const IID IID_IMy2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("01147C39-9DA0-4f7f-B525-D129745AAD1E")
    IMy2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SayHello( 
            /* [retval][out] */ CHAR *pret) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMy2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMy2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMy2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMy2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *SayHello )( 
            IMy2 * This,
            /* [retval][out] */ CHAR *pret);
        
        END_INTERFACE
    } IMy2Vtbl;

    interface IMy2
    {
        CONST_VTBL struct IMy2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMy2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMy2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMy2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMy2_SayHello(This,pret)	\
    (This)->lpVtbl -> SayHello(This,pret)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMy2_SayHello_Proxy( 
    IMy2 * This,
    /* [retval][out] */ CHAR *pret);


void __RPC_STUB IMy2_SayHello_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMy2_INTERFACE_DEFINED__ */



#ifndef __SSZLib_LIBRARY_DEFINED__
#define __SSZLib_LIBRARY_DEFINED__

/* library SSZLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SSZLib;

EXTERN_C const CLSID CLSID_SSZ;

#ifdef __cplusplus

class DECLSPEC_UUID("3BCFE27E-C88D-453C-8C94-F5F7B97E7841")
SSZ;
#endif
#endif /* __SSZLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


