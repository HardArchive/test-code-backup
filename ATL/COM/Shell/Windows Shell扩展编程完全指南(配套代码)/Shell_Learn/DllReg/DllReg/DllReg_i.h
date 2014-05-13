

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue May 13 12:55:13 2014
 */
/* Compiler settings for .\DllReg.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
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

#ifndef __DllReg_i_h__
#define __DllReg_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDllRegShlExt_FWD_DEFINED__
#define __IDllRegShlExt_FWD_DEFINED__
typedef interface IDllRegShlExt IDllRegShlExt;
#endif 	/* __IDllRegShlExt_FWD_DEFINED__ */


#ifndef __DllRegShlExt_FWD_DEFINED__
#define __DllRegShlExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class DllRegShlExt DllRegShlExt;
#else
typedef struct DllRegShlExt DllRegShlExt;
#endif /* __cplusplus */

#endif 	/* __DllRegShlExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDllRegShlExt_INTERFACE_DEFINED__
#define __IDllRegShlExt_INTERFACE_DEFINED__

/* interface IDllRegShlExt */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDllRegShlExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3F693544-B09B-4AEA-B927-97CA837CA77A")
    IDllRegShlExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDllRegShlExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDllRegShlExt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDllRegShlExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDllRegShlExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDllRegShlExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDllRegShlExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDllRegShlExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDllRegShlExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDllRegShlExtVtbl;

    interface IDllRegShlExt
    {
        CONST_VTBL struct IDllRegShlExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDllRegShlExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDllRegShlExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDllRegShlExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDllRegShlExt_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDllRegShlExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDllRegShlExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDllRegShlExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDllRegShlExt_INTERFACE_DEFINED__ */



#ifndef __DllRegLib_LIBRARY_DEFINED__
#define __DllRegLib_LIBRARY_DEFINED__

/* library DllRegLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DllRegLib;

EXTERN_C const CLSID CLSID_DllRegShlExt;

#ifdef __cplusplus

class DECLSPEC_UUID("AD5B56AC-5B3F-4E00-A136-9F2B31FD48EA")
DllRegShlExt;
#endif
#endif /* __DllRegLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


