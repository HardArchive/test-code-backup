

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jun 22 11:30:58 2011
 */
/* Compiler settings for .\GncShellExt.idl:
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

#ifndef __GncShellExt_h__
#define __GncShellExt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICopyPathExt_FWD_DEFINED__
#define __ICopyPathExt_FWD_DEFINED__
typedef interface ICopyPathExt ICopyPathExt;
#endif 	/* __ICopyPathExt_FWD_DEFINED__ */


#ifndef __CopyPathExt_FWD_DEFINED__
#define __CopyPathExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class CopyPathExt CopyPathExt;
#else
typedef struct CopyPathExt CopyPathExt;
#endif /* __cplusplus */

#endif 	/* __CopyPathExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ICopyPathExt_INTERFACE_DEFINED__
#define __ICopyPathExt_INTERFACE_DEFINED__

/* interface ICopyPathExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICopyPathExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("303FEFEF-6ABA-11D3-90E4-0090272D53E3")
    ICopyPathExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICopyPathExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICopyPathExt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICopyPathExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICopyPathExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICopyPathExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICopyPathExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICopyPathExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICopyPathExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ICopyPathExtVtbl;

    interface ICopyPathExt
    {
        CONST_VTBL struct ICopyPathExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICopyPathExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICopyPathExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICopyPathExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICopyPathExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICopyPathExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICopyPathExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICopyPathExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICopyPathExt_INTERFACE_DEFINED__ */



#ifndef __GNCSHELLEXTLib_LIBRARY_DEFINED__
#define __GNCSHELLEXTLib_LIBRARY_DEFINED__

/* library GNCSHELLEXTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_GNCSHELLEXTLib;

EXTERN_C const CLSID CLSID_CopyPathExt;

#ifdef __cplusplus

class DECLSPEC_UUID("303FEFF0-6ABA-11D3-90E4-0090272D53E3")
CopyPathExt;
#endif
#endif /* __GNCSHELLEXTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


