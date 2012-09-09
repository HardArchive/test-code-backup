

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Sun Jul 24 02:19:34 2005
 */
/* Compiler settings for .\Simple7.idl:
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


#ifndef __Simple7_h_h__
#define __Simple7_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDispSimple_FWD_DEFINED__
#define __IDispSimple_FWD_DEFINED__
typedef interface IDispSimple IDispSimple;
#endif 	/* __IDispSimple_FWD_DEFINED__ */


#ifndef __DispSimple_FWD_DEFINED__
#define __DispSimple_FWD_DEFINED__

#ifdef __cplusplus
typedef class DispSimple DispSimple;
#else
typedef struct DispSimple DispSimple;
#endif /* __cplusplus */

#endif 	/* __DispSimple_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __Simple7_LIBRARY_DEFINED__
#define __Simple7_LIBRARY_DEFINED__

/* library Simple7 */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Simple7;

#ifndef __IDispSimple_DISPINTERFACE_DEFINED__
#define __IDispSimple_DISPINTERFACE_DEFINED__

/* dispinterface IDispSimple */
/* [uuid] */ 


EXTERN_C const IID DIID_IDispSimple;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F9E6A98A-B849-475F-8D55-F951D86FCD37")
    IDispSimple : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDispSimpleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDispSimple * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDispSimple * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDispSimple * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDispSimple * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDispSimple * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDispSimple * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDispSimple * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDispSimpleVtbl;

    interface IDispSimple
    {
        CONST_VTBL struct IDispSimpleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDispSimple_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDispSimple_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDispSimple_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDispSimple_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDispSimple_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDispSimple_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDispSimple_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDispSimple_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DispSimple;

#ifdef __cplusplus

class DECLSPEC_UUID("C374DBA9-C292-42DA-B146-241748359875")
DispSimple;
#endif
#endif /* __Simple7_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


