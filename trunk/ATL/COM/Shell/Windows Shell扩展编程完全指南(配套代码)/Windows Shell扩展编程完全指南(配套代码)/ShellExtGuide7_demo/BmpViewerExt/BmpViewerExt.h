/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Aug 31 15:33:06 2000
 */
/* Compiler settings for C:\test\BmpViewerExt\BmpViewerExt.idl:
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

#ifndef __BmpViewerExt_h__
#define __BmpViewerExt_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IBmpCtxMenuExt_FWD_DEFINED__
#define __IBmpCtxMenuExt_FWD_DEFINED__
typedef interface IBmpCtxMenuExt IBmpCtxMenuExt;
#endif 	/* __IBmpCtxMenuExt_FWD_DEFINED__ */


#ifndef __BmpCtxMenuExt_FWD_DEFINED__
#define __BmpCtxMenuExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class BmpCtxMenuExt BmpCtxMenuExt;
#else
typedef struct BmpCtxMenuExt BmpCtxMenuExt;
#endif /* __cplusplus */

#endif 	/* __BmpCtxMenuExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IBmpCtxMenuExt_INTERFACE_DEFINED__
#define __IBmpCtxMenuExt_INTERFACE_DEFINED__

/* interface IBmpCtxMenuExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBmpCtxMenuExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6279F517-5097-44F8-A52F-F182935C485C")
    IBmpCtxMenuExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IBmpCtxMenuExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBmpCtxMenuExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBmpCtxMenuExt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBmpCtxMenuExt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IBmpCtxMenuExt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IBmpCtxMenuExt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IBmpCtxMenuExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IBmpCtxMenuExt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IBmpCtxMenuExtVtbl;

    interface IBmpCtxMenuExt
    {
        CONST_VTBL struct IBmpCtxMenuExtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBmpCtxMenuExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBmpCtxMenuExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBmpCtxMenuExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBmpCtxMenuExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IBmpCtxMenuExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IBmpCtxMenuExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IBmpCtxMenuExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBmpCtxMenuExt_INTERFACE_DEFINED__ */



#ifndef __BMPVIEWEREXTLib_LIBRARY_DEFINED__
#define __BMPVIEWEREXTLib_LIBRARY_DEFINED__

/* library BMPVIEWEREXTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BMPVIEWEREXTLib;

EXTERN_C const CLSID CLSID_BmpCtxMenuExt;

#ifdef __cplusplus

class DECLSPEC_UUID("D6F469CD-3DC7-408F-BB5F-74A1CA2647C9")
BmpCtxMenuExt;
#endif
#endif /* __BMPVIEWEREXTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
