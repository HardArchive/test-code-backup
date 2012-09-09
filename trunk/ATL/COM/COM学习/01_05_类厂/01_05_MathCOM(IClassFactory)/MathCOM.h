

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Sun Nov 13 17:07:52 2005
 */
/* Compiler settings for .\MathCOM.idl:
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

#ifndef __MathCOM_h__
#define __MathCOM_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISimpleMath_FWD_DEFINED__
#define __ISimpleMath_FWD_DEFINED__
typedef interface ISimpleMath ISimpleMath;
#endif 	/* __ISimpleMath_FWD_DEFINED__ */


#ifndef __IAdvancedMath_FWD_DEFINED__
#define __IAdvancedMath_FWD_DEFINED__
typedef interface IAdvancedMath IAdvancedMath;
#endif 	/* __IAdvancedMath_FWD_DEFINED__ */


#ifndef __MATHCOM_FWD_DEFINED__
#define __MATHCOM_FWD_DEFINED__

#ifdef __cplusplus
typedef class MATHCOM MATHCOM;
#else
typedef struct MATHCOM MATHCOM;
#endif /* __cplusplus */

#endif 	/* __MATHCOM_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ISimpleMath_INTERFACE_DEFINED__
#define __ISimpleMath_INTERFACE_DEFINED__

/* interface ISimpleMath */
/* [unique][object][helpstring][uuid] */ 


EXTERN_C const IID IID_ISimpleMath;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FAEAE6B7-67BE-42a4-A318-3256781E945A")
    ISimpleMath : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ int nOp1,
            /* [in] */ int nOp2,
            /* [retval][out] */ int *pret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Subtract( 
            /* [in] */ int nOp1,
            /* [in] */ int nOp2,
            /* [retval][out] */ int *pret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Multiply( 
            /* [in] */ int nOp1,
            /* [in] */ int nOp2,
            /* [retval][out] */ int *pret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Divide( 
            /* [in] */ int nOp1,
            /* [in] */ int nOp2,
            /* [retval][out] */ int *pret) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISimpleMathVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimpleMath * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimpleMath * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimpleMath * This);
        
        HRESULT ( STDMETHODCALLTYPE *Add )( 
            ISimpleMath * This,
            /* [in] */ int nOp1,
            /* [in] */ int nOp2,
            /* [retval][out] */ int *pret);
        
        HRESULT ( STDMETHODCALLTYPE *Subtract )( 
            ISimpleMath * This,
            /* [in] */ int nOp1,
            /* [in] */ int nOp2,
            /* [retval][out] */ int *pret);
        
        HRESULT ( STDMETHODCALLTYPE *Multiply )( 
            ISimpleMath * This,
            /* [in] */ int nOp1,
            /* [in] */ int nOp2,
            /* [retval][out] */ int *pret);
        
        HRESULT ( STDMETHODCALLTYPE *Divide )( 
            ISimpleMath * This,
            /* [in] */ int nOp1,
            /* [in] */ int nOp2,
            /* [retval][out] */ int *pret);
        
        END_INTERFACE
    } ISimpleMathVtbl;

    interface ISimpleMath
    {
        CONST_VTBL struct ISimpleMathVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimpleMath_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISimpleMath_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISimpleMath_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISimpleMath_Add(This,nOp1,nOp2,pret)	\
    (This)->lpVtbl -> Add(This,nOp1,nOp2,pret)

#define ISimpleMath_Subtract(This,nOp1,nOp2,pret)	\
    (This)->lpVtbl -> Subtract(This,nOp1,nOp2,pret)

#define ISimpleMath_Multiply(This,nOp1,nOp2,pret)	\
    (This)->lpVtbl -> Multiply(This,nOp1,nOp2,pret)

#define ISimpleMath_Divide(This,nOp1,nOp2,pret)	\
    (This)->lpVtbl -> Divide(This,nOp1,nOp2,pret)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISimpleMath_Add_Proxy( 
    ISimpleMath * This,
    /* [in] */ int nOp1,
    /* [in] */ int nOp2,
    /* [retval][out] */ int *pret);


void __RPC_STUB ISimpleMath_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISimpleMath_Subtract_Proxy( 
    ISimpleMath * This,
    /* [in] */ int nOp1,
    /* [in] */ int nOp2,
    /* [retval][out] */ int *pret);


void __RPC_STUB ISimpleMath_Subtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISimpleMath_Multiply_Proxy( 
    ISimpleMath * This,
    /* [in] */ int nOp1,
    /* [in] */ int nOp2,
    /* [retval][out] */ int *pret);


void __RPC_STUB ISimpleMath_Multiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISimpleMath_Divide_Proxy( 
    ISimpleMath * This,
    /* [in] */ int nOp1,
    /* [in] */ int nOp2,
    /* [retval][out] */ int *pret);


void __RPC_STUB ISimpleMath_Divide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISimpleMath_INTERFACE_DEFINED__ */


#ifndef __IAdvancedMath_INTERFACE_DEFINED__
#define __IAdvancedMath_INTERFACE_DEFINED__

/* interface IAdvancedMath */
/* [unique][object][helpstring][uuid] */ 


EXTERN_C const IID IID_IAdvancedMath;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("01147C39-9DA0-4f7f-B525-D129745AAD1E")
    IAdvancedMath : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Factorial( 
            /* [in] */ int nOp1,
            /* [retval][out] */ int *pret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Fabonacci( 
            /* [in] */ int nOp1,
            /* [retval][out] */ int *pret) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdvancedMathVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdvancedMath * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdvancedMath * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdvancedMath * This);
        
        HRESULT ( STDMETHODCALLTYPE *Factorial )( 
            IAdvancedMath * This,
            /* [in] */ int nOp1,
            /* [retval][out] */ int *pret);
        
        HRESULT ( STDMETHODCALLTYPE *Fabonacci )( 
            IAdvancedMath * This,
            /* [in] */ int nOp1,
            /* [retval][out] */ int *pret);
        
        END_INTERFACE
    } IAdvancedMathVtbl;

    interface IAdvancedMath
    {
        CONST_VTBL struct IAdvancedMathVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdvancedMath_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdvancedMath_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdvancedMath_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdvancedMath_Factorial(This,nOp1,pret)	\
    (This)->lpVtbl -> Factorial(This,nOp1,pret)

#define IAdvancedMath_Fabonacci(This,nOp1,pret)	\
    (This)->lpVtbl -> Fabonacci(This,nOp1,pret)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAdvancedMath_Factorial_Proxy( 
    IAdvancedMath * This,
    /* [in] */ int nOp1,
    /* [retval][out] */ int *pret);


void __RPC_STUB IAdvancedMath_Factorial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAdvancedMath_Fabonacci_Proxy( 
    IAdvancedMath * This,
    /* [in] */ int nOp1,
    /* [retval][out] */ int *pret);


void __RPC_STUB IAdvancedMath_Fabonacci_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdvancedMath_INTERFACE_DEFINED__ */



#ifndef __MATHCOMLib_LIBRARY_DEFINED__
#define __MATHCOMLib_LIBRARY_DEFINED__

/* library MATHCOMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MATHCOMLib;

EXTERN_C const CLSID CLSID_MATHCOM;

#ifdef __cplusplus

class DECLSPEC_UUID("3BCFE27E-C88D-453C-8C94-F5F7B97E7841")
MATHCOM;
#endif
#endif /* __MATHCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


