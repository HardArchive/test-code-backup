

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Jun 24 15:31:44 2011
 */
/* Compiler settings for .\DirBkgndExt.idl:
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

#ifndef __DirBkgndExt_h__
#define __DirBkgndExt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBkgndCtxMenuExt_FWD_DEFINED__
#define __IBkgndCtxMenuExt_FWD_DEFINED__
typedef interface IBkgndCtxMenuExt IBkgndCtxMenuExt;
#endif 	/* __IBkgndCtxMenuExt_FWD_DEFINED__ */


#ifndef __BkgndCtxMenuExt_FWD_DEFINED__
#define __BkgndCtxMenuExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class BkgndCtxMenuExt BkgndCtxMenuExt;
#else
typedef struct BkgndCtxMenuExt BkgndCtxMenuExt;
#endif /* __cplusplus */

#endif 	/* __BkgndCtxMenuExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IBkgndCtxMenuExt_INTERFACE_DEFINED__
#define __IBkgndCtxMenuExt_INTERFACE_DEFINED__

/* interface IBkgndCtxMenuExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBkgndCtxMenuExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6F6B303F-2EAD-4973-89EE-5FCEC7257962")
    IBkgndCtxMenuExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IBkgndCtxMenuExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBkgndCtxMenuExt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBkgndCtxMenuExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBkgndCtxMenuExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBkgndCtxMenuExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBkgndCtxMenuExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBkgndCtxMenuExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBkgndCtxMenuExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IBkgndCtxMenuExtVtbl;

    interface IBkgndCtxMenuExt
    {
        CONST_VTBL struct IBkgndCtxMenuExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBkgndCtxMenuExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBkgndCtxMenuExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBkgndCtxMenuExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBkgndCtxMenuExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IBkgndCtxMenuExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IBkgndCtxMenuExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IBkgndCtxMenuExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBkgndCtxMenuExt_INTERFACE_DEFINED__ */



#ifndef __DIRBKGNDEXTLib_LIBRARY_DEFINED__
#define __DIRBKGNDEXTLib_LIBRARY_DEFINED__

/* library DIRBKGNDEXTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DIRBKGNDEXTLib;

EXTERN_C const CLSID CLSID_BkgndCtxMenuExt;

#ifdef __cplusplus

class DECLSPEC_UUID("9E5E1445-6CEA-4761-8E45-AA19F654571E")
BkgndCtxMenuExt;
#endif
#endif /* __DIRBKGNDEXTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


