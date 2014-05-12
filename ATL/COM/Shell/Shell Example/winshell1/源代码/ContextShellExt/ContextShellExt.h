

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Mon May 12 16:53:24 2014
 */
/* Compiler settings for .\ContextShellExt.idl:
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

#ifndef __ContextShellExt_h__
#define __ContextShellExt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMenuShellExt_FWD_DEFINED__
#define __IMenuShellExt_FWD_DEFINED__
typedef interface IMenuShellExt IMenuShellExt;
#endif 	/* __IMenuShellExt_FWD_DEFINED__ */


#ifndef __MenuShellExt_FWD_DEFINED__
#define __MenuShellExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class MenuShellExt MenuShellExt;
#else
typedef struct MenuShellExt MenuShellExt;
#endif /* __cplusplus */

#endif 	/* __MenuShellExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMenuShellExt_INTERFACE_DEFINED__
#define __IMenuShellExt_INTERFACE_DEFINED__

/* interface IMenuShellExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMenuShellExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2BC3180D-3E77-465A-AFDD-0311D75A3374")
    IMenuShellExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IMenuShellExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMenuShellExt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMenuShellExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMenuShellExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMenuShellExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMenuShellExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMenuShellExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMenuShellExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IMenuShellExtVtbl;

    interface IMenuShellExt
    {
        CONST_VTBL struct IMenuShellExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMenuShellExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMenuShellExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMenuShellExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMenuShellExt_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMenuShellExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMenuShellExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMenuShellExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMenuShellExt_INTERFACE_DEFINED__ */



#ifndef __CONTEXTSHELLEXTLib_LIBRARY_DEFINED__
#define __CONTEXTSHELLEXTLib_LIBRARY_DEFINED__

/* library CONTEXTSHELLEXTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CONTEXTSHELLEXTLib;

EXTERN_C const CLSID CLSID_MenuShellExt;

#ifdef __cplusplus

class DECLSPEC_UUID("A7275E1B-E709-4484-AFA7-659369738F07")
MenuShellExt;
#endif
#endif /* __CONTEXTSHELLEXTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


