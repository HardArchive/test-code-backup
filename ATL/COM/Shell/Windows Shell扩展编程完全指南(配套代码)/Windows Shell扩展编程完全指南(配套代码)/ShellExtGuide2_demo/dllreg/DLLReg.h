/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Nov 11 13:52:44 2010
 */
/* Compiler settings for F:\Windows+Shell扩展编程完全指南(配套代码)\Windows Shell扩展编程完全指南(配套代码)\ShellExtGuide2_demo\dllreg\DLLReg.idl:
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

#ifndef __DLLReg_h__
#define __DLLReg_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDLLRegShlExt_FWD_DEFINED__
#define __IDLLRegShlExt_FWD_DEFINED__
typedef interface IDLLRegShlExt IDLLRegShlExt;
#endif 	/* __IDLLRegShlExt_FWD_DEFINED__ */


#ifndef __DLLRegShlExt_FWD_DEFINED__
#define __DLLRegShlExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class DLLRegShlExt DLLRegShlExt;
#else
typedef struct DLLRegShlExt DLLRegShlExt;
#endif /* __cplusplus */

#endif 	/* __DLLRegShlExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IDLLRegShlExt_INTERFACE_DEFINED__
#define __IDLLRegShlExt_INTERFACE_DEFINED__

/* interface IDLLRegShlExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDLLRegShlExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8AB81E71-CB2F-11D3-8D3B-AC2F34F1FA3C")
    IDLLRegShlExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDLLRegShlExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDLLRegShlExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDLLRegShlExt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDLLRegShlExt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDLLRegShlExt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDLLRegShlExt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDLLRegShlExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDLLRegShlExt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IDLLRegShlExtVtbl;

    interface IDLLRegShlExt
    {
        CONST_VTBL struct IDLLRegShlExtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDLLRegShlExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDLLRegShlExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDLLRegShlExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDLLRegShlExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDLLRegShlExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDLLRegShlExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDLLRegShlExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDLLRegShlExt_INTERFACE_DEFINED__ */



#ifndef __DLLREGLib_LIBRARY_DEFINED__
#define __DLLREGLib_LIBRARY_DEFINED__

/* library DLLREGLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DLLREGLib;

EXTERN_C const CLSID CLSID_DLLRegShlExt;

#ifdef __cplusplus

class DECLSPEC_UUID("8AB81E72-CB2F-11D3-8D3B-AC2F34F1FA3C")
DLLRegShlExt;
#endif
#endif /* __DLLREGLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
