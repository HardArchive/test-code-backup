/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Nov 12 16:11:45 2010
 */
/* Compiler settings for F:\Windows+Shell扩展编程完全指南(配套代码)\Windows Shell扩展编程完全指南(配套代码)\ShellExtGuide3_demo\TxtInfo\TxtInfo.idl:
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

#ifndef __TxtInfo_h__
#define __TxtInfo_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITxtInfoShlExt_FWD_DEFINED__
#define __ITxtInfoShlExt_FWD_DEFINED__
typedef interface ITxtInfoShlExt ITxtInfoShlExt;
#endif 	/* __ITxtInfoShlExt_FWD_DEFINED__ */


#ifndef __TxtInfoShlExt_FWD_DEFINED__
#define __TxtInfoShlExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class TxtInfoShlExt TxtInfoShlExt;
#else
typedef struct TxtInfoShlExt TxtInfoShlExt;
#endif /* __cplusplus */

#endif 	/* __TxtInfoShlExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ITxtInfoShlExt_INTERFACE_DEFINED__
#define __ITxtInfoShlExt_INTERFACE_DEFINED__

/* interface ITxtInfoShlExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITxtInfoShlExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F4D78AE0-05AB-11D4-8D3B-444553540000")
    ITxtInfoShlExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITxtInfoShlExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITxtInfoShlExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITxtInfoShlExt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITxtInfoShlExt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITxtInfoShlExt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITxtInfoShlExt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITxtInfoShlExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITxtInfoShlExt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ITxtInfoShlExtVtbl;

    interface ITxtInfoShlExt
    {
        CONST_VTBL struct ITxtInfoShlExtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITxtInfoShlExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITxtInfoShlExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITxtInfoShlExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITxtInfoShlExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITxtInfoShlExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITxtInfoShlExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITxtInfoShlExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITxtInfoShlExt_INTERFACE_DEFINED__ */



#ifndef __TXTINFOLib_LIBRARY_DEFINED__
#define __TXTINFOLib_LIBRARY_DEFINED__

/* library TXTINFOLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TXTINFOLib;

EXTERN_C const CLSID CLSID_TxtInfoShlExt;

#ifdef __cplusplus

class DECLSPEC_UUID("F4D78AE1-05AB-11D4-8D3B-444553540000")
TxtInfoShlExt;
#endif
#endif /* __TXTINFOLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
