/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Sep 04 00:01:47 2000
 */
/* Compiler settings for C:\test\DirBkgndExt\DirBkgndExt.idl:
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

#ifndef __DirBkgndExt_h__
#define __DirBkgndExt_h__

#ifdef __cplusplus
extern "C"{
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

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

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
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBkgndCtxMenuExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBkgndCtxMenuExt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBkgndCtxMenuExt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IBkgndCtxMenuExt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IBkgndCtxMenuExt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IBkgndCtxMenuExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IBkgndCtxMenuExt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IBkgndCtxMenuExtVtbl;

    interface IBkgndCtxMenuExt
    {
        CONST_VTBL struct IBkgndCtxMenuExtVtbl __RPC_FAR *lpVtbl;
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
